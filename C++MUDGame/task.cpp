#include "task.h"

using namespace std;

int ItemCollectingTask::checkProgress(Player* player) {
	if (getState() == false)
		return DISMISS;

	switch (itemType)
	{
	case SUPPLY: {
		if (find(
			player->getBag()->getSupplies().begin(),
			player->getBag()->getSupplies().end(),
			id) != player->getBag()->getSupplies().end()
			) {
			return COMPLISHED;
		}
	}break;

	case ARMOR: {
		if (find(
			player->getBag()->getArmors().begin(),
			player->getBag()->getArmors().end(),
			id) != player->getBag()->getArmors().end()
			) {
			return COMPLISHED;
		}
	}break;

	case WEAPON: {
		if (find(
			player->getBag()->getWeapons().begin(),
			player->getBag()->getWeapons().end(),
			id) != player->getBag()->getWeapons().end()
			) {
			return COMPLISHED;
		}
	}break;

	default:
		break;
	}

	return UNCOMPLISHED;
}
