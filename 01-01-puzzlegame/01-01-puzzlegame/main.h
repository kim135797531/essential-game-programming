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

#include "../../externals/kdm-c-library/src/library/array2d.h"

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
	Stage(CKDMArray2D<char> p_stage_map) :
		row(p_stage_map.getRow()),
		col(p_stage_map.getCol() - 1) //맨 뒤의 개행 문자는 스테이지 정보가 아니다
	{
		
		this->stageMap = new CKDMArray2D<char>(p_stage_map);
		this->originalStageMap = new CKDMArray2D<char>(p_stage_map);

		for (int i = 0; i < row; i++){
			for (int j = 0; j < col; j++){
				char* cCurrentData = &(*originalStageMap)(i, j);

				if (*cCurrentData == 'p' ||
					*cCurrentData == 'o'){
					*cCurrentData = ' ';
				}
				else if (*cCurrentData == '@'){
					*cCurrentData = '.';
				}
			}
		}
	}

	~Stage()
	{
		delete stageMap;
		delete originalStageMap;
		stageMap = 	originalStageMap = 0;
	}

	int getColCount(){ return col; };
	int getRowCount(){ return row; };

	const char getData(int p_row, int p_col) const {
		return (*stageMap)(p_row, p_col);
	}
	const char getData(const Position p_pos) const {
		return getData(p_pos.row, p_pos.col);
	}

	void setData(int p_row, int p_col, char data){
		(*stageMap)(p_row, p_col) = data;
	}
	void setData(const Position p_pos, char data){
		setData(p_pos.row, p_pos.col, data);
	}

	const char getOriginalData(int p_row, int p_col) const {
		return (*originalStageMap)(p_row, p_col);
	}
	const char getOriginalData(const Position p_pos) const {
		return getOriginalData(p_pos.row, p_pos.col);
	}
private:
	const int col;
	const int row;
	CKDMArray2D<char>* stageMap;
	CKDMArray2D<char>* originalStageMap;
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