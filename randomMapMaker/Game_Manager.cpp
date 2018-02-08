#include "drawConsole.h"
#include "Character.h"
#include "MapMaker.h"
#include "Game_Manager.h"
#include <iostream>
#include <string>



Game_Manager* Game_Manager::_Game_Manager = nullptr;




Game_Manager::~Game_Manager()
{
	delete[] battleLog;
	free(all_char);
}

Game_Manager* Game_Manager::GetUI()
{
	if (_Game_Manager == nullptr) {
		_Game_Manager = new Game_Manager();
	}
	return _Game_Manager;
}


int Game_Manager::ui_x(int x) { return ui_start_x + x; }
int Game_Manager::ui_y(int y) { return 1 + y; }
Character* Game_Manager::_getPlayer() { return player; }
RandomMap * Game_Manager::_getMap() { return map; }
Character** Game_Manager::get_all_char_list() { return all_char; }

//초기화 함수. 플레이어와 맵이 반드시 먼저 선언되어있어야한다
void Game_Manager::init_UI(Character* _player, RandomMap* _map)
{

	player = _player;
	map = _map;
	turn_cnt = 0;

	


	//게임상의 캐릭터 리스트 초기화
	all_char = new Character*[30]{ nullptr };


	//_char_cnt = 0;
	//all_char = new Character*[_char_cnt + 1];
	//all_char = (Character**)malloc(4*(_char_cnt + 1));

	

	//플레이어 리스트에 추가
	add_chr(_player);



	//배틀로그 관련 초기화
	_logLine_cnt = 0;


	
	Draw_UI();
}

void Game_Manager::Draw_UI() {
	//UI 그리기
	//ui의 시작, 끝점을 미리 지정해둠
	ui_start_x = map->_get_col() + 1;
	ui_end_y = map->_get_row() - 2;

	for (int i = 0; i < 17; i++) {
		DrawStr(map->_get_col() + i, 0, "▤");
		DrawStr(map->_get_col() + i, map->_get_row() - 1, "▤");
		DrawStr(map->_get_col() + i, 9, "▤");
		DrawStr(map->_get_col() + i, 11, "〓");
	}
	for (int i = 0; i<map->_get_row() - 1; i++)
		DrawStr(map->_get_col() + 16, i, "▤");



	DrawStr(ui_x(2), ui_y(1), "플레이어");
	DrawStr(ui_x(0), ui_y(2), "체력　　　："); // 5*2 바이트
	DrawStr(ui_x(0), ui_y(3), "레벨　　　：");
	DrawStr(ui_x(0), ui_y(4), "공격력　　：");
	DrawStr(ui_x(0), ui_y(5), "필요경험치：");
	DrawStr(ui_x(0), ui_y(6), "턴수      ：");

	DrawStr(ui_x(4), ui_y(10), "전투로그");

	DrawStr(ui_x(7), ui_y(2), player->_get_curHP());
	DrawStr(ui_x(9), ui_y(2), "/");
	DrawStr(ui_x(10), ui_y(2), player->_get_maxHP());
	DrawStr(ui_x(7), ui_y(3), player->_getLvl());
	DrawStr(ui_x(7), ui_y(4), player->_getAtt());
	DrawStr(ui_x(7), ui_y(5), player->_getExp());
	DrawStr(ui_x(7), ui_y(6), turn_cnt);
	battleLog = new string[ui_end_y - 12 - 1];
}



//각종 데이터 갱신
void Game_Manager::refresh_UI()
{
	DrawStr(ui_x(7), ui_y(2), player->_get_curHP());
	DrawStr(ui_x(10), ui_y(2), player->_get_maxHP());
	DrawStr(ui_x(7), ui_y(3), player->_getLvl());
	DrawStr(ui_x(7), ui_y(4), player->_getAtt());
	DrawStr(ui_x(7), ui_y(5), player->_getExp());
	DrawStr(ui_x(7), ui_y(6), turn_cnt);

	MoveCurser(ui_x(0), ui_y(12));
}

//배틀로그 갱신

///30바이트가 넘는 경우 줄바꾸기를 구현하고 싶다
//string 나누는 법?


void Game_Manager::add_chr(Character* added_chr)
{
	
	all_char[_char_cnt] = added_chr;
	_char_cnt++;

	//if (_char_cnt != 0) {
	//	Character** temp;


	//	//	temp = (Character**)malloc(4 * (_char_cnt + 2));
	//	temp = new Character*[_char_cnt];



	//	for (int i = 0; i < _char_cnt; i++) {
	//		temp[i] = all_char[i];
	//	}

	//	

	//	//free(all_char);
	//	//all_char = temp;

	//
	//	delete[] all_char;
	//	all_char = new Character*[_char_cnt + 2];

	//	for (int i = 0; i < _char_cnt; i++) {
	//		all_char[i] = temp[i];
	//	}
	//	delete[] temp;
	//}
	//

	//all_char[_char_cnt] = added_chr;
	//all_char[_char_cnt + 1] = nullptr;

	//_char_cnt++;
	
	return ;
}


void Game_Manager::BttLog(string txt)
{
	if (_logLine_cnt == ui_end_y - 12 - 1) {
		for (int i = 0; i < ui_end_y - 12 - 2; i++) {
			battleLog[i] = battleLog[i + 1];
			MoveCurser(ui_x(0), ui_y(12) + i);
			std::cout << "                              ";
			MoveCurser(ui_x(0), ui_y(12) + i);
			std::cout << battleLog[i] << endl;
		}
		_logLine_cnt--;
	}
	MoveCurser(ui_x(0), ui_y(12) + _logLine_cnt);
	std::cout << "                              ";
	MoveCurser(ui_x(0), ui_y(12) + _logLine_cnt);
	std::cout << txt << endl;
	battleLog[_logLine_cnt] = txt;

	if (_logLine_cnt <= ui_end_y - 12 - 2)
		_logLine_cnt++;
}

void Game_Manager::SendMsg(string str) {
	if (once_in_turn) {
		once_in_turn = false;
		BttLog(str);
	}
}

void Game_Manager::GAMEOVER()
{
	for (int i = 0; i <= 20; i++) {
		DrawStr(map->_get_col() / 2 - 10 + i, map->_get_row() / 2 - 5, "◆");
		DrawStr(map->_get_col() / 2 - 10 + i, map->_get_row() / 2 + 5, "◆");
	}
	for (int i = 0; i <= 10; i++) {
		DrawStr(map->_get_col() / 2 - 10, map->_get_row() / 2 - 5 + i, "◆");
		DrawStr(map->_get_col() / 2 + 10, map->_get_row() / 2 - 5 + i, "◆");
	}
	for(int i = 0;i<19;i++)
		for (int j = 0; j < 9; j++) {
			DrawStr(map->_get_col() / 2 - 9 + i, map->_get_row() / 2 - 4 + j, "　");
		}
	DrawStr(map->_get_col() / 2 - 4, map->_get_row() / 2, "ＧＡＭＥ  ＯＶＥＲ");
	game_on = false;

}

void Game_Manager::GAMECLEAR()
{
	for (int i = 0; i <= 20; i++) {
		DrawStr(map->_get_col() / 2 - 10 + i, map->_get_row() / 2 - 5, "◆");
		DrawStr(map->_get_col() / 2 - 10 + i, map->_get_row() / 2 + 5, "◆");
	}
	for (int i = 0; i <= 10; i++) {
		DrawStr(map->_get_col() / 2 - 10, map->_get_row() / 2 - 5 + i, "◆");
		DrawStr(map->_get_col() / 2 + 10, map->_get_row() / 2 - 5 + i, "◆");
	}
	for (int i = 0; i<19; i++)
		for (int j = 0; j < 9; j++) {
			DrawStr(map->_get_col() / 2 - 9 + i, map->_get_row() / 2 - 4 + j, "　");
		}
	DrawStr(map->_get_col() / 2 - 4, map->_get_row() / 2, "CONGRATULATION");
	game_on = false;
}



void Game_Manager::turns_go()
{
	turn_cnt++;
	once_in_turn = true;
}

int Game_Manager::_getTurn(){	return turn_cnt;	}



