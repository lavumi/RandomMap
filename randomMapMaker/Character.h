#pragma once


#define C_PLAYER 0
#define C_MINION 1
#define C_BOSS 2


#define RECOG_DIST 5



#define _ITEM_H_POTION 0
#define _ITEM_ESCAPE_ROPE 1


class Character {

public:
	
	int _get_maxHP() { return _maxHP; };
	int _get_curHP() { return _curHP; };
	int _getAtt() { return Att; };
	int _getposx() { return posx; };
	int _getposy() { return posy; };
	int _getLvl() { return Lvl; };
	int _getExp() { return _c_exp; };
	char* _getName() { return name; };
	int _getType() { return type; };
	bool _isAlive() { return isAlive; };


	Character();
	Character(int _type, char* _icon, int _HP, int _att);
	~Character();


	virtual void moveTo(int x, int y, Character** list);
	virtual void GetDamage(int damage, Character* target);
	void DrawChar();
	bool Crash(int x, int y, Character* list);
	void GetExp(int exp);
	void lvlup();
	




protected:
	int type;
	int _maxHP;
	int _curHP;
	int Att;
	int posx;
	int posy;
	int Lvl;
	int _c_exp;
	char* icon;
	char* name;
	bool isAlive = false;

	

	//RandomMap* map;

};


class Monster : public Character {
public :
	Monster();
	Monster(int _type, char* _icon, int _HP, int _att, int Lvl);

	void set_basepos(int pos) { base_pos = pos; };

	void Monster_lvlup();
	void MonsterPattern();
	void GetDamage(int damage, Character* target) override;
	void Spawn();
	bool _is_insight(int x, int y);
	void BossPattern();
private:
	int wander_dest = 999999;
	int dist_toP;
	int recog_dist = RECOG_DIST;
	bool chasing = false;
	int dead_turn;
	int sight_leng = 8;
	int sight[40]{ -1 };
	int base_pos;
	void setsight();

};

class Player : public Character {
public:

	Player(int _type, char* _icon, int _HP, int _att);
	void moveTo(int x, int y, Character** list) override;

private:
	int inventory[20];

};
