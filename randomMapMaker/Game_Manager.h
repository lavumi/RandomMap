#pragma once
#include "MapMaker.h" 
class Game_Manager
{
public:
	
	static Game_Manager* GetUI();

	
	void init_UI(Character* player, RandomMap* map);
	void refresh_UI();
	void BttLog(string txt);
	Character* _getPlayer();
	RandomMap* _getMap();
	Character** get_all_char_list();
	void turns_go();
	int _getTurn();

	void SendMsg(string str);

	void GAMEOVER();
	void GAMECLEAR();
	bool _game_on() { return game_on; };
	void add_chr(Character* added_chr);
	
private:

	Game_Manager() {}
	static Game_Manager* _Game_Manager;
	~Game_Manager();
	void Draw_UI();
	bool game_on = true;


	Character** all_char;
	


	int _char_cnt = 0;

	string* battleLog;
	Character* player;
	RandomMap* map;

	int _logLine_cnt;
	int ui_start_x;
	int ui_end_y;
	int ui_x(int x);
	int ui_y(int y);

	int turn_cnt;


	bool once_in_turn = true;
};

