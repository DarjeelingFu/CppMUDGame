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
			maxHealth = setValue;
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

// 物品收集任务检查进度
int ItemCollectingTask::checkProgress(Player* player) {
	if (getState() == false)
		return DISMISS;

	switch (getTargetItemType())
	{
	case SUPPLY: {
		if (find(
			player->getBag()->getSupplies().begin(),
			player->getBag()->getSupplies().end(),
			getTargetItemId()) != player->getBag()->getSupplies().end()
			) {
			return COMPLISHED;
		}
	}break;

	case ARMOR: {
		if (find(
			player->getBag()->getArmors().begin(),
			player->getBag()->getArmors().end(),
			getTargetItemId()) != player->getBag()->getArmors().end()
			) {
			return COMPLISHED;
		}
	}break;

	case WEAPON: {
		if (find(
			player->getBag()->getWeapons().begin(),
			player->getBag()->getWeapons().end(),
			getTargetItemId()) != player->getBag()->getWeapons().end()
			) {
			return COMPLISHED;
		}
	}break;

	default:
		break;
	}

	return UNCOMPLISHED;
}
