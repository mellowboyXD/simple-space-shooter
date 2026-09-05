#include "bullet_system.h"
#include "components.h"
#include "constants.h"
#include "coordinator.h"
#include "systems/system.h"
#include "utils.h"

static void _SpawnBullet(const Position *const pos, WeaponModifier *const wm)
{
	Entity bullet = CoordinatorCreateEntity();
	Velocity vel = { 0, -wm->bulletSpeed };
	Hitbox hb = { wm->bulletSize, wm->bulletSize };

	Render r = {
		.zIndex = BASE_LAYER - 1,
		.renderMode = RENDER_SPRITE,
		.textureId = CoordinatorLoadAsset(
			"resources/sprites/weapons/auto_cannon_projectile.png"),
		.frame = { 0, 0, 32, 32 },
		.renderColor = BLACK
	};

	Position bulletHbCenter =
		GetHitboxCenter(pos, &r, &hb, (Vector2){ 0, 0 });
	float offset = bulletHbCenter.x - pos->x;
	Position spawnPos = { pos->x - offset, pos->y - offset };

	CoordinatorAddComponent(bullet, COMPONENT_POSITION, &spawnPos);
	CoordinatorAddComponent(bullet, COMPONENT_VELOCITY, &vel);
	CoordinatorAddComponent(bullet, COMPONENT_HITBOX, &hb);
	CoordinatorAddComponent(bullet, COMPONENT_RENDER, &r);
	CoordinatorAddTag(bullet, TAG_BULLET);

	size_t maxSprites = 7;
	size_t index = wm->sprite.frame.x / SPRITE_SIZE;
	wm->sprite.frame.x = ((index + 1) % maxSprites) * SPRITE_SIZE;
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
			Vector2 offset = { 0, 0 };
			Position hbPos = GetHitboxCenter(pos, r, hb, offset);

			hbPos.y -= wm->bulletSize;

			_SpawnBullet(&hbPos, wm);
			wm->_cooldown = wm->fireRate;
		}
	}
}
