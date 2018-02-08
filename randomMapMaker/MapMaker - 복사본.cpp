// randommapgen.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//



#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "drawConsole.h"
#include "MapMaker.h"


using namespace std;



int RandomMap::randompick() {

	if (rand() % 100 > fill_prob)
		return TILE_WALL;
	else
		return TILE_WATER;
}
RandomMap::RandomMap(int input_row, int input_col) {
	row = input_row;
	col = input_col;
	region = new int[col*row];
	maptype = 0;
	initMap();
}
RandomMap::~RandomMap() {
	delete[]region;
}
void RandomMap::initMap() {
	for (int i = 0; i < col*row; i++) {
		region[i] = TILE_WALL;
	}

}
void RandomMap::_set_MapTYPE(int type) {
	maptype = type;
}
void RandomMap::print_map()
{
	{
		for (int i = 0; i < row; i++) {
			for (int j = 0; j <col; j++) {
				if (region[i * col + j] == TILE_WALL) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					DrawStr(j, i, "▩");
				}
				else if (region[i * col + j] == TILE_WATER) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					DrawStr(j, i, "∬");
				}
				else if (region[i * col + j] == TILE_BRIDGE) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					DrawStr(j, i, "▥");
				}
				else if (region[i * col + j] == TILE_BASE) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					DrawStr(j, i, "　");
				}
				else if (region[i * col + j] == TILE_EXIT) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
					DrawStr(j, i, "▣");
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
					DrawStr(j, i, "　");
					//cout << region[i*col + j];
				}
			}
			cout << "\n";
		}
	}
}

void RandomMap::_onepoint_redraw(int x, int y) { 
	if (x > col || y > row) return;


	if (region[y * col + x] == TILE_WALL) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		DrawStr(x, y, "▩");
	}
	else if (region[y * col + x] == TILE_WATER) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		DrawStr(x, y, "∬");
	}
	else if (region[y * col + x] == TILE_BRIDGE) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		DrawStr(x, y, "▥");
	}
	else if (region[y * col + x] == TILE_BASE) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		DrawStr(x, y, "　");
	}
	else if (region[y * col + x] == TILE_EXIT) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		DrawStr(x, y, "▣");
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		DrawStr(x, y, "　");
		//cout << region[i*col + j];
	}

}

void RandomMap::emptymap()
{
	_set_MapTYPE(MT_EMPTY);
	for (int i = 0; i < col-2; i++) {
		for (int j = 0; j < row - 2; j++) {
			region[i + 1 + (j + 1)*col] = TILE_BASE;
		}
	}



}

int RandomMap::_get_col(){	return col;}
int RandomMap::_get_row(){	return row;}

void RandomMap::set_exit() {
	if (maptype == MT_MAZE) {
	//	cout << "DDDDDDDDDDDDDDDDDD";
		exit_pos = col / 2 - 1 + (row / 2) * col;
		exit_pos = find_Deadend(exit_pos);
	}
	
	else if (maptype == MT_FOREST) {}
	else if (maptype == MT_ROOM) {}
	else if (maptype == MT_EMPTY) {
		exit_pos = col / 2 - 1 + (row / 2) * col;
	}



	region[exit_pos] = TILE_EXIT;

}


void RandomMap::makeRNDmap()
{
	switch (maptype) {
	case MT_MAZE:
		initMap();
		Make_Maze();
		set_exit();
		break;
	case MT_FOREST:
		initMap();
		if(rand() %2) Making_lake(4, 3, 100);
		Make_PointIsland_N_Way(9);
		break;
	case MT_ROOM:
		initMap();
		make_BSP();
		break;
	case MT_EMPTY:
		initMap();
		emptymap();
		set_exit();
		break;
	}
	print_map();
}


//호수 만들기
//cellular automata를 사용
//min_size 이하의 크기를 가진 호수는 채워버린다
void RandomMap::Making_lake(int size, int rpt_cnt, int min_size) {
	fill_prob = 42 + size;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (i == 0 || j == 0 || i == row - 1 || j == col - 1)
				region[i*col + j] = TILE_WALL;
			else
				region[i*col + j] = randompick();
		}
	}
	while (rpt_cnt > 0) {
		for (int i = 1; i < row - 1; i++) {
			for (int j = 1; j < col - 1; j++) {
				if (region[i*col + j] == TILE_WALL) {
					if (AroundWallCount(i, j, 1) <4) {
						region[i*col + j] = TILE_WATER;
					}
				}
				else {
					if (AroundWallCount(i, j, 1) >= 5) {
						region[i*col + j] = TILE_WALL;
					}
				}
			}
		}
		rpt_cnt--;
	}

	Fill_smallarea(min_size);


	for (int i = 0; i < col*row; i++) {

		if (region[i] == TILE_WATER) {
			if (region[i + 1] == TILE_WALL)				region[i + 1] = TILE_BASE;
			if (region[i - 1] == TILE_WALL)				region[i - 1] = TILE_BASE;
			if (region[i + col] == TILE_WALL)				region[i + col] = TILE_BASE;
			if (region[i - col] == TILE_WALL)				region[i - col] = TILE_BASE;
			if (region[i + 1 + col] == TILE_WALL)				region[i + 1 + col] = TILE_BASE;
			if (region[i - 1 - col] == TILE_WALL)				region[i - 1 - col] = TILE_BASE;
			if (region[i + col - 1] == TILE_WALL)				region[i + col - 1] = TILE_BASE;
			if (region[i - col + 1] == TILE_WALL)				region[i - col + 1] = TILE_BASE;
		}
	}
}

//1. 일정 거리를 벌리고 있는 점들을 랜덤하게 찍는다.
//2. 그 점들을 연결하는 선을 그린다.
//3. 그 점들 주위에 랜덤하게 구역을 판다.
//4. 그 점들 연결하는 선을 따라 길을 판다.
void RandomMap::Make_PointIsland_N_Way(int numberof_point) {




	//현재 맵 데이터 백업
	//숲은 호수를 먼저 만든 뒤에 구역을 파기에 만든 호수 데이터를 백업
	int* region_backup = new int[col*row];
	for (int i = 0; i < col*row; i++) {
		region_backup[i] = region[i];
	}



	/////////////////////////////
	///포인트 설정 시작//////////
	/////////////////////////////

	MapPoint* Map_point = new MapPoint[numberof_point];

	int repeat_cnt = rand() % 3 + 3;
	//초기 포인트 렌덤 설정
	for (int i = 0; i < numberof_point; i++) {
		Map_point[i].index = rand() % (col*row);
		Map_point[i].connected_cnt = 0;
		Map_point[i].connected_point = nullptr;
	}




	//포인트를 기준으로 각각의 보로노이 다이어그램 배치
	int closest_point = 0;
	int temp_dis = 9999999;
	for (int i = 0; i < col*row; i++) {
		for (int j = 0; j < numberof_point; j++) {
			if (distance(Map_point[j].index, i) < temp_dis) {
				closest_point = j;
				temp_dis = distance(Map_point[j].index, i);
			}
		}
		voronoi_region[closest_point].push(i);
		temp_dis = 99999999;
	}


	//포인트를 보로노이 그래프의 중심에 위치후 다시 보로노이 그래프 생성 반복
	while (repeat_cnt > 0) {
		//cout << repeat_cnt << "\n";
		for (int i = 0; i < numberof_point; i++) {
			Map_point[i].index = voronoi_center(i);
		}

		for (int i = 0; i < col*row; i++) {
			for (int j = 0; j < numberof_point; j++) {
				if (distance(Map_point[j].index, i) < temp_dis) {
					closest_point = j;
					temp_dis = distance(Map_point[j].index, i);
				}
			}
			voronoi_region[closest_point].push(i);
			temp_dis = 99999999;
		}
		repeat_cnt--;
	}


	//포인트 정리후 지역 초기화
	for (int i = 0; i < col*row; i++) {
		region[i] = region_backup[i];
	}

	delete[] region_backup;


	/////////////////////
	///포인터 설정 끝////
	/////////////////////





	///////////////////////////////////////////////////
	///포인터 사이를 잇는 라인 그리기 시작/////////////
	///////////////////////////////////////////////////

	//포인트 사이의 거리 측정 및 적당한 거리에 라인 그리기


	for (int i = 0; i <numberof_point; i++) {
		queue<int> point_N_dist;
		for (int j = 0; j < numberof_point; j++) {
			if (distance(Map_point[i].index, Map_point[j].index) < LineDistance && i != j) {
				Map_point[i].connected_cnt++;
				point_N_dist.push(j);
				point_N_dist.push(distance(Map_point[i].index, Map_point[j].index));
			}
		}

		Map_point[i].connected_point = new _point_dist[Map_point[i].connected_cnt];

		int k = 0;
		while (!point_N_dist.empty()) {
			Map_point[i].connected_point[k].point_number = point_N_dist.front();
			point_N_dist.pop();
			Map_point[i].connected_point[k].distance = point_N_dist.front();
			point_N_dist.pop();
			k++;
		}
	}


	int rand_num = rand() % numberof_point;


	int rst_group[9] = { 99 }; //방문 완료한 그룹
	int rst_group2[9] = { 99 }; //1번 그룹 각각의 포인트에 연결하는 그룹
	rst_group[0] = rand_num;
	bool visited[9] = { false };
	visited[rand_num] = true;




	///존나 헬인 부분
	//만들고도 너무 복잡하다
	//변수 이름도 병신, 포인트 쓰는법도 병신, 구조체 설계도 병신


	for (int j = 0; j < 8; j++) {			//전체 루프
		int closest_dist = 999999;
		int closest_point = 99;
		for (int k = 0; k < j + 1; k++) {
			for (int i = 0; i < Map_point[rst_group[k]].connected_cnt; i++) {					//  여기서 현재 방문한 지점들(rst_group[0~k])중 가장 가까운 연결 포인트(Map_point[rst_group[k]].connected_point)를 찾는다
				if (!visited[Map_point[rst_group[k]].connected_point[i].point_number] &&			// 방문한 적이 없고
					Map_point[rst_group[k]].connected_point[i].distance < closest_dist) {		//	거리가 지금 찾은것중에 가장 짧으면		
					closest_dist = Map_point[rst_group[k]].connected_point[i].distance;
					closest_point = Map_point[rst_group[k]].connected_point[i].point_number;	//번호를 등록한다
					rst_group2[j + 1] = rst_group[k];
				}
			}
		}
		rst_group[j + 1] = closest_point;
		visited[closest_point] = true;

	}

	/// 여기까지 뭔가 개선할수 있으면 좋겠다



	//라인 그리기
	for (int i = 1; i < 9; i++) {
		draw_line(Map_point[rst_group[i]].index, Map_point[rst_group2[i]].index);
	}




	////모든 라인 그리기
	//for (int i = 0; i < numberof_point; i++) {
	//	for (int j = 0; j < Map_point[i].connected_cnt; j++) {
	//		int target_point_num = Map_point[i].connected_point[j].point_number;

	//		draw_line(Map_point[target_point_num].index, Map_point[i].index);
	//	}
	//}


	//포인트 주변으로 원형의 공간 만들기
	for (int i = 0; i < numberof_point; i++) {

		//좌우 사이즈
		int point_size_l = rand() % 3 + 5;
		int point_size_r = rand() % 3 + 5;
		int point_size_d = rand() % 3 + 5;
		int point_size_u = rand() % 3 + 5;

		//좌측 원형부분
		for (int j = 0; j < point_size_l; j++) {

			//랜덤한 범위내에 구멍 파기
			for (int k = 0; k < point_size_u; k++) {
				region[Map_point[i].index - col*k - j] = 0;
			}
			for (int k = 0; k < point_size_d; k++) {
				region[Map_point[i].index + col*k - j] = 0;
			}

			if (point_size_u > 3)
				point_size_u = point_size_u - rand() % 2;
			else
				point_size_u = point_size_u - rand() % 3;

			if (point_size_d > 3)
				point_size_d = point_size_d - rand() % 2;
			else
				point_size_d = point_size_d - rand() % 3;
		}

		//왼쪽 그린후 오른쪽 다시 그리기 위해 초기화
		point_size_d = rand() % 3 + 4;
		point_size_u = rand() % 3 + 4;

		for (int j = 0; j < point_size_r; j++) {
			for (int k = 0; k < point_size_u; k++) {
				region[Map_point[i].index - col*k + j] = 0;
			}
			for (int k = 0; k < point_size_d; k++) {
				region[Map_point[i].index + col*k + j] = 0;
			}

			if (point_size_u > 2)
				point_size_u = point_size_u - rand() % 2;
			else
				point_size_u = point_size_u - rand() % 3;

			if (point_size_d > 2)
				point_size_d = point_size_d - rand() % 2;
			else
				point_size_d = point_size_d - rand() % 3;
		}



	}

	for (int i = 0; i < numberof_point; i++) {
		delete[] Map_point[i].connected_point;
	}

	delete[] Map_point;




	///////////////////////////////////////////////////
	///포인터 사이를 잇는 라인 그리기 끝///////////////
	///////////////////////////////////////////////////
}

//빈 공간 만들기 호수 만들기와 같다
void RandomMap::Making_cave(int size, int rpt_cnt, int min_size) {
	fill_prob = 50 + size;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (i == 0 || j == 0 || i == row - 1 || j == col - 1)
				region[i*col + j] = TILE_WALL;
			else {
				region[i*col + j] = randompick();
				if (region[i*col + j] == TILE_WATER) region[i*col + j] = TILE_BASE;
			}
		}
	}
	int rpt_cnt_temp = rpt_cnt;
	while (rpt_cnt > 0) {
		for (int i = 1; i < row - 1; i++) {
			for (int j = 1; j < col - 1; j++) {

				if (AroundWallCount(i, j, 1) >= 5 || AroundWallCount(i, j, 2) <= 2) {
					region[i*col + j] = TILE_WALL;
				}
				else {

					region[i * col + j] = TILE_BASE;
				}
			}



		}
		rpt_cnt--;
	}
	rpt_cnt = rpt_cnt_temp - 1;
	while (rpt_cnt > 0) {
		for (int i = 1; i < row - 1; i++) {
			for (int j = 1; j < col - 1; j++) {
				if (region[i*col + j] == TILE_WALL) {
					if (AroundWallCount(i, j, 1) <4) {
						region[i*col + j] = TILE_BASE;
					}
				}
				else {
					if (AroundWallCount(i, j, 1) >= 5) {
						region[i*col + j] = TILE_WALL;
					}
				}


			}
		}
		rpt_cnt--;
	}




}




///Making Lake 관련 함수들

int RandomMap::AroundWallCount(int _row, int _col, int radius) {
	int ans = 0;
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			if (i != 0 || j != 0)
				if (region[(_row + i)*col + _col + j] == TILE_WALL) {
					ans++;
				}
		}
	}

	return ans;
	/*return	region[(_row - 1)*col + _col - 1] + region[(_row)*col + _col - 1] + region[(_row + 1)*col + _col - 1] +
	region[(_row - 1)*col + _col] + region[(_row + 1)*col + _col] +
	region[(_row - 1)*col + _col + 1] + region[(_row)*col + _col + 1] + region[(_row + 1)*col + _col + 1];*/

}
int RandomMap::check_size(int i, int j, queue<int>* storage) {

	if (region[i*col + j] == TILE_WATER) {
		region[i*col + j] = 99;
		storage->push(i*col + j);

		if (region[(i - 1)*col + j - 1] == TILE_WATER) {
			check_size(i - 1, j - 1, storage);
		}
		if (region[(i - 1)*col + j] == TILE_WATER) {
			check_size(i - 1, j, storage);
		}
		if (region[(i - 1)*col + j + 1] == TILE_WATER) {
			check_size(i - 1, j + 1, storage);
		}
		if (region[(i)*col + j - 1] == TILE_WATER) {
			check_size(i, j - 1, storage);
		}
		if (region[(i)*col + j + 1] == TILE_WATER) {
			check_size(i, j + 1, storage);
		}
		if (region[(i + 1)*col + j - 1] == TILE_WATER) {
			check_size(i + 1, j - 1, storage);
		}
		if (region[(i + 1)*col + j] == TILE_WATER) {
			check_size(i + 1, j, storage);
		}
		if (region[(i + 1)*col + j + 1] == TILE_WATER) {
			check_size(i + 1, j + 1, storage);
		}
		checksize_result++;
	}

	return checksize_result;
}
void RandomMap::Fill_smallarea(int min_size) {

	queue<int> *area_temp = new queue<int>;
	int a;
	for (int i = 1; i < row - 1; i++) {
		for (int j = 1; j < col - 1; j++) {
			if (region[i*col + j] != TILE_WALL) {
				a = check_size(i, j, area_temp);
				if (a < min_size) {
					while (!area_temp->empty()) {
						region[area_temp->front()] = TILE_WALL;
						area_temp->pop();
					}
				}
				else {
					while (!area_temp->empty()) {
						area_temp->pop();
					}
				}
				checksize_result = 0;
			}
		}
	}
	delete area_temp;

	for (int i = 0; i < col*row; i++) {
		if (region[i] == 99) {
			region[i] = TILE_WATER;
		}
	}
}



///Make point N Way 관련 함수들

void RandomMap::draw_line(int pnt_index1, int pnt_index2) {


	int y = pnt_index2 / col - pnt_index1 / col;
	int x = (float)(pnt_index2 % col - pnt_index1 % col);
	if (abs(x) >= abs(y)) {
		for (int k = 0; k < abs(x); k++) {
			int mapindex_temp = pnt_index1
				+ k * x / abs(x)
				+ col * (int)((float)y / (float)abs(x) * k);

			if (region[mapindex_temp] != TILE_WATER && region[mapindex_temp] != TILE_BRIDGE) {
				region[mapindex_temp] = TILE_BASE;
				region[mapindex_temp + col] = TILE_BASE;
			}
		}
	}
	else {
		for (int k = 0; k < abs(y); k++) {
			int mapindex_temp = pnt_index1
				+ (int)((float)x / (float)abs(y) * k)
				+ col * k * y / abs(y);
			if (region[mapindex_temp] != TILE_WATER && region[mapindex_temp] != TILE_BRIDGE) {
				region[mapindex_temp] = TILE_BASE;
				region[mapindex_temp + 1] = TILE_BASE;
			}

		}
	}
}
int RandomMap::distance(int point_index_1, int point_index_2) {
	int point_1_x = point_index_1 / col;
	int point_1_y = point_index_1 % col;
	int point_2_x = point_index_2 / col;
	int point_2_y = point_index_2 % col;

	return (point_1_x - point_2_x)* (point_1_x - point_2_x) + (point_1_y - point_2_y) *(point_1_y - point_2_y);
}
int RandomMap::voronoi_center(int idx) {
	int count = 0;
	int total_x = 0, total_y = 0;
	if (voronoi_region[idx].empty()) {
		//	return 99999;
		return rand() % col*row;
	}
	while (!voronoi_region[idx].empty()) {
		total_x += voronoi_region[idx].front() / col;
		total_y += voronoi_region[idx].front() % col;
		count++;
		voronoi_region[idx].pop();
	}

	int ans = total_x / count * col + total_y / count;
	return ans;
}




///미로 만드는 함수1 (Kruskal algorithm)
typedef struct _AstarNode {
	int idx;
	int move;
	int dist;
	bool openlist = false;
	bool closelist = false;
	struct _AstarNode* parent;
}Astarnode;
int room_group(Room room) {

	if (room._parent_room == nullptr)
		return room.number;


	Room* temp;
	temp = &room;

	while (temp->_parent_room != nullptr) {
		temp = temp->_parent_room;

	}
	int ans = temp->number;

	return ans;
}
void RandomMap::Make_Maze() {
	initMap();


	int maze_row = (row - 1) / 2;
	int maze_col = (col - 1) / 2;

	Room* maze = new Room[maze_col*maze_row]; //모든 방을 동적 할당
	for (int i = 0; i < maze_col*maze_row; i++) {
		maze[i].number = i;
		maze[i]._parent_room = nullptr;


		int y = i / maze_col;
		int x = i % maze_col;

		region[(y * 2 + 1) * col + 1 + x * 2] = TILE_BASE;

	}








	//각 방을 구분짓는 모든 벽을 동적 할당 ( 0~ (maze_col * (maze_row -1) -1 ) 까지 가로벽, 그 뒤로 세로벽) 


	//ex) maze_col = 8, row = 7					가로벽								세로벽
	//            _|_|_|_|_|_|_|_		 0,  1,  2,  3,  4,  5,  6,  7				48, 49, 50, 51, 52, 53, 54,
	//            _|_|_|_|_|_|_|_		 8,  9, 10, 11, 12, 13, 14, 15				55,
	//            _|_|_|_|_|_|_|_		16, 17, 18, 19, 20, 21, 22, 23				62,
	//            _|_|_|_|_|_|_|_		24,
	//            _|_|_|_|_|_|_|_		32
	//            _|_|_|_|_|_|_|_		40, 41, 42, 43, 44, 45, 46, 47
	//			   | | | | | | |

	int wall_cnt = maze_col * (maze_row - 1) + (maze_col - 1) * maze_row;


	int* wall = new int[wall_cnt];

	for (int i = 0; i < wall_cnt; i++) {
		wall[i] = i;
	}




	//wall 배열의 내용물을 Random _ shuffle

	for (int i = 0; i < wall_cnt; i++) {
		int dest = rand() % wall_cnt;
		int temp = wall[i];
		wall[i] = wall[dest];
		wall[dest] = temp;
	}




	for (int i = 0; i < wall_cnt; i++) {

		//if (i > 139)			cout << i << " " << wall[i];
		if (wall[i] < maze_col * (maze_row - 1)) {												// 가로벽인 경우
			if (room_group(maze[wall[i]]) != room_group(maze[wall[i] + maze_col])) {			//가로벽 위의 방과 아래 방의 그룹이 다르면
				int big_head = max(room_group(maze[wall[i] + maze_col]), room_group(maze[wall[i]]));
				int small_head = min(room_group(maze[wall[i] + maze_col]), room_group(maze[wall[i]]));

				maze[big_head]._parent_room = &maze[small_head];							//낮은 번호의 방을 높은 번호의 방의 그룹에 넣는다. ( 부모로 만든다)

				int x = wall[i] / maze_col;
				int y = wall[i] % maze_col;
				region[(x * 2 + 1 + 1) * col + 1 + y * 2] = TILE_BASE;						//그 벽은 지운다

				x = i / maze_col;
				y = i % maze_col;

			}
		}
		else {
			int wall_idx = wall[i] - maze_col * (maze_row - 1);

			if (room_group(maze[wall_idx + wall_idx / (maze_col - 1) + 1]) != room_group(maze[wall_idx + wall_idx / (maze_col - 1)])) {			//세로벽 좌우방의 번호를 비교해서 다르면

				int big_head = max(room_group(maze[wall_idx + wall_idx / (maze_col - 1) + 1]), room_group(maze[wall_idx + wall_idx / (maze_col - 1)]));
				int small_head = min(room_group(maze[wall_idx + wall_idx / (maze_col - 1) + 1]), room_group(maze[wall_idx + wall_idx / (maze_col - 1)]));

				maze[big_head]._parent_room = &maze[small_head];					//낮은 번호의 방을 높은 번호의 방의 그룹에 넣는다. ( 부모로 만든다)


				int x = wall_idx / (maze_col - 1);
				int y = wall_idx % (maze_col - 1);

				//cout << x << " " << y << " " << maze_col << " "<< wall_idx <<"\n";
				region[(x * 2 + 1) * col + 1 + y * 2 + 1] = TILE_BASE;					//그 벽은 지운다




			}
		}

	}


	delete[] wall;
	delete[] maze;

}
int RandomMap::setRandomRoom()
{
	int maze_row = (row - 1) / 2; // 10
	int maze_col = (col - 1) / 2; // 18

	int randRoom = rand() % (2 * (maze_col + maze_row)) + 1; //28*2 = 56


	if (randRoom <= maze_col) 
		return col + randRoom * 2 - 1;
	else if (randRoom <= maze_col * 2) 
		return col * (row - 2) + (randRoom - maze_col) * 2 - 1;
	else if (randRoom <= maze_col * 2 + maze_row) 
		return col * ((randRoom - maze_col * 2) * 2 - 1) + 1;
	else
		return col * ((randRoom - maze_col * 2 - maze_row) * 2 - 1)  + col -2;


}
int RandomMap::find_Deadend(int startpos) {

	int way = rand() % 4; // 1 : UP, 2 : DOWN, 3: LEFT, 4: RIGHT
	int cur_pos = startpos;
	while (true) {
		switch (way)
		{
		case 0:
			if (_IsMovable(cur_pos - col)) {
				cur_pos = cur_pos - col;
			}
			else if (!_IsMovable(cur_pos - col) && _IsMovable(cur_pos + col) && !_IsMovable(cur_pos - 1) && !_IsMovable(cur_pos + 1))
				return cur_pos ;
			else
				way = rand() % 4;
			break;
		case 1:
			if (_IsMovable(cur_pos + col)) {
				cur_pos = cur_pos + col;
			}
			else if (_IsMovable(cur_pos - col) && !_IsMovable(cur_pos + col) && !_IsMovable(cur_pos - 1) && !_IsMovable(cur_pos + 1))
				return cur_pos;
			else
				way = rand() % 4;
			break;
		case 2:
			if (_IsMovable(cur_pos - 1)) {
				cur_pos = cur_pos - 1;
			}
			else if (!_IsMovable(cur_pos - col) && !_IsMovable(cur_pos + col) && !_IsMovable(cur_pos - 1) && _IsMovable(cur_pos + 1))
				return cur_pos;
			else
				way = rand() % 4;
			break;
		case 3:
			if (_IsMovable(cur_pos + 1)) {
				cur_pos = cur_pos+ 1;
			}
			else if (!_IsMovable(cur_pos - col) && !_IsMovable(cur_pos + col) && _IsMovable(cur_pos - 1) && !_IsMovable(cur_pos + 1))
				return cur_pos;
			else
				way = rand() % 4;
			break;
				
		default:
			break;
		}
		
	}


}



int RandomMap::A_Star(int start_x, int start_y, int dest_x, int dest_y)
{
	if (start_x > col || start_y > row || dest_x > col || dest_y > row)
		return start_x + start_y * col;

	Astarnode* node = new Astarnode[col*row];
	int* open_node = new int[col*row];
	int open_cnt = 0;
	for (int i = 0; i < col*row; i++) {
		node[i].idx = i;
		node[i].dist = abs(node[i].idx % col - dest_x) + abs(node[i].idx / col - dest_y);
		node[i].move = 99999999;
		node[i].parent = nullptr;
	}

	Astarnode cur_node = node[start_x + start_y * col];
	cur_node.move = 0;
	while (true) {
		node[cur_node.idx].closelist = true;
		if (_IsMovable(cur_node.idx - 1)	&& !node[cur_node.idx - 1].openlist    ) { 
			node[cur_node.idx - 1].openlist = true;		
			node[cur_node.idx - 1].move = cur_node.move + 1;	
			node[cur_node.idx - 1].parent = &node[cur_node.idx];
			open_node[open_cnt] = cur_node.idx - 1;		
			open_cnt++; 
		}
		if (_IsMovable(cur_node.idx + 1)	&& !node[cur_node.idx + 1].openlist    ) { 
			node[cur_node.idx + 1].openlist = true;		
			node[cur_node.idx + 1].move = cur_node.move + 1;
			node[cur_node.idx + 1].parent = &node[cur_node.idx];
			open_node[open_cnt] = cur_node.idx + 1;		
			open_cnt++; 
		}
		if (_IsMovable(cur_node.idx + col)	&& !node[cur_node.idx + col].openlist  ) { 
			node[cur_node.idx + col].openlist = true;	
			node[cur_node.idx + col].move = cur_node.move + 1;	
			node[cur_node.idx + col].parent = &node[cur_node.idx];
			open_node[open_cnt] = cur_node.idx + col;	
			open_cnt++; 
		}
		if (_IsMovable(cur_node.idx - col)	&& !node[cur_node.idx - col].openlist  ) { 
			node[cur_node.idx - col].openlist = true;	
			node[cur_node.idx - col].move = cur_node.move + 1;	
			node[cur_node.idx - col].parent = &node[cur_node.idx];
			open_node[open_cnt] = cur_node.idx - col;	
			open_cnt++; 
		}
		if (cur_node.idx == dest_x + dest_y * col)
			break;
		int short_dist = 99999;
		int short_node = 99999;
		int num_temp;
		for (int i = 0; i < open_cnt; i++) {
			if (short_dist > node[open_node[i]].move + node[open_node[i]].dist) {
				short_node = open_node[i];
				num_temp = i;
				short_dist = node[open_node[i]].move + node[open_node[i]].dist;
			}
		}
		open_cnt--;
		for (int i = num_temp; i < open_cnt; i++) {
			open_node[i] = open_node[i + 1];
		}
		cur_node = node[short_node];
	}
	while(cur_node.parent != nullptr) {
		if (cur_node.parent->idx == start_x + start_y * col) {
			break;
		}
			cur_node = *cur_node.parent;


	}

	
	delete(node);
	delete(open_node);

	return cur_node.idx;
}


///BSP fx

void RandomMap::make_BSP() {

	Section* head;
	head = new Section();
	head->col_start = 0;
	head->col_end = col;
	head->row_start = 0;
	head->row_end = row;
	head->grade = 0;

	BSP_loop(head);

	BSP_print(head);
	BSP_bridging2(head);


	BSP_erase(head);
}
bool RandomMap::_IsMovable(int pos)
{
	if (pos < 0 || pos >= col * row)
		return false;
	
	if (region[pos] == TILE_BASE || region[pos] == TILE_EXIT)
		return true;
	else
		return false;
}
void RandomMap::BSP_loop(Section* _parent) {
	int sect_row = _parent->row_end - _parent->row_start;
	int sect_col = _parent->col_end - _parent->col_start;



	//	cout << section_que.size() << "\n";
	if (sect_row < 12 || sect_col < 15) {
		return;
	}

	Section* Child1 = new Section;
	Section* Child2 = new Section;
	_parent->sub1 = Child1;
	_parent->sub2 = Child2;
	Child1->grade = _parent->grade + 1;
	Child2->grade = _parent->grade + 1;
	Child1->sub1 = nullptr;
	Child2->sub1 = nullptr;
	Child1->sub2 = nullptr;
	Child2->sub2 = nullptr;
	if (sect_col >= sect_row) {
		int newroomsize = rand() % (sect_col / 3) + sect_col / 3;

		Child1->row_start = _parent->row_start;
		Child1->col_start = _parent->col_start;
		Child1->col_end = Child1->col_start + newroomsize;
		Child1->row_end = _parent->row_end;

		Child2->row_start = Child1->row_start;
		Child2->row_end = Child1->row_end;
		Child2->col_end = _parent->col_end;
		Child2->col_start = Child1->col_end + 1;
	}
	else
	{
		int newroomsize = rand() % (sect_row / 3) + sect_row / 3;

		Child1->col_start = _parent->col_start;
		Child1->row_start = _parent->row_start;
		Child1->row_end = Child1->row_start + newroomsize;
		Child1->col_end = _parent->col_end;

		Child2->col_start = Child1->col_start;
		Child2->col_end = Child1->col_end;
		Child2->row_end = _parent->row_end;
		Child2->row_start = Child1->row_end + 1;
	}

	BSP_loop(Child1);
	BSP_loop(Child2);
}
void RandomMap::BSP_print(Section* _sect) {

	if (_sect->sub1 == nullptr && _sect->sub2 == nullptr) {

		Section sect1 = *_sect;
		int room_start_row;
		int room_size_row;
		int room_start_col;
		int room_size_col;



		if (sect1.row_end - sect1.row_start < 8) {
			room_start_row = sect1.row_start + 1;
			room_size_row = sect1.row_end - room_start_row - 1;

			room_start_col = sect1.col_start + rand() % 3 + 1;
			room_size_col = sect1.col_end - room_start_col - rand() % 3 - 2;
		}
		else if (sect1.col_end - sect1.col_start < 8) {
			room_start_row = sect1.row_start + rand() % 3 + 1;
			room_size_row = sect1.row_end - room_start_row - rand() % 3 - 2;

			room_start_col = sect1.col_start + 1;
			room_size_col = sect1.col_end - room_start_col - 1;
		}
		else {
			room_start_row = sect1.row_start + rand() % 2 + 1;
			room_size_row = sect1.row_end - room_start_row - rand() % 2 - 2;

			room_start_col = sect1.col_start + rand() % 2 + 1;
			room_size_col = sect1.col_end - room_start_col - rand() % 2 - 2;
		}


		_sect->col_start += room_start_col * 10000;
		_sect->col_end += (room_start_col + room_size_col) * 10000;
		_sect->row_start += room_start_row * 10000;
		_sect->row_end += (room_size_row + room_start_row) * 10000;



		for (int i = 0; i < room_size_row; i++) {
			for (int j = 0; j < room_size_col; j++) {
				region[(room_start_row + i)*col + room_start_col + j] = _sect->grade + 90;
			}
		}

	}
	else {
		BSP_print(_sect->sub1);
		BSP_print(_sect->sub2);
	}

}
//안씀
void RandomMap::BSP_bridging(Section* head) {

	Section* room1;
	Section* room2;

	room1 = head->sub1;
	room2 = head->sub2;



	int col_room1 = (room1->col_end + room1->col_start) / 2;
	int row_room1 = (room1->row_end + room1->row_start) / 2;
	int col_room2 = (room2->col_end + room2->col_start) / 2;
	int row_room2 = (room2->row_end + room2->row_start) / 2;


	if (col_room1 == col_room2) {
		for (int i = 0; i < row_room2 - row_room1; i++)
			region[col_room1 + (row_room1 + i) * col] = TILE_BASE;
	}
	else
		for (int i = 0; i < col_room2 - col_room1; i++)
			region[row_room1*col + col_room1 + i] = TILE_BASE;

	if (room1->sub1 != nullptr)
		BSP_bridging(room1);


	if (room2->sub1 != nullptr)
		BSP_bridging(room2);

}
void RandomMap::BSP_bridging2(Section* head) {
	Section* cur = head;
	if (cur->sub1 == nullptr)
		return;

	Section* StartRoom;
	Section* EndRoom;

	Section* curser_temp = cur;
	
	int temp;
	if (cur->col_end - cur->col_start >= cur->row_end - cur->row_start) { // 세로가 더 길거나 같은 경우 가로로 자른다.
		temp = curser_temp->sub1->col_end%10000;
		EndRoom = BSP_search(cur, temp, 1);
		StartRoom = BSP_search(cur, temp + 1, 0);

		///그리기
		int s_distance = 999;
		int e_distance = 999;
		s_distance = (EndRoom->col_end % 10000) - (EndRoom->col_end / 10000);
		int width =EndRoom->row_end / 10000 - EndRoom->row_start / 10000-2;
		if (width <= 0)
			width = 1;
		int s_pos = rand() % width + (EndRoom->row_start / 10000)+1;
		for (int i = 0; i < s_distance; i++) {
			region[( s_pos) * col + (EndRoom->col_end / 10000) + i] = TILE_BASE;
		}

		e_distance = (StartRoom->col_start / 10000) - (StartRoom->col_start % 10000);
		width = StartRoom->row_end / 10000 - StartRoom->row_start / 10000-2;
		if (width <= 0)
			width = 1;
		int e_pos = rand() % width + (StartRoom->row_start / 10000)+1;
		for (int i = -1; i < e_distance; i++) {
			region[( e_pos) * col + (StartRoom->col_start % 10000) + i] = TILE_BASE;
		}




		for (int i = min(s_pos, e_pos); i <= max(s_pos , e_pos); i++) {
			region[( i) * col + (StartRoom->col_start % 10000) -1] = TILE_BASE;
		}



	}
	else {
		temp = curser_temp->sub1->row_end%10000;
		EndRoom = BSP_search(cur, temp, 3);
		StartRoom = BSP_search(cur, temp + 1, 2);
		///그리기



		int s_distance = 999;
		int e_distance = 999;
		s_distance = (EndRoom->row_end % 10000) - (EndRoom->row_end / 10000);
		int width = EndRoom->col_end / 10000 - EndRoom->col_start / 10000-2;
		if (width <= 0)
			width = 1;
		int s_pos = rand() % width + (EndRoom->col_start / 10000)+1;
		for (int i = 0; i < s_distance; i++) {
			region[s_pos + ((EndRoom->row_end / 10000) + i) * col] = TILE_BASE;
		}



		e_distance = (StartRoom->row_start / 10000) - (StartRoom->row_start % 10000);
		width = StartRoom->col_end / 10000 - StartRoom->col_start / 10000-2;
		if (width <= 0)
			width = 1;
		int e_pos = rand() % width + (StartRoom->col_start / 10000)+1;
		for (int i = -1; i < e_distance; i++) {
			region[e_pos + ((StartRoom->row_start % 10000) + i) * col] = TILE_BASE;
		}




		for (int i = min(s_pos, e_pos); i <= max(s_pos, e_pos); i++) {
			region[i + ((StartRoom->row_start % 10000) -1) * col] = TILE_BASE;
		}

	}

	BSP_bridging2(cur->sub1);
	BSP_bridging2(cur->sub2);
}

Section* RandomMap::BSP_search(Section* head, int search_value, int value_pos) {
	// valuepos : 0. col_start, 1. col_end, 2. row_start, 3. row_end
	if (head == nullptr) return head;

	Section* res = nullptr;
	Section* temp = nullptr;
	switch (value_pos) {
	case 0:
		if (head->col_start % 10000 == search_value && head->sub1 == nullptr)
			return head;
		else {
			
			temp = BSP_search(head->sub1, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
			temp = BSP_search(head->sub2, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
		}
		break;
	case 1:
		if (head->col_end % 10000 == search_value && head->sub1 == nullptr)
			return head;
		else {
			temp = BSP_search(head->sub1, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
			temp = BSP_search(head->sub2, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
		}
		break;
	case 2:
		if (head->row_start % 10000 == search_value&& head->sub1 == nullptr)
			return head;
		else {
			temp = BSP_search(head->sub1, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
			temp = BSP_search(head->sub2, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
		}
		break;
	case 3:
		if (head->row_end % 10000 == search_value&& head->sub1 == nullptr)
			return head;
		else {
			temp = BSP_search(head->sub1, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
			temp = BSP_search(head->sub2, search_value, value_pos);
			if (temp != nullptr)
				res = temp;
		}
		break;
	default:
		break;

	}
	
	return res;
}
void RandomMap::BSP_erase(Section * head)
{
	if (head->sub1 != nullptr)
		BSP_erase(head->sub1);
	
	if(head->sub2 != nullptr)
		BSP_erase(head->sub2);

	delete head;
}



