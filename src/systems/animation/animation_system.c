#include "animation_system.h"
#include "animation.h"
#include "coordinator.h"
#include <assert.h>
#include <math.h>

static AnimFrame *_ResolveFrame(AnimClipId clipId, float currentTime)
{
	AnimClip *clip = AnimationGetClip(clipId);

	assert(clip != NULL && "Clip is null");
	assert(currentTime <= clip->totalDurations &&
	       "Current time cannot be larger than the total duration of clip.");

	// what is the AnimFrame at this time?
	float t = 0;
	for (size_t i = 0; i < clip->frameCount; i++) {
		assert(clip->frames != NULL && "Clip frames is null.");
		AnimFrame *curr = clip->frames + i;
		t += curr->duration;

		if (t >= currentTime) {
			return curr;
		}
	}

	// Should be unreacheable as currentTime should fall within that
	// durations ??
	return NULL;
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

		if (animator->animState != ANIM_PLAYING) {
			continue;
		}

		// TODO: for now, we just skip render_mode COLOR
		// this means events wont fire, we'll figure it out later
		if (render->renderMode == RENDER_COLOR) {
			continue;
		}

		AnimClip *clip = AnimationGetClip(animator->clipId);

		animator->currentTime += round(dt * animator->animationSpeed);
		if (animator->currentTime >= animator->totalDuration) {
			if (clip->shouldLoop) {
				// start from beginning again
				animator->currentTime = 0;
			} else {
				// stop the animation
				animator->animState = ANIM_STOP;
			}
		}

		// slap current clip frame onto the renderer
		if (animator->animState == ANIM_PLAYING) {
			AnimFrame *newFrame = _ResolveFrame(
				animator->clipId, animator->currentTime);
			assert(newFrame != NULL && "Invalid new frame");

			if (!_IsSameFrame(*newFrame, animator->currentFrame)) {
				// TODO: Fire all events belonging to the new frame

				animator->currentFrame = *newFrame;

				assert(render->renderMode == RENDER_SPRITE &&
				       "Should be rendering in sprite mode");
				render->frame.x += (newFrame->frameIndex *
						    render->frame.width);
			}
		}
	}
}
