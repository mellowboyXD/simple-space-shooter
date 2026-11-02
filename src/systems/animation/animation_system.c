#include "animation_system.h"
#include "coordinator.h"
#include "debug.h"
#include <assert.h>
#include <math.h>

static AnimFrame *_ResolveFrame(AnimClip *clip, float currentTime)
{
	assert(clip != NULL && "Clip is null");
	assert(currentTime <= clip->totalDurations &&
	       "Current time cannot be larger than the total duration of clip.");

        if (currentTime >= clip->totalDurations) {
                // hold last frame
                return clip->frames + (clip->frameCount - 1);
        } 

	// what is the AnimFrame at this time?
	float t = 0.0f;
	for (size_t i = 0; i < clip->frameCount; i++) {
		assert(clip->frames != NULL && "Clip frames is null.");
		t += clip->frames[i].duration;

		if (t > currentTime) {
			return clip->frames + i;
		}
	}

        // last frame if slightly below totalDurations
	return clip->frames + (clip->frameCount - 1);
}

static bool _IsSameFrame(AnimFrame a, AnimFrame b)
{
	return a.frameIndex == b.frameIndex && a.duration == b.duration;
}

AnimationSystem *AnimationSystemCreate()
{
	AnimationSystem *self = CoordinatorRegisterSystem(
		ANIMATION_SYSTEM_TYPE, AnimationSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_RENDER) |
			      COMPONENT_BIT(COMPONENT_ANIMATOR);

	CoordinatorSetSystemSignature(ANIMATION_SYSTEM_TYPE, signature);

	return self;
}

void AnimationSystemUpdate(AnimationSystem *self, float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];
		Render *render =
			GET_COMPONENT(Render, entity, COMPONENT_RENDER);
		Animator *animator =
			GET_COMPONENT(Animator, entity, COMPONENT_ANIMATOR);

		// TODO: for now, we just skip render_mode COLOR
		// this means events wont fire, we'll figure it out later
		if (render->renderMode == RENDER_COLOR) {
			continue;
		}

		AnimClip *clip = CoordinatorGetAnimClip(animator->clipId);
		assert(clip != NULL && "clip should not be null");

		animator->currentTime += dt * animator->animationSpeed;
		if (animator->currentTime >= clip->totalDurations) {
			if (clip->shouldLoop) {
				// start from beginning again
				animator->currentTime =
					fmodf(animator->currentTime,
					      clip->totalDurations);
				if (animator->currentTime < 0.0f)
					animator->currentTime +=
						clip->totalDurations;
			} else {
				// stop the animation
				animator->animState = ANIM_STOP;
			}
		}

		// slap current clip frame onto the renderer
		if (animator->animState == ANIM_PLAYING) {
			AnimFrame *newFrame =
				_ResolveFrame(clip, animator->currentTime);
			assert(newFrame != NULL && "Invalid new frame");

			if (!_IsSameFrame(*newFrame, animator->currentFrame)) {
				// TODO: Fire all events belonging to the new frame

				LOG(L_DEBUG, "New FRAME idx: %d",
				    newFrame->frameIndex);

				animator->currentFrame = *newFrame;

				assert(render->renderMode == RENDER_SPRITE &&
				       "Should be rendering in sprite mode");
				render->frame.x += (newFrame->frameIndex *
						    render->frame.width);
			}
		}
	}
}
