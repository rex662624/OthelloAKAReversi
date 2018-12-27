#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**************************************************
*   border有做padding,所以變成10*10                 *
*   empty地方用0 black方的棋子用1,white用2,牆壁用3    *
**************************************************/

#define BOARDSIZE 100
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define OUTER 3
//====================此處放置redundant code=====

//=========================
void othello (int (* ) (int, int *), int (* ) (int, int *), int);//下棋function
int human(int, int *);   //人類下棋
int AI(int, int *);      //ai下棋
int *initialboard(void); //初始化board,return的是board array

int main(void)
{
    //printflag用來是否要印出盤面，這裡預設都印出來,printflag=1
    int HumanIsFirst = 0, printflag = 1;
    do
    {
        printf("您要先手請輸入1,否則輸入2:  ");
        scanf("%d", &HumanIsFirst);
    } while (HumanIsFirst != 1 && HumanIsFirst != 2);

    if (HumanIsFirst == 1)
        othello(human, AI, printflag);
    else
        othello(AI, human, printflag);
}

int *initialboard(void)
{
    int i, *board;
    board = (int *)malloc(BOARDSIZE * sizeof(int));
    for (i = 0; i <= 9; i++)
        board[i] = OUTER; //外框用3表示
    for (i = 10; i <= 89; i++)
    {
        if (i % 10 >= 1 && i % 10 <= 8)
            board[i] = EMPTY;
        else
            board[i] = OUTER;
    }
    for (i = 90; i <= 99; i++)
        board[i] = OUTER;
    board[44] = WHITE;
    board[45] = BLACK;
    board[54] = BLACK;
    board[55] = WHITE;
    return board;
}

void othello(int (*blstrategy)(int, int *), int (*whstrategy)(int, int *), int printflag)
{ //參數：黑方的function,白方的function,是否每回合要印出
    int *board;
    int player;
    board = initialboard();
    blstrategy(1,board);
    whstrategy(2,board);

}

int human(int player, int *board)
{
    int move;
    printf("your turn to move:");
    scanf("%d", &move);
    return move;
}

//projection to  maxweighteddiffstrategy

int AI(int player, int *board)
{
    printf("AI move:\n");
    return 0;
}
