#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<limits.h>
#include<windows.h>
#define OK 1
#define ERROR 0
#define Max 100
#define INFINITY 101
#define Maxlen 300
#define Maxline 10
typedef int Path_Matrix[Maxlen][Maxlen];
typedef int Short_Matrix[Maxlen][Maxlen];
Path_Matrix p;
Short_Matrix d;
//抽象数据类型定义
typedef struct
{
    int num;//地铁线路,几号线
    int stopnum;//每趟地铁经过的总站数
    char Station[Maxlen][20];//每趟地铁经过的站点
}LineNode;//地铁线路信息

typedef struct// 每个结点存放的信息
{
    char StaName[20];
//	LineNode Linedata[Maxline];//经过的地铁线路
}VertexType;

typedef struct Arcell
{
    int weight;//权值，时间
    int linenum;
}Arcell,AdjMatrix[Maxlen][Maxlen];
typedef struct Graph
{
    AdjMatrix arcs;
    VertexType vexs[Maxlen];//顶点
    int vexnum;//站点数
}Graph;
Graph G;
LineNode Line[Maxline];
VertexType vexdata[Maxlen];

void Init(LineNode &Line,char *a)//将文件中的站台信息读取到Line中，地铁线号用字符表示
{
    FILE *fp;
    int i,no;
    if ((fp=fopen(a,"r"))==NULL)
    {
        printf("Can't open the file!\n");
        exit(0);
    }
    no=fscanf(fp,"%d %d",&(Line.num),&(Line.stopnum));
    no++;
    for (i=0;no!= NULL;i++)
    {
        no=fscanf(fp,"%s",Line.Station[i]);
        no++;
    }
    fclose(fp);
}

void InitS(LineNode *L)
{
    int i=0,j,t=0;
    while (t<Maxline)
    {
        {
            for (j=0;j<L[t].stopnum;j++)
            {
                strcpy(vexdata[i++].StaName,L[t].Station[j]);
            }
            t++;
        }
    }
}
void PrintMGraph()
{
    int i,j;
    for (i=0;i<Maxline;i++)
    {
        printf("地铁%d号线: ",Line[i].num);
        for (j=0;j<Line[i].stopnum;j++)
            printf("%s->",Line[i].Station[j]);
        printf("总的结点数%d",Line[i].stopnum);
        printf("\n\n");
    }
    printf("说明:地铁0号线表示地铁APM线!\n\n");
    return;
}

void PrintMGraph1(LineNode &Line)
{
    int j;
    printf("地铁%d号线: ",Line.num);
    for (j=0;j<Line.stopnum;j++)
        printf("%s->",Line.Station[j]);
    printf("总的结点数%d",Line.stopnum);
    printf("\n\n");
}


//LocateVex函数在下面建图函数中调用的时候是用来找到v1，v2结点分别在G向量中的位置的。
int LocateVex(Graph G,char u[20])
{
    int i;
    for (i=0;i<G.vexnum;i++)
        if (!strcmp(u,G.vexs[i].StaName))
            return i;
    if (i==G.vexnum)
        return -1;
}



//建立图模型将所有的初始化的数据放到图的每项中。
int CreateMGraph(LineNode *Line,VertexType *vexdata,Graph &G)
{
    int i,j,m,n,flag;
    for (i=0;i<Maxlen;i++)
    {
        flag=0;
        for (j=0;j<i;j++)
            if (!strcmp(G.vexs[j].StaName,vexdata[i].StaName))
                flag=1;
        if (!flag)
            strcpy(G.vexs[i].StaName,vexdata[i].StaName);
    }
    G.vexnum=Maxlen;
    for (i=0;i<G.vexnum;i++)
        for (j=0;j<G.vexnum;j++)
            G.arcs[i][j].weight=INFINITY;
    for (m=0;m<Maxline;m++)
        for (n=0;n<Line[m].stopnum-1;n++)
        {
            i=LocateVex(G,Line[m].Station[n]);//i表示在G向量结点数组中的位置
            j=LocateVex(G,Line[m].Station[n+1]);	//j表示在G向量结点数组中的位置
            G.arcs[i][j].weight=3;//将数组b中的时间值赋给i到j的弧上
            G.arcs[j][i].weight=3;
            G.arcs[i][j].linenum=Line[m].num;
            G.arcs[j][i].linenum=Line[m].num;
        }
    return 0;
}


/* 用Dijkstra算法求有向网G的v0顶点到其余顶点v的最短路径P[i][v]及其带权长度D[v]。*/
/* P[i][v]的值为前驱结点下标，D[i][v]表示v0到v的最短路径之和。 */
void Dijkstra(Graph G,Path_Matrix *p,Short_Matrix *d)
{
    int v,w,k,min,i,t,s;
    int final[Maxlen];  /* final[w]=1表示求得顶点v0到v的最短路径 */
    for (i=0;i<G.vexnum;i++) /* 初始化数据 */
    {
        for (v=0;v<G.vexnum;v++)
        {
            final[v]=0; /* 全部顶点初始化为未知最短路径状态 */
            (*d)[i][v]=G.arcs[i][v].weight;/* 将与v0点有连线的顶点加上权值 */
            (*p)[i][v]=-1; /* 初始化路径数组p为0 */
        }
        (*d)[i][i]=0;/* v0到v的路径为0 */
        /* 开始主循环，每次求得vo到每个v顶点的最短路径 */
        for (v=1;v<G.vexnum;v++)
        {
            min=INFINITY;  /* 当前所知离v0顶点的最短距离 */
            for (w=0;w<G.vexnum;w++)
            {
                if (!final[w]&&(*d)[i][w]<min)
                {
                    k=w;
                    min=(*d)[i][w]; /* w顶点离v0顶点更近 */
                }
            }
            final[k]=1; /* 将目前找到的最近的顶点置为1 */
            for (w=0;w<G.vexnum;w++) /* 修正当前最短路径及距离 */
            {
                s=(*p)[i][k];/*求节点的上一条路径*/
                if (s==-1) s=i;/*第一次找到最短路线时对应的路径*/
                if ((G.arcs[k][s].linenum!=G.arcs[k][w].linenum)&&s!=-1)
                    t=min+G.arcs[k][w].weight+5;/*如果两条路径对应路线不同，即车辆换乘，所用时间需增加5分钟*/
                else
                    t=min+G.arcs[k][w].weight;/*如果两条路径对应路线相同，即车辆未换乘，所用时间不改变*/
                /* 说明找到了更短的路径，修正d[i][w]和 p[i][w]*/
                if (!final[w]&&(t<(*d)[i][w]))
                {
                    (*d)[i][w]=t;
                    (*p)[i][w]=k;
                }
            }
        }
    }
}


void panduan(char p[20],int &m,int &n)//判断起始站和终点站是否为换乘站
{
    int i,j;
    for (i=0;i<Maxline;i++)
        for (j=0;j<Line[i].stopnum;j++)
            if (!strcmp(p,Line[i].Station[j]))
                m=i;
    for (i=0;i<Maxline;i++)
    {
        if (i!=m)
        {
            for (j=0;j<Line[i].stopnum;j++)
                if (!strcmp(p,Line[i].Station[j]))
                    n=i;
        }
    }
}



void hc(int a,int b,int c)
{
    if (G.arcs[a][b].linenum!=G.arcs[b][c].linenum)
        printf("(请在该站下车，换乘%d号线)",G.arcs[b][c].linenum);
}


void Find()
{
    int i,j,k,a,b;
    char v1[20],v2[20];
    CreateMGraph(Line,vexdata,G);//初始化图
    Dijkstra(G,&p,&d);//耗时最短路径算法
    printf("\n**************路线查询***************\n\n");
    int m1=-1,m2=-1,n1=-1,n2=-1;
    printf("请输入你要查询的起始站点:");
loop:
    scanf("%s",v1);
    j=LocateVex(G,v1);
    if (j==-1)
    {
        printf("你输入的查询起点有误，请重新输入!\n");
        goto loop;
    }
    else
    {
        panduan(v1,m1,n1);
        if (m1>=0&&n1<0)//v1不为换乘站
        {
            if (m1==2)
                printf("你当前所在地铁线路为地铁APM线\n\n");
            else
                printf("你当前所在地铁线路为地铁%d号线\n\n",Line[m1].num);
        }
        else if (m1>=0&&n1>=0)
        {
            if (m1==2)
                printf("你当前所在站为换乘车站，是在地铁APM线与地铁%d号线的交点处\n\n",Line[n1].num);
            else if (n1==2)
                printf("你当前所在站点为换乘车站，是在地铁%d号线与地铁APM线的交点处\n\n",Line[m1].num);
            else
                printf("你当前所在站点为换乘车站，是在地铁%d号线与地铁%d线的交点处\n\n",Line[m1].num,Line[n1].num);
        }
    }

    printf("请输入你要查询的终站点:");
loop1:
    scanf("%s",v2);
    i=LocateVex(G,v2);
    if (i==-1)
    {
        printf("你输入的查询终点有误，请重新输入!\n");
        goto loop1;
    }
    else
    {
        panduan(v2,m2,n2);
        if (m2>=0&&n2<0)//v2不为换乘站
        {
            if (m2==2)
                printf("你所要到的站点在地铁APM线\n\n");
            else
                printf("你所要到的站点在地铁%d号线\n\n",Line[m2].num);
        }
        else if (m2>=0&&n2>=0)
        {
            if (m2==2)
                printf("你所要到的站点为换乘车站，是在地铁APM线与地铁%d号线的交点处\n\n",Line[n2].num);
            else if (n2==2)
                printf("你所要到的站点为换乘车站，是在地铁%d号线与地铁APM线的交点处\n\n",Line[m2].num);
            else
                printf("你所要到的站点为换乘车站，是在地铁%d号线与地铁%d号线的交点处\n\n",Line[m2].num,Line[n2].num);
        }
    }
    k=j;
    printf("最短路径到达的路线为:\n\n");
    while (p[i][k]!=-1)
    {
        printf("%s->",G.vexs[k].StaName);
        a=k;
        k=p[i][k];
        b=p[i][k];
        if (b!=-1)
            hc(a,k,b);

    }
    printf("%s->",G.vexs[k].StaName);
    hc(a,k,i);
    if (k!=i)
        printf("%s",G.vexs[i].StaName);
    printf("\n\n");
  
}
void choose()
{
    printf("\t             ----------------------------------------------                      \n");
    printf("\t	               欢迎你使用广州地铁换乘路线查询系统            		           \n");
    printf("\t             ----------------------------------------------                    \n\n");
    printf("\t             ******1.查看广州的地铁线路信息****************                      \n");
    printf("\t             ******2.按两地最短路径查询********************                      \n");
    printf("\t             ******3.退出查询******************************\n\n");
    printf("请输入你的选择:\n");
}

void chose()
{
    printf("***********4.继续查询**************\n");
    printf("***********5.返回主函数************\n");
    printf("***********6.直接退出两地最短时间路径查询系统**********\n");
    printf("请输入你的选择:\n");
}



int main()
{
    int i;
    char a;
    Init(Line[0],"地铁1号线.txt");
    Init(Line[1],"地铁2号线.txt");
    Init(Line[2],"地铁APM线.txt");
    Init(Line[3],"地铁4号线.txt");
    Init(Line[4],"地铁5号线.txt");
    Init(Line[5],"地铁6号线.txt");
    Init(Line[6],"地铁7号线.txt");
    Init(Line[7],"地铁8号线.txt");
    Init(Line[8],"地铁9号线.txt");
    Init(Line[9],"地铁13号线.txt");
    InitS(Line);
loop:
    choose();
    scanf("%d",&i);
    a=getchar();
    system("cls");
    switch (i)
    {
    case 1:
        printf("*************欢迎来到查看广州地铁线路信息界面****************\n\n");
        printf("具体每条地铁线路信息\n");
        int d;
        scanf("%d",&d);
        switch (d)
        {
        case 1:
            PrintMGraph();
            break;
        default :
            printf("输入错误!\n");
        }
        printf("输入1返回主函数界面，输入2直接退出整个查询系统\n");
        int a1;
        scanf("%d",&a1);
        switch (a1)
        {
        case 1:
        {
            char t;
            t=getchar();
            printf("...............按回车键进入................\n\n");
            t=getchar();
            system("cls");
            goto loop;
            break;
        }
        case 2:
            break;
        default :
            printf("输入错误!");
        }
        break;
loop1:
    case 2:
    {
        printf("\t*******进入两地时间最短查询*******\n\n");
        printf("请问是否要查看地铁线路的详细信息，是请输入1，不是的话请输入2!\n");
        int c;
        scanf("%d",&c);
        switch (c)
        {
        case 1:
        {
            printf("请问要查看具体哪一条地铁线的信息还是全部的地铁线路信息，如果要查看全部的线路信息，请输入2，否则输入1!\n");
            int e;
            scanf("%d",&e);
            switch (e)
            {
            case 1:
            {
                printf("请输入你要查看的地铁线路号(说明:0号线表示地铁APM线):\n");
                int g;
                scanf("%d",&g);
                switch (g)
                {
                case 0:
                    PrintMGraph1(Line[2]);
                    break;
                case 1:
                    PrintMGraph1(Line[0]);
                    break;
                case 2:
                    PrintMGraph1(Line[1]);
                    break;
                case 4:
                    PrintMGraph1(Line[3]);
                    break;
                case 5:
                    PrintMGraph1(Line[4]);
                    break;
                case 6:
                    PrintMGraph1(Line[5]);
                    break;
                case 7:
                    PrintMGraph1(Line[6]);
                    break;
                case 8:
                    PrintMGraph1(Line[7]);
                    break;
                case 9:
                    PrintMGraph1(Line[8]);
                    break;
                case 13:
                    PrintMGraph1(Line[9]);
                    break;
                default :
                    printf("输入错误!\n");
                }
                break;
            }
            case 2:
                PrintMGraph();
                break;
            default :
                printf("输入错误!\n");
            }
            char t1;
            t1=getchar();
            printf("...............按回车键进入输入查询界面................\n\n");
            t1=getchar();
            system("cls");
        }
        break;
        case 2:
            break;
        default :
            printf("输入错误!\n");
        }
        Find();
        chose();
        int b;
        scanf("%d",&b);
        switch (b)
        {
        case 4:
        {
            char t2;
            t2=getchar();
            printf("...............按回车键进入输入查询界面................\n\n");
            t2=getchar();
            system("cls");
            goto loop1;
            break;
        }
        case 5:
        {
            char t3;
            t3=getchar();
            printf("...............按回车键进入输入主函数界面................\n\n");
            t3=getchar();
            system("cls");
            goto loop;
            break;
        }
        case 6:
            break;
        default :
            printf("输入错误!\n");
        }

    }
    printf("输入12返回主函数界面，输入11直接退出整个查询系统\n");
    int a2;
    scanf("%d",&a2);
    switch (a2)
    {
    case 12:
    {
        char t;
        t=getchar();
        printf("...............按回车键进入................\n\n");
        t=getchar();
        system("cls");
        goto loop;
        break;
    }
    case 11:
        break;
    default :
        printf("输入错误!");
    }

    break;
    case 3:
        printf("\t********不进行查询!谢谢你使用广州地铁换乘路线查询系统*********\n");
        break;
    default :
        printf("输入错误!!\n");
    }
    return 0;
}



