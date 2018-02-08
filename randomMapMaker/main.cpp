#include <ctime>
#include <Windows.h>
#include <conio.h>
#include "drawConsole.h"
#include "MapMaker.h"
#include "Character.h"
#include "Monster_AI.h"
#include "Game_Manager.h"


using namespace std;

int main()
{
	system("mode con:cols=200 lines=70");
	system("cls");


	//bool testmode = true;

	srand((int)time(NULL));




	//testintigwaegwg
	//1. 맵을 만든다
	int test_row = 51, test_col = 77;
//	int test_row = 31, test_col = 47;
	RandomMap textmap(test_row, test_col);
//	textmap._set_MapTYPE(MT_ROOM);
	//textmap._set_MapTYPE(MT_FOREST);
	textmap.makeRNDmap(MT_FOREST);

	int maptype = 0;
	while (1) {
		printf("1 : FOREST         2 : ROOM         3 : MAZE      -->");
		maptype = _getch();
		if(maptype == 49)
			textmap.makeRNDmap(MT_FOREST);
		else if (maptype == 50)
			textmap.makeRNDmap(MT_ROOM);
		else 
			textmap.makeRNDmap(MT_MAZE);



	}

	
	return 0;
	
	//2. 플레이어 캐릭터 만든다
	Player _player(C_PLAYER, "＠", 10, 2);
	
	//3. UI 초기화 시킨다
	Game_Manager::GetUI()->init_UI(&_player, &textmap);
	
//	if (!testmode) {
		//여기까지의 순서가 틀리면 곤란해집니다

		//플레이어 시작지점 설정
		int startpos = textmap.setRandomRoom();
		_player.moveTo(startpos % test_col, startpos / test_col, Game_Manager::GetUI()->get_all_char_list());





		//몬스터 생성 시작
		Monster _Monster[10];

		for (int i = 0; i < 10; i++) {  ///몬스터마리수!!!수정
			_Monster[i].Spawn();
		}


		//보스몹 생성
		Monster _boss(C_BOSS, "♠", 30, 6, 5);
		//Game_Manager::GetUI()->add_char(&_boss);
		int bosspos = Game_Manager::GetUI()->_getMap()->_get_exit_pos();
		_boss.moveTo(bosspos % test_col, bosspos / test_col, Game_Manager::GetUI()->get_all_char_list());
		_boss.MonsterPattern();
		_boss.set_basepos(_boss._getposx() + _boss._getposy() * Game_Manager::GetUI()->_getMap()->_get_col());


	//}

	//시작전에 UI 갱신 한번
	Game_Manager::GetUI()->refresh_UI();

	while (Game_Manager::GetUI()->_game_on()) {

		//입력 부분
		int input_;
		input_ = _getch();//버퍼 한번 거르고 입력받기
		if (input_ == 224) {
			input_ = _getch();
		}
		switch (input_)
		{
		case UP:
			if (textmap._IsMovable(_player._getposx() + (_player._getposy()-1)*test_col)) {
				_player.moveTo(_player._getposx(), _player._getposy()-1, Game_Manager::GetUI()->get_all_char_list());
			}
			break;
		case DOWN:
			if (textmap._IsMovable(_player._getposx() + (_player._getposy() + 1)*test_col)) {
				_player.moveTo(_player._getposx(), _player._getposy() + 1, Game_Manager::GetUI()->get_all_char_list());
			}
			break;
		case LEFT:
			if (textmap._IsMovable(_player._getposx() -1 + (_player._getposy() )*test_col)) {
				_player.moveTo(_player._getposx()-1, _player._getposy(), Game_Manager::GetUI()->get_all_char_list());
			}
			break;
		case RIGHT:
			if (textmap._IsMovable(_player._getposx() +1 + (_player._getposy() )*test_col)) {
				_player.moveTo(_player._getposx() +1 , _player._getposy(), Game_Manager::GetUI()->get_all_char_list());
			}
			break;
		case 'q':
			Game_Manager::GetUI()->GAMEOVER();
			break;
		default:
			break;

		}

		Game_Manager::GetUI()->turns_go();
	//	if (testmode) {
			//몬스터의 AI 실행
			for (int i = 0; i < 10; i++) { ///몬스터마리수!!!수정
			_Monster[i].MonsterPattern();

			}


			//보스 AI 실행

		_boss.BossPattern();
//		}
		//UI 업데이트
		Game_Manager::GetUI()->refresh_UI();
	}

	system("pause");
	system("pause");

	return 0;
}
