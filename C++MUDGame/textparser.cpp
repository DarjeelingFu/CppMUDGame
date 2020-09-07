#include "game.h"
#include <iostream>
#include <fstream>

using namespace std;

void Game::loadScene(string path) {
	fstream file(path, ios::in);
	char c;
	int order = 0;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string name, description, connected, isBlocked;
			getline(file, name);
			getline(file, name);
			getline(file, description);
			getline(file, connected);
			getline(file, isBlocked);

			scenes.emplace_back(Scene(order++, name, description, connected, stoi(isBlocked)));
		}
	}
	file.close();
	for (auto ite = scenes.begin(); ite != scenes.end(); ite++) {
		vector<int> neighborID;
		string connected = ite->getConnected();

		int j = 0;
		for (int i = 0; i < connected.length(); i++) {
			if (connected[i] == ' ') {
				neighborID.emplace_back(atoi(connected.substr(j, i).c_str()));
				j = i;
			}
		}
		neighborID.emplace_back(atoi(connected.substr(j, connected.length()).c_str()));

		for (auto ite1 = neighborID.begin(); ite1 != neighborID.end(); ite1++) {
			ite->getNeighbors().emplace_back(&scenes[*ite1]);
		}
	}
}

void Game::loadSupplyItems(string path) {
	fstream file(path, ios::in);
	int order = 0;
	char c;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string name, description, valueString, soldValue, typeString, effectValueString;
			getline(file, name); // 消释掉一行

			getline(file, name);
			getline(file, description);
			getline(file, valueString);
			getline(file, soldValue);
			getline(file, typeString);
			getline(file, effectValueString);

			supplies.emplace_back(Supply(
				order++, name, description,
				atoi(valueString.c_str()), atoi(soldValue.c_str()),
				atoi(typeString.c_str()), atof(effectValueString.c_str())));
		}
	}
	file.close();
}

void Game::loadArmors(string path) {
	fstream file(path, ios::in);
	int order = 0;
	char c;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string name, description, valueString, soldValue, position, denfenceEffect, sensitiveEffect;
			getline(file, name); // 消释掉一行

			getline(file, name);
			getline(file, description);
			getline(file, valueString);
			getline(file, soldValue);
			getline(file, position);
			getline(file, denfenceEffect);
			getline(file, sensitiveEffect);

			armors.emplace_back(Armor(
				order++, name, description, atoi(valueString.c_str()), atoi(soldValue.c_str()),
				atoi(position.c_str()), atof(denfenceEffect.c_str()), atof(sensitiveEffect.c_str())));
		}
	}
	file.close();
}

void Game::loadWeapons(string path) {
	fstream file(path, ios::in);
	int order = 0;
	char c;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string name, description, valueString, soldValue, damage;
			getline(file, name); // 消释掉一行

			getline(file, name);
			getline(file, description);
			getline(file, valueString);
			getline(file, soldValue);
			getline(file, damage);

			weapons.emplace_back(Weapon(
				order++, name, description, atoi(valueString.c_str()),
				atoi(soldValue.c_str()), atof(damage.c_str())));
		}
	}
	file.close();
}

void Game::loadNPCs(string path) {
	fstream file(path, ios::in);
	int order = 0;
	char c;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string name, health, maxHealth, strength, defence, sensitive, damage, money, sceneID;
			string item_s, armor_s, weapon_s, career;
			getline(file, name); // 消释掉一行

			getline(file, name);
			getline(file, health);
			getline(file, maxHealth);
			getline(file, strength);
			getline(file, defence);
			getline(file, sensitive);
			getline(file, damage);
			getline(file, money);
			getline(file, sceneID);
			getline(file, item_s);
			getline(file, armor_s);
			getline(file, weapon_s);
			getline(file, career);

			NPCs.emplace_back(NPC(
				order,
				name,
				atof(health.c_str()),
				atof(maxHealth.c_str()),
				atof(strength.c_str()),
				atof(defence.c_str()),
				atof(sensitive.c_str()),
				atof(damage.c_str()),
				atoi(money.c_str()),
				atoi(sceneID.c_str()),
				item_s,
				armor_s,
				weapon_s,
				atoi(career.c_str())
			));

			string dialogue;
			getline(file, dialogue);
			while (dialogue != "$") {
				NPCs[order].getDialogues().emplace_back(dialogue);
				getline(file, dialogue);
			}
			order++;
		}
	}

	file.close();

	for (auto ite = NPCs.begin(); ite != NPCs.end(); ite++) {

		if (ite->getItemList() != "-1") {
			vector<int> itemList;
			string connected = ite->getItemList();

			int j = 0;
			for (int i = 0; i < connected.length(); i++) {
				if (connected[i] == ' ') {
					itemList.emplace_back(atoi(connected.substr(j, i).c_str()));
					j = i;
				}
			}
			itemList.emplace_back(atoi(connected.substr(j, connected.length()).c_str()));

			for (auto ite1 = itemList.begin(); ite1 != itemList.end(); ite1++) {
				ite->getBag()->addSupply(*ite1);
			}
		}

		if (ite->getArmorList() != "-1") {
			vector<int> itemList;
			string connected = ite->getArmorList();

			int j = 0;
			for (int i = 0; i < connected.length(); i++) {
				if (connected[i] == ' ') {
					itemList.emplace_back(atoi(connected.substr(j, i).c_str()));
					j = i;
				}
			}
			itemList.emplace_back(atoi(connected.substr(j, connected.length()).c_str()));

			for (auto ite1 = itemList.begin(); ite1 != itemList.end(); ite1++) {
				ite->getBag()->addArmor(*ite1);
			}
		}

		if (ite->getWeaponList() != "-1") {
			vector<int> itemList;
			string connected = ite->getWeaponList();

			int j = 0;
			for (int i = 0; i < connected.length(); i++) {
				if (connected[i] == ' ') {
					itemList.emplace_back(atoi(connected.substr(j, i).c_str()));
					j = i;
				}
			}
			itemList.emplace_back(atoi(connected.substr(j, connected.length()).c_str()));

			for (auto ite1 = itemList.begin(); ite1 != itemList.end(); ite1++) {
				ite->getBag()->addWeapon(*ite1);
			}
		}


	}
}

void Game::loadItemCollectingTasks(string path) {
	fstream file(path, ios::in);
	int order = 0;
	char c;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string npc, rewardItenType, rewardItemId, targetItemType, targetItemId;
			string dismiss, uncomplished, complished;
			getline(file, npc); // 消释掉一行

			getline(file, npc);
			getline(file, rewardItenType);
			getline(file, rewardItemId);
			getline(file, targetItemType);
			getline(file, targetItemId);

			getline(file, dismiss);
			getline(file, uncomplished);
			getline(file, complished);

			collectingTasks.emplace_back(ItemCollectingTask(
				atoi(npc.c_str()), atoi(rewardItenType.c_str()),
				atoi(rewardItemId.c_str()), atoi(targetItemType.c_str()),
				atoi(targetItemId.c_str()), dismiss, uncomplished, complished
			));
		}
	}
	file.close();

	for (auto ite = collectingTasks.begin(); ite != collectingTasks.end(); ite++) {
		NPCs[ite->getNPC()].getTask().emplace_back(&*ite);
	}

}