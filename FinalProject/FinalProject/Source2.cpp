#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>



char tmp_map2[18][38];//map height and width of map 2

char map2[18][38] = {
	"+###################################+",
	"|                                   |",
	"|## ####  ##### #   #######  #  ### |",
	"|   |               |             | |",
	"|   |    |  |   2       |   ######| |",
	"| | |### |  |       |   |           |",
	"| |         | |###  |   |  |      | |",
	"| |######|  | |      ## |         | |",
	"|             |###  |      |######| |",
	"| |##### ###         ##           | |",
	"| |    |   #  ######_##### ###      |",
	"|                                   |",
	"| |### ####   ###  ###   ######  ## |",
	"|                |                  |",
	"|#######  ###### ########  ### ###  |",
	"|         |                |     |  |",
	"|                                   |",
	"+###################################+"
};


void ShowMap2()
{
	for (int i = 0; i < 18; i++) {
		printf("%s\n", map2[i]);
	}
}


void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}

class entity {
public:
	entity(int x, int y){
		this->x = x;
		this->y = y;
	}

	void move_x(int p){
		if (map2[y][x + p] == ' ') x += p;
	}

	void move_y(int p){
		if (map2[y + p][x] == ' ') y += p;
	}

	void move(int p, int q){
		x += p;
		y += q;
	}

	int get_x(){ return x; }
	int get_y(){ return y; }

	void draw(char p){
		map2[x][y] = p;
		gotoxy(x, y); printf("%c", p);
	}

private:
	int x;
	int y;
};

struct walk {
	short walk_count;
	short x;
	short y;
	short back;
};

struct target {
	short x;
	short y;
};

std::vector<target> walk_queue;

std::vector<walk> BFSArray;

void AddArray(int x, int y, int wc, int back){
	if (tmp_map2[y][x] == ' ' || tmp_map2[y][x] == '.'){
		tmp_map2[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back(tmp);
	}
}

void FindPath(int sx, int sy, int x, int y){
	memcpy(tmp_map2, map2, sizeof(map2));
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back(tmp);

	int i = 0;
	while (i < BFSArray.size()){
		if (BFSArray[i].x == x && BFSArray[i].y == y){
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0){
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back(tmp2);

				i = BFSArray[i].back;
			}

			break;
		}

		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);


		i++;
	}

	BFSArray.clear();
}



int main()
{
	int ChoiceYes;
	bool running = true;
	int x = 15; // hero x
	int y = 16; // hero y
	int old_x;
	int old_y;

	int ex = 1;
	int ey = 1;


	int pts = 0;


	std::cout << "Instructions:\n1. Use arrow Keys to move your character\n2. Eat the dots made by the Ghost to gain poins\n3. Don't get caught by the Ghost\n\n";
	std::cout << "E = Easy\nN = Normal\nH = Hard\n\nChoose your difficulty : ";  //start menu


	char difficulty;
	int speedmod = 3;//hard difficulty

	std::cin >> difficulty;

	if (difficulty == 'N' || difficulty == 'n'){//meduim difficulty
		speedmod = 2;
	}
	else if (difficulty == 'H' || difficulty == 'h'){//easy difficulty
		speedmod = 1;
	}

	system("cls");
	ShowMap2();

	gotoxy(x, y); std::cout << "H";

	int frame = 0;

	FindPath(ex, ey, x, y);

	while (running){
		gotoxy(x, y); std::cout << " ";

		old_x = x;
		old_y = y;

		if (GetAsyncKeyState(VK_UP)){
			if (map2[y - 1][x] == '.'){ y--; pts++; }//controls for user
			else
			if (map2[y - 1][x] == ' ') y--;
		}
		if (GetAsyncKeyState(VK_DOWN)){
			if (map2[y + 1][x] == '.'){ y++; pts++; }//controls for user
			else
			if (map2[y + 1][x] == ' ') y++;
		}
		if (GetAsyncKeyState(VK_LEFT)){
			if (map2[y][x - 1] == '.'){ x--; pts++; }//controls for user
			else
			if (map2[y][x - 1] == ' ') x--;
		}
		if (GetAsyncKeyState(VK_RIGHT)){
			if (map2[y][x + 1] == '.'){ x++; pts++; }//controls for user
			else
			if (map2[y][x + 1] == ' ') x++;
		}

		if (old_x != x || old_y != y){
			FindPath(ex, ey, x, y);
		}

		gotoxy(x, y); std::cout << "C";//user character

		map2[ey][ex] = '.';
		gotoxy(ex, ey); std::cout << ".";

		if (frame%speedmod == 0 && walk_queue.size() != 0){
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}

		gotoxy(ex, ey); std::cout << "G";//ghost Character

		if (ex == x && ey == y){
			break;
		}


		gotoxy(32, 18);
		gotoxy(32, 1); std::cout << pts;
		Sleep(100);
		frame++;
		

	}

	system("cls");
	printf("           You Lose and your score is : %i", pts);
	printf("                                                ");//Game Over screen
	printf("                       Type 1 to play again!      ");
	std::cin >> ChoiceYes;
	if (ChoiceYes == 1)//if user types "1" they can play again
	{
		main();
	}
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();
	std::cin.get();

	return 0;



}