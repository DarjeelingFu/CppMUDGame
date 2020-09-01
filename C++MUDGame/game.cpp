#include <iostream>
#include <fstream>
#include "game.h"

using namespace std;

// ���캯��
Game::Game() {
	currentScene = nullptr;

	loadScene("data/scenes.dat");
	loadSupplyItems("data/supplies.dat");
	loadArmors("data/armors.dat");
	loadWeapons("data/weapons.dat");
	loadNPCs("data/npcs.dat");
	loadItemCollectingTasks("data/itemcollectingtask.dat");
	
}

// �浵
void Game::saveProfile(string playerProfile, string sceneProfile, Player* player, Scene* currentScene) {
	fstream playerFile(playerProfile, ios::out | ios::binary);
	const char* playerPt = (char*)player;
	playerFile.write(playerPt, sizeof(Player));
	playerFile.close();

	fstream sceneFile(sceneProfile, ios::out | ios::binary);
	const char* scenePt = (char*)currentScene;
	sceneFile.write(scenePt, sizeof(Scene));
	sceneFile.close();

}

// ����
void Game::loadProfile(string playerProfile, string sceneProfile, Player* player, Scene* currentScene) {
	fstream playerFile(playerProfile, ios::out | ios::binary);
	char* playerPt = (char*)player;
	playerFile.read(playerPt, sizeof(Player));
	playerFile.close();

	fstream sceneFile(sceneProfile, ios::out | ios::binary);
	char* scenePt = (char*)currentScene;
	sceneFile.read(scenePt, sizeof(Scene));
	sceneFile.close();

}

// ���ɷָ���
void generateDecorations(string mark, int num) {
	for (int i = 0; i < num; i++)
		cout << mark;
	cout << endl;

}

// ˫���
void Game::bindDoubleConnectedSences(Scene& scene_1, Scene& scene_2) {
	scene_1.getNeighbors().emplace_back(&scene_2);
	scene_2.getNeighbors().emplace_back(&scene_1);
}

// �����
void Game::bindSingleConnectedSences(Scene& depart, Scene& destination) {
	depart.getNeighbors().emplace_back(&destination);
}

// չʾ������Ϣ
void Game::showSceneInformation(Scene* scene) {
	generateDecorations("*", 20);
	cout << scene->getName() << endl;
	cout << scene->getDescription() << endl;
	cout << "[����] ";
	vector<Scene*>& neighbors = scene->getNeighbors();
	for (int i = 0; i < neighbors.size(); i++)
		cout << neighbors[i]->getName() << " ";	
	cout << endl;
	generateDecorations("*", 20);
}

// չʾ�����Ϣ
void Game::showPlayerInformation(Player* player) {
	generateDecorations("=", 20);
	cout << "[����]\t" << player->getName() << endl;
	cout << "[Ѫ��]\t" << player->attr(HEALTH) << endl;
	cout << "[����]\t" << player->attr(DEFENCE) << endl;
	cout << "[����]\t" << player->attr(SENSITIVE) << endl;
	cout << "[��Ǯ]\t" << (int)player->attr(MONEY) << endl;
	generateDecorations("-", 20);

	if (!player->getBag()->headArmorSlot.empty())
		cout << "[����]\t" << armors[player->getBag()->weaponSlot.top()].getName() << endl;
	else
		cout << "[����]\t" << "��" << endl;

	if (!player->getBag()->headArmorSlot.empty())
		cout << "[ͷ��]\t" << armors[player->getBag()->headArmorSlot.top()].getName() << endl;
	else
		cout << "[ͷ��]\t" << "��" << endl;

	if (!player->getBag()->bodyArmorSlot.empty())
		cout << "[����]\t" << armors[player->getBag()->bodyArmorSlot.top()].getName() << endl;
	else
		cout << "[����]\t" << "��" << endl;

	if (!player->getBag()->legArmorSlot.empty())
		cout << "[�Ȳ�]\t" << armors[player->getBag()->legArmorSlot.top()].getName() << endl;
	else
		cout << "[�Ȳ�]\t" << "��" << endl;

	if (!player->getBag()->footArmorSlot.empty())
		cout << "[�㲿]\t" << armors[player->getBag()->footArmorSlot.top()].getName() << endl;
	else
		cout << "[�㲿]\t" << "��" << endl;

	generateDecorations("=", 20);
}

// ���ɲ˵�
void Game::generateMenuFromVector(vector<string>& menuItems) {
	int i = 1;
	for (auto ite = menuItems.begin(); ite != menuItems.end(); ite++) {
		cout << i++ << "." << *ite << '\t';
	}
	cout << endl;
}

// ���ָ���Ĳ˵���
string Game::getSelectedMenuItem(vector<string>& menuItems) {
	short option;
	cin >> option;
	if (option >= 1 && option <= menuItems.size())
		return menuItems[option - 1];
	return "error";
}

// ��ʾ�ٽ��ص�
void Game::listNeighbors(Scene* currentScene) {
	int i = 1;
	for (int i = 0; i < currentScene->getNeighbors().size(); i++)
		cout << i + 1 << "." << currentScene->getNeighbors()[i]->getName() << '\t';
	cout << endl;
}

// չʾ��Ʒ��ϸ��Ϣ
void Game::showItemInfo(Item* item) {
	generateDecorations("+", 20);
	cout << "[����]" << '\t' << item->getName() << endl;
	cout << "[����]" << '\t' << item->getDescription() << endl;
	generateDecorations("+", 20);
}

// չʾ����Ʒ
void Game::listSupplies(Bag* bag) {
	int i = 1;
	generateDecorations("-", 20);
	for (auto ite = bag->getSupplies().begin(); ite != bag->getSupplies().end(); ite++) {
		int id = *ite;
		cout << i ++ << "." << supplies[id].getName() << endl;;
	}
	generateDecorations("-", 20);
}

// ʹ�ò���Ʒ
void Game::useSupply(Player* player, short id) {
	switch (supplies[id].getType())
	{
	case ADDHEALTH: {
		float health = player->attr(HEALTH) + supplies[id].getEffectValue();
		if (health >= player->attr(MAXHEALTH))
			health = player->attr(MAXHEALTH);
		player->attr(HEALTH, health);
	}break;
		
	case ADDSTRENGTH: {
		float strength = player->attr(STRENGTH) + supplies[id].getEffectValue();
		player->attr(STRENGTH, strength);
	}break;

	default:
		break;
	}
}

// չʾ�����е�װ��
void Game::listArmors(Bag* bag) {
	int i = 1;
	generateDecorations("-", 20);
	for (auto ite = bag->getArmors().begin(); ite != bag->getArmors().end(); ite++) {
		int id = *ite;
		cout << i++ << "." << armors[id].getName() << endl;;
	}
	generateDecorations("-", 20);
}

// ����װ��
void Game::equipArmor(Player* player, short id) {

	switch (armors[id].getPosition())
	{
	case HEAD: {
		if (!player->getBag()->headArmorSlot.empty())
			player->getBag()->headArmorSlot.pop();
		player->getBag()->headArmorSlot.push(id);
	}break;

	case BODY: {
		if (!player->getBag()->bodyArmorSlot.empty())
			player->getBag()->bodyArmorSlot.pop();
		player->getBag()->bodyArmorSlot.push(id);
	}break;

	case LEG: {
		if (!player->getBag()->legArmorSlot.empty())
			player->getBag()->legArmorSlot.pop();
		player->getBag()->legArmorSlot.push(id);
	}break;

	case FOOT: {
		if (!player->getBag()->footArmorSlot.empty())
			player->getBag()->footArmorSlot.pop();
		player->getBag()->footArmorSlot.push(id);
	}break;

	default:
		break;
	}

	refreshAttrAboutArmor(player);
}

// ���ź�ˢ������ֵ
void Game::refreshAttrAboutArmor(Player* player) {
	float newDenfence = 50;
	float newSensitive = 50;

	Bag* bag = player->getBag();
	if (!bag->headArmorSlot.empty()) {
		newDenfence += armors[bag->headArmorSlot.top()].getDenfence();
		newSensitive += armors[bag->headArmorSlot.top()].getSensitive();
	}

	if (!bag->bodyArmorSlot.empty()) {
		newDenfence += armors[bag->bodyArmorSlot.top()].getDenfence();
		newSensitive += armors[bag->bodyArmorSlot.top()].getSensitive();
	}

	if (!bag->legArmorSlot.empty()) {
		newDenfence += armors[bag->legArmorSlot.top()].getDenfence();
		newSensitive += armors[bag->legArmorSlot.top()].getSensitive();
	}

	if (!bag->footArmorSlot.empty()) {
		newDenfence += armors[bag->footArmorSlot.top()].getDenfence();
		newSensitive += armors[bag->footArmorSlot.top()].getSensitive();
	}

	if (newSensitive < 0)
		newSensitive = 0;

	player->attr(DEFENCE, newDenfence);
	player->attr(SENSITIVE, newSensitive);
}

// չʾ�����е�����
void Game::listWeapons(Bag* bag) {
	int i = 1;
	generateDecorations("-", 20);
	for (auto ite = bag->getWeapons().begin(); ite != bag->getWeapons().end(); ite++) {
		int id = *ite;
		cout << i++ << "." << weapons[id].getName() << endl;;
	}
	generateDecorations("-", 20);
}

// װ��������ˢ���˺�ֵ
void Game::equipWeapon(Player* player, short id) {
	if (!player->getBag()->weaponSlot.empty())
		player->getBag()->weaponSlot.pop();
	player->getBag()->weaponSlot.push(id);
	
	player->attr(DAMAGE, 10 + weapons[id].getDamage());
}

// ��NPC������
void Game::bindNPCWToScene(Scene* scene, NPC* npc) {
	scene->getNPCs().emplace_back(npc);
}

// ��������NPC���ͼ�İ�
void Game:: bindAllNPCsToScene() {
	for (auto ite = NPCs.begin(); ite != NPCs.end(); ite++) {
		if (
			find(scenes[ite->getSceneID()].getNPCs().begin(), scenes[ite->getSceneID()].getNPCs().end(), &*ite) ==
			scenes[ite->getSceneID()].getNPCs().end()
			) {
			bindNPCWToScene(&scenes[ite->getSceneID()], &*ite);
		}
	}
}

// ��ʾ�����е�NPC
void Game::showNPCInScene(Scene* scene) {
	if (!scene->getNPCs().empty()) {
		generateDecorations("#", 20);
		for (auto ite = scene->getNPCs().begin(); ite != scene->getNPCs().end(); ite++) {
			NPC* npc = *ite;
			cout << npc->getName() << endl;
		}
		generateDecorations("#", 20);
	}
}

// ����ʵ���ܵ����˺�
float Game::calculateRealDamage(Player* player, float defaultDamage) {
	float defenceSapped = 1 - (player->attr(DEFENCE) / 500);
	float randFactor = 1 - (rand() % 20 / 100.0);
	return (int)( defaultDamage *  defenceSapped * randFactor );
}

// ��һغ�
int Game::round(Player* player, NPC* enemy) {
	cout << "[Ѫ��]" << player->attr(HEALTH) << "/" << player->attr(MAXHEALTH) << endl;
	vector<string> fightMenuItems = { "����", "����" };
	generateMenuFromVector(fightMenuItems);
	string fightMenuItem = getSelectedMenuItem(fightMenuItems);
	system("cls");

	if (fightMenuItem == "����") {
		int realDamage = calculateRealDamage(enemy, player->attr(DAMAGE));
		enemy->alter(HEALTH, -realDamage);
		cout << player->getName() << "���" << realDamage << "���˺�" << endl;
		return 0;
	}

	if (fightMenuItem == "����") {
		float escapePossibility = 0.6 * (player->attr(SENSITIVE) / enemy->attr(SENSITIVE));
		if (escapePossibility > 0.5)
			return 1;
		else return 0;
	}
}

// ���ж�ս
void Game::fight(Player* player, NPC* enemy) {
	cout << "��ʼս��" << endl;
	system("Pause");
	system("cls");

	if (player->attr(SENSITIVE) >= enemy->attr(SENSITIVE)) {
		round(player, enemy);
	}

	while (player->attr(HEALTH) > 0 && enemy->attr(HEALTH) > 0) {

		float damageFromEnemy = calculateRealDamage(player, enemy->attr(DAMAGE));
		player->alter(HEALTH, -damageFromEnemy);
		cout << "����ܵ���" << damageFromEnemy << "���˺�" << endl;

		int ifEscape = round(player, enemy);
		if (ifEscape) {
			cout << "���ܳɹ�" << endl;
			system("Pause");
			return;
		}

	}

}

// ��õ�����Ʒ
void Game::getRandItemsForDefeatedEnemy(Player* player, NPC* enemy) {
	int itemType = rand() % 2;
	switch (itemType)
	{
	case 0:
		if (!enemy->getBag()->getSupplies().empty()) {
			int randId = rand() % enemy->getBag()->getSupplies().size();
			player->getBag()->getSupplies().emplace_back(
				supplies[enemy->getBag()->getSupplies()[randId]].getID()
			);
			cout << player->getName() << "�����" << supplies[enemy->getBag()->getSupplies()[randId]].getName() << endl;
		}
		break;

	case 1:
		if (!enemy->getBag()->getArmors().empty()) {
			int randId = rand() % enemy->getBag()->getArmors().size();
			player->getBag()->getArmors().emplace_back(
				supplies[enemy->getBag()->getArmors()[randId]].getID()
			);
			cout << player->getName() << "�����" << armors[enemy->getBag()->getArmors()[randId]].getName() << endl;
		}
		break;

	case 2:
		if (!enemy->getBag()->getWeapons().empty()) {
			int randId = rand() % enemy->getBag()->getWeapons().size();
			player->getBag()->getWeapons().emplace_back(
				supplies[enemy->getBag()->getWeapons()[randId]].getID()
			);
			cout << player->getName() << "�����" << weapons[enemy->getBag()->getWeapons()[randId]].getName() << endl;
		}
		break;

	default:
		break;
	}

	player->alter(MONEY, enemy->attr(MONEY));
	cout << "��ý�Ǯ" << enemy->attr(MONEY) << endl;
}

// ������Ʒ
void Game::giveItem(Player* player, short id, int prompt) {
	switch (prompt)
	{
	case SUPPLY: {
		player->getBag()->addSupply(id);
	}break;

	case ARMOR: {
		player->getBag()->addArmor(id);
	}break;

	case WEAPON: {
		player->getBag()->addWeapon(id);
	}break;

	default:
		break;
	}
}

// �Ƴ���Ʒ
void Game::eraeItwm(Player* player, short id, int prompt) {
	switch (prompt)
	{
	case SUPPLY: {
		player->getBag()->getSupplies().erase(
			find(player->getBag()->getSupplies().begin(), player->getBag()->getSupplies().end(), id)
		);
	}break;

	case ARMOR: {
		player->getBag()->getArmors().erase(
			find(player->getBag()->getArmors().begin(), player->getBag()->getArmors().end(), id)
		);
	}break;

	case WEAPON: {
		player->getBag()->getWeapons().erase(
			find(player->getBag()->getWeapons().begin(), player->getBag()->getWeapons().end(), id)
		);
	}break;

	default:
		break;
	}
}

// ����
void Game::trade(Player* player, NPC* npc) {

	cout << "1.����" << '\t' << "2.����" << endl;
	char option;
	cin >> option;
	if (option == '1') {
		char goodsID = '#';
		while (goodsID != '0') {
			system("cls");
			int order = 1;
			for (auto ite = npc->getBag()->getSupplies().begin(); ite != npc->getBag()->getSupplies().end(); ite++) {
				cout << order++ << " " << supplies[*ite].getName() << '\t' << supplies[*ite].getValue() << endl;
			}
			cout << "������Ź�������0�뿪" << endl;
			cin >> goodsID;
			if (goodsID == '0')
				return;

			else if (goodsID > '0' && goodsID <= '9') {
				int id = atoi(&goodsID);
				short targetGoodsID = npc->getBag()->getSupplies()[id - 1];
				if (player->attr(MONEY) >= supplies[targetGoodsID].getValue()) {
					giveItem(player, targetGoodsID, SUPPLY);
					eraeItwm(npc, targetGoodsID, SUPPLY);

					//float randomFactor = 1.0 - rand() % 20 / 100;
					//int price = (int)(supplies[targetGoodsID].getValue() * randomFactor);

					int price = supplies[targetGoodsID].getValue();

					player->alter(MONEY, -price);
					cout << "��Ǯ-" << price << ", �����" << supplies[targetGoodsID].getName() << endl;
					system("Pause");
				}
				else {
					cout << "��Ǯ����" << endl;
					system("Pause");
				}
			}
		}
	}

	if (option == '2') {
		char goodsID = '#';
		while (goodsID != '0') {
			system("cls");
			int order = 1;
			for (auto ite = player->getBag()->getSupplies().begin(); ite != player->getBag()->getSupplies().end(); ite++) {
				cout << order++ << " " << supplies[*ite].getName() << '\t' << supplies[*ite].getSoldValue() << endl;
			}
			cout << "������ų��ۣ�����0�뿪" << endl;
			cin >> goodsID;
			if (goodsID == '0')
				return;

			else if (goodsID > '0' && goodsID <= '9') {
				int id = atoi(&goodsID);
				short targetGoodsID = player->getBag()->getSupplies()[id - 1];
				if (npc->attr(MONEY) >= supplies[targetGoodsID].getValue()) {
					giveItem(npc, targetGoodsID, SUPPLY);
					eraeItwm(player, targetGoodsID, SUPPLY);

					//float randomFactor = 1.0 - rand() % 20 / 100;
					//int price = (int)(supplies[targetGoodsID].getValue() * randomFactor);

					int price = supplies[targetGoodsID].getSoldValue();

					npc->alter(MONEY, -price);
					cout << "��Ǯ+" << price << ", ʧȥ��" << supplies[targetGoodsID].getName() << endl;
					system("Pause");
				}
				else {
					cout << "��Ǯ����" << endl;
					system("Pause");
				}
			}

		}
	}

}

// ����
void Game::run() {

	system("cls");

	// ȷ����ϷĬ��״̬
	int gameState = TITLE;

	// ��Ϸ�Ƿ�������
	bool isRunning = true;

	// ��ѭ��
	while (isRunning) {
		
		switch (gameState) {

		case TITLE: {
			system("cls");

			vector<string> titleMenuItems = { "��ʼ��Ϸ", "��ȡ����", "�˳���Ϸ" };
			generateMenuFromVector(titleMenuItems);
			string selected = getSelectedMenuItem(titleMenuItems);

			if (selected == "��ʼ��Ϸ") {

				// ��������Ϸ��������ɫ
				player = new Player("Player", 100, 100, 100, 0, 50, 10, 100);
				player->getBag()->addSupply(0);
				player->getBag()->addSupply(1);
				player->getBag()->addArmor(0);
				player->getBag()->addArmor(1);
				player->getBag()->addArmor(2);
				player->getBag()->addWeapon(0);

				// ȷ����ǰ�ص�
				currentScene = &scenes[0];

				// ��NPC
				bindAllNPCsToScene();

				gameState = SCENE;

			}

			if (selected == "��ȡ����") {
				loadProfile("profile/playerProfile.sav", "profile/sceneProfile.sav", player, currentScene);
				system("cls");
				cout << "��ȡ�ɹ�" << endl;
				system("Pause");
				gameState = SCENE;
			}

			if (selected == "�˳���Ϸ")
				isRunning = false;

		}break;

		case SCENE: {
			system("cls");

			// չʾ��Ϣ
			showSceneInformation(currentScene);
			showNPCInScene(currentScene);
			showPlayerInformation(player);

			// ��ʾ�˵�
			vector<string> sceneMenuItems = { "�ƶ�", "����", "����", "ϵͳ" };
			generateMenuFromVector(sceneMenuItems);

			// ��������
			string selected = getSelectedMenuItem(sceneMenuItems);

			// �ƶ�
			if (selected == "�ƶ�") {
				system("cls");
				listNeighbors(currentScene);
				short option;
				cin >> option;
				if (option >= 1 && option <= currentScene->getNeighbors().size()) {
					currentScene = currentScene->getNeighbors()[option - 1];
				}
			}

			// ����
			if (selected == "����") {
				gameState = BAG;
			}

			// ����
			if (selected == "����") {
				system("cls");
				char option;
				cout << "1.����\t" << "2.����\t" << "3.����" << endl;
				cin >> option;
				if (option == '1')
					gameState = TRADE;
				if (option == '2')
					gameState = FIGHT;
			}

			// ϵͳ
			if (selected == "ϵͳ") {
				system("cls");

				vector<string> systemMenuItems = { "�ص���Ϸ", "���ر���", "�������" };
				generateMenuFromVector(systemMenuItems);
				string option = getSelectedMenuItem(systemMenuItems);

				if (option == "���ر���")
					gameState = TITLE;

				if (option == "�������") {
					saveProfile("profile/playerProfile.sav", "profile/sceneProfile.sav", player, currentScene);
					system("cls");
					cout << "����ɹ�" << endl;
					system("Pause");
				}
			}

		}break;

		case BAG: {
			system("cls");
			vector<string> itemTypeMenuItems = { "����", "װ��", "����", "����" };
			generateMenuFromVector(itemTypeMenuItems);
			string itemTypeMenuItem = getSelectedMenuItem(itemTypeMenuItems);

			if (itemTypeMenuItem == "����") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listSupplies(player->getBag());
					cout << "������Ų鿴������0�˳�" << endl;
					cin >> option;
					if (option == 0)
						gameState = SCENE;
					else {
						if (option > 0 && option <= player->getBag()->getSupplies().size()) {
							Supply* supplyItem = &supplies[player->getBag()->getSupplies()[option - 1]];
							showItemInfo(supplyItem);

							vector<string> bagMenuItems = { "ʹ��", "����", "����" };
							generateMenuFromVector(bagMenuItems);
							string bagMenuItem = getSelectedMenuItem(bagMenuItems);

							if (bagMenuItem == "ʹ��") {
								useSupply(player, supplyItem->getID());
								player->getBag()->getSupplies().erase(player->getBag()->getSupplies().begin() + option - 1);
							}
							if (bagMenuItem == "����") {
								player->getBag()->getSupplies().erase(player->getBag()->getSupplies().begin() + option - 1);
							}
							if (bagMenuItem == "����") {

							}
						}
					}

				}
			}

			if (itemTypeMenuItem == "װ��") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listArmors(player->getBag());
					cout << "������Ų鿴������0�˳�" << endl;
					cin >> option;
					if (option == 0)
						gameState = SCENE;
					else {
						if (option > 0 && option <= player->getBag()->getArmors().size()) {
							Armor* armor = &armors[player->getBag()->getArmors()[option - 1]];
							showItemInfo(armor);

							vector<string> bagMenuItems = { "ʹ��", "����", "����" };
							generateMenuFromVector(bagMenuItems);
							string bagMenuItem = getSelectedMenuItem(bagMenuItems);

							if (bagMenuItem == "ʹ��") {
								if(player->getBag())
								equipArmor(player, armor->getID());
							}

							if (bagMenuItem == "����") {
								player->getBag()->getArmors().erase(player->getBag()->getArmors().begin() + option - 1);
							}
						}
					}
				}
			}

			if (itemTypeMenuItem == "����") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listWeapons(player->getBag());
					cout << "������Ų鿴������0�˳�" << endl;
					cin >> option;
					if (option == 0)
						gameState = SCENE;
					else {
						if (option > 0 && option <= player->getBag()->getWeapons().size()) {
							Weapon* weapon = &weapons[player->getBag()->getWeapons()[option - 1]];
							showItemInfo(weapon);

							vector<string> bagMenuItems = { "ʹ��", "����", "����" };
							generateMenuFromVector(bagMenuItems);
							string bagMenuItem = getSelectedMenuItem(bagMenuItems);

							if (bagMenuItem == "ʹ��") {
								equipWeapon(player, weapon->getID());
							}

							if (bagMenuItem == "����") {
								player->getBag()->getWeapons().erase(player->getBag()->getWeapons().begin() + option - 1);
							}
						}

					}
				}
			}

			if (itemTypeMenuItem == "����") {
				gameState = SCENE;
			}

		}break;

		case FIGHT:  {
			system("cls");
			if (!currentScene->getNPCs().empty()) {
				int order = 0;
				for (auto ite = currentScene->getNPCs().begin(); ite != currentScene->getNPCs().end(); ite++) {
					NPC npc = **ite;
					cout << ++order << "." << npc.getName() << '\t';
				}
				cout << endl;
				int option;
				cin >> option;
				if (option >= 1 && option <= currentScene->getNPCs().size()) {
					fight(player, currentScene->getNPCs()[option - 1]);
					if (player->attr(HEALTH) < 0) {
						gameState = TITLE;
						cout << "������" << endl;
						system("Pause");
					}
					else if (currentScene->getNPCs()[option - 1]->attr(HEALTH) < 0) {
						cout << "���˱�����" << endl;
						getRandItemsForDefeatedEnemy(player, currentScene->getNPCs()[option - 1]);
						currentScene->getNPCs().erase(currentScene->getNPCs().begin() + option - 1);
						system("Pause");
					}
					gameState = SCENE;
				}
			}
			else {
				cout << "�޹�������" << endl;
				gameState = SCENE;
				system("Pause");
			}

		}break;

		case TRADE: {
			system("cls");
			if (!currentScene->getNPCs().empty()) {
				int order = 0;
				for (auto ite = currentScene->getNPCs().begin(); ite != currentScene->getNPCs().end(); ite++) {
					NPC npc = **ite;
					cout << ++order << "." << npc.getName() << '\t';
				}
				cout << endl;
				char target;
				cin >> target;
				if (target >= '0' && target <= '9' ) {
					int option = atoi(&target);
					if (currentScene->getNPCs()[option - 1]->getCareer() != 0) {
						cout << "��NPC���ɽ���" << endl;
						system("Pause");
					}
					else {
						trade(player, currentScene->getNPCs()[option - 1]);
					}
					gameState = SCENE;
				}
			}
			else {
				cout << "�޽��׶������" << endl;
				gameState = SCENE;
				system("Pause");
			}

		}break;

		default:
			break;
		}

	}

}
