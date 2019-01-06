#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

/**************************************************
*   border有做padding,所以變成10*10                 *
*   empty地方用0 black方的棋子用1,white用2,牆壁用3    *
**************************************************/
#define WIN 2000
#define LOSS -2000
#define BOARDSIZE 100
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define OUTER 3

//-11:向左上（因為一排10個人） -10：向上 -9：右上
const int ALLDIRECTIONS[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int NEWmaxchoice (int, int * , int , int (* evalfn) (int, int *),int alpha,int beta);
int NEWminchoice (int, int * , int , int (* evalfn) (int, int *),int alpha,int beta);
int NEWminmax (int player, int * board, int ply, int (* evalfn) (int, int *));
void makemove(int, int, int *);
int *FindAllMoves(int, int *);
int wouldflip(int, int, int *, int);
int LegalPosition(int, int, int *);
int anylegalmove(int, int *);
char nameof(int);
int CountChessNumber(int, int *);
void printboard(int *);
int nexttoplay(int *, int,int);
int opponent(int);
void othello(int (*)(int, int *), int (*)(int, int *));
void getmove(int (*)(int, int *), int, int *);
int human(int, int *);
int AI(int, int *);
int *initialboard(void);
int * copyboard (int * board);
int diffeval (int player, int * board);
//====================此處放置redundant code=====

//=========================

int main(void)
{

    srand(time(NULL));
    int HumanIsFirst = 0;
    do
    {
        printf("您要先手請輸入1,否則輸入2:  ");
        scanf("%d", &HumanIsFirst);
    } while (HumanIsFirst != 1 && HumanIsFirst != 2);

    if (HumanIsFirst == 1)
        othello(human, AI);
    else
        othello(AI, human);
}

//========================主要運作程式
//下棋function
void othello(int (*blstrategy)(int, int *), int (*whstrategy)(int, int *))
{ //參數：黑方的function,白方的function,是否每回合要印出
    int *board;
    int player;
    board = initialboard();
    //player = BLACK WHITE: 黑/白方走 0:終局盤面
    player = BLACK; //一開始黑子先走
    do
    {
        if (player == BLACK)
            getmove(blstrategy, BLACK, board); //換黑子走的話
        else
            getmove(whstrategy, WHITE, board); //換白子走
        player = nexttoplay(board, player,1);
    } while (player != 0); //終局盤面,player = 0

    printf("The game is over. Final result:\n");
    printboard(board);
}
//人類下棋
int human(int player, int *board)
{
    /*
    int move;
    printf("your turn to move:");
    scanf("%d", &move);
    return move;
    */
    int move;
    char* moveHuman=(char*) malloc(10);
    printf("your turn to move:");
    scanf("%s",moveHuman);
    while(strlen(moveHuman)!=2){
        printf("your turn to move:");
        scanf("%s",moveHuman);
    }
    move =(moveHuman[1]-48)*10+moveHuman[0]-64;
    return move;
    //AI(player,board);
}
//================================AI
int diffeval (int player, int * board) { /* utility is measured */
  int i, ocnt, pcnt, opp;                /* by the difference in */
  pcnt=0; ocnt = 0;                      /* number of pieces */
  opp = opponent(player); 
  for (i=1; i<=88; i++) {
    if (board[i]==player) pcnt++;
    if (board[i]==opp) ocnt++;
  }
  return (pcnt-ocnt);
}

int * copyboard (int * board) {
  int i, * newboard;
  newboard = (int *)malloc(BOARDSIZE * sizeof(int));
  for (i=0; i<BOARDSIZE; i++) newboard[i] = board[i];
  return newboard;
}

int weighteddiffeval (int player, int * board) {
  int i, ocnt, pcnt, opp;
  const int weights[100]={0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                          0,120,-20, 20,  5,  5, 20,-20,120,  0,
                          0,-20,-40, -5, -5, -5, -5,-40,-20,  0,
                          0, 20, -5, 15,  3,  3, 15, -5, 20,  0,
                          0,  5, -5,  3,  3,  3,  3, -5,  5,  0, 
                          0,  5, -5,  3,  3,  3,  3, -5,  5,  0, 
                          0, 20, -5, 15,  3,  3, 15, -5, 20,  0,
                          0,-20,-40, -5, -5, -5, -5,-40,-20,  0,
                          0,120,-20, 20,  5,  5, 20,-20,120,  0,
                          0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
  pcnt=0; ocnt=0;      
  opp = opponent(player);
  for (i=1; i<=88; i++) {
     if (board[i]==player) pcnt=pcnt+weights[i];
     if (board[i]==opp) ocnt=ocnt+weights[i];
  }
  return (pcnt - ocnt);
}


int NEWminmax (int player, int * board, int ply, int (* evalfn) (int, int *)) {//evalfn:addweight 或是 diffweight


  int i, max, ntm, newscore, bestmove, * moves, * newboard;
  int maxchoice (int, int *, int, int (*) (int, int *)); 
  int minchoice (int, int *, int, int (*) (int, int *)); 
  moves = FindAllMoves(player, board); /* get all legal moves for player */
  max = LOSS - 1;  /* any legal move will exceed this score */

  for (i=1; i <= moves[0]; i++) {//moves[0]存有幾個位置可以移動

    newboard = copyboard(board);//準備一個複製的board準備simulate

    makemove(moves[i], player, newboard);//在新盤面移動這一步看看
    ntm = nexttoplay(newboard, player,0);//看下一個人是誰
    if (ntm == 0) {  /* game over, so determine winner */
         newscore = diffeval(player, newboard);
         if (newscore > 0) newscore = WIN; /* a win for player */
         if (newscore < 0) newscore = LOSS; /* a win for opp */
    }
    if (ntm == player)   /* opponent cannot move *///下一個人又是我
       newscore = NEWmaxchoice(player, newboard, ply-1, evalfn,-2000,2000);
    if (ntm == opponent(player))//下一個人是對方,要找min
       newscore = NEWminchoice(player, newboard, ply-1, evalfn,-2000,2000);
    
    if (newscore > max) {
        max = newscore;
        bestmove = moves[i];  /* a better move found */
    }
    free(newboard);
  }
  free(moves);
  return(bestmove);
}

int NEWminchoice (int player, int * board, int ply, int (* evalfn) (int, int *),int alpha,int beta) {
  int i, min, ntm, newscore, * moves, * newboard;
  if (ply == 0) return((* evalfn) (player, board));
  moves =FindAllMoves(opponent(player), board);

  min = beta;

  for (i=1; i <= moves[0]; i++) {
    newboard = copyboard(board);
    makemove(moves[i], opponent(player), newboard);
    ntm = nexttoplay(newboard, opponent(player),0);
    if (ntm == 0) {
         newscore = diffeval(player, newboard);
         if (newscore > 0) newscore = WIN;
         if (newscore < 0) newscore = LOSS;
    }
    if (ntm == player) 
       newscore = NEWmaxchoice(player, newboard, ply-1, evalfn,alpha,min);
    if (ntm == opponent(player))
       newscore = NEWminchoice(player, newboard, ply-1, evalfn,alpha,min);
    if (newscore < min) min = newscore;
    if(min<=alpha)return(min) ;
    free(newboard);
  }
  free(moves);
  return(min);
}

int NEWmaxchoice (int player, int * board, int ply, int (* evalfn) (int, int *),int alpha,int beta) {

  int i, max, ntm, newscore, * moves, * newboard;
  int minchoice (int, int *, int, int (*) (int, int *)); 
  if (ply == 0) return((* evalfn) (player, board));//向前看的步數看完了，去看看到那裡的時候，盤面好不好
  moves = FindAllMoves(player, board);
  max = alpha;

  for (i=1; i <= moves[0]; i++) {
    newboard = copyboard(board);
    makemove(moves[i], player, newboard);
    ntm = nexttoplay(newboard, player,0);
    if (ntm == 0) {
         newscore = diffeval(player, newboard);
         if (newscore > 0) newscore = WIN;
         if (newscore < 0) newscore = LOSS;
    }
    if (ntm == player) 
       newscore = NEWmaxchoice(player, newboard, ply-1, evalfn,max,beta);
    if (ntm == opponent(player))
       newscore = NEWminchoice(player, newboard, ply-1, evalfn,max,beta);
    if (newscore > max) max = newscore;
    if(max>=beta)return(max) ;
    free(newboard);
  }
  free(moves);
  return(max);
}



//projection to  maxweighteddiffstrategy
//ai下棋
int AI(int player, int *board)
{   /*
    int r, *moves;
    moves = FindAllMoves(player, board);
    r = moves[(rand() % moves[0]) + 1];
    free(moves);
    return (r);*/
    return(NEWminmax(player, board,6, weighteddiffeval));
}

//把所有可以移動的位置找出來並存在return array moves中
//其中moves[0]存全部有幾個位置可以移動
int *FindAllMoves(int player, int *board)
{
    int move, i, *moves;
    moves = (int *)malloc(65 * sizeof(int)); //最多就是64個可以移動的步
    moves[0] = 0;
    i = 0;
    for (move = 11; move <= 88; move++)
        if (LegalPosition(move, player, board))
        {
            i++;
            moves[i] = move;
        }
    moves[0] = i; //0的位置存總共有幾個位置可以移動
    return moves;
}

//========================棋局規則相關
//下子function, 下的strategy,player是誰,board,是否要印出
void getmove(int (*strategy)(int, int *), int player, int *board)
{
    int move;
    printboard(board);
    move = (*strategy)(player, board); //下子
    if (LegalPosition(move, player, board))
    { //如果他下的是正常的子
        printf("%c moves to %d (%c%d)\n", nameof(player), move,'A'+(move%10)-1,move/10);
        makemove(move, player, board);
    }
    else //如果他下的不是正常的位置 重下
    {
        printf("Illegal move,please move again\n");
        getmove(strategy, player, board);
    }
}
//下子的動作,出來做function的原因是後面的AI有可能會用到試試看下子的動作
//move代表下的位置，player代表下的人
void makemove(int move, int player, int *board)
{
    int i;
    board[move] = player;
    //以我為邊界，總共有8個方向可以翻（move---o或o---move......）
    for (i = 0; i <= 7; i++)
    {

        int FlipToPosition, c;
        //wouldflip()會return要翻到哪個位置
        FlipToPosition = wouldflip(move, player, board, ALLDIRECTIONS[i]);
        if (FlipToPosition)
        { //如果這個方向是可以翻的
            //開始做翻的動作
            c = move + ALLDIRECTIONS[i];
            do
            {
                board[c] = player;
                c = c + ALLDIRECTIONS[i];
            } while (c != FlipToPosition);
        }
    }
}

//========================初始化和小功能
//初始化board,return的是board array
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
//算下一個要玩的人是誰
int nexttoplay(int *board, int Nowplayer,int printflag)
{
    int NowOpp;
    NowOpp = opponent(Nowplayer); //換人下
    if (anylegalmove(NowOpp, board))
        return NowOpp; //如果對方可以下，一定要給他下
    if (anylegalmove(Nowplayer, board))
    { //會到這裡表示對方不可以下，再換我下

        if (printflag) printf("%c has no moves and must pass.\n", nameof(NowOpp));
        return Nowplayer;
    }
    return 0; //沒人可以下，終局
}

//算我對手是誰
int opponent(int player)
{ //我的對手是誰
    if (player == 1)
        return 2;
    else if (player == 2)
        return 1;
    else
    {
        printf("illegal player\n");
        return 0;
    }
}
//印出盤面
void printboard(int *board)
{
    int row, col;
    printf("     1 2 3 4 5 6 7 8 [black=%d white=%d]\n     A B C D E F G H\n", CountChessNumber(BLACK, board), CountChessNumber(WHITE, board));
    for (row = 1; row <= 8; row++)
    {
        printf("%d %c  ", row, row + 64); //ascii: '1'+16='A'
        for (col = 1; col <= 8; col++)
            printf("%c ", nameof(board[col + (10 * row)]));
        printf("\n");
    }
}
//算player共有多少子在場上
int CountChessNumber(int player, int *board)
{
    int i, count = 0;
    for (i = 1; i <= 88; i++)
        if (board[i] == player)
            count++;
    return count;
}
//印出盤面是 b w .
char nameof(int player)
{
    //.代表0的位置,是empty. 1代表black 2代表white,3代表牆壁
    static char piecenames[5] = ".bw#";

    return (piecenames[player]);
}

//是否盤面內有任何位置是player可以移動的
int anylegalmove(int player, int *board)
{
    int move;
    move = 11;
    while (move <= 88 && !LegalPosition(move, player, board))
        move++;
    if (move <= 88)
        return 1;
    else
        return 0;
}

//看看移動的位置move是否合法 參數: 要移動的位置,移動的人,盤面
//條件：必須是可以把別人框起來的位置，我才能下子
int LegalPosition(int move, int player, int *board)
{
    int i = 0;
    if (!((move >= 11) && (move <= 88) && (move % 10 >= 1) && (move % 10 <= 8)))
        return 0;             //表示要移動的位置是牆壁
    if (board[move] == EMPTY) //表示要移動的位置沒人
    {
        while (i <= 7 && !wouldflip(move, player, board, ALLDIRECTIONS[i]))
            i++; //合法的移動必須要是我可以把其他人框起來的那一步
        if (i == 8)
            return 0; //8個方向都沒有可以把人框起來,表示這個移動位置不合法
        else
            return 1; //在8之前就停下來了
    }
    else
        return 0;
}

//看看這個位置是否是可以把別人翻起來的位置
//參數： 下子位置,下子的人,盤面,要看的方向
int wouldflip(int move, int player, int *board, int dir)
{
    int current;
    current = move + dir;
    if (board[current] == opponent(player)) //如果我的下一個人是對方，表示這個方向可能可以翻
    {
        while (board[current] == opponent(player))
            current = current + dir; //如果是對方，就一直加，直到不是對方
        if (board[current] == player)
            return current; //如果不是別人後之後是我表示我把它圍起來
        else
            return 0; //如果不是我表示撞牆，並沒有圍起來
    }
    else
        return 0;
}

//=============================