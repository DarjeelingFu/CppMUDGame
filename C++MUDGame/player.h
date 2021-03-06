#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>
#include <map>
#include "bag.h"

using namespace std;

enum TaskProgress {
	DISMISS,
	UNCOMPLISHED,
	COMPLISHED
};

enum PlayerAttr {
	HEALTH,
	MAXHEALTH,
	STRENGTH,
	DEFENCE,
	SENSITIVE,
	DAMAGE,
	MONEY
};

enum NPCCareer {
	MERCHANT,
	FIGHTER,
};

class Player {
public:
	Player(
		string name,
		float health,
		float maxHealth,
		float strength,
		float defence,
		float sensitive,
		float damage,
		int money
	) :
		name(name), health(health), maxHealth(maxHealth),
		strength(strength), defence(defence), sensitive(sensitive),
		damage(damage), money(money) {}
	
	Player(const Player& player) :
		name(player.name), health(player.health), maxHealth(player.maxHealth),
		strength(player.strength), defence(player.defence), sensitive(player.sensitive),
		damage(player.damage), money(player.money) {}

	~Player() {}

public:
	string getName() { return name; }
	Bag* getBag() { return &bag; }
	float attr(int prompt, float setValue = -1); // 获取属性
	void alter(int prompt, float value); // 用变化量改变属性
	vector<short>& getBagWithItemTpye(int prompt);

private:
	string name;		// 名称
	float health;		// HP
	float maxHealth;	// 最大HP
	float strength;		// 体力
	float defence;		// 防御
	float sensitive;	// 灵敏
	float damage;		// 伤害
	int money;			// 金钱
	string skill;       //技能
	Bag bag;			// 背包
};

class Task {
public:
	Task(
		int npc, int rewardItemType, int rewardItemId, 
		int targetItemType, int targetItemId, string dismiss, string uncomplished,
		string complished
	) :
		npc(npc), rewardItemType(rewardItemType), rewardItemId(rewardItemId),
		targetItemType(targetItemType), targetItemId(targetItemId) {
		state = false;
		dialogues[DISMISS] = dismiss;
		dialogues[UNCOMPLISHED] = uncomplished;
		dialogues[COMPLISHED] = complished;
		taskState = DISMISS;
	}

public:
	int getNPC() { return npc; }
	string getDialogueWithProgress(int progress) { return dialogues[progress]; }
	bool getState() { return state; }
	void setState(bool state) { this->state = state; }

	int getRewardItemType() { return rewardItemType; }
	int getRewardItemId() { return rewardItemId; }

	int getTargetItemType() { return targetItemType; }
	int getTargetItemId() { return targetItemId; }

	virtual int checkProgress(Player* player) = 0;
	int getTaskState() { return taskState; }
	void setTaskState(int taskState) { this->taskState = taskState; }

private:
	int npc;
	bool state; // 是否处于任务状态
	int taskState;

	map<int, string> dialogues;
	int rewardItemType;
	int rewardItemId;

	int targetItemType;
	short targetItemId;

};

class ItemCollectingTask : public Task {
public:
	ItemCollectingTask(
		int npc, int rewardItemType, int rewardItemId, int targetItemType, int targetItemId,
		string dismiss, string uncomplished, string complished
	) :
		Task(
			npc, rewardItemType, rewardItemId, targetItemType, targetItemId,
			dismiss, uncomplished, complished
		) {}

public:

private:
	virtual int checkProgress(Player* player);
};

class NPC : public Player {
public:
	NPC(
		int id,
		string name,
		float health,
		float maxHealth,
		float strength,
		float defence,
		float sensitive,
		float damage,
		int money,
		int sceneID,
		string itemList,
		string armorList,
		string weaponList,
		int career
		// string dialogue
	) :
		Player(name, health, maxHealth, strength, defence, sensitive, damage, money),
		sceneID(sceneID), itemList(itemList), career(career), id(id), armorList(armorList),
		weaponList(weaponList)
	{
		
	}

	NPC(const NPC& npc) :
		Player(npc), sceneID(npc.sceneID), itemList(npc.itemList), career(npc.career), id(npc.id),
		armorList(npc.armorList), weaponList(npc.weaponList)
	{
		for (auto ite = npc.dialogues.begin(); ite != npc.dialogues.end(); ite++) {
			this->dialogues.emplace_back(*ite);
		}
	}

public:
	vector<string>& getDialogues() { return dialogues; }
	int getSceneID() { return sceneID; }
	string getItemList() { return itemList; }
	string getArmorList() { return armorList; }
	string getWeaponList() { return weaponList; }
	int getCareer() { return career; }
	vector<Task*>& getTask() { return tasks; }
	int getId() { return id; }
	// string getDialogue() { return dialogue; }

private:
	int id;
	vector<string> dialogues;
	int sceneID;
	string itemList;
	string armorList;
	string weaponList;
	int career;
	vector<Task*> tasks;
	// string dialogue;

};

#endif // !_PLAYER_H
