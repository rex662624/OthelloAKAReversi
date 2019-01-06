#include"stdio.h"
#include"string.h"
#include"stdlib.h"
int main(){
	/*	
	//=========================
	int move;
	int printS[78];
	int i =0;
	char temp[78];
	for(move=11;move<=88;move++){
		//printf("%c%d  ", 'A'+(move%10)-1,move/10);
		sprintf(temp,"%c%d",'A'+(move%10)-1,move/10);
		printf("%s ",temp);
		printS[i]=(temp[1]-48)*10+temp[0]-64;
		printf("%d ",printS[i++]);
		if(move%10==0)printf("\n");
	}
	printf("\n\n");
	//=========================
	for(i=1;i<sizeof(printS)/4+1;i++)
		{
		printf("%d ",printS[i-1]);
		if(i%8==0)printf("\n");
		}
	//=========================
	char* moveHuman=(char*) malloc(10);
	scanf("%s",moveHuman);
	printf("%ld\n",strlen(moveHuman));
	while(strlen(moveHuman)!=2)
	{	
		printf("your turn to move:");
		scanf("%s",moveHuman);
	}
	printf("%d%d\n",moveHuman[1]-48,moveHuman[0]-64);
	*/
	int move;
    char* moveHuman=(char*) malloc(10);
	while(1){
    printf("your turn to move:");
    scanf("%s",moveHuman);
    while(strlen(moveHuman)!=2){
        printf("your turn to move:");
        scanf("%s",moveHuman);
    }
    move =(moveHuman[1]-48)*10+moveHuman[0]-64;
	printf("%d\n",move);
	}
}
