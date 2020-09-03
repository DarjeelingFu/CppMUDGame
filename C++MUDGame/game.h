#pragma once
#ifndef _GAME_H
#define _GAME_H

#include "player.h"
#include "scene.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>

enum GameState {
	TITLE,
	SCENE,
	BAG,
	FIGHT,
	TRADE,
	DIALOGUE
};

class Game {
public:
	Game();
	~Game() {}

private:
	Scene* currentScene;	// 当前地点
	vector<Scene> scenes;	// 所有地点
	vector<Supply> supplies;	// 所有物品
	vector<Armor> armors; // 所有装备
	vector<Weapon> weapons; // 所有武器
	vector<NPC> NPCs;	// 所有NPC
	vector<ItemCollectingTask> collectingTasks; // 所有任务
	Player* player;			// 玩家

public:
	void loadScene(string path); // 加载场景
	void loadSupplyItems(string path); // 加载补给品
	void loadArmors(string path); // 加载装备
	void loadWeapons(string path); // 加载武器
	void loadNPCs(string path); // 加载NPC
	void loadItemCollectingTasks(string path); // 加载所有物品收集任务
	void saveProfile(string profile, Player* player, Scene* currentScene); // 存档
	void loadProfile(string path, Player* player, Scene* currentScene); // 读档
	void bindDoubleConnectedSences(Scene& scene_1, Scene& scene_2); // 双向连接
	void bindSingleConnectedSences(Scene& depart, Scene& destination); // 单向连接
	void showSceneInformation(Scene* scene); // 展示场景信息
	void showPlayerInformation(Player* player); // 展示玩家信息
	void generateMenuFromVector(vector<string>& menuItems); // 生成菜单
	string getSelectedMenuItem(vector<string>& menuItems); // 获得指定的菜单项
	void listNeighbors(Scene* currentScene); // 显示临近地点
	void showItemInfo(Item* item); // 展示物品详细信息
	void listSupplies(Bag* bag); // 展示背包中的补给品
	void useSupply(Player* player, short id); // 使用补给品
	void listArmors(Bag* bag); // 展示背包中的装备
	void equipArmor(Player* player, short id); // 穿着装备
	void refreshAttrAboutArmor(Player* player); // 穿着后刷新属性值
	void listWeapons(Bag* bag); // 展示背包中的武器
	void equipWeapon(Player* player, short id); // 装备武器
	void bindNPCWToScene(Scene* scene, NPC* npc); // 绑定NPC到场景
	void bindAllNPCsToScene(); // 处理所有NPC与地图的绑定
	void showNPCInScene(Scene* scene); // 显示场景中的NPC
	void fight(Player* player, NPC* enemy); // 进行对战
	float calculateRealDamage(Player* player, float defaultDamage); // 计算实际受到的伤害
	int round(Player* player, NPC* enemy); // 玩家回合
	void getRandItemsForDefeatedEnemy(Player* player, NPC* enemy); // 获得掉落物品
	void giveItem(Player* player, short id, int prompt); // 给予物品
	void eraeItem(Player* player, short id, int prompt); // 移除物品
	void trade(Player* player, NPC* npc); // 交易
	string getRandomDialogue(NPC* npc); // 随机获得一句对话
	int getInputNumber(int limit); // 获得输入
	bool isDigital(string input); // 判断是否为数字
	void run(); // 游戏运行入口

};

#endif // !_GAME_H
