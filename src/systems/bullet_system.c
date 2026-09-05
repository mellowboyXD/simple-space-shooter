#include "bullet_system.h"
#include "components.h"
#include "coordinator.h"
#include "systems/system.h"
#include "utils.h"

static void _SpawnBullet(const Position *const pos,
			 const WeaponModifier *const wm)
{
	Entity bullet = CoordinatorCreateEntity();
	Velocity vel = { 0, -wm->bulletSpeed };
	Hitbox hb = { wm->bulletSize, wm->bulletSize };
	Position spawnPos = { pos->x + hb.width / 2.0f, pos->y };

	CoordinatorAddComponent(bullet, COMPONENT_POSITION, &spawnPos);
	CoordinatorAddComponent(bullet, COMPONENT_VELOCITY, &vel);
	CoordinatorAddComponent(bullet, COMPONENT_HITBOX, &hb);
	CoordinatorAddComponent(bullet, COMPONENT_RENDER, &RENDER_C(BLACK));
	CoordinatorAddTag(bullet, TAG_BULLET);
}

BulletSystem *BulletSystemCreate()
{
	BulletSystem *self = CoordinatorRegisterSystem(BULLET_SYSTEM_TYPE,
						       BulletSystemUpdate);

	Signature signature = COMPONENT_BIT(COMPONENT_PLAYER_INPUT) |
			      COMPONENT_BIT(COMPONENT_POSITION) |
			      COMPONENT_BIT(COMPONENT_RENDER) |
			      COMPONENT_BIT(COMPONENT_HITBOX) |
			      COMPONENT_BIT(COMPONENT_WEAPON_MODIFIER);

	CoordinatorSetSystemSignature(BULLET_SYSTEM_TYPE, signature);
	return self;
}

void BulletSystemUpdate(BulletSystem *self, float dt)
{
	for (size_t i = 0; i < self->count; i++) {
		Entity entity = self->entities[i];

		PlayerInput *input = GET_COMPONENT(PlayerInput, entity,
						   COMPONENT_PLAYER_INPUT);
		Position *pos =
			GET_COMPONENT(Position, entity, COMPONENT_POSITION);
		Render *r = GET_COMPONENT(Render, entity, COMPONENT_RENDER);
		Hitbox *hb = GET_COMPONENT(Hitbox, entity, COMPONENT_HITBOX);
		WeaponModifier *wm = GET_COMPONENT(WeaponModifier, entity,
						   COMPONENT_WEAPON_MODIFIER);

		wm->_cooldown -= dt;
		if (input->fire && wm->_cooldown <= 0.0f) {
			Vector2 offset = { 0 };
			Position hbPos = GetHitboxPos(pos, r, hb, offset);

			float x = hbPos.x + hb->width / 2.0f - wm->bulletSize;
			float y = hbPos.y - wm->bulletSize;
			Position spawnPos = { x, y };

			_SpawnBullet(&spawnPos, wm);
			wm->_cooldown = wm->fireRate;
		}
	}
}
