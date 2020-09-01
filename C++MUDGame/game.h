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
	Scene* currentScene;	// ��ǰ�ص�
	vector<Scene> scenes;	// ���еص�
	vector<Supply> supplies;	// ������Ʒ
	vector<Armor> armors; // ����װ��
	vector<Weapon> weapons; // ��������
	vector<NPC> NPCs;	// ����NPC
	vector<ItemCollectingTask> collectingTasks; // ��������
	Player* player;			// ���

public:
	void loadScene(string path); // ���س���
	void loadSupplyItems(string path); // ���ز���Ʒ
	void loadArmors(string path); // ����װ��
	void loadWeapons(string path); // ��������
	void loadNPCs(string path); // ����NPC
	void loadItemCollectingTasks(string path); // ����������Ʒ�ռ�����
	void saveProfile(string playerProfile, string sceneProfile, Player* player, Scene* currentScene); // �浵
	void loadProfile(string playerProfile, string sceneProfile, Player* player, Scene* currentScene); // ����
	void bindDoubleConnectedSences(Scene& scene_1, Scene& scene_2); // ˫������
	void bindSingleConnectedSences(Scene& depart, Scene& destination); // ��������
	void showSceneInformation(Scene* scene); // չʾ������Ϣ
	void showPlayerInformation(Player* player); // չʾ�����Ϣ
	void generateMenuFromVector(vector<string>& menuItems); // ���ɲ˵�
	string getSelectedMenuItem(vector<string>& menuItems); // ���ָ���Ĳ˵���
	void listNeighbors(Scene* currentScene); // ��ʾ�ٽ��ص�
	void showItemInfo(Item* item); // չʾ��Ʒ��ϸ��Ϣ
	void listSupplies(Bag* bag); // չʾ�����еĲ���Ʒ
	void useSupply(Player* player, short id); // ʹ�ò���Ʒ
	void listArmors(Bag* bag); // չʾ�����е�װ��
	void equipArmor(Player* player, short id); // ����װ��
	void refreshAttrAboutArmor(Player* player); // ���ź�ˢ������ֵ
	void listWeapons(Bag* bag); // չʾ�����е�����
	void equipWeapon(Player* player, short id); // װ������
	void bindNPCWToScene(Scene* scene, NPC* npc); // ��NPC������
	void bindAllNPCsToScene(); // ��������NPC���ͼ�İ�
	void showNPCInScene(Scene* scene); // ��ʾ�����е�NPC
	void fight(Player* player, NPC* enemy); // ���ж�ս
	float calculateRealDamage(Player* player, float defaultDamage); // ����ʵ���ܵ����˺�
	int round(Player* player, NPC* enemy); // ��һغ�
	void getRandItemsForDefeatedEnemy(Player* player, NPC* enemy); // ��õ�����Ʒ
	void giveItem(Player* player, short id, int prompt); // ������Ʒ
	void eraeItwm(Player* player, short id, int prompt); // �Ƴ���Ʒ
	void trade(Player* player, NPC* npc); // ����
	void bindTaskToNPC(Player* player, Task* task); // ������NPC
	void run(); // ��Ϸ�������

};

#endif // !_GAME_H
