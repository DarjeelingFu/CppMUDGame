#include <iostream>
#include <fstream>
#include "game.h"

using namespace std;

// 构造函数
Game::Game() {
	currentScene = nullptr;

	loadScene("data/scenes.dat");
	loadSupplyItems("data/supplies.dat");
	loadArmors("data/armors.dat");
	loadWeapons("data/weapons.dat");
	loadNPCs("data/npcs.dat");
	loadItemCollectingTasks("data/itemcollectingtask.dat");
	
}

// 存档
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

// 读档
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

// 生成分隔符
void generateDecorations(string mark, int num) {
	for (int i = 0; i < num; i++)
		cout << mark;
	cout << endl;

}

// 双向绑定
void Game::bindDoubleConnectedSences(Scene& scene_1, Scene& scene_2) {
	scene_1.getNeighbors().emplace_back(&scene_2);
	scene_2.getNeighbors().emplace_back(&scene_1);
}

// 单向绑定
void Game::bindSingleConnectedSences(Scene& depart, Scene& destination) {
	depart.getNeighbors().emplace_back(&destination);
}

// 展示场景信息
void Game::showSceneInformation(Scene* scene) {
	generateDecorations("*", 20);
	cout << scene->getName() << endl;
	cout << scene->getDescription() << endl;
	cout << "[附近] ";
	vector<Scene*>& neighbors = scene->getNeighbors();
	for (int i = 0; i < neighbors.size(); i++)
		cout << neighbors[i]->getName() << " ";	
	cout << endl;
	generateDecorations("*", 20);
}

// 展示玩家信息
void Game::showPlayerInformation(Player* player) {
	generateDecorations("=", 20);
	cout << "[名称]\t" << player->getName() << endl;
	cout << "[血量]\t" << player->attr(HEALTH) << endl;
	cout << "[防御]\t" << player->attr(DEFENCE) << endl;
	cout << "[敏捷]\t" << player->attr(SENSITIVE) << endl;
	cout << "[金钱]\t" << (int)player->attr(MONEY) << endl;
	generateDecorations("-", 20);

	if (!player->getBag()->headArmorSlot.empty())
		cout << "[武器]\t" << armors[player->getBag()->weaponSlot.top()].getName() << endl;
	else
		cout << "[武器]\t" << "无" << endl;

	if (!player->getBag()->headArmorSlot.empty())
		cout << "[头部]\t" << armors[player->getBag()->headArmorSlot.top()].getName() << endl;
	else
		cout << "[头部]\t" << "无" << endl;

	if (!player->getBag()->bodyArmorSlot.empty())
		cout << "[身体]\t" << armors[player->getBag()->bodyArmorSlot.top()].getName() << endl;
	else
		cout << "[身体]\t" << "无" << endl;

	if (!player->getBag()->legArmorSlot.empty())
		cout << "[腿部]\t" << armors[player->getBag()->legArmorSlot.top()].getName() << endl;
	else
		cout << "[腿部]\t" << "无" << endl;

	if (!player->getBag()->footArmorSlot.empty())
		cout << "[足部]\t" << armors[player->getBag()->footArmorSlot.top()].getName() << endl;
	else
		cout << "[足部]\t" << "无" << endl;

	generateDecorations("=", 20);
}

// 生成菜单
void Game::generateMenuFromVector(vector<string>& menuItems) {
	int i = 1;
	for (auto ite = menuItems.begin(); ite != menuItems.end(); ite++) {
		cout << i++ << "." << *ite << '\t';
	}
	cout << endl;
}

// 获得指定的菜单项
string Game::getSelectedMenuItem(vector<string>& menuItems) {
	short option;
	cin >> option;
	if (option >= 1 && option <= menuItems.size())
		return menuItems[option - 1];
	return "error";
}

// 显示临近地点
void Game::listNeighbors(Scene* currentScene) {
	int i = 1;
	for (int i = 0; i < currentScene->getNeighbors().size(); i++)
		cout << i + 1 << "." << currentScene->getNeighbors()[i]->getName() << '\t';
	cout << endl;
}

// 展示物品详细信息
void Game::showItemInfo(Item* item) {
	generateDecorations("+", 20);
	cout << "[名称]" << '\t' << item->getName() << endl;
	cout << "[描述]" << '\t' << item->getDescription() << endl;
	generateDecorations("+", 20);
}

// 展示补给品
void Game::listSupplies(Bag* bag) {
	int i = 1;
	generateDecorations("-", 20);
	for (auto ite = bag->getSupplies().begin(); ite != bag->getSupplies().end(); ite++) {
		int id = *ite;
		cout << i ++ << "." << supplies[id].getName() << endl;;
	}
	generateDecorations("-", 20);
}

// 使用补给品
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

// 展示背包中的装备
void Game::listArmors(Bag* bag) {
	int i = 1;
	generateDecorations("-", 20);
	for (auto ite = bag->getArmors().begin(); ite != bag->getArmors().end(); ite++) {
		int id = *ite;
		cout << i++ << "." << armors[id].getName() << endl;;
	}
	generateDecorations("-", 20);
}

// 穿着装备
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

// 穿着后刷新属性值
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

// 展示背包中的武器
void Game::listWeapons(Bag* bag) {
	int i = 1;
	generateDecorations("-", 20);
	for (auto ite = bag->getWeapons().begin(); ite != bag->getWeapons().end(); ite++) {
		int id = *ite;
		cout << i++ << "." << weapons[id].getName() << endl;;
	}
	generateDecorations("-", 20);
}

// 装备武器并刷新伤害值
void Game::equipWeapon(Player* player, short id) {
	if (!player->getBag()->weaponSlot.empty())
		player->getBag()->weaponSlot.pop();
	player->getBag()->weaponSlot.push(id);
	
	player->attr(DAMAGE, 10 + weapons[id].getDamage());
}

// 绑定NPC到场景
void Game::bindNPCWToScene(Scene* scene, NPC* npc) {
	scene->getNPCs().emplace_back(npc);
}

// 处理所有NPC与地图的绑定
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

// 显示场景中的NPC
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

// 计算实际受到的伤害
float Game::calculateRealDamage(Player* player, float defaultDamage) {
	float defenceSapped = 1 - (player->attr(DEFENCE) / 500);
	float randFactor = 1 - (rand() % 20 / 100.0);
	return (int)( defaultDamage *  defenceSapped * randFactor );
}

// 玩家回合
int Game::round(Player* player, NPC* enemy) {
	cout << "[血量]" << player->attr(HEALTH) << "/" << player->attr(MAXHEALTH) << endl;
	vector<string> fightMenuItems = { "攻击", "逃跑" ,"技能"};
	generateMenuFromVector(fightMenuItems);
	string fightMenuItem = getSelectedMenuItem(fightMenuItems);
	system("cls");

	if (fightMenuItem == "攻击") {
		int realDamage = calculateRealDamage(enemy, player->attr(DAMAGE));
		enemy->alter(HEALTH, -realDamage);
		cout << player->getName() << "造成" << realDamage << "点伤害" << endl;
		return 0;
	}

	if (fightMenuItem == "逃跑") {
		float escapePossibility = 0.6 * (player->attr(SENSITIVE) / enemy->attr(SENSITIVE));
		if (escapePossibility > 0.5)
			return 1;
		else return 0;
	}
	if (fightMenuItem == "技能") {
		int realDamage = calculateRealDamage(enemy, player->attr(DAMAGE)*2.5);
		enemy->alter(HEALTH, -realDamage);
		cout << player->getName() << "造成" << realDamage << "点伤害" << endl;
		int skilllost = calculateRealDamage(player, player->attr(DAMAGE)*0.5);
		player->alter(HEALTH, -skilllost);
		cout << player->getName() << "损失" << skilllost << "点血量" << endl;
		return 0;
	}
}

// 进行对战
void Game::fight(Player* player, NPC* enemy) {
	cout << "开始战斗" << endl;
	system("Pause");
	system("cls");

	if (player->attr(SENSITIVE) >= enemy->attr(SENSITIVE)) {
		round(player, enemy);
	}

	while (player->attr(HEALTH) > 0 && enemy->attr(HEALTH) > 0) {

		float damageFromEnemy = calculateRealDamage(player, enemy->attr(DAMAGE));
		player->alter(HEALTH, -damageFromEnemy);
		cout << "玩家受到了" << damageFromEnemy << "点伤害" << endl;

		if (player->attr(HEALTH) <= 0)
			return;

		int ifEscape = round(player, enemy);
		if (ifEscape) {
			cout << "逃跑成功" << endl;
			system("Pause");
			return;
		}

	}

}

// 获得掉落物品
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
			cout << player->getName() << "获得了" << supplies[enemy->getBag()->getSupplies()[randId]].getName() << endl;
		}
		break;

	case 1:
		if (!enemy->getBag()->getArmors().empty()) {
			int randId = rand() % enemy->getBag()->getArmors().size();
			player->getBag()->getArmors().emplace_back(
				supplies[enemy->getBag()->getArmors()[randId]].getID()
			);
			cout << player->getName() << "获得了" << armors[enemy->getBag()->getArmors()[randId]].getName() << endl;
		}
		break;

	case 2:
		if (!enemy->getBag()->getWeapons().empty()) {
			int randId = rand() % enemy->getBag()->getWeapons().size();
			player->getBag()->getWeapons().emplace_back(
				supplies[enemy->getBag()->getWeapons()[randId]].getID()
			);
			cout << player->getName() << "获得了" << weapons[enemy->getBag()->getWeapons()[randId]].getName() << endl;
		}
		break;

	default:
		break;
	}

	player->alter(MONEY, enemy->attr(MONEY));
	cout << "获得金钱" << enemy->attr(MONEY) << endl;
}

// 给予物品
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

// 移除物品
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

// 交易
void Game::trade(Player* player, NPC* npc) {

	cout << "1.购买" << '\t' << "2.出售" << endl;
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
			cout << "输入序号购买，输入0离开" << endl;
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
					npc->alter(MONEY, price);

					cout << "金钱-" << price << ", 获得了" << supplies[targetGoodsID].getName() << endl;
					system("Pause");
				}
				else {
					cout << "金钱不足" << endl;
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
			cout << "输入序号出售，输入0离开" << endl;
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
					player->alter(MONEY, price);
					cout << "金钱+" << price << ", 失去了" << supplies[targetGoodsID].getName() << endl;
					system("Pause");
				}
				else {
					cout << "金钱不足" << endl;
					system("Pause");
				}
			}

		}
	}

}

// 运行
void Game::run() {

	system("cls");

	// 确定游戏默认状态
	int gameState = TITLE;

	// 游戏是否在运行
	bool isRunning = true;

	// 主循环
	while (isRunning) {
		
		switch (gameState) {

		case TITLE: {
			system("cls");

			vector<string> titleMenuItems = { "开始游戏", "读取进度", "退出游戏" };
			generateMenuFromVector(titleMenuItems);
			string selected = getSelectedMenuItem(titleMenuItems);

			if (selected == "开始游戏") {

				// 进行新游戏，创建角色
				player = new Player("Player", 100, 100, 100, 0, 50, 10, 100);
				player->getBag()->addSupply(0);
				player->getBag()->addSupply(1);
				player->getBag()->addArmor(0);
				player->getBag()->addArmor(1);
				player->getBag()->addArmor(2);
				player->getBag()->addWeapon(0);

				// 确定当前地点
				currentScene = &scenes[0];

				// 绑定NPC
				bindAllNPCsToScene();

				gameState = SCENE;

			}

			if (selected == "读取进度") {
				loadProfile("profile/playerProfile.sav", "profile/sceneProfile.sav", player, currentScene);
				system("cls");
				cout << "读取成功" << endl;
				system("Pause");
				gameState = SCENE;
			}

			if (selected == "退出游戏")
				isRunning = false;

		}break;

		case SCENE: {
			system("cls");

			// 展示信息
			showSceneInformation(currentScene);
			showNPCInScene(currentScene);
			showPlayerInformation(player);

			// 显示菜单
			vector<string> sceneMenuItems = { "移动", "背包", "交互", "系统" };
			generateMenuFromVector(sceneMenuItems);

			// 处理输入
			string selected = getSelectedMenuItem(sceneMenuItems);

			// 移动
			if (selected == "移动") {
				system("cls");
				listNeighbors(currentScene);
				short option;
				cin >> option;
				if (option >= 1 && option <= currentScene->getNeighbors().size()) {
					currentScene = currentScene->getNeighbors()[option - 1];
				}
			}

			// 背包
			if (selected == "背包") {
				gameState = BAG;
			}

			// 攻击
			if (selected == "交互") {
				system("cls");
				char option;
				cout << "1.交易\t" << "2.攻击\t" << "3.返回" << endl;
				cin >> option;
				if (option == '1')
					gameState = TRADE;
				if (option == '2')
					gameState = FIGHT;
			}

			// 系统
			if (selected == "系统") {
				system("cls");

				vector<string> systemMenuItems = { "回到游戏", "返回标题", "保存进度" };
				generateMenuFromVector(systemMenuItems);
				string option = getSelectedMenuItem(systemMenuItems);

				if (option == "返回标题")
					gameState = TITLE;

				if (option == "保存进度") {
					saveProfile("profile/playerProfile.sav", "profile/sceneProfile.sav", player, currentScene);
					system("cls");
					cout << "保存成功" << endl;
					system("Pause");
				}
			}

		}break;

		case BAG: {
			system("cls");
			vector<string> itemTypeMenuItems = { "补给", "装备", "武器", "返回" };
			generateMenuFromVector(itemTypeMenuItems);
			string itemTypeMenuItem = getSelectedMenuItem(itemTypeMenuItems);

			if (itemTypeMenuItem == "补给") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listSupplies(player->getBag());
					cout << "输入序号查看，输入0退出" << endl;
					cin >> option;
					if (option == 0)
						gameState = SCENE;
					else {
						if (option > 0 && option <= player->getBag()->getSupplies().size()) {
							Supply* supplyItem = &supplies[player->getBag()->getSupplies()[option - 1]];
							showItemInfo(supplyItem);

							vector<string> bagMenuItems = { "使用", "丢弃", "返回" };
							generateMenuFromVector(bagMenuItems);
							string bagMenuItem = getSelectedMenuItem(bagMenuItems);

							if (bagMenuItem == "使用") {
								useSupply(player, supplyItem->getID());
								player->getBag()->getSupplies().erase(player->getBag()->getSupplies().begin() + option - 1);
							}
							if (bagMenuItem == "丢弃") {
								player->getBag()->getSupplies().erase(player->getBag()->getSupplies().begin() + option - 1);
							}
							if (bagMenuItem == "返回") {

							}
						}
					}

				}
			}

			if (itemTypeMenuItem == "装备") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listArmors(player->getBag());
					cout << "输入序号查看，输入0退出" << endl;
					cin >> option;
					if (option == 0)
						gameState = SCENE;
					else {
						if (option > 0 && option <= player->getBag()->getArmors().size()) {
							Armor* armor = &armors[player->getBag()->getArmors()[option - 1]];
							showItemInfo(armor);

							vector<string> bagMenuItems = { "使用", "丢弃", "返回" };
							generateMenuFromVector(bagMenuItems);
							string bagMenuItem = getSelectedMenuItem(bagMenuItems);

							if (bagMenuItem == "使用") {
								if(player->getBag())
								equipArmor(player, armor->getID());
							}

							if (bagMenuItem == "丢弃") {
								player->getBag()->getArmors().erase(player->getBag()->getArmors().begin() + option - 1);
							}
						}
					}
				}
			}

			if (itemTypeMenuItem == "武器") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listWeapons(player->getBag());
					cout << "输入序号查看，输入0退出" << endl;
					cin >> option;
					if (option == 0)
						gameState = SCENE;
					else {
						if (option > 0 && option <= player->getBag()->getWeapons().size()) {
							Weapon* weapon = &weapons[player->getBag()->getWeapons()[option - 1]];
							showItemInfo(weapon);

							vector<string> bagMenuItems = { "使用", "丢弃", "返回" };
							generateMenuFromVector(bagMenuItems);
							string bagMenuItem = getSelectedMenuItem(bagMenuItems);

							if (bagMenuItem == "使用") {
								equipWeapon(player, weapon->getID());
							}

							if (bagMenuItem == "丢弃") {
								player->getBag()->getWeapons().erase(player->getBag()->getWeapons().begin() + option - 1);
							}
						}

					}
				}
			}

			if (itemTypeMenuItem == "返回") {
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
						cout << "已死亡" << endl;
						system("Pause");
					}
					else if (currentScene->getNPCs()[option - 1]->attr(HEALTH) < 0) {
						cout << "敌人被击败" << endl;
						getRandItemsForDefeatedEnemy(player, currentScene->getNPCs()[option - 1]);
						currentScene->getNPCs().erase(currentScene->getNPCs().begin() + option - 1);
						system("Pause");
					}
					gameState = SCENE;
				}
			}
			else {
				cout << "无攻击对象" << endl;
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
						cout << "该NPC不可交易" << endl;
						system("Pause");
					}
					else {
						trade(player, currentScene->getNPCs()[option - 1]);
					}
					gameState = SCENE;
				}
			}
			else {
				cout << "无交易对象对象" << endl;
				gameState = SCENE;
				system("Pause");
			}

		}break;

		default:
			break;
		}

	}

}
