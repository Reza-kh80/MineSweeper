#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

typedef struct Cell {
    int IsBomb;
    int IsFlag;
    int IsOpen;
    int Num;
} Cell;

int EndGame;
int Score;
char Name[30];

void PrintNum(int num)
{
    char buf[4];

    if (num == 0)
    {
        printf("  .");
    }
    else
    {
        sprintf(buf, "%3d", num);
        printf(buf);
    }
}

int IsInBoard (int x, int y, int n)
{
    if ((x >= 0 && x < n) && (y >= 0 && y < n))
    {
        return 1;
    }
    return 0;
}

void CountMines(int x, int y, int n, struct Cell GameCell[n][n])
{
    int i, j;

    for (i=-1; i<2; i++)
    {
        for (j=-1; j<2; j++)
        {
            if (IsInBoard(x + i, y + j, sizeof(GameCell)) == 1 && (i != 0 || j != 0))
            {
                if (GameCell[x + i][y + j].IsBomb > 0)
                {
                    GameCell[x][y].Num++;
                }
            }
        }
    }
}

void CheckWinner(int n, struct Cell GameCell[n][n]){
    int i, j;
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            if (GameCell[i][j].IsBomb == 0 && GameCell[i][j].IsOpen == 0)
            {
                return;
            }
        }
    }
    Score = n*n;
    EndGame = 1;
}

void PrintGame(int n ,struct Cell GameCell[n][n])
{
    int i, j;
    char buf[4];

    system("cls");
    printf("\n\n");
    printf("      ");
    for (i=0; i<n; i++)
    {
        sprintf(buf, "%3d", i+1);
        printf(buf);
    }
    printf("\n\n");
    for (i=0; i<n; i++)
    {
        sprintf(buf, "%3d   ", i+1);
        printf(buf);

        for (j=0; j<n; j++)
        {
            if (EndGame != 0)
            {
                if (GameCell[i][j].IsBomb == 1)
                {
                    printf("  *");
                }
                else
                {
                    PrintNum(GameCell[i][j].Num);
                }
            }
            else
            {
                if (GameCell[i][j].IsFlag == 1)
                {
                    printf("  F");
                }
                else if (GameCell[i][j].IsOpen == 1)
                {
                    PrintNum(GameCell[i][j].Num);
                }
                else
                {
                    printf("  -");
                }
            }
        }
        printf("\n");
    }
    printf("\n\nPlayer Name:%s --- Score: %d\n\n", Name, Score);
    if (EndGame == 1)
    {
        printf("\n\n********** YOOHOO! YOU WON THE GAME **********\n\n\a");
        system("color f2");
    }
    else if (EndGame == -1)
    {
        printf("\n\n********** GAME OVER **********\n\n\a");
        system("color f4");
    }
}

void OpenCell(int x, int y, int n, struct Cell GameCell[n][n])
{
    int i ,j;
    GameCell[x][y].IsOpen = 1;
    GameCell[x][y].IsFlag = 0;
    if (GameCell[x][y].Num == 0)
    {
        for (i=-1; i<2; i++)
        {
            for (j=-1; j<2; j++)
            {
                if (IsInBoard(x + i, y + j, n))
                {
                    if (i != 0 || j != 0)
                    {
                        if (GameCell[x + i][y + j].IsOpen == 0 && GameCell[x + i][y + j].IsFlag == 0)
                        {
                            OpenCell(x + i, y + j, sizeof(GameCell), GameCell);
                        }
                    }
                }
            }
        }
    }
    Score++;
}
int main()
{
    system("color 0e");
    int i, j;
    char a;
    int selectedX, selectedY;
    char buf[4];

    system("color f0");
    printf("Hello! Please enter your Name:\n");
    gets(Name);
    printf("\n\nWelcome %s!\n\n", Name);
    printf("1. 5*5 Wiht 10 bombs\n");
    printf("2. 10*10 Wiht 20 bombs\n");
    printf("3. 15*15 Wiht 80 bombs\n");
    printf("4. 15*15 Wiht 100 bombs\n\n\n");
    printf("Choose the best option :\n");
    int answer = scanf("%d", &answer);
    int n;
    int bomb;
    if(answer == 1) {
        bomb = 10;
        n = 5;
    }
    else if(answer == 2) {
        bomb = 20;
        n = 10;
    }
    else if(answer == 3) {
        bomb = 80;
        n = 15;
    }
    else if(answer == 4) {
        bomb = 100;
        n = 15;
    }
    struct Cell GameCell[n][n];
    loop1:
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            GameCell[i][j].IsBomb = 0;
            GameCell[i][j].IsFlag = 0;
            GameCell[i][j].IsOpen = 0;
            GameCell[i][j].Num = 0;
        }
    }
    EndGame = 0;
    Score = 0;
    system("color f0");

    gets(buf);
    printf("\n\nLet's start the game :-)\n\nReady? (Press Enter)\n");
    gets(buf);

    srand(time(0));
    for (int i = 0; i < bomb; i++)
    {
        int random = rand() % (n*n);
        GameCell[random/n][random%n].IsBomb = 1;
    }
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            CountMines(i, j, n, GameCell);
        }
    }
    while(1)
    {
        CheckWinner(n, GameCell);
        PrintGame(n, GameCell);
        if (EndGame != 0)
        {
            printf("\nWould you like to play again? (y/n) ");
            gets(buf);
            if(tolower(buf[0]) == 'y')
            {
                system("cls");
                goto loop1;
            }
            else
                return 0;
        }

        printf("\n\nSelect a cell:\n");
        selectedX=0;
        selectedY=0;
        while(selectedX <= 0 || selectedX > n)
        {
            printf("X: ");
            gets(buf);
            a = sscanf(buf, "%d", &selectedX);
            if (a > 0 && selectedX > 0 && selectedX <= n)
            {
                while(selectedY <= 0 || selectedY > n)
                {
                    printf("Y: ");
                    gets(buf);
                    a = sscanf(buf, "%d", &selectedY);
                    if (a > 0 && selectedY > 0 && selectedY <= n)
                    {
                        break;
                    }
                }
                break;
            }
        }
        while(1)
        {
            printf("O=Open, F=Flag: ");
            gets(buf);
            if (tolower(buf[0]) == 'o')
            {
                if (GameCell[selectedX - 1][selectedY - 1].IsFlag == 1)
                {
                    printf("\nAre you sure? (Y / N): ");
                    while(1)
                    {
                        gets(buf);
                        if (tolower(buf[0]) == 'y')
                        {
                            if (GameCell[selectedX - 1][selectedY - 1].IsBomb == 1)
                            {
                                EndGame = -1;
                            }
                            else
                            {
                                OpenCell(selectedX - 1, selectedY - 1, GameCell, n);
                            }
                            break;
                        }
                        else if (buf[0] == 'n' || buf[0] == 'N')
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if (GameCell[selectedX - 1][selectedY - 1].IsBomb == 1)
                    {
                        EndGame = -1;
                    }
                    else
                    {
                        OpenCell(selectedX - 1, selectedY - 1, GameCell, n);
                    }
                }
                break;
            }
            else if (tolower(buf[0]) == 'f')
            {
                if (GameCell[selectedX - 1][selectedY - 1].IsOpen == 0)
                {
                    GameCell[selectedX - 1][selectedY - 1].IsFlag = 1;
                }
                break;
            }
        }
    }

    return 0;
}



