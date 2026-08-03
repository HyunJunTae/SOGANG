#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);
	
	srand((unsigned int)time(NULL));
	createRankList();
	
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_RECOMMEND: clear(); recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();


	recRoot = (RecPointer)malloc(sizeof(RecNode));
	recRoot->score = 0;
	recRoot->lv = 0;
	recRoot->blank_count = 0;
	recRoot->min_y = 100;
	recRoot->f = (char (*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			recRoot->f[i][j] = field[i][j];
		}
	}
	modified_recommend(recRoot);

	free(recRoot->f);
	free(recRoot);


	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(10,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:	
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
	//CheckToMove는 이동 가능하면 1을 리턴
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}

void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(11+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	for (int i = 0; i < BLOCK_HEIGHT; i++) {
        for (int j = 0; j < BLOCK_WIDTH; j++) {
            if (block[currentBlock][blockRotate][i][j] == 1) {
                int y = i + blockY;
                int x = j + blockX;
                if (y >= HEIGHT || x < 0 || x >= WIDTH)
                    return 0;
				if (f[y][x])
					return 0;
            }
        }
    }

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	// 이전 블록의 좌표 및 회전 정보를 저장
    int prevY = blockY;
    int prevX = blockX;
    int prevR = blockRotate;



	// switch 문을 이용해 command에 따라 블록의 이전 상태 계산
    switch (command) {
        case KEY_UP:    // 회전 전 상태
            prevR = (blockRotate + 3) % 4;
            break;
        case KEY_DOWN:  // 한 칸 위
            prevY = blockY - 1;
            break;
        case KEY_LEFT:  // 오른쪽
            prevX = blockX + 1;
            break;
        case KEY_RIGHT: // 왼쪽
            prevX = blockX - 1;
            break;
    }


	// 이전 그림자 위치를 찾아낸다.
	int temp_blockY = prevY;
	while (CheckToMove(f, nextBlock[0], prevR, temp_blockY, prevX)) {
        temp_blockY++;
    }
	temp_blockY--;





	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.

	for (int i = 0; i < BLOCK_HEIGHT; i++) {
        for (int j = 0; j < BLOCK_WIDTH; j++) {
            if (block[currentBlock][prevR][i][j] == 1) {
                int y = prevY + i + 1;
                int x = prevX + j + 1;
                if (y >= 0 && y <= HEIGHT && x >= 0 && x <= WIDTH) {
                    move(y, x);
                    printw(".");
                }
            }
        }
	}

	// 2. 이전 그림자 위치 정보를 지운다.
	for (int i = 0; i < BLOCK_HEIGHT; i++) {
        for (int j = 0; j < BLOCK_WIDTH; j++) {
            if (block[currentBlock][prevR][i][j] == 1) {
                int y = temp_blockY + i + 1;
                int x = prevX + j + 1;
                if (y >= 0 && y <= HEIGHT && x >= 0 && x <= WIDTH) {
                    move(y, x);
                    printw(".");
                }
            }
        }
	}

	move(HEIGHT, WIDTH+10);
	//3. 새로운 블록 정보를 그린다. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	
}

void BlockDown(int sig){
	// user code

	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    } 
	else {
		if (blockY == -1) {
            gameOver = 1;
			return;
        } 
		else {
            score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
            score += DeleteLine(field);
			PrintScore(score);
            nextBlock[0] = nextBlock[1];
			nextBlock[1] = nextBlock[2];
			nextBlock[2] = rand() % 7;
			blockX = WIDTH / 2 - 2;
            blockY = -1;
            blockRotate = 0;



			recRoot = (RecPointer)malloc(sizeof(RecNode));
			recRoot->score = 0;
			recRoot->lv = 0;
			recRoot->blank_count = 0;
			recRoot->min_y = 100;
			recRoot->f = (char (*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					recRoot->f[i][j] = field[i][j];
				}
			}
			modified_recommend(recRoot);

			free(recRoot->f);
			free(recRoot);


			DrawNextBlock(nextBlock);
			DrawField();
        }
    }

	timed_out = 0;

	//강의자료 p26-27의 플로우차트를 참고한다.
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	int touched = 0;
    for (int i = 0; i < BLOCK_HEIGHT; i++) {
        for (int j = 0; j < BLOCK_WIDTH; j++) {
            if (block[currentBlock][blockRotate][i][j] == 1) {
				int y = blockY + i;
				int x = blockX + j;
				f[y][x] = 1;
				if (y == HEIGHT - 1 || f[y + 1][x])
					touched++;
            }
        }
    }
    return touched * 10; // 면적 닿은 점수 계산 방식 예시

	// //Block이 추가된 영역의 필드값을 바꾼다.
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	int erased = 0;
    for (int i = 0; i < HEIGHT; i++) {
        int full = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (f[i][j] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            erased++;
            for (int y = i; y > 0; y--) {
                for (int x = 0; x < WIDTH; x++) {
                    f[y][x] = f[y - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) f[0][x] = 0;
        }
    }
    return erased * erased * 100;

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("/");
				attroff(A_REVERSE);
			}
		}
	}

	move(HEIGHT,WIDTH+10);
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {

	int temp_blockY = blockY;
	while (CheckToMove(field, nextBlock[0], blockRotate, temp_blockY, blockX)) {
        temp_blockY++;
    }
	temp_blockY--;

	DrawRecommend(recommendY, recommendX, blockID, recommendR);

	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	
	DrawShadow(temp_blockY, blockX, blockID, blockRotate);

}



///////////////////////// 2주차 실습에서 구현해야 할 함수/////////////////////////

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *stream;
	char name[NAMELEN];
    int score;

    RankPointer current = NULL;  // 현재 노드 포인터
    RankPointer newNode = NULL;

	//1. 파일 열기
	stream = fopen("rank.txt", "r");
	if (stream == NULL) {
        // 파일이 열리지 않으면 빈 리스트로 초기화
		score_number = -1;
		return;
    }

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	if (fscanf(stream, "%d", &score_number) == 1) {
		for (int i = 0; i < score_number; i++) {
			fscanf(stream, "%s %d", name, &score);
			// 새로운 노드 생성
            newNode = (RankPointer)malloc(sizeof(RankNode));
			// 메모리 할당 실패 시 처리
            if (newNode == NULL) {
                break;
            }

            // 노드 데이터 설정
			// strcpy(newNode->name, name);
            strncpy(newNode->name, name, NAMELEN - 1);
            newNode->name[NAMELEN - 1] = '\0';  // 이름 끝에 NULL 문자 보장
            newNode->score = score;
            newNode->nextRank = NULL;

            // 리스트에 추가
            if (head == NULL) {
                head = newNode;
                current = newNode;
            } else {
                current->nextRank = newNode;
                current = newNode;
            }
        }
	}
	else {


	}

	// 4. 파일닫기
	fclose(stream);

	return;
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	RankPointer current; // 현재 노드 포인터
	current = head;
	// 엔터키의 경우 wgetch 값 10
	if (ch == '1') {
		printw("X:");
		refresh();
		echo();
		scanw("%d", &X);
		noecho();

		printw("Y:");
		refresh();
		echo();
		scanw("%d", &Y);
		noecho();

		
		// 예외 처리
		if (X > Y || X < 0 || Y < 0 || Y > score_number) {
			printw("search failure: no rank in the list\n");
		}
		else {
			for(int i=0; i<X-1; i++) {
				// X-1회 만큼 current 노드를 다음 노드로 이동
				current = current->nextRank;
			}
			printw("name             | score\n");
			printw("------------------------\n");
			for(int i=X; i <= Y; i++) {
				// printw("b");
				printw("%-16s | %d\n", current->name, current->score);
				current = current->nextRank;
			}
		}
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;

		// 이름 입력
		printw("Input the name : ");
		echo();
		scanw("%s", str);
		noecho();

		// currentNode 설정
		RankPointer currentNode = head;

		// currentNode를 한 칸씩 옮기면서 검사
		for (int i=0; i<score_number; i++) {

			// 입력받은 이름이랑 currentNode의 이름이 같다면
			if (strcmp(currentNode->name, str) == 0) {
				// 만약 이름 같은데, 처음 찾았다면 name score 출력
				if (check == 0) {
					printw("name             | score\n");
					printw("------------------------\n");
				}

				// 이름 점수 출력
				printw("%-16s | %d\n", currentNode->name, currentNode->score);
				
				// check를 1로 설정
				check = 1;
			}

			// 다음 노드로
			currentNode = currentNode->nextRank;
		}

		if (check == 0) {
			printw("search failure: no name in the list\n");
		}
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;

		// 삭제하길 원하는 랭크 등수 입력
		printw("Input the rank : ");
		echo();
		scanw("%d", &num);
		noecho();

		// 만약 삭제하길 원하는 랭크 등수가 out of value 라면 예외 출력
		if (num > score_number || num < 1) {
			printw("search failure: the rank not in the list\n");
			return;
		}


		// currentNode 설정
		RankPointer currentNode = head;
		
		// num == 1 이라면 head노드를 한 칸 뒤로 밀고 기존 헤드 free
		if (num == 1) {
			head = head->nextRank;
			free(currentNode);
		}

		// num이 2보다 크다면
		else {
		// currentNode를  num-1번째 등수로 옮기기
		for (int i=2; i<num; i++) currentNode = currentNode->nextRank;

		// 현재 currentNode는 지우고자 하는 노드의 이전 노드를 가리키고 있음.
		// nodeToDelete를 지우고자 하는 노드로 설정
		RankPointer nodeToDelete = currentNode->nextRank;
		
		// currentNode의 다음 랭크를 nodeToDelete의 다음 랭크로 설정해서
		// 삭제할 노드를 skip하도록 함.
		currentNode->nextRank = nodeToDelete->nextRank;

		// 삭제할 노드를 free 시킴.
		free(nodeToDelete);
		}

		// score_number 를 1 감소시키고 파일에 적어주기
		score_number--;
		writeRankFile();

		// 랭크가 삭제되었음을 알리기
		printw("result: the rank deleted\n");
	}
	getch();

	return;
}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
	//1. "rank.txt" 연다
	FILE *stream = fopen("rank.txt", "r");
	fscanf(stream, "%d", &sn);
	fclose(stream);


	stream = fopen("rank.txt", "w");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(stream, "%d\n", score_number);

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	if ( sn == score_number) return;
	else {

		RankPointer currentNode = head;
		for (int i=0; i<score_number; i++) {
			fprintf(stream, "%s %d\n", currentNode->name, currentNode->score);
			currentNode = currentNode->nextRank;
		}

	}

	fclose(stream);
}


void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
	printw("your name: ");
	refresh();
	echo();
	scanw("%s", str);
	noecho();

	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가

	// 이름 제대로 입력 안하는 경우 그냥 리턴
	if(str == '\0') {
		return;
	}

	// 이름 제대로 입력 됐으면 랭크 리스트에 추가
	else {
		RankPointer newNode, currentNode;

		currentNode = head;

		// head의 점수보다 높으면, 맨 위에 넣기
		if (score > currentNode->score) {
			// newNode 설정
			newNode = (RankPointer)malloc(sizeof(RankNode));
			strncpy(newNode->name, str, NAMELEN - 1);
			newNode->name[NAMELEN - 1] = '\0'; // 이름 끝에 NULL 문자 보장
			newNode->score = score;

			// newNode를 헤드노드로 설정하고, 
			// newNode의 다음 랭크를 currentNode (기존의 헤드노드)로 설정
			head = newNode;
			newNode->nextRank = currentNode;
		}

		else {
		// 그게 아닌 경우, 그 다음 점수보다는 높다면, 거기서 멈추기
		while(score < currentNode->nextRank->score) {
			currentNode = currentNode->nextRank;
		}

		// newNode 설정
		newNode = (RankPointer)malloc(sizeof(RankNode));
		strncpy(newNode->name, str, NAMELEN - 1);
		newNode->name[NAMELEN - 1] = '\0'; // 이름 끝에 NULL 문자 보장
		newNode->score = score;
		newNode->nextRank = currentNode->nextRank;

		

		// currentNode의 nextRank를 newNode로 설정
		currentNode->nextRank = newNode;
		}

		score_number++;
		writeRankFile();
	}
}


///////////////////////////////////////////////////////////////////////////
void DrawRecommend(int y, int x, int blockID, int blockRotate)
{
	int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("R");
				attroff(A_REVERSE);
			}
		}
	}

move(HEIGHT,WIDTH+10);
}

int recommend(RecNode * root)
{
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code
	
	int temp_blockY;

	
	RecPointer newRecNode;
	for (int i=0; i<4; i++) { // 블럭 회전수

		for (int j=-2; j<=WIDTH; j++) { // 블럭 x축 위치

			// x좌표 잘못되었으면 continue
			if (CheckToMove(root->f, nextBlock[root->lv], i, temp_blockY, j) == 0) continue;

			temp_blockY = 0;
			
			
			// 노드 초기화
			newRecNode = (RecPointer)malloc(sizeof(RecNode));
			newRecNode->lv = root->lv+1;
			newRecNode->score = root->score;
			newRecNode->f = (char (*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					newRecNode->f[i][j] = root->f[i][j];
				}
			}

			// 블록이 해당 위치, 해당 회전값에서 어디까지 내려갈 수 있는지 계산
			while (CheckToMove(root->f, nextBlock[root->lv], i, temp_blockY, j)) {
				temp_blockY++;
			}
			temp_blockY--;
			// 해당 위치의 블럭 그려넣고 맞닿은 면 점수 계산
			newRecNode->score += AddBlockToField(newRecNode->f, nextBlock[root->lv], i, temp_blockY, j);
			// 없어진 줄의 수 계산해서 score 계산
			newRecNode->score += DeleteLine(newRecNode->f);



			// 만약 아직 최대깊이에 도달하지 못했다면 recursive하게 다음 노드 시작
			if (newRecNode->lv < VISIBLE_BLOCKS) {
				newRecNode->score = recommend(newRecNode);
			}

			if (max < newRecNode->score) {
				max = newRecNode->score;
				if (newRecNode->lv == 1) {
					recommendX = j;
					recommendY = temp_blockY;
					recommendR = i;
				}
			}

			free(newRecNode->f);
			free(newRecNode);
		}
	}

	return max;
}

int modified_recommend(RecNode * root)
{
	int count = 5;
	int max = 0;
	int blank_count = 100;
	int min_y = 0;

	int root_blank_count = root->blank_count;
	int root_score = root->score;

	// user code
	
	int temp_blockY;

	int rotate_num;
	if (nextBlock[root->lv] == 4) {
		rotate_num = 1;
	}
	else if (nextBlock[root->lv] == 0 || nextBlock[root->lv] == 5 || nextBlock[root->lv] == 6) {
		rotate_num = 2;
	}
	else {
		rotate_num = 4;
	}
	
	RecPointer newRecNode;
	for (int i=0; i<rotate_num; i++) { // 블럭 회전수

		for (int j=-2; j<=WIDTH; j++) { // 블럭 x축 위치

			temp_blockY = 0;

			// x좌표 잘못되었으면 continue
			if (CheckToMove(root->f, nextBlock[root->lv], i, temp_blockY, j) == 0) continue;



			// 노드 초기화
			newRecNode = (RecPointer)malloc(sizeof(RecNode));
			newRecNode->lv = root->lv+1;
			newRecNode->score = root_score;
			newRecNode->blank_count = root_blank_count;
			newRecNode->min_y = 0;
			newRecNode->f = (char (*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
			for (int p = 0; p < HEIGHT; p++) {
				for (int q = 0; q < WIDTH; q++) {
					newRecNode->f[p][q] = root->f[p][q];
				}
			}


			// 블록이 해당 위치, 해당 회전값에서 어디까지 내려갈 수 있는지 계산
			while (CheckToMove(newRecNode->f, nextBlock[root->lv], i, temp_blockY, j)) {
				temp_blockY++;
			}
			temp_blockY--;

			
			// 해당 블럭 아래에 빈칸 개수 계산
			for (int a = 0; a<4; a++) {
				for (int b = 0; b<4; b++) {
					if (block[nextBlock[root->lv]][i][a][b] == 1) {

						if (block[nextBlock[root->lv]][i][a+1][b] == 0) {

							int c = temp_blockY+a+1;
							while (newRecNode->f[c][j+b] == 0 && c <HEIGHT) {
								newRecNode->blank_count++;
								c++;
							}	
							// 블록의 가장 낮은 지점을 저장
							newRecNode->min_y = temp_blockY+a;
						}
					}
				}
			}

			// for (int b = 0; b < 4; b++) {
			// 	int max_a = -1;
			// 	for (int a = 3; a >= 0; a--) {
			// 		if (block[nextBlock[root->lv]][i][a][b] == 1) {
			// 			max_a = a;
			// 			break;
			// 		}
			// 	}
			// 	if (max_a != -1) {
			// 		int c = temp_blockY + max_a + 1;
			// 		while (c < HEIGHT && root->f[c][j+b] == 0) {
			// 			newRecNode->blank_count++;
			// 			c++;
			// 		}

			// 		if (newRecNode->min_y < temp_blockY + max_a) {
			// 			newRecNode->min_y = temp_blockY + max_a;
			// 		}
			// 	}
			// }

			
			
			// 만약 계산된 블록 아래 빈칸 수가 다른 경우에 계산된 빈칸 수보다 작거나 같다면,
			// min_y를 비교해서 min_y의 값이 더 큰 노드, 즉 블록이 더 아래에 있는 노드를 선택.
			// min_y가 같다면, 점수가 더 큰 노드를 선택
			
			// 일단 이번 노드의 점수 먼저 계산
			// 없어진 줄의 수 계산해서 score 계산
			// 해당 위치의 블럭 그려넣고 맞닿은 면 점수 계산
			// move(count, 40);
			// printw("blank_count: %d, tempblockY : %d, i:%d, j:%d, min_y:%d, score:%d", newRecNode->blank_count, temp_blockY, i, j, newRecNode->min_y, newRecNode->score);
			// count++;
			newRecNode->score += AddBlockToField(newRecNode->f, nextBlock[root->lv], i, temp_blockY, j);
			newRecNode->score += DeleteLine(newRecNode->f);

			
			// 만약 계산된 블록 아래 빈칸 수가, 다른 경우에 계산된 빈칸 수보다 크다면, 그 노드는 스킵
			if (newRecNode->blank_count > blank_count) {
				free(newRecNode->f);
				free(newRecNode);
				continue;
			}

			// 만약 아직 최대깊이에 도달하지 못했다면 recursive하게 다음 노드 시작
			if (newRecNode->lv < VISIBLE_BLOCKS) {
				modified_recommend(newRecNode);
			}

			

			// 만약에 blank_count 가 기존의 값보다 더 작다면,
			// blank_count, min_y, max 값을 갱신하고,
			// recommendX, recommendY, recommendR 값도 갱신
			if (newRecNode->blank_count < blank_count) {
				// move(count, 40);
				// printw("black count renew!\n");
				// count++;
				blank_count = newRecNode->blank_count;
				min_y = newRecNode->min_y;
				max = newRecNode->score;

				root->blank_count = blank_count;
				// root->min_y = min_y;
				root->score = max;

				if (newRecNode->lv == 1) {
					recommendX = j;
					recommendY = temp_blockY;
					recommendR = i;
				}
			}

			// 만약에 blank_count 가 기존의 값과 같다면,
			else if (blank_count == newRecNode->blank_count) {

				// min_y 값이 기존의 값보다 더 크다면, 즉 블럭이 더 아래에 있다면
				// min_y, max 값을 갱신하고, recommendX, recommendY, recommendR 값도 갱신
				if (min_y < newRecNode->min_y) {
					// move(count, 40);
					// printw("miny renew!\n");
					// count++;
					min_y = newRecNode->min_y;
					max = newRecNode->score;

					// root->min_y = min_y;
					root->score = max;

					if (newRecNode->lv == 1) {
						recommendX = j;
						recommendY = temp_blockY;
						recommendR = i;
					}
				}

				// min_y 값이 기존의 값과 같다면, 즉 블럭이 같은 높이에 있다면
				else if (min_y == newRecNode->min_y) {
					

					// max 값이 기존의 값보다 더 크다면
					// max 값 갱신하고, recommendX, recommendY, recommendR 값도 갱신
					if (max < newRecNode->score) {
						// move(count, 40);
						// printw("max renew!\n");
						// count++;
						max = newRecNode->score;

						// root->blank_count = blank_count;
						// root->min_y = min_y;
						root->score = max;

						if (newRecNode->lv == 1) {
							recommendX = j;
							recommendY = temp_blockY;
							recommendR = i;
						}
					}
				}
			}

			// 이렇게 하고 나면, blank_count, min_y, max 값이 해당 반복문에서의 최적값으로 설정됨.

			free(newRecNode->f);
			free(newRecNode);
		}
	}
}

void recommendedPlay()
{

	nodelay(stdscr, TRUE); // getch를 해도 코드가 멈추지 않도록 설정

	// InitTetris 로 테트리스 기본 설정
	InitTetris();

	// 게임 오버가 될 때까지
	// recommend 함수로 추천 위치 받아내고
	// 해당 위치에 블록 그리기
	int temp_score;
	char ch;
	do{
		// recRoot 선언
		recRoot = (RecPointer)malloc(sizeof(RecNode));
		recRoot->score = 0;
		recRoot->lv = 0;
		recRoot->blank_count = 0;
		recRoot->min_y = 100;
		recRoot->f = (char (*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				recRoot->f[i][j] = field[i][j];
			}
		}
		
		// 추천 함수 사용해서 추천 위치 전역변수에 저장
		modified_recommend(recRoot);

		// 저장된 전역변수에 따라 점수 계산 및 블록 그리기
		temp_score = 0;
		temp_score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX);
		temp_score += DeleteLine(field);
		score += temp_score;

		// 다음 블록 준비
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7;
		blockX = WIDTH / 2 - 2;
		blockY = -1;
		blockRotate = 0;

		// 계산된 점수 출력, 필드 다시 그리기, 블럭 다시 그리기
		PrintScore(score);
		DrawField();
		DrawNextBlock(nextBlock);
		refresh();

		// 만약 시작 위치에 새로운 블럭 못들어가면, 게임 오버
        if (!CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX)) {
            gameOver = 1;
		}
		
		// 1초 대기
		sleep(1);

		ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break; // q 입력 시 종료
        }

	}while(!gameOver);

	nodelay(stdscr, FALSE);

	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
}
