#include <iostream>
#include <fstream>
#include "game.h"

using namespace std;

// 构造函数
Game::Game() {

	loadScene("data/scenes.dat");
	loadSupplyItems("data/supplies.dat");
	loadArmors("data/armors.dat");
	loadWeapons("data/weapons.dat");
	loadNPCs("data/npcs.dat");
	loadItemCollectingTasks("data/itemcollectingtask.dat");

	bindAllNPCsToScene();

	player = new Player("Player", 100, 100, 100, 0, 50, 10, 100);

	currentScene = &scenes[0];
}

// 存档
void Game::saveProfile(string path, Player* player, Scene* currentScene) {
	fstream profile(path, ios::binary | ios::out);

	// 写入一系列属性
	for (int i = 0; i < 6; i++) {
		float attr = player->attr(i);
		profile.write((char*)&attr, sizeof(float));
	}
	int money = player->attr(MONEY);
	profile.write((char*)&money, sizeof(int));

	// 写入背包库存信息，各种类型先用int储存长度
	int supplyLen = player->getBag()->getSupplies().size();
	int armorLen = player->getBag()->getArmors().size();
	int weaponLen = player->getBag()->getWeapons().size();

	profile.write((char*)&supplyLen, sizeof(int));
	profile.write((char*)&armorLen, sizeof(int));
	profile.write((char*)&weaponLen, sizeof(int));

	for (auto ite = player->getBag()->getSupplies().begin(); ite != player->getBag()->getSupplies().end(); ite++) {
		profile.write((char*) & *ite, sizeof(short));
	}
	for (auto ite = player->getBag()->getArmors().begin(); ite != player->getBag()->getArmors().end(); ite++) {
		profile.write((char*) & *ite, sizeof(short));
	}
	for (auto ite = player->getBag()->getWeapons().begin(); ite != player->getBag()->getWeapons().end(); ite++) {
		profile.write((char*) & *ite, sizeof(short));
	}

	// 写入装备穿着信息
	vector<short> equipments;
	if (player->getBag()->weaponSlot.empty())
		equipments.emplace_back(-1);
	else
		equipments.emplace_back(player->getBag()->weaponSlot.top());

	if (player->getBag()->headArmorSlot.empty())
		equipments.emplace_back(-1);
	else
		equipments.emplace_back(player->getBag()->headArmorSlot.top());

	if (player->getBag()->bodyArmorSlot.empty())
		equipments.emplace_back(-1);
	else
		equipments.emplace_back(player->getBag()->bodyArmorSlot.top());

	if (player->getBag()->legArmorSlot.empty())
		equipments.emplace_back(-1);
	else
		equipments.emplace_back(player->getBag()->legArmorSlot.top());

	if (player->getBag()->footArmorSlot.empty())
		equipments.emplace_back(-1);
	else
		equipments.emplace_back(player->getBag()->footArmorSlot.top());

	for (auto ite = equipments.begin(); ite != equipments.end(); ite++) {
		profile.write((char*) & *ite, sizeof(short));
	}

	// 确定当前地图id
	int sceneId = currentScene->getId();
	profile.write((char*)&sceneId, sizeof(int));

	// 写入任务状态
	int tasksNum = collectingTasks.size();
	profile.write((char*)&tasksNum, sizeof(int));

	for (auto ite = collectingTasks.begin(); ite != collectingTasks.end(); ite++) {
		bool state = ite->getState();
		int taskState = ite->getTaskState();
		profile.write((char*)&state, sizeof(bool));
		profile.write((char*)&taskState, sizeof(int));
	}

	// 写入各场景NPC信息
	int sceneNum = scenes.size();
	profile.write((char*)&sceneNum, sizeof(int));
	for (auto ite = scenes.begin(); ite != scenes.end(); ite++) {
		int NPCNum = ite->getNPCs().size();
		profile.write((char*)&NPCNum, sizeof(int));

		if (NPCNum != 0) {
			for (auto npc = ite->getNPCs().begin(); npc != ite->getNPCs().end(); npc++) {
				NPC* NPCPt = *npc;
				int NPCId = NPCPt->getId();
				profile.write((char*)&NPCId, sizeof(int));
			}
		}

	}

	// 写入各NPC背包信息
	int NPCNum = NPCs.size();
	profile.write((char*)&NPCNum, sizeof(int));
	for (auto ite = NPCs.begin(); ite != NPCs.end(); ite++) {
		for (int i = 0; i < 3; i++) {
			int itemNum = ite->getBagWithItemTpye(i).size();
			profile.write((char*)&itemNum, sizeof(int));
			for (auto item = ite->getBagWithItemTpye(i).begin(); item != ite->getBagWithItemTpye(i).end(); item++) {
				short id = *item;
				profile.write((char*)&id, sizeof(short));
			}
		}
		
	}

	profile.close();
}

// 读档
void Game::loadProfile(string path, Player* player, Scene** currentScene) {
	fstream profile(path, ios::binary | ios::in);

	// 读入一系列属性
	profile.seekg(ios::beg);
	for (int i = 0; i < 6; i++) {
		float attr;
		profile.read((char*)&attr, sizeof(float));
		player->attr(i, attr);
	}
	int money;
	profile.read((char*)&money, sizeof(int));
	player->attr(MONEY, money);

	// 读入一系列物品
	int lens[3];
	profile.read((char*)lens, 3 * sizeof(int));

	player->getBag()->getSupplies().clear();
	for (int i = 0; i < lens[0]; i++) {
		short id;
		profile.read((char*)&id, sizeof(short));
		player->getBag()->getSupplies().emplace_back(id);
	}

	player->getBag()->getArmors().clear();
	for (int i = 0; i < lens[1]; i++) {
		short id;
		profile.read((char*)&id, sizeof(short));
		player->getBag()->getArmors().emplace_back(id);
	}

	player->getBag()->getWeapons().clear();
	for (int i = 0; i < lens[2]; i++) {
		short id;
		profile.read((char*)&id, sizeof(short));
		player->getBag()->getWeapons().emplace_back(id);
	}

	// 读入已装备的物品
	short equipments[5];
	profile.read((char*)equipments, 5 * sizeof(short));

	if (equipments[0] != -1) {
		equipWeapon(player, equipments[0], false);
	}
	for (int i = 1; i < 5; i++) {
		if (equipments[i] != -1) {
			equipArmor(player, equipments[i], false);
		}
	}

	// 读入当前地图
	int sceneId;
	profile.read((char*)&sceneId, sizeof(int));
	*currentScene = &scenes[sceneId];

	// 读入任务状态
	int tasksNum;
	profile.read((char*)&tasksNum, sizeof(int));
	for (int i = 0; i < tasksNum; i++) {
		bool state;
		int taskState;
		profile.read((char*)&state, sizeof(bool));
		profile.read((char*)&taskState, sizeof(int));
		collectingTasks[i].setState(state);
		collectingTasks[i].setTaskState(taskState);
	}

	// 读入各场景NPC信息
	int sceneNum;
	profile.read((char*)&sceneNum, sizeof(int));
	for (int i = 0; i < sceneNum; i++) {
		int NPCNum;
		profile.read((char*)&NPCNum, sizeof(int));
		scenes[i].getNPCs().clear();
		if (NPCNum != 0) {
			for (int j = 0; j < NPCNum; j++) {
				int NPCId;
				profile.read((char*)&NPCId, sizeof(int));
				scenes[i].getNPCs().emplace_back(&NPCs[NPCId]);
			}
		}
	}

	// 读入NPC背包信息
	int NPCNum;
	profile.read((char*)&NPCNum, sizeof(int));
	for (int i = 0; i < NPCNum; i++) {
		for (int j = 0; j < 3; j++) {
			NPCs[i].getBagWithItemTpye(j).clear();
			int itemNum;
			profile.read((char*)&itemNum, sizeof(int));
			for (int k = 0; k < itemNum; k++) {
				int id;
				profile.read((char*)&id, sizeof(short));
				NPCs[i].getBagWithItemTpye(j).emplace_back(id);
			}
		}
	}

	profile.close();
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
	changeColor(0x0B);
	cout << scene->getName() << endl;
	cout << scene->getDescription() << endl;

	changeColor(0x0A);
	cout << "[附近] ";
	changeColor(0x06);

	vector<Scene*>& neighbors = scene->getNeighbors();
	for (int i = 0; i < neighbors.size(); i++)
		cout << neighbors[i]->getName() << " ";	
	cout << endl;

	changeColor(0x0F);
	generateDecorations("*", 20);
}

// 展示玩家信息
void Game::showPlayerInformation(Player* player) {
	generateDecorations("=", 20);
	changeColor(0x02);
	cout << "[名称]\t" << player->getName() << endl;
	cout << "[血量]\t" << player->attr(HEALTH) << endl;
	cout << "[防御]\t" << player->attr(DEFENCE) << endl;
	cout << "[敏捷]\t" << player->attr(SENSITIVE) << endl;
	cout << "[伤害]\t" << player->attr(DAMAGE) << endl;
	cout << "[金钱]\t" << (int)player->attr(MONEY) << endl;
	changeColor(0x0F);
	generateDecorations("-", 20);

	changeColor(0x0D);
	if (!player->getBag()->weaponSlot.empty())
		cout << "[武器]\t" << weapons[player->getBag()->weaponSlot.top()].getName() << endl;
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
	changeColor(0x0F);
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

// 获得输入
int Game::getInputNumber(int limit) {
	string input;
	cin >> input;
	if (isDigital(input) && stoi(input) <= limit) {
		return stoi(input);
	}
	return 0;
}

// 判断是否为数字
bool Game::isDigital(string input) {
	for (int i = 0; i < input.length(); i++) {
		if (input[i] < '0' || input[0] > '9')
			return false;
	}
	return true;
}

// 获得指定的菜单项
string Game::getSelectedMenuItem(vector<string>& menuItems) {

	generateMenuFromVector(menuItems);

	int option = getInputNumber(menuItems.size());

	if (option != 0) {
		return menuItems[option - 1];
	}

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
void Game::equipArmor(Player* player, short id, bool toggle) {

	switch (armors[id].getPosition())
	{
	case HEAD: {
		if (!player->getBag()->headArmorSlot.empty()) {
			if (player->getBag()->headArmorSlot.top() == id && toggle) {
				player->getBag()->headArmorSlot.pop();
				break;
			}
			else
				player->getBag()->headArmorSlot.pop();
		}
		player->getBag()->headArmorSlot.push(id);
	}break;

	case BODY: {
		if (!player->getBag()->bodyArmorSlot.empty()) {
			if (player->getBag()->bodyArmorSlot.top() == id && toggle) {
				player->getBag()->bodyArmorSlot.pop();
				break;
			}
			else
				player->getBag()->bodyArmorSlot.pop();
		}
		player->getBag()->bodyArmorSlot.push(id);
	}break;

	case LEG: {
		if (!player->getBag()->legArmorSlot.empty()) {
			if (player->getBag()->legArmorSlot.top() == id && toggle) {
				player->getBag()->legArmorSlot.pop();
				break;
			}
			else
				player->getBag()->legArmorSlot.pop();
		}
		player->getBag()->legArmorSlot.push(id);
	}break;

	case FOOT: {
		if (!player->getBag()->footArmorSlot.empty()) {
			if (player->getBag()->footArmorSlot.top() == id & toggle) {
				player->getBag()->footArmorSlot.pop();
				break;
			}
			else
				player->getBag()->footArmorSlot.pop();
		}
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
void Game::equipWeapon(Player* player, short id, bool toggle) {
	if (!player->getBag()->weaponSlot.empty()) {
		if (player->getBag()->weaponSlot.top() == id && toggle) {
			player->getBag()->weaponSlot.pop();
			player->alter(DAMAGE, -weapons[id].getDamage());
			return;
		}
		else {
			player->getBag()->weaponSlot.pop();
		}
	}
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

// 改变字体颜色
void Game::changeColor(WORD word) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), word);
}

// 显示场景中的NPC
void Game::showNPCInScene(Scene* scene) {
	changeColor(0x0E);
	if (!scene->getNPCs().empty()) {
		for (auto ite = scene->getNPCs().begin(); ite != scene->getNPCs().end(); ite++) {
			NPC* npc = *ite;
			cout << npc->getName() << endl;
		}
	}
	changeColor(0x0F);
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
	vector<string> fightMenuItems = { "攻击", "逃跑" };
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
void Game::eraseItem(Player* player, short id, int prompt) {
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

// 交易时展示物品信息
int Game::showItemInfoForTrading(Player* npc, int prompt, bool isSold) {
	int order = 1;
	if (npc->getBagWithItemTpye(prompt).empty()) return 0;
	switch (prompt)
	{
	case SUPPLY: {
		for (auto ite = npc->getBag()->getSupplies().begin(); ite != npc->getBag()->getSupplies().end(); ite++) {
			cout << order++ << " " << supplies[*ite].getName() << '\t';
			if (!isSold) cout << supplies[*ite].getValue() << endl;
			else cout << supplies[*ite].getSoldValue() << endl;
		}
		return npc->getBag()->getSupplies().size();
	}break;

	case ARMOR: {
		for (auto ite = npc->getBag()->getArmors().begin(); ite != npc->getBag()->getArmors().end(); ite++) {
			cout << order++ << " " << armors[*ite].getName() << '\t';
			if (!isSold) cout << armors[*ite].getValue() << endl;
			else cout << armors[*ite].getSoldValue() << endl;
		}
		return npc->getBag()->getArmors().size();
	}break;

	case WEAPON: {
		for (auto ite = npc->getBag()->getWeapons().begin(); ite != npc->getBag()->getWeapons().end(); ite++) {
			cout << order++ << " " << weapons[*ite].getName() << '\t';
			if (!isSold) cout << weapons[*ite].getValue() << endl;
			else cout << weapons[*ite].getSoldValue() << endl;
		}
		return npc->getBag()->getWeapons().size();
	}break;

	default:
		break;
	}
}

// 获得物品价格
int Game::getPrice(short id, int prompt, bool isSold) {
	switch (prompt)
	{
	case SUPPLY:
		if (isSold) return supplies[id].getSoldValue();
		else return supplies[id].getValue();
	case ARMOR:
		if (isSold) return armors[id].getSoldValue();
		else return armors[id].getSoldValue();
	case WEAPON:
		if (isSold) return weapons[id].getSoldValue();
		else return weapons[id].getValue();
	default:
		break;
	}
}

// 获得某一库存
auto Game::getSource(int prompt) {
	
}

// 交易
void Game::trade(Player* player, NPC* npc) {
	system("cls");
	cout << "1.购买" << '\t' << "2.出售\t" << "3.返回\t" << endl;
	char option;
	cin >> option;
	if (option == '1') {

		system("cls");
		cout << "1.补给\t" << "2.装备\t" << "3.武器\t" << endl;
		int option = getInputNumber(3);

		if (option != 0) {
			system("cls");
			int size = showItemInfoForTrading(npc, option - 1, false);

			cout << "输入序号购买，输入0离开" << endl;
			int targetGood = getInputNumber(size);

			if (targetGood != 0) {
				int price = getPrice(npc->getBagWithItemTpye(option - 1)[targetGood - 1], option - 1, false);
				if (player->attr(MONEY) > price) {
					giveItem(player, npc->getBagWithItemTpye(option - 1)[targetGood - 1], option - 1);
					eraseItem(npc, npc->getBagWithItemTpye(option - 1)[targetGood - 1], option - 1);
					player->alter(MONEY, -price);
					npc->alter(MONEY, price);
					cout << "花费" << price << endl;
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
		system("cls");
		cout << "1.补给\t" << "2.装备\t" << "3.武器\t" << endl;
		int option = getInputNumber(3);

		if (option != 0) {
			system("cls");
			int size = showItemInfoForTrading(player, option - 1, true);

			cout << "输入序号购买，输入0离开" << endl;
			int targetGood = getInputNumber(size);

			if (targetGood != 0) {
				int price = getPrice(player->getBagWithItemTpye(option - 1)[targetGood - 1], option - 1, true);
				if (npc->attr(MONEY) > price) {
					giveItem(npc, player->getBagWithItemTpye(option - 1)[targetGood - 1], option - 1);
					eraseItem(player, player->getBagWithItemTpye(option - 1)[targetGood - 1], option - 1);
					npc->alter(MONEY, -price);
					player->alter(MONEY, price);
					cout << "获得金钱" << price << endl;
					system("Pause");
				}
				else {
					cout << npc->getName() << "的金钱不足" << endl;
					system("Pause");
				}
			}

		}
	}

}

// 随机获得一句对话
string Game::getRandomDialogue(NPC* npc) {
	return npc->getDialogues()[rand() % npc->getDialogues().size()];
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
			string selected = getSelectedMenuItem(titleMenuItems);

			if (selected == "开始游戏") {

				// 进行新游戏，创建角色
				player = new Player("Player", 100, 100, 100, 0, 50, 10, 100);

				// 确定当前地点
				currentScene = &scenes[0];

				gameState = SCENE;

			}

			if (selected == "读取进度") {
				loadProfile("profile/profile.sav", player, &currentScene);
				cout << "读取成功" << endl;
				gameState = SCENE;
				system("Pause");
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

			// 处理输入
			string selected = getSelectedMenuItem(sceneMenuItems);

			// 移动
			if (selected == "移动") {
				system("cls");
				listNeighbors(currentScene);
				int option = getInputNumber(currentScene->getNeighbors().size());

				if (option != 0) {
					currentScene = currentScene->getNeighbors()[option - 1];
				}
			}

			// 背包
			if (selected == "背包") {
				gameState = BAG;
			}

			// 攻击
			if (selected == "交互") {
				char option = '0';
				while (option != '4') {
					system("cls");
					cout << "1.交易\t" << "2.攻击\t" << "3.交谈\t" << "4.返回" << endl;
					cin >> option;

					if (option == '1') {
						gameState = TRADE;
						break;
					}
					if (option == '2') {
						gameState = FIGHT;
						break;
					}
					if (option == '3') {
						gameState = DIALOGUE;
						break;
					}
					
				}
			}

			// 系统
			if (selected == "系统") {
				system("cls");

				vector<string> systemMenuItems = { "回到游戏", "返回标题", "保存进度" };
				string option = getSelectedMenuItem(systemMenuItems);

				if (option == "返回标题")
					gameState = TITLE;

				if (option == "保存进度") {
					saveProfile("profile/profile.sav", player, currentScene);
					cout << "保存成功" << endl;
					system("Pause");
				}
			}

		}break;

		case BAG: {
			system("cls");
			vector<string> itemTypeMenuItems = { "补给", "装备", "武器", "返回" };
			string itemTypeMenuItem = getSelectedMenuItem(itemTypeMenuItems);

			if (itemTypeMenuItem == "补给") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listSupplies(player->getBag());
					cout << "输入序号查看，输入0退出" << endl;
					option = getInputNumber(player->getBag()->getSupplies().size());
					if (option == 0)
						gameState = SCENE;
					else {
						Supply* supplyItem = &supplies[player->getBag()->getSupplies()[option - 1]];
						showItemInfo(supplyItem);

						vector<string> bagMenuItems = { "使用", "丢弃", "返回" };
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

			if (itemTypeMenuItem == "装备") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listArmors(player->getBag());
					cout << "输入序号查看，输入0退出" << endl;
					option = getInputNumber(player->getBag()->getArmors().size());
					if (option == 0)
						gameState = SCENE;
					else {
						Armor* armor = &armors[player->getBag()->getArmors()[option - 1]];
						showItemInfo(armor);

						vector<string> bagMenuItems = { "使用", "丢弃", "返回" };
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

			if (itemTypeMenuItem == "武器") {
				short option = -1;
				while (option != 0) {
					system("cls");
					listWeapons(player->getBag());
					cout << "输入序号查看，输入0退出" << endl;
					option = getInputNumber(player->getBag()->getWeapons().size());
					if (option == 0)
						gameState = SCENE;
					else {
						Weapon* weapon = &weapons[player->getBag()->getWeapons()[option - 1]];
						showItemInfo(weapon);

						vector<string> bagMenuItems = { "使用", "丢弃", "返回" };
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
				cout << ++order << ".返回" << endl;
				int option;
				option = getInputNumber(currentScene->getNPCs().size() + 1);
				if (option != 0 && option != order) {
					fight(player, currentScene->getNPCs()[option - 1]);
					if (player->attr(HEALTH) < 0) {
						cout << "已死亡" << endl;
						system("Pause");
						gameState = TITLE;
					}
					else if (currentScene->getNPCs()[option - 1]->attr(HEALTH) < 0) {
						cout << "敌人被击败" << endl;
						getRandItemsForDefeatedEnemy(player, currentScene->getNPCs()[option - 1]);
						currentScene->getNPCs().erase(currentScene->getNPCs().begin() + option - 1);
						system("Pause");
						gameState = SCENE;
					}
				}

				else if (option == order) {
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
				cout << ++order << ".返回" <<endl;
				int option = getInputNumber(currentScene->getNPCs().size() + 1);
				if (option != 0 && option != order) {
					if (currentScene->getNPCs()[option - 1]->getCareer() != 0) {
						cout << "该NPC不可交易" << endl;
						system("Pause");
					}
					else {
						trade(player, currentScene->getNPCs()[option - 1]);
					}
					gameState = SCENE;
				}

				else if (option == order) {
					gameState = SCENE;
				}
			}
			else {
				cout << "无交易对象对象" << endl;
				gameState = SCENE;
				system("Pause");
			}

		}break;

		case DIALOGUE: {
			system("cls");
			if (!currentScene->getNPCs().empty()) {
				int order = 0;
				for (auto ite = currentScene->getNPCs().begin(); ite != currentScene->getNPCs().end(); ite++) {
					NPC npc = **ite;
					cout << ++order << "." << npc.getName() << '\t';
				}
				cout << endl;
				int option = getInputNumber(currentScene->getNPCs().size());
				if (option != 0) {
					NPC* npc = currentScene->getNPCs()[option - 1];
					if (npc->getTask().empty()) {
						cout << getRandomDialogue(npc) << endl;
						gameState = SCENE;
						system("Pause");
					}
					else {
						Task* task = nullptr;

						for (auto ite = npc->getTask().begin(); ite != npc->getTask().end(); ite++) {
							task = *ite;
							if (task->getState() == true)
								break;
						}

						if(task->getState() == true) {
							int state = task->checkProgress(player);
							if (state == UNCOMPLISHED) {
								cout << task->getDialogueWithProgress(UNCOMPLISHED) << endl;
								gameState = SCENE;
								system("Pause");
							}
							if (state == COMPLISHED) {

								cout << task->getDialogueWithProgress(COMPLISHED) << endl;
								giveItem(player, task->getRewardItemId(), task->getRewardItemType());
								switch (task->getRewardItemType())
								{
								case SUPPLY: {
									cout << "获得了" << supplies[task->getRewardItemId()].getName() << endl;
								}break;

								case ARMOR: {
									cout << "获得了" << armors[task->getRewardItemId()].getName() << endl;
								}break;

								case WEAPON: {
									cout << "获得了" << weapons[task->getRewardItemId()].getName() << endl;
								}break;

								default:
									break;
								}

								eraseItem(player, task->getTargetItemId(), task->getTargetItemType());
								cout << "移交了任务物品" << endl;

								task->setState(false);
								task->setTaskState(COMPLISHED);
								gameState = SCENE;
								system("Pause");
							}
						}

						else {

							for (auto ite = npc->getTask().begin(); ite != npc->getTask().end(); ite++) {
								task = *ite;
								if (task->getTaskState() == DISMISS)
									break;
							}

							if (task->getTaskState() == DISMISS) {
								cout << task->getDialogueWithProgress(DISMISS) << endl;
								cout << "1.接受\t" << "2.拒绝\t" << endl;
								char option;
								cin >> option;
								if (option == '1') {
									task->setState(true);
									cout << endl << "已接受任务" << endl;
									gameState = SCENE;
									system("Pause");
								}
								else {
									gameState = SCENE;
									system("Pause");
								}

							}

							else {
								cout << getRandomDialogue(npc) << endl;
								gameState = SCENE;
								system("Pause");
							}

						}

					}
				}
			}
			else {
				cout << "无交谈对象" << endl;
				gameState = SCENE;
				system("Pause");
			}
		}

		default:
			break;
		}

	}

}
