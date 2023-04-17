#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

// Permanent variables
#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define ROWS 25 // rows
#define COLS 75 // cols

typedef struct gameProps
{
	int snakeSpeed;
	int foodEaten;
} gameProps;

typedef struct Snake
{
	int x;
	int y;
} Snake;

typedef struct Food
{
	int x;
	int y;
	bool isEaten;
} Food;

//Announcement of functions
void thesnakemove(Snake snakebody[], int x, int y, int snakeSize);
void snakeappear(Snake snakebody[], int x, int y, int snakeSize);
void spreadFood(Food* food, Snake snakebody[], int snakeSize);
bool hittingHimSelf(Snake snakebody[], int x, int y, int snakeSize);
void theGame(int snakeSpeed, int foodEaten, int snakeSize);
void gotoxy(int y, int x);
void printThegameboard();
void printmenu();
int getKey();
void youWon();
void youLose();

void main()
{
	int snakeSize = 10;
	bool gameProcess = true;

	while (gameProcess)//while gameProcess is true , keep staying in the loop and play
	{
		int choice;
		gameProps game;
		printmenu(); //Call to function
		scanf("%d", &choice);

		switch (choice) // Check user menu choice input.
		{

		case 0: // In case user chose to exit game.
			printf("\n  Goodbye Hope To See You Soon!  \n");
			gameProcess = false;
			continue;

		case 1: // In case user chose to start the game.

			game.snakeSpeed = 1000;
			game.foodEaten = 10;
			theGame(game.snakeSpeed, game.foodEaten, snakeSize);//enter the game function. 
			break;

		default://case input isn't 0 OR 1.
			continue;
		}
	}
}

//A function in which the whole game takes place
void theGame(int snakeSpeed, int foodEaten, int snakeSize)
{
	system("cls");//Clear the screen

	printThegameboard();//Call to function
	char c;
	int x = COLS / 2, y = ROWS / 2;  // x for col and y for row 

	Food food = { 0,0,true };

	//Allocation of memory to a snake
	Snake* snakebody = (Snake*)malloc(sizeof(Snake) * 20);

	//If the assignment fails, send a message to the user that the assignment failed, and exit the code.
	if (snakebody == NULL)
	{
		printf("Allocation failed");
		exit(0);
	}

	//With the first click of the game, as long as the user does not press one of the arrow keys
	//the game will not start and will remain in the loop
	do {
		c = getKey();
	} while (c != RIGHT && c != LEFT && c != DOWN && c != UP);

	snakeappear(snakebody, x, y, snakeSize);//Call to function
	spreadFood(&food, snakebody, snakeSize);//Call to function

	char temp;
	while (c != ESC)
	{
		temp = c;
		Sleep(snakeSpeed);// sleeps for half a second before continuing to the next command

		// only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
			c = getKey();  // change direction

		if (c != RIGHT && c != LEFT && c != DOWN && c != UP && c != ESC)
			c = temp;

		// make a move in current direction if no change
		switch (c)
		{
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case UP:
			y--;
			break;
		case ESC:
			x = 0;
			break;
		}

		//The snake's head is located exactly in the cell where food is found
		if (snakebody[0].x == food.x && snakebody[0].y == food.y)
		{
			food.isEaten = true;   //Remove the food from the screen
			foodEaten--;           //Subtract from the food inventory by 1
			snakeSize++;          //And increase the size of the snake by 1

			//If the food inventory is equal to 0, then print a victory message
			if (foodEaten == 0)
			{
				food.isEaten = false;//Remove the food from the screen
				system("cls");
				c = ESC;

				youWon();//Call to function
				continue;
			}

			//If there is no food on the screen at the moment, go in and call the food scattering function on the board
			if (food.isEaten == true)
			{
				spreadFood(&food, snakebody, snakeSize);//Call to function
			}
		}

		bool hittinghimself = hittingHimSelf(snakebody, x, y, snakeSize);//Compare the return value from the function to the Boolean value

		thesnakemove(snakebody, x, y, snakeSize);//Call to function

		// If the snake hits the board or itself print a loose message
		if (x == COLS - 1 || x == 0 || y == ROWS || y == 0 || hittinghimself == true)
		{
			system("cls");
			c = ESC;

			youLose();//Call to function
			continue;
		}
		gotoxy(26, 81);//Call to function
	}
	free(snakebody);//Memory release
}

//This function will randomly scatter the food to the snake across the field
void spreadFood(Food* food, Snake snakebody[], int snakeSize)
{
	srand((unsigned)time(NULL));//Using the rand function

	int rand1;
	int rand2;
	bool thesnakeisthere = true;
	while (thesnakeisthere == true)
	{
		rand1 = (rand() % (COLS - 2)) + 1;
		rand2 = (rand() % (ROWS - 2)) + 1;
		for (int i = 0; i < snakeSize; i++)
		{
			if (snakebody[i].x == rand1 && snakebody[i].y == rand2)
			{
				continue;
			}
		}
		thesnakeisthere = false;
	}
	food->x = rand1;
	food->y = rand2;
	gotoxy(food->x, food->y);
	printf("$");
	food->isEaten = false;
}

//This function is responsible for the movement of the snake in each iteration
void thesnakemove(Snake snakebody[], int x, int y, int sankeSize)
{
	gotoxy(snakebody[sankeSize - 1].x, snakebody[sankeSize - 1].y);
	printf(" ");

	for (int i = sankeSize - 1; i > 0; i--)
	{
		snakebody[i] = snakebody[i - 1];
		gotoxy(snakebody[i].x, snakebody[i].y);
		printf("@");
	}
	snakebody[0].x = x;
	snakebody[0].y = y;
	gotoxy(snakebody[0].x, snakebody[0].y);
	printf("@");
}

//This function is responsible for the appearance of the snake on the screen
void snakeappear(Snake snakebody[], int x, int y, int snakeSize)
{
	for (int i = 0; i < snakeSize; i++)
	{
		snakebody[i].x = x;
		snakebody[i].y = y;
		gotoxy(snakebody[i].x, snakebody[i].y);
		printf("@");
	}
}

//print the menu of the game for the user to choose
void printmenu()
{
	printf("*******************************************************\n");
	printf("* Pick Your Level, to exit press 0\n");
	printf("* 0 -Exit\n");
	printf("* 1 - Basic Level\n");
	printf("* 2 - Intermidiate Level\n");
	printf("* 3 - Advanced Level\n");
	printf("*******************************************************\n");
}

//Victory message printing function
void youWon()
{
	printf("<<<<<<<<<<<<<<<<<<<->>>>>>>>>>>>>>>>>>>>\n");
	printf("                You Won                 \n");
	printf("<<<<<<<<<<<<<<<<<<<->>>>>>>>>>>>>>>>>>>>\n");
}

//Loss message printing function
void youLose()
{
	printf("<<<<<<<<<<<<<<<<<<<->>>>>>>>>>>>>>>>>>>>\n");
	printf("               GAME OVER                \n");
	printf("<<<<<<<<<<<<<<<<<<<->>>>>>>>>>>>>>>>>>>>\n");
}

//this function prints the entire game board where the snake will move.
void printThegameboard()
{
	system("cls");

	for (int i = 0; i < ROWS; i++)
	{
		gotoxy(0, i);
		printf("#");
		gotoxy(COLS, i);
		printf("#");
	}
	for (int i = 0; i <= COLS; i++)
	{
		gotoxy(i, 0);
		printf("#");
		gotoxy(i, ROWS);
		printf("#");
	}
}

// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

//A function that checks at any moment a side of the snake, if it has hit itself.
bool hittingHimSelf(Snake snakebody[], int x, int y, int snakeSize)
{
	for (int i = 1; i < snakeSize; i++)
	{
		if (snakebody[i].x == x && snakebody[i].y == y)
		{
			return true;
		}
	}
	return false;
}
