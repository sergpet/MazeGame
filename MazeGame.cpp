#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include<cstdlib>
#include<ctime>
#include <unistd.h>

using namespace std;

bool impasse(int, int, int**, int, int); //визначає тупики
void makeMaze(int**, int, int); //алгоритм

const int wall = 0, pass = 1, hero = 2, exitPortal = 3;//позначення в масиві
int mode = 1;//режим гри
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //доступ до консолі для зміни кольору і положення курсора
COORD position; // позиція курсора
class Maze{ //лабіринт
public:
    int** maze = new int*[100]; //масив лабіринту не більше 100х100
    int height, width; //висота і ширина лабіринту
    Maze(int h, int w){ //конструктор
        height = h, width = w;
        for(int i = 0; i < height; i++)//виділення пам'яті
            maze[i] = new int[width];

        for(int i = 0; i < height; i++) // Масив заповнюється стінами
        	for(int j = 0; j < width; j++)
        		maze[i][j] = wall;

        makeMaze(maze, height, width); // створюється лабіринт
        maze[1][width - 2] = 3; // встановлення точки виходу
    }

    void visual(){ //відрисувати весь кадр
    	for(int i = 0; i < height; i++){
    		for(int j = 0; j < width; j++)
    			switch(maze[i][j]){
    				case wall:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 2)); cout<<"# "; break;
    				case pass:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7)); cout<<"  "; break;
                    case hero:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12)); cout<<"@ "; break;
                    case exitPortal:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13)); cout<<"O "; break;
    			}
    		cout<<endl;
    		}
    	cout<<endl;
    }

    void visual(int height){ // відрисувати частину кадра за рядком
        for(int j = 0; j < width; j++){
            switch(maze[height][j]){
                case wall:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 2)); cout<<"# "; break;
                case pass:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7)); cout<<"  "; break;
                case hero:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12)); cout<<"@ "; break;
                case exitPortal:SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13)); cout<<"O "; break;
            cout<<endl;
            }
        }
    }
};

class Hero{ //герой
public:
    int x, y; //положення героя
    Hero(int h, int w){ // конструктор
        y = h - 2;
        x = 1;
    }
    void up(){ //крок вверх
        y = y - 1;
    }
    void down(){ //крок вниз
        y = y + 1;
    }
    void right(){ //крок вправо
        x = x + 1;
    }
    void left(){ //крок вліво
        x = x - 1;
    }
};

void stop(){
    Sleep(100);
    sleep(1/100);
}

void win(){
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 5));
    cout<<"      ___                          \n";
    cout<<"  _.-|   |          |\\__/,|   (`\\  \n";
    cout<<" {   |   |          |o o  |__ _) ) \n";
    cout<<"  \"-.|___|        _.( T   )  `  /  \n";
    cout<<"  .--'-`-.     _((_ `^--' /_<  \\   \n";
    cout<<".+|______|__.-||__)`-'(((/  (((/   \n";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 10));
    cout<<" ,--.   ,--.    ,--.    ,--.  ,--. \n";
    cout<<" |  |   |  |    |  |    |  ,'.|  | \n";
    cout<<" |  |.'.|  |    |  |    |  |' '  | \n";
    cout<<" |   ,'.   |    |  |    |  | `   | \n";
    cout<<" '--'   '--'    `--'    `--'  `--' \n";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
}

void prew(){
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 10));
    cout<<"      ,--.   ,--.      ,---.      ,-------.    ,------. \n";
    cout<<"      |   `.'   |     /  O  \\     `--.   /     |  .---' \n";
    cout<<"      |  |'.'|  |    |  .-.  |      /   /      |  `--,  \n";
    cout<<"      |  |   |  |    |  | |  |     /   `--.    |  `---. \n";
    cout<<"      `--'   `--'    `--' `--'    `-------'    `------' \n";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13));
    cout<<"________________________________________________________________  \n";
    cout<<"|____|____|____|____|____|____|____|____|____|____|____|____|____ \n";
    cout<<"____|____|____|____|____|____|____|____|____|____|____|____|____| \n";
    cout<<"__|____|____|____|____|___|_         ____|____|____|____|____|__  \n";
    cout<<"|____|____|____|____|___|";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12));
    cout<<"     (\\.-./)";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13));
    cout<<" _|____|____|____|___|___|_  \n";
    cout<<"____|____|____|____|____|_";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12));
    cout<<"  = (^ Y ^) =  ";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13));
    cout<<"_|____|____|____|____|__ \n";
    cout<<"|____|____|____|____|____|___";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12));
    cout<<" /`---`\\ ";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13));
    cout<<"__|____|____|____|____|____|\n";
    cout<<"__|____|____|____|____|____|_";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12));
    cout<<"U";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13));
    cout<<"___|_";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12));
    cout<<"U";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 13));
    cout<<"|____|____|____|____|____|___ \n";
    cout<<"|____|____|____|____|____|____|____|____|____|____|____|____|____ \n";
    cout<<"____|____|____|____|____|____|____|____|____|____|____|____|____|_\n";
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
}

void crushWall(Maze maze, Hero hero, int key){// up-1 down-2 left-3 right-4
    switch(key){
        case 1:
            maze.maze[hero.y - 1][hero.x] = pass;
            position.Y = hero.y - 1;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(hero.y - 1); break;
        case 2:
            maze.maze[hero.y + 1][hero.x] = pass;
            position.Y = hero.y + 1;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(hero.y + 1); break;
            break;
        case 3:
            maze.maze[hero.y][hero.x - 1] = pass;
            position.Y = hero.y;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(hero.y); break;
            break;
        case 4:
            maze.maze[hero.y][hero.x + 1] = pass;
            position.Y = hero.y;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(hero.y); break;
            break;
    }
}

void crushPoint(int crush, Maze maze){
    position.Y = maze.height;
    SetConsoleCursorPosition(hConsole, position);
    if(crush != 0){
        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 12));
        cout << "Crush : ";
        for(int i = 0; i < crush; i++){
            cout<< " * ";
        }
        cout << "   ";
        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 2));
    }else{
        position.Y = maze.height;
        SetConsoleCursorPosition(hConsole, position);
        cout << "        ";
    }
}

void game(Maze maze, Hero hero, int crush){
    unsigned int start_time, end_time, search_time;
    int flag = 0; // індикатор проходження лабіринту
    position.X = 0;
    position.Y = 0;
    int moveCounter = 0;

    system("cls"); // очищаємо поле
    SetConsoleCursorPosition(hConsole, position);//курсор в початок
    maze.maze[hero.y][hero.x] = 2; // початкове полложення героя
    maze.visual(); // відрисувати кадр
    crushPoint(crush, maze);

    start_time = clock();
    while(flag == 0){
        if(hero.y == 1 && hero.x == maze.width - 2){
            flag++;
            system("cls");
            win();
        }else{
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_UP) && maze.maze[hero.y - 1][hero.x] == wall){
                if(hero.y - 1 != maze.height-1 && hero.y - 1 != 0 && crush != 0){
                    crushWall(maze, hero, 1);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_DOWN) && maze.maze[hero.y + 1][hero.x] == wall){
                if(hero.y + 1 != maze.height-1 && hero.y + 1 != 0 && crush != 0){
                    crushWall(maze, hero, 2);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_LEFT) && maze.maze[hero.y][hero.x - 1] == wall){
                if(hero.x - 1 != maze.width-1 && hero.x - 1 != 0 && crush != 0){
                    crushWall(maze, hero, 3);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_RIGHT) && maze.maze[hero.y][hero.x + 1] == wall){
                if(hero.x + 1 != maze.width-1 && hero.x + 1 != 0 && crush != 0){
                    crushWall(maze, hero, 4);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_UP) && maze.maze[hero.y - 1][hero.x] != 0){
                hero.up();
                moveCounter++;
                maze.maze[hero.y][hero.x] = 2;
                maze.maze[hero.y+1][hero.x] = 1;

                position.Y = hero.y;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(hero.y);

                position.Y = hero.y + 1;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(hero.y + 1);
                stop();
            }
            if(GetAsyncKeyState(VK_DOWN) && maze.maze[hero.y + 1][hero.x] != 0){
                hero.down();
                moveCounter++;
                maze.maze[hero.y][hero.x] = 2;
                maze.maze[hero.y-1][hero.x] = 1;

                position.Y = hero.y;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(hero.y);

                position.Y = hero.y - 1;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(hero.y - 1);
                stop();
            }
            if(GetAsyncKeyState(VK_LEFT) && maze.maze[hero.y][hero.x - 1] != 0){
                hero.left();
                moveCounter++;
                maze.maze[hero.y][hero.x] = 2;
                maze.maze[hero.y][hero.x + 1] = 1;

                position.Y = hero.y;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(hero.y);
                stop();
            }
            if(GetAsyncKeyState(VK_RIGHT) && maze.maze[hero.y][hero.x + 1] != 0){
                hero.right();
                moveCounter++;
                maze.maze[hero.y][hero.x] = 2;
                maze.maze[hero.y][hero.x - 1] = 1;
                position.Y = hero.y;

                SetConsoleCursorPosition(hConsole, position);
                maze.visual(hero.y);
                stop();
            }
        }
    }
    end_time = clock();
    search_time = end_time - start_time;
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
    cout << " Time : " << search_time/1000.0 << "        Move : " << moveCounter << endl;
    cout <<"__________________________________\n";
    system("pause");
}

int main(){
    srand((unsigned)time(NULL));
    int mode = 1;
    int crush = 0;
    int width = 5,  height = 5;

    while(true){
        system("cls");
        prew();
        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
        cout << "Select mode: \n0)for generate maze arbitrary size not more 99x99\n1)Easy mode\n2)Medium mode\n3)Hard mode\n";
        cin >> mode;
        if(mode == 1){
            width = 19;
            height = 19;
        }else if(mode == 2){
            width = 31;
            height = 31;
        }else if(mode == 3){
            width = 41;
            height = 41;
        }else if(mode == 0){
            int flag = 0;
            while(flag == 0){
                cout << "Enter odd width, height, crush: ";
                cin >> width >> height >> crush;
                if(width % 2 == 0 || height % 2 == 0){
                    cout << "Error: no odd width or height\n";
                }else{
                    flag++;
                }
            }
        }else{
            cout << "incorect mode build maze 5x5\n";
            system("pause");
        }

        Maze maze = Maze(height, width);
        Hero hero = Hero(height, width);
        if(mode == 1){
            game(maze,hero,0);
        }else if(mode == 2){
            game(maze,hero,1);
        }else if(mode == 3){
            game(maze,hero,2);
        }else if(mode == 0){
            game(maze,hero,crush);
        }
    }
    return 0;
}

bool impasse(int x, int y, int** maze, int height, int width){ //перевірка на тупік
	int a = 0;

	if(x != 1){
		if(maze[y][x-2] == pass)
			a+=1;
		}
	else a+=1;

	if(y != 1){
		if(maze[y-2][x] == pass)
			a+=1;
		}
	else a+=1;

	if(x != width-2){
		if(maze[y][x+2] == pass)
			a+=1;
		}
	else a+=1;

	if(y != height-2){
		if(maze[y+2][x] == pass)
			a+=1;
		}
	else a+=1;

	if(a == 4)
		return 1;
	else
		return 0;
}

void makeMaze(int** maze, int height, int width){ //генеруємо лабіринт
	int x, y, c, a;
	bool b;

	x = 3; y = 3; a = 0; // точка початку прокладання лабіринту і лічильник
	while(a < 10000){
		maze[y][x] = pass; a++; //прокладаємо шлях
		while(1){
			c = rand()%4;//вибираємо напрямок
			switch(c){
				case 0: if(y != 1)
					if(maze[y-2][x] == wall){ // Вверх
                    maze[y-1][x] = pass;
					maze[y-2][x] = pass;
					y-=2;
				}
				case 1: if(y != height-2)
					if(maze[y+2][x] == wall){ // Вниз
					maze[y+1][x] = pass;
					maze[y+2][x] = pass;
					y+=2;
				}
				case 2: if(x != 1)
					if(maze[y][x-2] == wall){ // Наліво
					maze[y][x-1] = pass;
					maze[y][x-2] = pass;
					x-=2;
				}
				case 3: if(x != width-2)
					if(maze[y][x+2] == wall){ // Направо
					maze[y][x+1] = pass;
					maze[y][x+2] = pass;
					x+=2;
				}
			}
			if(impasse(x,y,maze,height,width)) // перевіряємо на тупік
			   break;
		}
		if(impasse(x,y,maze,height,width)) //перекидаємо прокладчик в іншу точку
			do{
				x = 2*(rand()%((width-1)/2))+1;
				y = 2*(rand()%((height-1)/2))+1;
			}
			while(maze[y][x] != pass);
	}
}
