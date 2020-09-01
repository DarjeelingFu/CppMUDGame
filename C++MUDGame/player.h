#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>
#include <map>
#include "bag.h"

enum TaskProgress {
	DISMISS,
	UNCOMPLISHED,
	COMPLISHED
};

using namespace std;

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
	}

public:
	int getNPC() { return npc; }
	string getDialogueWithProgress(int progress) { return dialogues[progress]; }
	bool getState() { return state; }

	int getRewardItemType() { return rewardItemType; }
	int getRewardItemId() { return rewardItemId; }

	int getTargetItemType() { return targetItemType; }
	int getTargetItemId() { return targetItemId; }

	virtual int checkProgress(Player* player) = 0;

private:
	int npc;
	bool state;

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
		int career
	) :
		Player(name, health, maxHealth, strength, defence, sensitive, damage, money),
		sceneID(sceneID), itemList(itemList), career(career)
	{
		// task = NULL;
	}

	NPC(const NPC& npc) :
		Player(npc), sceneID(npc.sceneID), itemList(npc.itemList), career(npc.career)
	{
		// task = npc.task;
	}

public:
	vector<string>& getDialogues() { return dialogues; }
	int getSceneID() { return sceneID; }
	string getItemList() { return itemList; }
	int getCareer() { return career; }
	vector<Task*>& getTask() { return tasks; }

private:
	vector<string> dialogues;
	int sceneID;
	string itemList;
	int career;
	vector<Task*> tasks;

};

#endif // !_PLAYER_H
