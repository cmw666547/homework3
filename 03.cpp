#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define MAX 1000
using namespace std;
typedef char elemtype;//任务类型
typedef struct node//建立项目任务结点,因为涉及到项目的删除所以我先用链表存储项目任务
{
	elemtype project;
	struct node *next;
}node;
typedef struct arc//图中的弧
{
	int tailvex,headvex;//弧尾和弧头结点的位置
	struct arc *next;//指向下一个弧
}arc;
typedef struct vex//图中的顶点
{
	elemtype data;
	int rudu;
	arc *firstin,*firstout;//和该顶点相连的第一条入弧和第一条出弧
}vex;
typedef struct//以十字链表为存储结构的图
{
	vex xlist[MAX];//图中的每个表头结点
	int vexnum,arcnum;//图中的顶点数和边数
}Graph;
typedef struct//建立一个栈存储图中入度为0的点
{
	vex *base;
	vex *top;
	int stacksize;
}stack;
void initstack(stack &s)//栈的初始化
{
	s.base=(vex *)malloc(MAX*sizeof(vex));
	if(!s.base)
		return ;
	s.top=s.base;
	s.stacksize=MAX;
}
void push(stack &s,vex e)//入栈
{
	if(s.top-s.base>=s.stacksize)
		return ;
	*s.top++=e;
}
int pop(stack &s,vex &e)//出栈
{
	if(s.top==s.base)
		return 0;
	e=*--s.top;
	return 1;
}
node *luru()//项目任务录入函数
{
	int i,n;
	node *head=NULL,*p1=NULL,*p2=NULL;
	head=(node *)malloc(sizeof(node));
	head->next=NULL;
	p2=head;
	printf("录入项目任务数量n:\n");
	cin>>n;
	printf("录入n个项目任务:\n");
	for(i=0;i<=n-1;i++)
	{
		p1=(node *)malloc(sizeof(node));
		p1->next=NULL;
		cin>>p1->project;
		p2->next=p1;
		p2=p1;
	}
	return head;
}
void xiugaishanchu(elemtype changeremove,node * &p1,node * &p2)//修改和删除都可以用这个函数
{
	while(p1!=NULL)
	{
		if(p1->project==changeremove)
			break;
		p2=p2->next;
		p1=p2->next;
	}//p1是要找查找或者删除的项目任务，p2是所找项目任务的前一个为了方便删除
}
void jiancha(node *p1)//检查自己代码是否满足更改项目任务的要求
{
	while(p1!=NULL)
	{
		cout<<p1->project;
		p1=p1->next;
	}
}
void zheban(elemtype a[MAX],int low,int high,elemtype chazhao,int &e)//折半查找
{
	int mid;
	mid=(low+high)/2;
	if(a[mid]==chazhao)
		e=mid;
	else
	{
		if(chazhao<a[mid])
		{
			high=mid-1;
			zheban(a,low,high,chazhao,e);
		}
		else
		{
			low=mid+1;
			zheban(a,low,high,chazhao,e);
		}
	}
}
int locate(Graph &G,elemtype u)//找到项目任务在图中对应的位置下标
{
	int i;
	for(i=0;i<=G.vexnum-1;i++)
	{
		if(G.xlist[i].data==u)
			break;
	}
	return i;
}
void create(Graph &G,elemtype a[MAX],int m)//输入项目任务c[i]的先决条件b[i]和c[i]构造图,每组对应一条边
{
     elemtype b[MAX],c[MAX];
	 int i,j,k;
	 arc *p=NULL;
	 G.vexnum=m;
	 for(i=0;i<=G.vexnum-1;i++)
	 {
		 G.xlist[i].data=a[i];//把项目任务的值赋值给对应图的顶点
		 G.xlist[i].firstin=NULL;
		 G.xlist[i].firstout=NULL;
		 G.xlist[i].rudu=0;
	 }
	 printf("\n输入图的边的数量:\n");
	 cin>>G.arcnum;
	 printf("输入项目任务先决条件和项目任务:\n");
	 for(i=0;i<=G.arcnum-1;i++)
	 {
		 cin>>b[i]>>c[i];
		 j=locate(G,b[i]);
		 k=locate(G,c[i]);
		 p=(arc *)malloc(sizeof(arc));
		 p->headvex=k;
		 p->next=G.xlist[j].firstout;
		 G.xlist[j].firstout=p;//所有以j为弧尾结点的弧
		 p=(arc *)malloc(sizeof(arc));
		 p->tailvex=j;
		 p->next=G.xlist[k].firstin;
		 G.xlist[k].firstin=p;//所有以k为弧头结点的弧
	 }
}
void tuopu(Graph &G)//对项目任务进行拓扑排序
{
	arc *p;
	int i,v;
	stack s;
	vex e;
	initstack(s);
	for(i=0;i<=G.vexnum-1;i++)
	{
		p=G.xlist[i].firstin;
		while(p!=NULL)
		{
			G.xlist[i].rudu++;
			p=p->next;
		}
		if(G.xlist[i].rudu==0)
			push(s,G.xlist[i]);
	}
	for(i=0;i<=G.vexnum-1;i++)
	{
		pop(s,e);
		printf("%c",e);//输出一个入度为0的结点并且删除以他为尾的弧
		p=e.firstout;
		while(p!=NULL)
		{
			v=p->headvex;
			G.xlist[v].rudu--;
			if(G.xlist[v].rudu==0)
				push(s,G.xlist[v]);
			p=p->next;
		}
	}
}
int main()
{
	int i,j,m,k,e,on;
	Graph G;
	elemtype a[MAX],change,changea,remove,t,chazhao;
	node *head=NULL,*p1=NULL,*p2=NULL;
	printf("系统主要功能如下\n");
	printf("1 项目任务的录入和修改\n2 项目任务的录入和删除\n3 折半查找项目任务\n4 实现项目任务拓扑排序\n请输入你的选择\n");
	cin>>on;
	head=luru();
    p1=head->next;
	printf("录入后项目任务为:\n");
	jiancha(p1);//检查项目录入是否成功
    if(on==1)
	{
		printf("\n输入要修改的项目任务:\n");
        cin>>change;
	    p2=head;
	    p1=p2->next;
	    xiugaishanchu(change,p1,p2);
	    //此刻p1所指的就是想要修改的项目任务，把p1的project域改成想要修改成任务完成了项目的修改
	    printf("输入想要改成的项目任务:\n");
        cin>>changea;//举个例子，把项目任务修改成任务changea
	    p1->project=changea;//此时p1所指的项目任务已修改
	    p1=head->next;
	    printf("修改后项目为:\n");
	    jiancha(p1);//检查项目任务是否修改，输出修改后项目任务
	}
	else if(on==2)
	{
		p2=head;
	    p1=p2->next;
	    printf("\n输入要删除的项目任务:\n");
	    cin>>remove;//remove是想要删除的项目任务
	    xiugaishanchu(remove,p1,p2);
	    //此刻p1是要删除项目任务的位置，p2是为了保证删除
	    p1=p1->next;
	    p2->next=p1;//此时想要删除的项目任务已经删除
	    p1=head->next;
	    printf("删除后项目为:\n");
	    jiancha(p1);//检查项目任务是否删除，输出删除后的项目任务
	}
	else if(on==3)
	{
		p1=head->next;
	    for(m=0;;m++)//把项目任务放到数组里以便升序后的折半查找
	    {
		   if(p1==NULL)
			  break;
		   a[m]=p1->project;
		   p1=p1->next;
	    }
	   for(i=0;i<=m-2;i++)//选择排序对项目任务升序排序为折半查找准备
	   {
		  k=i;
		  for(j=i+1;j<=m-1;j++)
		  {
			 if(a[j]<a[k])
				 k=j;
		  }
		  t=a[i];
		  a[i]=a[k];
		  a[k]=t;
	    }
	    printf("\n输出对项目任务升序排序后的结果:\n");
        for(i=0;i<=m-1;i++)
	    {
		   printf("%c",a[i]);
	    }
	    printf("\n输入要查找的项目任务:\n");
	    cin>>chazhao;
	    zheban(a,0,m-1,chazhao,e);
	    printf("输出查找到的项目任务位置对应数组下标和项目任务:\n%d %c",e,a[e]);
	}
	else if(on==4)
	{
		p1=head->next;
	    for(m=0;;m++)//把项目任务放到数组里以便赋值给图中的结点
	    {
		   if(p1==NULL)
			  break;
		   a[m]=p1->project;
		   p1=p1->next;
	    }
		create(G,a,m);
	    printf("拓扑排序后的结果为:\n");
	    tuopu(G);
	}
	system("pause");
	return 0;
}
