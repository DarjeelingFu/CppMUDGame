#include "player.h"

// 属性函数，setValue为默认时为getter，否则为setter
float Player::attr(int prompt, float setValue) {
	switch (prompt)
	{
	case HEALTH:
		if (setValue != -1)
			health = setValue;
		return health;

	case MAXHEALTH:
		if (setValue != -1)
			health = setValue;
		return maxHealth;

	case STRENGTH:
		if (setValue != -1)
			strength = setValue;
		return strength;

	case DEFENCE:
		if (setValue != -1)
			defence = setValue;
		return defence;

	case SENSITIVE:
		if (setValue != -1)
			sensitive = setValue;
		return sensitive;

	case DAMAGE:
		if (setValue != -1)
			damage = setValue;
		return damage;

	case MONEY:
		if (setValue != -1)
			money = (int)setValue;
		return money;

	default:
		break;
	}
}

// 用变化量改变属性
void Player::alter(int prompt, float value) {
	switch (prompt)
	{
	case HEALTH:
		health += value;
		break;

	case MAXHEALTH:
		maxHealth += value;
		break;

	case STRENGTH:
		strength += value;
		break;

	case DEFENCE:
		defence += value;
		break;

	case SENSITIVE:
		sensitive += value;
		break;

	case DAMAGE:
		damage += value;
		break;

	case MONEY:
		money += value;
		break;

	default:
		break;
	}

}
