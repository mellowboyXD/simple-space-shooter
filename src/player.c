#include "player.h"
#include "constants.h"
#include "coordinator.h"

void PlayerInit(Entity player)
{
	AssetId id = CoordinatorLoadAsset(
		"resources/sprites/main_ship/base_full_health.png");

	Rectangle frame = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
	Position pos = { PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y };
	PlayerModifiers mod = { .speed = PLAYER_SPEED };
	Velocity vel = { 0, 0 };
	Hitbox hb = { PLAYER_HITBOX_SIZE, PLAYER_HITBOX_SIZE };
	WeaponModifier weapon = { .bulletSize = BULLET_SIZE,
				  .bulletSpeed = BULLET_SPEED,
				  .type = AUTO_CANNON_WEAPON,
				  .fireRate = NORMAL_FIRE_RATE };

	CoordinatorAddTag(player, TAG_PLAYER);

	CoordinatorAddComponent(player, COMPONENT_POSITION, &pos);
	CoordinatorAddComponent(player, COMPONENT_VELOCITY, &vel);
	CoordinatorAddComponent(player, COMPONENT_PLAYER_MODIFIERS, &mod);
	CoordinatorAddComponent(player, COMPONENT_HITBOX, &hb);

	CoordinatorAddComponent(player, COMPONENT_RENDER, &RENDER_S(id, frame));

	CoordinatorAddComponent(player, COMPONENT_WEAPON_MODIFIER, &weapon);
	CoordinatorAddComponent(player, COMPONENT_PLAYER_INPUT,
				&PLAYER_INPUT_INIT);
}
