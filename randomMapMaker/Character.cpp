
#include <iostream>
#include <Windows.h>
#include <string>
#include "Character.h"
#include "drawConsole.h"
#include "Game_Manager.h"
#include "Monster_AI.h"

using namespace std;


Character::Character()
{

}
Character::Character(int _type, char* _icon, int _HP, int _att)
{
	type = _type;
	icon = _icon;
	_maxHP = _HP;
	_curHP = _HP;
	Att = _att;
	posx = 1;
	posy = 1;
	Lvl = 1;
	_c_exp = Lvl * 5;
	if (type == C_BOSS)
		name = "����";
	else if (type == C_PLAYER)
		name = "�÷��̾�";
	
}
//�⺻ ���� ������
Monster::Monster() {

	
	type = C_MINION;

	switch (rand() % 8) {
	case 0:
		icon = "��";
		break;
	case 1:
		icon = "��";
		break;
	case 2:
		icon = "��";
		break;
	case 3:
		icon = "��";
		break;
	case 4:
		icon = "��";
		break;
	case 5:
		icon = "��";
		break;
	case 6:
		icon = "��";
		break;
	case 7:
		icon = "��";
		break;
	}
	_maxHP = 4;
	_curHP = 4;
	Att = 2;
	posx = 1;
	posy = 1;
	Lvl = 1;
	_c_exp = 5;
	name = "����";

	Game_Manager::GetUI()->add_chr(this);
}
//Ư�� ���� ������
Monster::Monster(int _type, char * _icon, int _HP, int _att, int _Lvl) :Character(_type, _icon, _HP, _att)
{
	Lvl = _Lvl;
	_c_exp = Lvl * 5;
	Game_Manager::GetUI()->add_chr(this);
}
//�÷��̾� ����
Player::Player(int _type, char * _icon, int _HP, int _att) : Character(_type, _icon, _HP, _att)
{

}


Character::~Character()
{

}







void Character::moveTo(int x, int y, Character** char_list)
{
	if (_curHP <= 0) return; //�׾������� �۵�x
	if (x == posx && y == posy) return; //���ڸ��� �ִ� ��쵵 �۵�x

	//���ӳ��� ��� ĳ���͵�� �浹üũ�� �Ѵ�
	for(int i = 0;i<30;i++){ /// ���͸�����!!!����
		if (char_list[i] == nullptr) break;
		if (Crash(x, y, char_list[i])) { //�浹�ϸ�
			char_list[i]->GetDamage(Att, this); //������ �ش�
			return;
		}
	}

	Game_Manager::GetUI()->_getMap()->_onepoint_redraw(posx, posy);
	posx = x;
	posy = y;
	DrawChar();  //���ο� �ڸ��� �׸�
}
void Player::moveTo(int x, int y, Character** char_list) {

	if (x + y * Game_Manager::GetUI()->_getMap()->_get_col() == Game_Manager::GetUI()->_getMap()->_get_exit_pos()) {
		Game_Manager::GetUI()->GAMECLEAR();
		return;
	}

	Character::moveTo(x, y, char_list);

}

void Character::GetDamage(int damage, Character* target) {
	_curHP -= damage;
	string txt;
	txt = target->_getName();
	txt += "�� ";
	txt += name;
	txt += "�� �����Ͽ�";

	Game_Manager::GetUI()->BttLog(txt);

	txt.clear();
	txt += std::to_string(damage);
	txt += "�������� ������";
	Game_Manager::GetUI()->BttLog(txt);

	if (_curHP <= 0) {
		string txt;
		txt = name;
		txt += "�� �׾���.";
		if (type == C_PLAYER)
			Game_Manager::GetUI()->GAMEOVER();

		Game_Manager::GetUI()->BttLog(txt);
		Game_Manager::GetUI()->_getMap()->_onepoint_redraw(posx, posy);
		target->GetExp(Lvl*5);
		_maxHP = 4;
		_curHP = 0;
		Att = 2;
		Lvl = 1;
		_c_exp = 5;
		posx = 999;  //�ָ��ָ� ���󰡶�
		posy = 999;
		
	}
}
void Monster::GetDamage(int damage, Character* target) {

	//���ͳ����� �浹�� ������x
	//��� wandering ������ ��� wander ��ǥ�� �ٲ�
	if (target->_getType() == C_MINION) {
		RandomMap* _map = Game_Manager::GetUI()->_getMap();
		int maze_row = (_map->_get_row() - 1) / 2;
		int maze_col = (_map->_get_col() - 1) / 2;
		wander_dest = rand() % (maze_row * maze_col);
		return;  
		
	}
	Character::GetDamage(damage, target);
	dead_turn = Game_Manager::GetUI()->_getTurn();



}



void Character::GetExp(int _exp) {
	_c_exp -= _exp;
	if (_c_exp <= 0) {
		lvlup();
	}
}
void Character::lvlup()
{
	Lvl++;
	Att++;
	_c_exp = Lvl*5;
	_maxHP += 2;
	_curHP = _maxHP;
	string txt;
	txt = name;
	txt += " ������!";
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	if (type != C_PLAYER)
		Game_Manager::GetUI()->SendMsg("���Ͱ� �������ϴ�...");
	else
		Game_Manager::GetUI()->BttLog(txt);
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
}

void Character::DrawChar()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	DrawStr(posx, posy, icon);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
}

bool Character::Crash(int x, int y, Character* target) {
	if (x == target->posx && y == target->posy)
		return true;
	else
		return false;
}

void Monster::Monster_lvlup()
{
	int turn = Game_Manager::GetUI()->_getTurn();

	if (turn % 20 == 0)
		GetExp(2);

}

void Monster::MonsterPattern()
{




	if (_curHP <= 0) { 
		if (dead_turn + 50 <= Game_Manager::GetUI()->_getTurn()) {
			Spawn();
		//	cout << "������ �Ϸ�!!!";
		}
		else
			return; 
	}
	Monster_lvlup();
	Character* _player = Game_Manager::GetUI()->_getPlayer();
	RandomMap* _map = Game_Manager::GetUI()->_getMap();

	dist_toP = abs(_player->_getposx() - posx) + abs(_player->_getposy() - posy);
	int dest;

	if (dist_toP <= recog_dist + Lvl * 10) {
		dest = _map->A_Star(posx, posy, _player->_getposx(), _player->_getposy());
		chasing = true;
	}

	else {
		int maze_row = (_map->_get_row() - 1) / 2;
		int maze_col = (_map->_get_col() - 1) / 2;
		if (wander_dest == 999999)
			wander_dest = rand() % (maze_row * maze_col);
		else if (posx == (wander_dest % maze_col) * 2 + 1 && posy == (wander_dest / maze_col) * 2 + 1) {
			wander_dest = rand() % (maze_row * maze_col);
		}
		int dest_x = (wander_dest % maze_col) * 2 + 1;
		int dest_y = (wander_dest / maze_col) * 2 + 1;
		dest = _map->A_Star(posx, posy, dest_x, dest_y);
		chasing = false;
	}


	moveTo(dest % _map->_get_col(), dest / _map->_get_col(), Game_Manager::GetUI()->get_all_char_list());

}


void Monster::Spawn() {
	int startpos;

	while (true) {
		startpos = Game_Manager::GetUI()->_getMap()->setRandomRoom();

		//���� ��ġ�� �ߺ������Ǵ°��� ����
		bool overlap = false;
		for (int i = 0; i < 30; i++) {
			if (Game_Manager::GetUI()->get_all_char_list()[i] == nullptr) break;
			int prt_pos = Game_Manager::GetUI()->get_all_char_list()[i]->_getposx() + Game_Manager::GetUI()->get_all_char_list()[i]->_getposy() * Game_Manager::GetUI()->_getMap()->_get_col();
			if (prt_pos == startpos)	overlap = true;
		}
		if (!overlap) break;
	}
	_curHP = _maxHP;
	moveTo(startpos % Game_Manager::GetUI()->_getMap()->_get_col(), startpos / Game_Manager::GetUI()->_getMap()->_get_col(), Game_Manager::GetUI()->get_all_char_list());

}




bool Monster::_is_insight(int x, int y) {

	int targetpos = x + y * Game_Manager::GetUI()->_getMap()->_get_col();
	int*  temp = sight;
	int i = 0;
	while (sight[i] != -1) {
		if (sight[i] == targetpos)
			return true;
		i++;
	}
	return false;
}

void Monster::setsight() {
	RandomMap* map = Game_Manager::GetUI()->_getMap();
	int mypos = posx + posy * map->_get_col();
	for (int i = 0; i < 40; i++) {
		sight[i] = -1;
	}

	int sight_cnt=0;
	for (int i = 0; i < sight_leng; i++) {
		if (map->_IsMovable(mypos + i)) {
			sight[sight_cnt] = mypos + i;
			sight_cnt++;
		}
		else
			break; 
	}
	for (int i = 0; i < sight_leng; i++) {
		if (map->_IsMovable(mypos - i)) {
			sight[sight_cnt] = mypos - i;
			sight_cnt++;
		}
		else
			break;
	}
	for (int i = 0; i < sight_leng; i++) {
		if (map->_IsMovable(mypos + map->_get_col())) {
			sight[sight_cnt] = mypos + map->_get_col()*i;
			sight_cnt++;
		}
		else
			break;
	}
	for (int i = 0; i < sight_leng; i++) {
		if (map->_IsMovable(mypos - map->_get_col())) {
			sight[sight_cnt] = mypos - map->_get_col()*i;
			sight_cnt++;
		}
		else
			break;
	}

	
}


void Monster::BossPattern() {
	if (type == C_BOSS) {
		setsight();
		int dest;
		Character* _player = Game_Manager::GetUI()->_getPlayer();
		RandomMap* _map = Game_Manager::GetUI()->_getMap();

		if (_is_insight(Game_Manager::GetUI()->_getPlayer()->_getposx(), Game_Manager::GetUI()->_getPlayer()->_getposy()))
			dest = _map->A_Star(posx, posy, _player->_getposx(), _player->_getposy());
		else
			dest = _map->A_Star(posx, posy, base_pos % _map->_get_col(), base_pos / _map->_get_col());

		moveTo(dest % _map->_get_col(), dest / _map->_get_col(), Game_Manager::GetUI()->get_all_char_list());

		return;
	}
}