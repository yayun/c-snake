#include<stdio.h>
#include<curses.h>//use curses library
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>//Define alarm

typedef struct point{
    int x,y;
} point;

typedef struct node {
    int x,y;
    struct node * next;
    struct node * back;
} node;

void create_link();
void insert_node(int,int);
void delete_node();   
int get_station();
void show_snake();
void init();

node *talloc(void)
{
    return (node *)malloc(sizeof(node));
}
point food,pt;
node *head,*tail;
int length;
int main()
{
    init();
    signal(SIGALRM,show_snake);
    get_station();
}
void init()
{
    initscr();//初始化屏幕 不过现在我们并没有建立自己的窗口 用的是标准屏幕 类似于c的stdin 
    cbreak();
    noecho();//使你在键盘中输入的字符不可见
    curs_set(0);//使光标不可见
    keypad(stdscr, true);   
    box(stdscr,ACS_VLINE,ACS_HLINE);//画一个框
    pt.x=1;
    pt.y=0;
    length=1;
    food.y= rand() % COLS;
    food.x= rand() % (LINES-2) + 2;
    create_link();
    alarm(1);
}           
void show_snake()
{
    bool len_increase=false;
    insert_node(head->next->x+pt.x,head->next->y+pt.y);
    move(food.x,food.y);
    printw("@");
    if(head->next->x==food.x && head->next->y==food.y)
    {
        length++;
        len_increase=true;
        food.y = rand() % COLS;
        food.x = rand() % (LINES+2) + 2;//刚开始设置的食物的位置是初始化的位置 现在要重新设置食物的位置
    }
    if(!len_increase)
    {
        move(tail->back->x, tail->back->y);
        waddstr(stdscr," ");
        delete_node();
    }
    if(length>1)
    {
        move(head->next->next->x,head->next->next->y);
        waddstr(stdscr,"@");
    }
    move(head->next->x,head->next->y);
    waddstr(stdscr,"#");
    refresh();
    alarm(1);
}
int get_station()
{
  while(1)  
  {   
    int ch=getch();//KEY_LEFT 是在curses中的宏定义 是int型数据
    if (KEY_LEFT==ch)
    {
        pt.x=0;
        pt.y=-1;
    }
    else if (KEY_RIGHT==ch)
    {
        pt.x=0;
        pt.y=1;
    }
    else if (KEY_UP==ch)
    {
        pt.x=-1;
        pt.y=0;
    }
    else if (KEY_DOWN==ch)
    {
        pt.x=1;
        pt.y=0;
    }
  }  
}
void create_link()
{
    head=tail=talloc();
    node *t;
    t=talloc();
    t->x=3;
    t->y=4;
    head->back =NULL;
    tail->next =NULL;
    head->next = t;
    t->next = tail;
    tail->back = t;
    t->back = head; 
}
void insert_node(int x,int y)//蛇每前进一步增加一个链表节点
{
    node *t;
    t=talloc();
    t->next=head->next;
    head->next=t;
    t->back=head;
    t->next->back=t;
    t->x=x;
    t->y=y;//在head和head->next 中插入t 
}
void delete_node()
{
    node *t= tail->back;
    t->back->next = tail;
    tail->back =  t->back;
    free(t);
    t= NULL;
}

