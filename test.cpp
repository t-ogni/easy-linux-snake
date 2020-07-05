#define WIDTH 140
#define HEIGHT 38

#include <iostream>
#include "getchLinux.h"
#include "kbhit3.h"
#include <ctime>

void sleep(long double togo){
	togo += clock()/1000000.0;
	while(clock()/1000000.0 < togo);
}

void clear(){
	system("clear");
}

bool play = true;
int x = WIDTH / 2;
int y = HEIGHT / 2;
int score = 0;
int tail[WIDTH * HEIGHT][2]; 
int tailtotal = 1;
bool taildetect = false;
int frx = x + WIDTH / 10;
int fry = y;
char prev = 'w';
char moveTo = 'p';

int main(){
	tail[0][0] = x;
	tail[0][1] = y;
	
	do{
		clear();
		printf("pos: [%d, %d] fruit: [%d, %d] score: %d \n",
			           x, y,         frx, fry,     score);
			           
		// up wall
		for (int i = 0; i < WIDTH+2; i++){
			std::cout << "#";
		}   std::cout << std::endl;
		
		// board
		for (int i = 0; i < HEIGHT; i++)
		{	
			std::cout << "#";
			for (int j = 0; j < WIDTH; j++)
			{
				if (i == y && j == x)
				{
					// \033[32m - green foreground 
					// @ - player head
					// \033[00;40m - clear; black background
					std::cout << "\033[32m@\033[00;40m";
				} else if (i == fry && j==frx) {
					// \033[31m - red foreground
					// F - fruit
					// \033[00;40m - clear; black background
					std::cout << "\033[31mФ\033[00;40m";
				} else { 
					for (int ii = 0; ii < tailtotal; ii++){
						if (j == tail[ii][0] &&
							i == tail[ii][1]){
							// player tail
							std::cout << "\033[32m#\033[00;40m";
							taildetect = true;
							break;
						}
					}
					if(taildetect) { taildetect = false; }
					else { std::cout << " ";  }
					
				}
			}
			std::cout << "#" << std::endl;
		}
		
		// down wall
		for (int i = 0; i < WIDTH+2; i++){
			std::cout << "#";
		}   std::cout << std::endl;
		
		// change that for change speed 
		sleep(0.05);
		
		//check press and change move vector
		if(kbhit()){
			moveTo = getch();
		
			if (moveTo != 'w' &&
				moveTo != 'a' &&
				moveTo != 's' &&
				moveTo != 'd' &&    
				moveTo != 'q' &&    // cheat - +1 tail segment
				moveTo != 'p'){     // pause
				moveTo = prev;
			}
			
			if (moveTo == 'q'){
				tailtotal++;
				moveTo = prev;
			}
		} 

		switch(moveTo){
			case 'w':
				prev = 'w';
				y--;
				break;
				
			case 's':
				prev = 's';
				y++;
				break;
				
			case 'a':
				prev = 'a';
				x--;
				break;
				
			case 'd':
				prev = 'd';
				x++;
				break;
				
			case 'p':
				std::cout<< "paused\n";
				while(!kbhit());
				break;
		}
			
		
		// check wall collision
		if( x < 0 ||
			x >= WIDTH ||
			y < 0 ||
			y >= HEIGHT){
				std::cout << "GAME OVER : zone \n";
				play = false;
		}
		
		// check if eating fruit
		if (x == frx && y == fry){
					frx = rand() % (WIDTH-1);
					fry = rand() % (HEIGHT-1);
					score += rand() % 5 + 2;
					tail[tailtotal][0] = tail[tailtotal-1][0];
					tail[tailtotal][1] = tail[tailtotal-1][1];
					tailtotal++;
				}
		
		// change tail segment position
		int nowtail[2] = {tail[0][0], tail[0][1]};
		for (int i = 1; i < tailtotal; i++)
		{
			if (x == tail[i][0] && y == tail[i][1] && i > 1){
				std::cout << "GAME OVER : tail \n";
				play = false;
				break;
			}
			int prevtail[2] = {tail[i][0], tail[i][1]};
			tail[i][0] = nowtail[0];
			tail[i][1] = nowtail[1];
			nowtail[0] = prevtail[0];
			nowtail[1] = prevtail[1];
		}
		tail[0][0] = x;
		tail[0][1] = y;
		
		if (!play){ 
			std::cout << "play? [y/n]\n";
			std::cin >> prev;
			if(prev == 'n'){ play = false; }
			else { play = true;
				x = WIDTH / 2;
				y = HEIGHT / 2;
				tailtotal = 1;
				moveTo = 'p';
			}
		}
		
	} while (play);
	return 0;
}
