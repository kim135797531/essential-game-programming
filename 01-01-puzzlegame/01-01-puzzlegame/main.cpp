#include "main.h"

void init(){
	if (stageCount >= MAX_STAGE){
		cout << "stage storage full!" << endl;
		exit(0);
	}
	else{
		Stage* stage_1 = new Stage(8, 5, &stage1Map[0][0]);
		stageList[stageCount++] = stage_1;
	}

	gameState = PLAYING;
}

void wrongInput(){
	cout << "wrong input!" << endl;
	command = '!';
}

void getInput(){
	cout << "w:up s:down a:left d:right. command?" << endl;

	string input;
	getline(cin, input);
	if (input.size() != 1){
		wrongInput();
		return;
	}
	command = input.at(0);
	switch (command){
	case 'w':
	case 's':
	case 'a':
	case 'd':
		break;
	default:
		wrongInput();
		break;
	}
}


bool isValidPos(Position p_pos){
	Stage* curStage = getCurrentStage();
	if (p_pos.row >= curStage->getRowCount()){
		return false;
	}
	else if (p_pos.col >= curStage->getColCount()){
		return false;
	}
	else{
		return true;
	}
}

Position getPlayerPos(){
	Stage* curStage = getCurrentStage();
	Position ret;

	int targetCount = 0;

	for (int i = 0; i < curStage->getRowCount(); i++){
		for (int j = 0; j < curStage->getColCount(); j++){
			if (stageList[0]->getData(i, j) == 'p'){
				ret.row = i;
				ret.col = j;
				return ret;
			}
		}
	}
	return ret;
}

NextSpaceInfo getNextSpaceInfo(Position p_player_pos){
	Stage* curStage = getCurrentStage();
	NextSpaceInfo ret;
	switch (command){
	case 'w':
		ret.nextPosition.row = p_player_pos.row - 1;
		ret.nextPosition.col = p_player_pos.col;
		ret.nextNextPosition.row = p_player_pos.row - 2;
		ret.nextNextPosition.col = p_player_pos.col;
		break;
	case 's':
		ret.nextPosition.row = p_player_pos.row + 1;
		ret.nextPosition.col = p_player_pos.col;
		ret.nextNextPosition.row = p_player_pos.row + 2;
		ret.nextNextPosition.col = p_player_pos.col;
		break;
	case 'a':
		ret.nextPosition.row = p_player_pos.row;
		ret.nextPosition.col = p_player_pos.col - 1;
		ret.nextNextPosition.row = p_player_pos.row;
		ret.nextNextPosition.col = p_player_pos.col - 2;
		break;
	case 'd':
		ret.nextPosition.row = p_player_pos.row;
		ret.nextPosition.col = p_player_pos.col + 1;
		ret.nextNextPosition.row = p_player_pos.row;
		ret.nextNextPosition.col = p_player_pos.col + 2;
		break;
	default:
		break;
	}

	if (isValidPos(ret.nextPosition)){
		ret.nextInfo = curStage->getData(ret.nextPosition);
	}
	else{
		ret.nextPosition.row = ret.nextPosition.col = -1;
	}

	if (isValidPos(ret.nextNextPosition)){
		ret.nextNextInfo = curStage->getData(ret.nextNextPosition);
	}
	else{
		ret.nextNextPosition.row = ret.nextNextPosition.col = -1;
	}
	return ret;
}

void movePlayer(Position p_position, NextSpaceInfo p_next_space_info){
	Stage* curStage = getCurrentStage();
	char nextInfo = p_next_space_info.nextInfo;
	char nextNextInfo = p_next_space_info.nextNextInfo;

	switch (nextInfo){
	case 'o':
	case '@':
		switch (nextNextInfo){
		case ' ':
			curStage->setData(p_next_space_info.nextNextPosition, 'o');
			curStage->setData(p_next_space_info.nextPosition, 'p');
			curStage->setData(p_position, curStage->getOriginalData(p_position));
			break;
		case '.':
			curStage->setData(p_next_space_info.nextNextPosition, '@');
			curStage->setData(p_next_space_info.nextPosition, 'p');
			curStage->setData(p_position, curStage->getOriginalData(p_position));
			break;
		case '#':
		case 'o':
		case '@':
		default:
			//do nothing
			break;
		}
		break;
	case ' ':
	case '.':
		curStage->setData(p_next_space_info.nextPosition, 'p');
		curStage->setData(p_position, curStage->getOriginalData(p_position));
		break;
	case '#':
	case 'p':
	default:
		//do nothing
		break;
	}
}

int checkTargetCount(){
	Stage* curStage = getCurrentStage();

	int targetCount = 0;

	for (int i = 0; i < curStage->getRowCount(); i++){
		for (int j = 0; j < curStage->getColCount(); j++){
			if (curStage->getOriginalData(i, j) == '.' &&
				curStage->getData(i, j) != '@'){
				targetCount++;
			}

		}
	}

	return targetCount;
}

void updateGame(){
	Position playerPos = getPlayerPos();
	NextSpaceInfo nextSpaceInfo = getNextSpaceInfo(playerPos);
	movePlayer(playerPos, nextSpaceInfo);

	if (checkTargetCount() == 0)
		gameState = FINISH;
}


void draw(){
	system("cls");
	Stage* curStage = getCurrentStage();

	for (int i = 0; i < curStage->getRowCount(); i++){
		for (int j = 0; j < curStage->getColCount(); j++){
			cout << stageList[0]->getData(i, j);
		}
		cout << endl;
	}
}

void end(){
	cout << "congratulations!" << endl;
	for (int i = stageCount - 1; i >= 0; i--){
		delete stageList[i];
		stageList[i] = 0;
	}
}


int main(){
	init();

	while (gameState != FINISH){
		draw();
		getInput();
		updateGame();
	}

	draw();
	end();
}