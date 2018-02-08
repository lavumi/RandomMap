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
	//1. ���� �����
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
	
	//2. �÷��̾� ĳ���� �����
	Player _player(C_PLAYER, "��", 10, 2);
	
	//3. UI �ʱ�ȭ ��Ų��
	Game_Manager::GetUI()->init_UI(&_player, &textmap);
	
//	if (!testmode) {
		//��������� ������ Ʋ���� ��������ϴ�

		//�÷��̾� �������� ����
		int startpos = textmap.setRandomRoom();
		_player.moveTo(startpos % test_col, startpos / test_col, Game_Manager::GetUI()->get_all_char_list());





		//���� ���� ����
		Monster _Monster[10];

		for (int i = 0; i < 10; i++) {  ///���͸�����!!!����
			_Monster[i].Spawn();
		}


		//������ ����
		Monster _boss(C_BOSS, "��", 30, 6, 5);
		//Game_Manager::GetUI()->add_char(&_boss);
		int bosspos = Game_Manager::GetUI()->_getMap()->_get_exit_pos();
		_boss.moveTo(bosspos % test_col, bosspos / test_col, Game_Manager::GetUI()->get_all_char_list());
		_boss.MonsterPattern();
		_boss.set_basepos(_boss._getposx() + _boss._getposy() * Game_Manager::GetUI()->_getMap()->_get_col());


	//}

	//�������� UI ���� �ѹ�
	Game_Manager::GetUI()->refresh_UI();

	while (Game_Manager::GetUI()->_game_on()) {

		//�Է� �κ�
		int input_;
		input_ = _getch();//���� �ѹ� �Ÿ��� �Է¹ޱ�
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
			//������ AI ����
			for (int i = 0; i < 10; i++) { ///���͸�����!!!����
			_Monster[i].MonsterPattern();

			}


			//���� AI ����

		_boss.BossPattern();
//		}
		//UI ������Ʈ
		Game_Manager::GetUI()->refresh_UI();
	}

	system("pause");
	system("pause");

	return 0;
}
