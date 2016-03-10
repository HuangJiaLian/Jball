/*Jball2d05.c 实现了：
 * 一个小球在边框周围弹
 * 挡板的自动移动 空格反向 f加速 s减速
 * 球和挡板用了同一个定时器，挡板快球也快
 * 判断球是否出界 
 * 记分
 *Linux系统需要安装curses库
 *编译: gcc Jball2d05.c set_ticker.c -lcurses -o Jball2d05
 *运行: ./Jball2d05
 *状态: 更新中
 * */
#include<curses.h>
#include<signal.h>
#include<sys/time.h>
#include<stdlib.h>
#include<termios.h>
#define BALL 'o'
#define BALL_BLANK ' '
#define	BAN	    "=========="
#define BAN_BLANK   "          "

void jmove();
void welcom();
int pos_x=5;
int pos_y=5;
int pos_xo;
int pos_yo;
int dir_x=1;
int dir_y=1;            //球的属性

int bpos_x; 
int bpos_y;		
int bpos_xo;
int bpos_yo;		
int bdir_y=2;		//挡板的属性

int c;			//用户输入
int delay;
int ndelay;		

int scores=0;		//分数

int main()
{
	welcom();
	tty_mode(0);//储存TTY的模式
	initscr();
	crmode();
	noecho();
	clear();
	box(stdscr,'|','-');
	mvaddch(pos_x,pos_y,BALL);
	bpos_x=LINES-4;
	bpos_y=4;
	mvaddstr(bpos_x,bpos_y,BAN);
	move(LINES-1,COLS-1);			//移动光标到右下角
	refresh();
	signal(SIGALRM,jmove);
	delay=50;	//200ms=0.2s
	if(set_ticker(delay) == -1)
		perror("set_ticker");
	else
		while(1)
		{
			ndelay = 0;
			c=getch();
			if(c=='Q') break;
			if(c=='f' && delay > 2) ndelay = delay/2;
			if(c=='s') ndelay = delay*2;
			if(c==' ') bdir_y=-bdir_y;
			if(ndelay > 0) set_ticker (delay = ndelay);
			//重置定时器
		}
}

void jmove()
{
	pos_xo=pos_x;
	pos_yo=pos_y;

	bpos_xo=bpos_x;
	bpos_yo=bpos_y;
	if(pos_x==(LINES-5)&&pos_y>(bpos_y-1)&&pos_y<(bpos_y+11)) 
	{
		scores+=5;
		dir_x=-dir_x;//碰到挡板就反向
	}
	if(pos_x<2)	dir_x=-dir_x;
	/*if((pos_x<2)||(pos_x>LINES-3))	dir_x=-dir_x;*/
	if((pos_y<2)||(pos_y>COLS-3))	dir_y=-dir_y;
	pos_x+=dir_x;
	pos_y+=dir_y;
	mvaddch(pos_xo,pos_yo,BALL_BLANK);	
	mvaddch(pos_x,pos_y,BALL);
	if(pos_x>LINES-1)	
	{
		tty_mode(1);//恢复tty的模式,否则退出程序后，终端没有回显功能，因为这个程序把终端的属性修改了
		if(scores==0)
		printf("\nScores:%d\nCome on kid,try again!\n",scores);
		else if(scores>=5&&scores<20)
			printf("\nScores:%d\nYou did good job!\n",scores);
		else if(scores>=20&&scores<40)
			printf("\nScores:%d\nGreat,have I told you how to do this?\n",scores);
		else if(scores>=40&&scores<60)
			printf("\nScores:%d\nYou are genius,congratulations!\n",scores);
		else if(scores>=60)
			printf("\nScores:%d\nI love you so much:)!\n",scores);
		exit(0);
	}
	//挡板的移动
	if(bpos_y<3||bpos_y==3||bpos_y>COLS-12) 	bdir_y=-bdir_y;
	bpos_y+=bdir_y;
	mvaddstr(bpos_xo,bpos_yo,BAN_BLANK);
	mvaddstr(bpos_x,bpos_y,BAN);
        //光标放在右下角
	move(LINES-1,COLS-1);
	refresh();
}

tty_mode(int how)
{
	static struct termios original_mode;
	if(how == 0)
	{
		tcgetattr(0,&original_mode);
	}
	else
	{
		return tcsetattr(0,TCSANOW,&original_mode);
	}
}

void welcom()
{
	printf("     _ _           _ _  \n");
	printf("    | | |__   __ _| | |\n"); 
	printf(" _  | | '_ \\ / _` | | |\n");
        printf("| |_| | |_) | (_| | | |\t\tHint:\n"); 	
	printf(" \\___/|_.__/ \\__,_|_|_|\t\tUse SPACE key to control\n");
	printf("3...\n");
	sleep(1);
	printf("2..\n");
	sleep(1);
	printf("1.\nHave fun!\n");
	sleep(1);
}
