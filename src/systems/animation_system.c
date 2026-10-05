#include "animation_system.h"
#include "components.h"
#include "coordinator.h"
#include <math.h>

// AnimEvents describes what kind of event should occur at a particular frame
typedef enum { ANIM_EVENT_SHOOT } AnimEventType;

// the current animation frame and how long to keep showing this frame
typedef struct {
	size_t frameIndex;
	float duration; // ms
} AnimFrame;

// the animation event object and what frame it resides at
typedef struct {
	size_t frameIndex;
	AnimEventType eventType;
} AnimEvent;

// the animation clip component itself
typedef struct {
	AnimFrame *frames;
	size_t frameCount;
	AnimEvent *events;
	size_t eventCount;
	bool shouldLoop;
	size_t totalDurations; // ms round up
} AnimClip;

static constexpr AnimClipId MAX_ANIM_CLIPS = 100;

static AnimClip animClips[MAX_ANIM_CLIPS];

static AnimClip *_GetClip(AnimClipId clipId)
{
        // TODO: validate clip id
        return animClips + clipId;
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

                AnimClip *clip = _GetClip(animator->clipId);

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
	}
}
