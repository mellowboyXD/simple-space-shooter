#include "animation.h"
#include <assert.h>

#define ASSERT_MANAGER_NOT_NULL (assert(self != NULL && "Animation Manager should not be null"))
#define ASSERT_VALID_CLIP_ID(manager, id) (assert((id) < (self->count) && "Invalid clipId. Out of range"))

void AnimationInit(AnimManager *self)
{
        self->count= 0;
}

void AnimationDeinit(AnimManager *self)
{
        self->count = 0;
}

AnimClip *AnimationGetClip(AnimManager *self, AnimClipId clipId)
{
        ASSERT_MANAGER_NOT_NULL;
        ASSERT_VALID_CLIP_ID(self, clipId);
        if (!self || clipId >= self->count) {
                return NULL;
        }

        return self->clips + clipId;
}

bool AnimationAddFrameToClip(AnimManager *self, AnimClipId clipId, AnimFrame frame)
{
        ASSERT_MANAGER_NOT_NULL;
        ASSERT_VALID_CLIP_ID(self, clipId);
        if (!self || clipId >= self->count) {
                return false;
        }

        AnimClip *clip = self->clips + clipId;
        clip->frames[clip->frameCount++] = frame;
        return true;
}

bool AnimationBulkAddFramesToClip(AnimManager *self, AnimClipId clipId, AnimFrame *frames, size_t nFrames)
{
        ASSERT_MANAGER_NOT_NULL;

        for (size_t i = 0; i < nFrames; i++) {
                if (!AnimationAddFrameToClip(self, clipId, frames[i])) {
                        return false;
                }
        }

        return true;
}

bool AnimationAddClip(AnimManager *self, AnimClip clip)
{
        ASSERT_MANAGER_NOT_NULL;
        assert(self->count < MAX_ANIM_CLIPS && "Not enough space to add new clip");
        if (self->count < MAX_ANIM_CLIPS) {
                return false;
        }

        self->clips[self->count++] = clip;
        return true;
}
