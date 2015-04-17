/*
15/04/15 p.50 퍼즐맨 짜기
- 2시간 30분만에 버그 없이 완성 헿
잘한 점
- 여러 스테이지로 확장 가능하게 만듦
- const 사용
- 뒷정리 잘 함
*/

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

typedef struct _tagPosition{
	int row = -1;
	int col = -1;
}Position;

typedef struct _tagNextSpaceInfo{
	Position nextPosition;
	Position nextNextPosition;
	char nextInfo = '!';
	char nextNextInfo = '!';
}NextSpaceInfo;

/*
개수 : Count
인덱스: Index
번호: Number

= space
# = wall
. = target
o = box
@ = delivered
p = player
*/

class Stage{
public:
	Stage(int p_col, int p_row, char* p_stage_map)
		:col(p_col), row(p_row)
	{
		//문자열때문에 1 더함
		int mapSize = (p_row*(p_col + 1));

		this->stageMap = new char[mapSize];
		this->originalStageMap = new char[mapSize];
		for (int i = 0; i < mapSize; i++){
			this->stageMap[i] = p_stage_map[i];

			if (p_stage_map[i] == 'p' ||
				p_stage_map[i] == 'o'){
				this->originalStageMap[i] = ' ';
			}
			else if (p_stage_map[i] == '@'){
				this->originalStageMap[i] = '.';
			}
			else{
				this->originalStageMap[i] = p_stage_map[i];
			}
		}
	}

	~Stage(){
		delete stageMap;
	}

	int getColCount(){ return col; };
	int getRowCount(){ return row; };

	char getData(int p_row, int p_col){
		return stageMap[(col + 1)*p_row + p_col];
	}
	char getData(Position p_pos){
		return getData(p_pos.row, p_pos.col);
	}

	void setData(int p_row, int p_col, char data){
		stageMap[(col + 1)*p_row + p_col] = data;
	}
	void setData(Position p_pos, char data){
		setData(p_pos.row, p_pos.col, data);
	}

	char getOriginalData(int p_row, int p_col){
		return originalStageMap[(col + 1)*p_row + p_col];
	}
	char getOriginalData(Position p_pos){
		return getOriginalData(p_pos.row, p_pos.col);
	}
private:
	const int col;
	const int row;
	char* stageMap;
	char* originalStageMap;
};

/*
define command list

! = wrong input
w = up
s = down
a = left
d = right
*/
char command = '!';

/* define list of stage */
#define MAX_STAGE 1
Stage* stageList[MAX_STAGE];
int stageCount = 0;

/* current stage number */
int stageCurIndex = 0;
Stage* getCurrentStage(){ return stageList[stageCurIndex]; }

/* define game state */
#define LOADING 0
#define PLAYING 1
#define FINISH 2
int gameState = LOADING;

/* pre-defined stage information*/
#define STAGE_1_ROW 5
#define STAGE_1_COL 8

char stage1Map[STAGE_1_ROW][STAGE_1_COL + 1] =
{
	"########",
	"# .. p #",
	"# oo   #",
	"#      #",
	"########"
};