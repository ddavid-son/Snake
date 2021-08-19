#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct snake {
	int x, y;
	struct snake* next;
}SNAKE;


#define BORDSIDE 76
#define BORDUP 26

// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
// "\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
// but rather a command to the terminal (or most likely terminal emulatoion).

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs);


void printboard();

void printsnake(SNAKE* psnake);

SNAKE* addheadlink(int x, int y, SNAKE* prevhead);

void removetaillink(SNAKE* psnake);

void deletesnaketail(SNAKE* psnake);

bool isgameover(SNAKE* psnake);

void kbmove(int *x, int* y, char *wheretomove);

void defnextmove(int *x, int* y, char *wheretomove);

void freesnake(SNAKE* psnake);

int main()
{
	bool gameover = false;
	SNAKE* psnake = (SNAKE*)malloc(1 * sizeof(SNAKE));
	int x, y, movescount = 0;
	char wheretomove = 'd';
	float sec = 1;

	psnake->x = BORDUP / 2;
	psnake->y = BORDSIDE / 2;

	x = psnake->x;
	y = psnake->y;
	psnake->next = NULL;


	init();
	printboard();
	printsnake(psnake);


	while (gameover == false)
	{

		if (_kbhit())
			kbmove(&x, &y, &wheretomove);
		else
			defnextmove(&x, &y, &wheretomove);

		psnake = addheadlink(x, y, psnake);

		gameover = isgameover(psnake);
		if (gameover)
			break;

		if (movescount % 5 == 0 && movescount != 0)
		{
			if (sec > 0.1)
				sec = sec - 0.03;
		}
		else
		{
			deletesnaketail(psnake);
			removetaillink(psnake);
		}

		movescount++;

		printsnake(psnake);

		sleep(sec);
	}


	init();
	printboard();
	gotoxy(13, 34);
	printf("GAME OVER!");
	freesnake(psnake);
	free(psnake);


	gotoxy(28, 0);
}

// Turn escape codes ON.
void init(void) {
	system("cls");
}

// moves the cursor to position (x,y) on screen.
// parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y)
{
	printf("\x1b[%d;%df", x, y);
}

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs*CLOCKS_PER_SEC);

}

//prints the snake
void printsnake(SNAKE* psnake)
{
	while (psnake)
	{
		gotoxy(psnake->x, psnake->y);
		printf("@");
		psnake = psnake->next;
	}

	gotoxy(0, 80);
}

//brint the board
void printboard()
{
	for (int i = 0; i <= BORDUP; i++)
	{

		for (int j = 0; j <= BORDSIDE; j++)
		{
			if (i == 0 || i == BORDUP)
				printf("#");
			else
			{
				if (j == 0 || j == BORDSIDE)
					printf("#");
				else
					printf(" ");
			}
		}
		printf("\n");
	}
}

//adds new head to the snake
SNAKE* addheadlink(int x, int y, SNAKE* prevhead)
{
	SNAKE* newhead = NULL;

	newhead = (SNAKE*)malloc(sizeof(SNAKE));
	newhead->x = x;
	newhead->y = y;
	newhead->next = prevhead;

	return newhead;

}

//frees the snakes tail from the link
void removetaillink(SNAKE* tailremove)
{
	while (tailremove)
	{
		if (tailremove->next == NULL)
			tailremove = NULL;

		if (tailremove->next->next != NULL)
			tailremove = tailremove->next;
		else
		{
			free(tailremove->next);
			tailremove->next = NULL;
			tailremove = tailremove->next;
		}
	}
}

//delets snakes tail from the screen
void deletesnaketail(SNAKE* psnake) {

	while (psnake)
	{
		if (psnake->next == NULL)
		{
			gotoxy(psnake->x, psnake->y);
			printf(" ");
		}
		psnake = psnake->next;
	}
}

// checkes if snake cross borders or is overlapping
bool isgameover(SNAKE* psnake)
{
	SNAKE* body = psnake->next;

	if (psnake->x == BORDUP + 1 || psnake->x == 1 || psnake->y == BORDSIDE + 1 || psnake->y == 1)
		return true;

	while (body)
	{
		if (psnake->x == body->x && psnake->y == body->y)
			return true;
		body = body->next;
	}

	return false;
}

// makes the move incase keyboardhit is registerd
void kbmove(int* x, int* y, char *wheretomove)
{
	char temp;

	temp = _getch();

	if (temp == 'a' || temp == 'w' || temp == 's' || temp == 'd')
	{
		*wheretomove = temp;
		switch (*wheretomove)
		{
		case 'a':
			(*y)--;
			break;
		case 'w':
			(*x)--;
			break;
		case 's':
			(*x)++;
			break;
		case 'd':
			(*y)++;
			break;
		default:
			break;
		}
	}
	else
		defnextmove(x, y, wheretomove);
}

// makes the move if keyboard hit did not registerd or a non "awsd" char was registerd 
void defnextmove(int *x, int* y, char *wheretomove)
{
	switch (*wheretomove)
	{
	case 'a':
		(*y)--;
		break;
	case 'w':
		(*x)--;
		break;
	case 's':
		(*x)++;
		break;
	case 'd':
		(*y)++;
		break;
	default:
		break;
	}
}

//free snake's links memory
void freesnake(SNAKE* psnake)
{
	if (psnake->next->next)
		freesnake(psnake->next);

	free(psnake->next);
	psnake->next = NULL;

}
