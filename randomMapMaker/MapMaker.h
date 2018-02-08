#pragma once



#include <queue>

using namespace std;
#define MAP_ROW 500 
#define MAP_COL 200

#define TILE_WALL 1
#define TILE_BASE 0
#define TILE_WATER 10
#define TILE_BRIDGE 11
#define TILE_EXIT 999

#define LineDistance 1550


#define MT_MAZE 0
#define MT_FOREST 1
#define MT_ROOM 2
#define MT_EMPTY 3

struct _point_dist {
	int point_number;
	int distance;
};

typedef struct _MapPoint {
	struct _point_dist* connected_point;
	int index;
	int connected_cnt;
}MapPoint;

typedef struct _section {
	int col_start;
	int col_end;
	int row_start;
	int row_end;
	int grade;
	struct _section* sub1;
	struct _section* sub2;
}Section;

typedef struct _room {
	int number;
	struct _room* _parent_room;
}Room;



class RandomMap {
public:

	RandomMap(int input_row, int input_col);
	~RandomMap();

	int _get_col();
	int _get_row();
	void _set_MapTYPE(int type);
	int _get_exit_pos() {
		return exit_pos;
	};

	void makeRNDmap(int type);

	bool _IsMovable(int pos);

	int setRandomRoom();

	int A_Star(int start_x, int start_y, int dest_x, int dest_y);

	void emptymap();

	void print_map();
	void _onepoint_redraw(int x, int y);


	
	//void PrintMapDirect2D();


	//void Render();


	//void TestMap();

private:


	int row;
	int col;

	int* region;
	int maptype;

	void initMap();
	//void print_map();

	void set_exit();
	int exit_pos;


	void Making_lake(int lake_size, int rpt_cnt, int min_size);
	void Make_PointIsland_N_Way(int numberof_point);
	void Make_Maze();
	void Making_cave(int lake_size, int rpt_cnt, int min_size);
	void make_BSP();

	//lake 관련
	int checksize_result = 0;
	int fill_prob;

	int randompick();
	int distance(int point_index_1, int point_index_2);
	int AroundWallCount(int _row, int _col, int radius);
	int check_size(int i, int j, queue<int>* storage);
	void Fill_smallarea(int min_size);


	///미로 관련

	queue<int> voronoi_region[9];
	void draw_line(int pnt_index1, int pnt_index2);
	int voronoi_center(int voronoi_region_idx);
	int find_Deadend(int pos);


	///BSP 관련
	void BSP_loop(Section* _parent);
	void BSP_print(Section* sect);
	void BSP_bridging(Section* head);
	void BSP_bridging2(Section* head);
	Section* BSP_search(Section* head, int value, int value_pos);
	void BSP_erase(Section* head);

	Section* Goto_End(Section* head, int type);
	Section* head;




	/////그리기 관련
	//map<wstring, TextureDrawer*> tiles;
	//vector<D3DXMATRIX> worlds;
};

