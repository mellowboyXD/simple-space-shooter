#include "bullet_system.h"
#include "components.h"
#include "coordinator.h"
#include "systems/system.h"
#include "utils.h"

static void _SpawnBullet(const Position *const pos, float bulletSpeed)
{
	Entity bullet = CoordinatorCreateEntity();
	Velocity vel = { 0, -bulletSpeed };
	Hitbox hb = { 10, 10 };
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
			Position hbPos =
				GetHitboxPos(pos, r, hb, (Vector2){ 0, -10 });
                        _SpawnBullet(&hbPos, wm->bulletSpeed);
			wm->_cooldown = wm->fireRate;
		}
	}
}
