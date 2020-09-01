#include "game.h"
#include <iostream>
#include <fstream>

using namespace std;

void Game::loadScene(string path) {
	fstream file(path, ios::in);
	char c;
	while (!file.eof()) {
		c = file.get();
		if (c == '#') {
			string name, description, connected;
			getline(file, name);
			getline(file, name);
			getline(file, description);
			getline(file, connected);

			scenes.emplace_back(Scene(name, description, connected));
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
			getline(file, name); // ���͵�һ��

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
			getline(file, name); // ���͵�һ��

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
			getline(file, name); // ���͵�һ��

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
			string name, health, maxHealth, strength, defence, sensitive, damage, money, sceneID, items, career;
			getline(file, name); // ���͵�һ��

			getline(file, name);
			getline(file, health);
			getline(file, maxHealth);
			getline(file, strength);
			getline(file, defence);
			getline(file, sensitive);
			getline(file, damage);
			getline(file, money);
			getline(file, sceneID);
			getline(file, items);
			getline(file, career);

			NPCs.emplace_back(NPC(
				name,
				atof(health.c_str()),
				atof(maxHealth.c_str()),
				atof(strength.c_str()),
				atof(defence.c_str()),
				atof(sensitive.c_str()),
				atof(damage.c_str()),
				atoi(money.c_str()),
				atoi(sceneID.c_str()),
				items,
				atoi(career.c_str())
			));

			string dialogue;
			getline(file, dialogue);
			while (dialogue != "$") {
				NPCs[order].getDialogues().emplace_back(dialogue);
				getline(file, dialogue);
			}

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

	}
}
