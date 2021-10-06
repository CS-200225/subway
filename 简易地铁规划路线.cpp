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
//�����������Ͷ���
typedef struct
{
    int num;//������·,������
    int stopnum;//ÿ�˵�����������վ��
    char Station[Maxlen][20];//ÿ�˵���������վ��
}LineNode;//������·��Ϣ

typedef struct// ÿ������ŵ���Ϣ
{
    char StaName[20];
//	LineNode Linedata[Maxline];//�����ĵ�����·
}VertexType;

typedef struct Arcell
{
    int weight;//Ȩֵ��ʱ��
    int linenum;
}Arcell,AdjMatrix[Maxlen][Maxlen];
typedef struct Graph
{
    AdjMatrix arcs;
    VertexType vexs[Maxlen];//����
    int vexnum;//վ����
}Graph;
Graph G;
LineNode Line[Maxline];
VertexType vexdata[Maxlen];

void Init(LineNode &Line,char *a)//���ļ��е�վ̨��Ϣ��ȡ��Line�У������ߺ����ַ���ʾ
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
        printf("����%d����: ",Line[i].num);
        for (j=0;j<Line[i].stopnum;j++)
            printf("%s->",Line[i].Station[j]);
        printf("�ܵĽ����%d",Line[i].stopnum);
        printf("\n\n");
    }
    printf("˵��:����0���߱�ʾ����APM��!\n\n");
    return;
}

void PrintMGraph1(LineNode &Line)
{
    int j;
    printf("����%d����: ",Line.num);
    for (j=0;j<Line.stopnum;j++)
        printf("%s->",Line.Station[j]);
    printf("�ܵĽ����%d",Line.stopnum);
    printf("\n\n");
}


//LocateVex���������潨ͼ�����е��õ�ʱ���������ҵ�v1��v2���ֱ���G�����е�λ�õġ�
int LocateVex(Graph G,char u[20])
{
    int i;
    for (i=0;i<G.vexnum;i++)
        if (!strcmp(u,G.vexs[i].StaName))
            return i;
    if (i==G.vexnum)
        return -1;
}



//����ͼģ�ͽ����еĳ�ʼ�������ݷŵ�ͼ��ÿ���С�
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
            i=LocateVex(G,Line[m].Station[n]);//i��ʾ��G������������е�λ��
            j=LocateVex(G,Line[m].Station[n+1]);	//j��ʾ��G������������е�λ��
            G.arcs[i][j].weight=3;//������b�е�ʱ��ֵ����i��j�Ļ���
            G.arcs[j][i].weight=3;
            G.arcs[i][j].linenum=Line[m].num;
            G.arcs[j][i].linenum=Line[m].num;
        }
    return 0;
}


/* ��Dijkstra�㷨��������G��v0���㵽���ඥ��v�����·��P[i][v]�����Ȩ����D[v]��*/
/* P[i][v]��ֵΪǰ������±꣬D[i][v]��ʾv0��v�����·��֮�͡� */
void Dijkstra(Graph G,Path_Matrix *p,Short_Matrix *d)
{
    int v,w,k,min,i,t,s;
    int final[Maxlen];  /* final[w]=1��ʾ��ö���v0��v�����·�� */
    for (i=0;i<G.vexnum;i++) /* ��ʼ������ */
    {
        for (v=0;v<G.vexnum;v++)
        {
            final[v]=0; /* ȫ�������ʼ��Ϊδ֪���·��״̬ */
            (*d)[i][v]=G.arcs[i][v].weight;/* ����v0�������ߵĶ������Ȩֵ */
            (*p)[i][v]=-1; /* ��ʼ��·������pΪ0 */
        }
        (*d)[i][i]=0;/* v0��v��·��Ϊ0 */
        /* ��ʼ��ѭ����ÿ�����vo��ÿ��v��������·�� */
        for (v=1;v<G.vexnum;v++)
        {
            min=INFINITY;  /* ��ǰ��֪��v0�������̾��� */
            for (w=0;w<G.vexnum;w++)
            {
                if (!final[w]&&(*d)[i][w]<min)
                {
                    k=w;
                    min=(*d)[i][w]; /* w������v0������� */
                }
            }
            final[k]=1; /* ��Ŀǰ�ҵ�������Ķ�����Ϊ1 */
            for (w=0;w<G.vexnum;w++) /* ������ǰ���·�������� */
            {
                s=(*p)[i][k];/*��ڵ����һ��·��*/
                if (s==-1) s=i;/*��һ���ҵ����·��ʱ��Ӧ��·��*/
                if ((G.arcs[k][s].linenum!=G.arcs[k][w].linenum)&&s!=-1)
                    t=min+G.arcs[k][w].weight+5;/*�������·����Ӧ·�߲�ͬ�����������ˣ�����ʱ��������5����*/
                else
                    t=min+G.arcs[k][w].weight;/*�������·����Ӧ·����ͬ��������δ���ˣ�����ʱ�䲻�ı�*/
                /* ˵���ҵ��˸��̵�·��������d[i][w]�� p[i][w]*/
                if (!final[w]&&(t<(*d)[i][w]))
                {
                    (*d)[i][w]=t;
                    (*p)[i][w]=k;
                }
            }
        }
    }
}


void panduan(char p[20],int &m,int &n)//�ж���ʼվ���յ�վ�Ƿ�Ϊ����վ
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
        printf("(���ڸ�վ�³�������%d����)",G.arcs[b][c].linenum);
}


void Find()
{
    int i,j,k,a,b;
    char v1[20],v2[20];
    CreateMGraph(Line,vexdata,G);//��ʼ��ͼ
    Dijkstra(G,&p,&d);//��ʱ���·���㷨
    printf("\n**************·�߲�ѯ***************\n\n");
    int m1=-1,m2=-1,n1=-1,n2=-1;
    printf("��������Ҫ��ѯ����ʼվ��:");
loop:
    scanf("%s",v1);
    j=LocateVex(G,v1);
    if (j==-1)
    {
        printf("������Ĳ�ѯ�����������������!\n");
        goto loop;
    }
    else
    {
        panduan(v1,m1,n1);
        if (m1>=0&&n1<0)//v1��Ϊ����վ
        {
            if (m1==2)
                printf("�㵱ǰ���ڵ�����·Ϊ����APM��\n\n");
            else
                printf("�㵱ǰ���ڵ�����·Ϊ����%d����\n\n",Line[m1].num);
        }
        else if (m1>=0&&n1>=0)
        {
            if (m1==2)
                printf("�㵱ǰ����վΪ���˳�վ�����ڵ���APM�������%d���ߵĽ��㴦\n\n",Line[n1].num);
            else if (n1==2)
                printf("�㵱ǰ����վ��Ϊ���˳�վ�����ڵ���%d���������APM�ߵĽ��㴦\n\n",Line[m1].num);
            else
                printf("�㵱ǰ����վ��Ϊ���˳�վ�����ڵ���%d���������%d�ߵĽ��㴦\n\n",Line[m1].num,Line[n1].num);
        }
    }

    printf("��������Ҫ��ѯ����վ��:");
loop1:
    scanf("%s",v2);
    i=LocateVex(G,v2);
    if (i==-1)
    {
        printf("������Ĳ�ѯ�յ���������������!\n");
        goto loop1;
    }
    else
    {
        panduan(v2,m2,n2);
        if (m2>=0&&n2<0)//v2��Ϊ����վ
        {
            if (m2==2)
                printf("����Ҫ����վ���ڵ���APM��\n\n");
            else
                printf("����Ҫ����վ���ڵ���%d����\n\n",Line[m2].num);
        }
        else if (m2>=0&&n2>=0)
        {
            if (m2==2)
                printf("����Ҫ����վ��Ϊ���˳�վ�����ڵ���APM�������%d���ߵĽ��㴦\n\n",Line[n2].num);
            else if (n2==2)
                printf("����Ҫ����վ��Ϊ���˳�վ�����ڵ���%d���������APM�ߵĽ��㴦\n\n",Line[m2].num);
            else
                printf("����Ҫ����վ��Ϊ���˳�վ�����ڵ���%d���������%d���ߵĽ��㴦\n\n",Line[m2].num,Line[n2].num);
        }
    }
    k=j;
    printf("���·�������·��Ϊ:\n\n");
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
    printf("\t	               ��ӭ��ʹ�ù��ݵ�������·�߲�ѯϵͳ            		           \n");
    printf("\t             ----------------------------------------------                    \n\n");
    printf("\t             ******1.�鿴���ݵĵ�����·��Ϣ****************                      \n");
    printf("\t             ******2.���������·����ѯ********************                      \n");
    printf("\t             ******3.�˳���ѯ******************************\n\n");
    printf("���������ѡ��:\n");
}

void chose()
{
    printf("***********4.������ѯ**************\n");
    printf("***********5.����������************\n");
    printf("***********6.ֱ���˳��������ʱ��·����ѯϵͳ**********\n");
    printf("���������ѡ��:\n");
}



int main()
{
    int i;
    char a;
    Init(Line[0],"����1����.txt");
    Init(Line[1],"����2����.txt");
    Init(Line[2],"����APM��.txt");
    Init(Line[3],"����4����.txt");
    Init(Line[4],"����5����.txt");
    Init(Line[5],"����6����.txt");
    Init(Line[6],"����7����.txt");
    Init(Line[7],"����8����.txt");
    Init(Line[8],"����9����.txt");
    Init(Line[9],"����13����.txt");
    InitS(Line);
loop:
    choose();
    scanf("%d",&i);
    a=getchar();
    system("cls");
    switch (i)
    {
    case 1:
        printf("*************��ӭ�����鿴���ݵ�����·��Ϣ����****************\n\n");
        printf("����ÿ��������·��Ϣ\n");
        int d;
        scanf("%d",&d);
        switch (d)
        {
        case 1:
            PrintMGraph();
            break;
        default :
            printf("�������!\n");
        }
        printf("����1�������������棬����2ֱ���˳�������ѯϵͳ\n");
        int a1;
        scanf("%d",&a1);
        switch (a1)
        {
        case 1:
        {
            char t;
            t=getchar();
            printf("...............���س�������................\n\n");
            t=getchar();
            system("cls");
            goto loop;
            break;
        }
        case 2:
            break;
        default :
            printf("�������!");
        }
        break;
loop1:
    case 2:
    {
        printf("\t*******��������ʱ����̲�ѯ*******\n\n");
        printf("�����Ƿ�Ҫ�鿴������·����ϸ��Ϣ����������1�����ǵĻ�������2!\n");
        int c;
        scanf("%d",&c);
        switch (c)
        {
        case 1:
        {
            printf("����Ҫ�鿴������һ�������ߵ���Ϣ����ȫ���ĵ�����·��Ϣ�����Ҫ�鿴ȫ������·��Ϣ��������2����������1!\n");
            int e;
            scanf("%d",&e);
            switch (e)
            {
            case 1:
            {
                printf("��������Ҫ�鿴�ĵ�����·��(˵��:0���߱�ʾ����APM��):\n");
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
                    printf("�������!\n");
                }
                break;
            }
            case 2:
                PrintMGraph();
                break;
            default :
                printf("�������!\n");
            }
            char t1;
            t1=getchar();
            printf("...............���س������������ѯ����................\n\n");
            t1=getchar();
            system("cls");
        }
        break;
        case 2:
            break;
        default :
            printf("�������!\n");
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
            printf("...............���س������������ѯ����................\n\n");
            t2=getchar();
            system("cls");
            goto loop1;
            break;
        }
        case 5:
        {
            char t3;
            t3=getchar();
            printf("...............���س���������������������................\n\n");
            t3=getchar();
            system("cls");
            goto loop;
            break;
        }
        case 6:
            break;
        default :
            printf("�������!\n");
        }

    }
    printf("����12�������������棬����11ֱ���˳�������ѯϵͳ\n");
    int a2;
    scanf("%d",&a2);
    switch (a2)
    {
    case 12:
    {
        char t;
        t=getchar();
        printf("...............���س�������................\n\n");
        t=getchar();
        system("cls");
        goto loop;
        break;
    }
    case 11:
        break;
    default :
        printf("�������!");
    }

    break;
    case 3:
        printf("\t********�����в�ѯ!лл��ʹ�ù��ݵ�������·�߲�ѯϵͳ*********\n");
        break;
    default :
        printf("�������!!\n");
    }
    return 0;
}



