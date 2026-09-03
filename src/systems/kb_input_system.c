#include "kb_input_system.h"
#include "components.h"
#include "constants.h"
#include "coordinator.h"
#include "raylib.h"
#include "utils.h"

#include "debug.h"
#include <assert.h>
#include <math.h>

static Velocity _NormalizeVelocity(Velocity vel)
{
	double len = sqrt(vel.dx * vel.dx + vel.dy * vel.dy);
	if (len == 0)
		return vel;

	return (Velocity){ vel.dx / len, vel.dy / len };
}

static void _SpawnBullet(const Position *const pos)
{
	Entity bullet = CoordinatorCreateEntity();
	Velocity vel = { 0, -BULLET_SPEED };
	Hitbox hb = { 10, 10 };
	Position spawnPos = { pos->x + hb.width / 2.0f, pos->y };

	CoordinatorAddComponent(bullet, COMPONENT_POSITION, &spawnPos);
	CoordinatorAddComponent(bullet, COMPONENT_VELOCITY, &vel);
	CoordinatorAddComponent(bullet, COMPONENT_HITBOX, &hb);
	CoordinatorAddComponent(bullet, COMPONENT_RENDER, &RENDER_C(BLACK));
	CoordinatorAddTag(bullet, TAG_BULLET);
}

// Public Functions

KBInputSystem *KBInputSystemCreate()
{
	KBInputSystem *self = CoordinatorRegisterSystem(KB_INPUT_SYSTEM_TYPE,
							KBInputSystemUpdate);
	Signature signature = COMPONENT_BIT(COMPONENT_VELOCITY) |
			      COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(COMPONENT_HITBOX) |
			      COMPONENT_BIT(TAG_PLAYER);
	CoordinatorSetSystemSignature(KB_INPUT_SYSTEM_TYPE, signature);
	return self;
}

static float _cooldown = 0;
void KBInputSystemUpdate(KBInputSystem *self, [[maybe_unused]] float dt)
{
	_cooldown -= dt;
	for (size_t i = 0; i < self->count; i++) {
		Entity player = self->entities[i];

		Position *pos =
			GET_COMPONENT(Position, player, COMPONENT_POSITION);

		Velocity *vel =
			GET_COMPONENT(Velocity, player, COMPONENT_VELOCITY);
		Hitbox *hb = GET_COMPONENT(Hitbox, player, COMPONENT_HITBOX);

		// if you are player, you should already have that component
		Render *r = GET_COMPONENT(Render, player, COMPONENT_RENDER);
		assert(r != NULL && "Player should have a render component");

		Velocity dir = { 0 };
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
			dir.dy = -1;
		}

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
			dir.dy = 1;
		}

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			dir.dx = -1;
		}

		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			dir.dx = 1;
		}

		if (IsKeyPressed(KEY_P)) {
			LOG(L_INFO, "player: (%f, %f)", pos->x, pos->y);
		}

		if (IsKeyDown(KEY_SPACE) && _cooldown <= 0.0f) {
			Position spawnPos =
				GetHitboxPos(pos, r, hb, (Vector2){ 0, 0 });
			spawnPos.y = pos->y;
			_SpawnBullet(&spawnPos);
			_cooldown = NORMAL_FIRE_RATE;
		}

		Velocity normalized = _NormalizeVelocity(dir);
		vel->dy = normalized.dy * PLAYER_SPEED;
		vel->dx = normalized.dx * PLAYER_SPEED;
	}
}
