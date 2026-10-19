// Animation Manager
// Owns all the animation components

#ifndef ANIMATION_H
#define ANIMATION_H

#include "components.h"

constexpr AnimClipId MAX_ANIM_CLIPS = 100;

// AnimEvents describes what kind of event should occur at a particular frame
typedef enum { ANIM_EVENT_SHOOT } AnimEventType;

// the animation event object and what frame it resides at
typedef struct {
	size_t frameIndex;
	AnimEventType eventType;
} AnimEvent;

// the animation clip component itself
// TODO: clip should specify its source. maybe a id like TextureId to a texture
// that shall live indefinitely.
// If texture and current frame is owned by the render component how does that
// work??
typedef struct {
	AnimFrame *frames;
	size_t frameCount;
	AnimEvent *events;
	size_t eventCount;
	bool shouldLoop;
	float totalDurations; // ms round up
} AnimClip;

typedef struct {
	AnimClip clips[MAX_ANIM_CLIPS];
	AnimClipId count;
} AnimManager;

// Public Functions

// Initializes the animation manager
void AnimationInit(AnimManager *self);

// Deinitializes the animation manager
void AnimationManagerDeinit(AnimManager *self);

// Creates an animation clip component
AnimClip AnimationCreateClip(AnimManager *self, size_t nFrames, size_t nEvents,
			     bool shouldLoop);

// Retrieves a Clip from the clip pool
AnimClip *AnimationGetClip(AnimManager *self, AnimClipId clipId);

// Appends a frame to the clip specified by clipId. returns true on success
bool AnimationAddFrameToClip(AnimManager *self, AnimClipId clipId,
			     AnimFrame frame);

// Appends multiple frames to the clip specified by clipId. returns true on success
bool AnimationBulkAddFramesToClip(AnimManager *self, AnimClipId clipId,
				  AnimFrame frames[], size_t nFrames);

// Add an animation clip to the pool. Returns true on success, false otherwise
bool AnimationAddClip(AnimManager *self, AnimClip clip);

// TODO: Append Events to clips
// TODO: Retrieve Events from clip
// TODO: Initialize the manager in coordinator and provide a reference that can
// be used or a helper function to retrieve clips inside coordinator

#endif // ANIMATION_H
