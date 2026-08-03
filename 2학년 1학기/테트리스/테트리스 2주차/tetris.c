// 1주차 과제 버전
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
		// printw("%d\n", score_number);
		// printw("%d\n", a);
		// printw("%d\n", b);

		// printw("%s\n", head->name);
		// printw("%d\n", head->score);
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
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
				if (field[y][x])
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
	while (CheckToMove(field, nextBlock[0], prevR, temp_blockY, prevX)) {
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
				field[y][x] = 1;
				if (y == HEIGHT - 1 || field[y + 1][x])
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
            if (field[i][j] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            erased++;
            for (int y = i; y > 0; y--) {
                for (int x = 0; x < WIDTH; x++) {
                    field[y][x] = field[y - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) field[0][x] = 0;
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


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;

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
	// for ( i= 1; i < score_number+1 ; i++) {
	// 	free(a.rank_name[i]);
	// }
	// free(a.rank_name);
	// free(a.rank_score);
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
		// user code
	}

	int recommend(RecNode * root)
	{
		int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

		// user code

		return max;
	}

	void recommendedPlay()
	{
		// user code
	}
