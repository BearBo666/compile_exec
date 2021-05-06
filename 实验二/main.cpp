#include "stdio.h"
#include "stdlib.h"

#define MaxRuleNum 8
#define MaxVnNum 5
#define MaxVtNum 5
#define MaxStackDepth 20
#define MaxPLength 20
#define MaxStLength 50

//�Ҳ��ַ��ڵ�
struct pRNode
{
  int rCursor;         //�ַ����ս��������±�
  struct pRNode *next; //��һ�ڵ�ָ��
};

//����ʽ���ṹ
struct pNode
{
  int lCursor;          //���ڷ��ս��������±�+100
  int rLength;          //�Ҳ�����
  struct pRNode *rHead; //�Ҳ�����ͷָ��
};

//���ս��
char Vn[MaxVnNum + 1];
int vnNum;

//�ս��
char Vt[MaxVtNum + 1];
int vtNum;

//����ʽ
struct pNode P[MaxRuleNum];
int PNum;

//������
char buffer[MaxPLength + 1];
char ch;
//���������Ŵ�
char st[MaxStLength];

//����(first or follow��)�ڵ�
struct collectNode
{
  int nVt; //�ս���±�
  struct collectNode *next;
};

//first��
struct collectNode *first[MaxVnNum + 1];
//follow��
struct collectNode *follow[MaxVnNum + 1];

//Ԥ�������,���Ϊ����ʽ�ı�ţ�+1���ڴ�Ž���������+1���ڴ��#(-1)
int analyseTable[MaxVnNum + 1][MaxVtNum + 1 + 1];

//����ջ,ջ��ָ��
int analyseStack[MaxStackDepth + 1];
int topAnalyse;

//��ʼ��
void Init();
//�����ս��,���ս����vn,vt�е�λ��,-1��ʾû�ҵ�
int IndexCh(char ch);

//�����ս��,���ս��
void InputVt();
void InputVn();

//����ս������ս��
void ShowChArray(char *collect, int num);
//�������ʽ
void InputP();
//�жϲ���ʽ�Ƿ���ȷ
bool CheckP(char *st);

//����first��
void First(int U);
//����һ��first����Ԫ��
void AddFirst(int U, int nCh);
//�ж�first�����Ƿ��п�
bool HaveEmpty(int nVn);

//follow���ϲ���
void Follow(int V);
void AddFollow(int V, int nCh, int kind);

//���first��,follow��;����first��,follow��
void ShowCollect(struct collectNode **collect);
void FirstFollow();

//����Ԥ�������;��ӡԤ�������
void CreateAT();
void ShowAT();
//���س���,Ϊ��������
void Identify(char *st);

//ջ����:��ʼ��,��ӡ,��ջ,��ջ
void InitStack();
void ShowStack();
void Pop();
void Push(int r);

int main()
{
  char todo, ch;
  Init();        //��ʼ��
  InputVn();     //���ս������
  InputVt();     //�ս������
  InputP();      //����ʽ����
  getchar();     //��������
  FirstFollow(); //����first,follow��

  //��ӡfirst,follow��
  printf("����first��Ϊ��");
  ShowCollect(first);
  printf("����follow��Ϊ��");
  ShowCollect(follow);
  //����Ԥ�������
  CreateAT();
  //���Ԥ�������
  ShowAT();
  todo = 'y';

  //ѯ���Ƿ�������ͷ���
  while ('y' == todo)
  {
    printf("\n�Ƿ�������о��ͷ�����(y / n):");
    todo = getchar();
    while ('y' != todo && 'n' != todo)
    {
      printf("\n(y / n)  ");
      todo = getchar();
    }
    //�������ͷ���
    if ('y' == todo)
    {
      int i;
      //��ʼ��ջ
      InitStack();
      printf("��������Ŵ�(��#����) : ");
      ch = getchar();
      i = 0;
      //δ����#���߳�����С��������뻺����,���˿ո�ͻ���
      while ('#' != ch && i < MaxStLength)
      {
        if (' ' != ch && '\n' != ch)
        {
          st[i++] = ch;
        }
        ch = getchar();
      }
      //��#�����ַ���ĩβ
      if ('#' == ch && i < MaxStLength)
      {
        st[i] = ch;
        Identify(st);
      }
      else
      {
        printf("�������\n");
      }
    }
  }
  getchar();
}

//==================== ��ʼ����ȡ ======================
void Init()
{
  int i, j;
  //�ս��,���ս��,����ʽ���� ����
  vnNum = 0;
  vtNum = 0;
  PNum = 0;
  //�ս��,���ս��,����ʽ����ռ� �ÿ�
  for (i = 0; i <= MaxVnNum; i++)
    Vn[i] = '\0';
  for (i = 0; i <= MaxVtNum; i++)
    Vt[i] = '\0';
  for (i = 0; i < MaxRuleNum; i++)
  {
    P[i].lCursor = NULL;
    P[i].rHead = NULL;
    P[i].rLength = 0;
  }
  //������,first��,follow���ռ� �ÿ�
  for (i = 0; i <= MaxPLength; i++)
    buffer[i] = '\0';
  for (i = 0; i < MaxVnNum; i++)
  {
    first[i] = NULL;
    follow[i] = NULL;
  }
  //Ԥ��������ʼ��
  for (i = 0; i <= MaxVnNum; i++)
  {
    for (j = 0; j <= MaxVnNum + 1; j++)
      analyseTable[i][j] = -1;
  }
}
//�����ַ���vn,vt�е��±�,��vn���򷵻��±�+100,��vt���򷵻��±�
int IndexCh(char ch)
{
  int n;
  n = 0;
  while (ch != Vn[n] && '\0' != Vn[n])
    n++;
  if ('\0' != Vn[n])
    return 100 + n;
  n = 0;
  while (ch != Vt[n] && '\0' != Vt[n])
    n++;
  if ('\0' != Vt[n])
    return n;
  return -1;
}
//���Vn��Vt������
void ShowChArray(char *collect)
{
  int k = 0;
  while ('\0' != collect[k])
  {
    printf(" %c ", collect[k++]);
  }
  printf("\n");
}
//������ս��
void InputVn()
{
  int inErr = 1;
  int n, k;
  char ch;
  while (inErr)
  {
    printf("\n���������еķ��ս����ע�⣺�뽫��ʼ�����ڵ�һλ������#�Ž���:\n");
    n = 0;
    //��ʼ������
    while (n < MaxVnNum)
    {
      Vn[n++] = '\0';
    }
    n = 0, ch = ' ';
    while (('#' != ch) && (n < MaxVnNum))
    {
      //�˿ո�,����,�����ַ������������ս������ս���������
      if (' ' != ch && '\n' != ch && -1 == IndexCh(ch))
      {
        Vn[n++] = ch;
        vnNum++;
      }
      ch = getchar();
    }
    //�ԡ�#����־���������жϳ����Ƿ�Ϸ�
    Vn[n] = '#';
    k = n;
    //������һ�������ַ�����#,���ͷ����
    if ('#' != ch)
    {
      if ('#' != (ch = getchar()))
      {
        while ('#' != (ch = getchar()))
          ;
        printf("\n������Ŀ�������ƣ�\n");
        inErr = 1;
        continue;
      }
    }
    /*��ȷ��ȷ�ϣ���ȷ��ִ�������棬������������*/
    Vn[k] = '\0';
    ShowChArray(Vn);
    ch = ' ';
    while ('y' != ch && 'n' != ch)
    {
      if ('\n' != ch)
      {
        printf("������ȷȷ�� (y/n):");
      }
      scanf("%c", &ch);
    }
    if ('n' == ch)
    {
      printf("¼������������룡\n");
      inErr = 1;
    }
    else
    {
      inErr = 0;
    }
  }
}
//�����ս�� ͬ���ս������
void InputVt()
{
  int inErr = 1;
  int n, k;
  char ch;
  while (inErr)
  {
    printf("\n���������е��ս����ע�⣺");
    printf("��#�Ž���:\n");
    ch = ' ';
    n = 0;
    /*��ʼ������*/
    while (n < MaxVtNum)
    {
      Vt[n++] = '\0';
    }
    n = 0;
    while (('#' != ch) && (n < MaxVtNum))
    {
      if (' ' != ch && '\n' != ch && -1 == IndexCh(ch))
      {
        Vt[n++] = ch;
        vtNum++;
      }
      ch = getchar();
    }
    Vt[n] = '#'; /*�ԡ�#����־����*/
    k = n;       /*k���ڼ�¼n�Ա��Vt[n]='\0'*/
    if ('#' != ch)
    {
      if ('#' != (ch = getchar()))
      {
        while ('#' != (ch = getchar()))
          printf("\n������Ŀ�������ƣ�\n");
        inErr = 1;
        continue;
      }
    }
    /*��ȷ��ȷ�ϣ���ȷ��ִ�������棬������������*/
    Vt[k] = '\0';
    ShowChArray(Vt);
    ch = ' ';
    while ('y' != ch && 'n' != ch)
    {
      if ('\n' != ch)
      {
        printf("������ȷȷ�� (y/n):");
      }
      scanf("%c", &ch);
    }
    if ('n' == ch)
    {
      printf("¼������������룡\n");
      inErr = 1;
    }
    else
    {
      inErr = 0;
    }
  }
}
//����ʽ����
void InputP()
{
  char ch;
  int i = 0, n, num;
  printf("�������ķ�����ʽ�ĸ�����");
  scanf("%d", &num);
  PNum = num;
  getchar(); //�����س���
  printf("\n�������ķ���%d������ʽ,���Իس��ָ�ÿ������ʽ��\n", num);
  while (i < num)
  {
    printf("��%d����", i);
    //��ʼ��������
    for (n = 0; n < MaxPLength; n++)
      buffer[n] = '\0';
    //����һ������ʽ��
    ch = ' ';
    n = 0;
    //ֻҪû�������з����߳������ֵ���������
    while ('\n' != (ch = getchar()) && n < MaxPLength)
    {
      if (' ' != ch)
        buffer[n++] = ch;
    }
    buffer[n] = '\0';
    //�жϲ���ʽ�Ƿ�Ϸ�
    if (CheckP(buffer))
    {
      //��д�����ʽ�ṹ��
      pRNode *pt, *qt;
      //�󲿱����ڷ��ս���е��±�+100
      P[i].lCursor = IndexCh(buffer[0]);
      //����ʽ��һ���Ҳ��ڵ�
      pt = (pRNode *)malloc(sizeof(pRNode));
      pt->rCursor = IndexCh(buffer[3]);
      pt->next = NULL;
      P[i].rHead = pt;
      n = 4;
      //���ж���ڵ�,��������ʽ����
      while ('\0' != buffer[n])
      {
        qt = (pRNode *)malloc(sizeof(pRNode));
        qt->rCursor = IndexCh(buffer[n]);
        qt->next = NULL;
        pt->next = qt;
        pt = qt;
        n++;
      }
      P[i].rLength = n - 3;
      i++;
    }
    else
      printf("������ź��Ƿ��ڳɷ֣����������룡\n");
  }
}
//�жϲ���ʽ��ȷ��
bool CheckP(char *st)
{
  //��ʽ�����ս��->����ս��|���ս��
  int n;
  if (100 > IndexCh(st[0]))
    return false;
  if ('-' != st[1])
    return false;
  if ('>' != st[2])
    return false;
  for (n = 3; '\0' != st[n]; n++)
  {
    if (-1 == IndexCh(st[n]))
      return false;
  }
  return true;
}

/*====================first & follow======================*/
//����first��
void First(int U)
{
  int i, j;
  //ѭ����������ʽ
  for (i = 0; i < PNum; i++)
  {
    //����󲿵���Ҫ����ķ��ս��
    if (P[i].lCursor == U)
    {
      //�����Ҳ��ڵ�
      struct pRNode *pt;
      pt = P[i].rHead;
      j = 0;
      while (j < P[i].rLength)
      {
        //����˽ڵ��Ƿ��ս��
        if (100 > pt->rCursor)
        {
          //�����Ҳ��ڵ��first����Ӹ�U���ս��
          AddFirst(U, pt->rCursor);
          break;
        }
        else
        {
          //����˽ڵ����ս��
          if (NULL == first[pt->rCursor - 100])
          {
            First(pt->rCursor);
          }
          AddFirst(U, pt->rCursor);
          if (!HaveEmpty(pt->rCursor))
          {
            break;
          }
          else
          {
            pt = pt->next;
          }
        }
        j++;
      }
      if (j >= P[i].rLength) /*������ʽ�Ҳ������Ƴ���ʱ*/
        AddFirst(U, -1);
    }
  }
}
//����first��
void AddFirst(int U, int nCh)
{
  struct collectNode *pt, *qt;
  int ch;
  pt = NULL;
  qt = NULL;
  //��nCh<100ʱ��ζ�����ս��
  if (nCh < 100)
  {
    pt = first[U - 100];
    while (NULL != pt)
    {
      if (pt->nVt == nCh)
        break;
      else
      {
        qt = pt;
        pt = pt->next;
      }
    }
    if (NULL == pt)
    {
      pt = (struct collectNode *)malloc(sizeof(struct collectNode));
      pt->nVt = nCh;
      pt->next = NULL;
      if (NULL == first[U - 100])
      {
        first[U - 100] = pt;
      }
      else
      {
        qt->next = pt; /*qtָ��first�������һ��Ԫ��*/
      }
      pt = pt->next;
    }
  }
  else
  {
    pt = first[nCh - 100];
    while (NULL != pt)
    {
      ch = pt->nVt;
      if (-1 != ch)
      {
        AddFirst(U, ch);
      }
      pt = pt->next;
    }
  }
}
/*�ж�first�����Ƿ��п�(-1)*/
bool HaveEmpty(int nVn)
{
  if (nVn < 100) /*Ϊ�ս��ʱ(��-1)����follow�����õ�*/
    return false;
  struct collectNode *pt;
  pt = first[nVn - 100];
  while (NULL != pt)
  {
    if (-1 == pt->nVt)
      return true;
    pt = pt->next;
  }
  return false;
}
/*����follow��,����U->xVy,U->xV.(ע����ʼ���غ�#����"-1")*/
void Follow(int V)
{
  int i;
  struct pRNode *pt;
  if (100 == V) /*��Ϊ��ʼ��ʱ*/
    AddFollow(V, -1, 0);
  for (i = 0; i < PNum; i++)
  {
    pt = P[i].rHead;
    while (NULL != pt && pt->rCursor != V) /*ע�˲��ܴ���U->xVyVz�����*/
      pt = pt->next;
    if (NULL != pt)
    {
      pt = pt->next;  /*V�Ҳ�ķ���*/
      if (NULL == pt) /*��V��Ϊ��ʱV->xV���������follow������V��follow����*/
      {
        if (NULL == follow[P[i].lCursor - 100] && P[i].lCursor != V)
        {
          Follow(P[i].lCursor);
        }
        AddFollow(V, P[i].lCursor, 0);
      }
      else /*��Ϊ��ʱV->xVy,(ע�⣺y->)������AddFollow����Vt��y��first��*/
      {
        while (NULL != pt && HaveEmpty(pt->rCursor))
        {
          AddFollow(V, pt->rCursor, 1); /*y��ǰ׺���п�ʱ������first��*/
          pt = pt->next;
        }
        if (NULL == pt) /*��������ַ������Ƴ���ʱ*/
        {
          if (NULL == follow[P[i].lCursor - 100] && P[i].lCursor != V)
          {
            Follow(P[i].lCursor);
          }
          AddFollow(V, P[i].lCursor, 0);
        }
        else /*���ֲ�Ϊ�յ��ַ�ʱ*/
        {
          AddFollow(V, pt->rCursor, 1);
        }
      }
    }
  }
}
/*����ֵС��100ʱnChΪVt*/
/*#��-1��ʾ,kind���������ǲ�����ŵ�first��������follow��
kind = 0�����follow����kind = 1����first��*/
void AddFollow(int V, int nCh, int kind)
{
  struct collectNode *pt, *qt;
  int ch; /*���ڴ���Vn*/
  pt = NULL;
  qt = NULL;
  if (nCh < 100) /*Ϊ�ս��ʱ*/
  {
    pt = follow[V - 100];
    while (NULL != pt)
    {
      if (pt->nVt == nCh)
        break;
      else
      {
        qt = pt;
        pt = pt->next;
      }
    }
    if (NULL == pt)
    {
      pt = (struct collectNode *)malloc(sizeof(struct collectNode));
      pt->nVt = nCh;
      pt->next = NULL;
      if (NULL == follow[V - 100])
      {
        follow[V - 100] = pt;
      }
      else
      {
        qt->next = pt; /*qtָ��follow�������һ��Ԫ��*/
      }
      pt = pt->next;
    }
  }
  else /*Ϊ���ս��ʱ��Ҫ�����Ǽ�first����follow*/
  {
    if (0 == kind)
    {
      pt = follow[nCh - 100];
      while (NULL != pt)
      {
        ch = pt->nVt;
        AddFollow(V, ch, 0);
        pt = pt->next;
      }
    }
    else
    {
      pt = first[nCh - 100];
      while (NULL != pt)
      {
        ch = pt->nVt;
        if (-1 != ch)
        {
          AddFollow(V, ch, 1);
        }
        pt = pt->next;
      }
    }
  }
}
/*���first��follow��*/
void ShowCollect(struct collectNode **collect)
{
  int i;
  struct collectNode *pt;
  i = 0;
  while (NULL != collect[i])
  {
    pt = collect[i];
    printf("\n%c:\t", Vn[i]);
    while (NULL != pt)
    {
      if (-1 != pt->nVt)
      {
        printf(" %c", Vt[pt->nVt]);
      }
      else
        printf(" #");
      pt = pt->next;
    }
    i++;
  }
  printf("\n");
}
/*����first��follow*/
void FirstFollow()
{
  int i;
  i = 0;
  while ('\0' != Vn[i])
  {
    if (NULL == first[i])
      First(100 + i);
    i++;
  }
  i = 0;
  while ('\0' != Vn[i])
  {
    if (NULL == follow[i])
      Follow(100 + i);
    i++;
  }
}

//================= ����Ԥ������� =============//
void CreateAT()
{
  int i;
  struct pRNode *pt;
  struct collectNode *ct;
  for (i = 0; i < PNum; i++)
  {
    pt = P[i].rHead;
    while (NULL != pt && HaveEmpty(pt->rCursor))
    {
      /*������ս������Ϊ�ս��ʱ��������Ϊ������*/
      ct = first[pt->rCursor - 100];
      while (NULL != ct)
      {
        if (-1 != ct->nVt)
          analyseTable[P[i].lCursor - 100][ct->nVt] = i;
        ct = ct->next;
      }
      pt = pt->next;
    }
    if (NULL == pt)
    {
      /*NULL == pt��˵��xyz->,�õ�follow�еķ���*/
      ct = follow[P[i].lCursor - 100];
      while (NULL != ct)
      {
        if (-1 != ct->nVt)
          analyseTable[P[i].lCursor - 100][ct->nVt] = i;
        else /*������#��ʱ*/
          analyseTable[P[i].lCursor - 100][vtNum] = i;
        ct = ct->next;
      }
    }
    else
    {
      if (100 <= pt->rCursor) /*�����յķ��ս��*/
      {
        ct = first[pt->rCursor - 100];
        while (NULL != ct)
        {
          analyseTable[P[i].lCursor - 100][ct->nVt] = i;
          ct = ct->next;
        }
      }
      else /*�ս�����߿�*/
      {
        if (-1 == pt->rCursor) /*-1Ϊ�ղ���ʽʱ*/
        {
          ct = follow[P[i].lCursor - 100];
          while (NULL != ct)
          {
            if (-1 != ct->nVt)
              analyseTable[P[i].lCursor - 100][ct->nVt] = i;
            else /*������#��ʱ*/
              analyseTable[P[i].lCursor - 100][vtNum] = i;
            ct = ct->next;
          }
        }
        else /*Ϊ�ս��*/
        {
          analyseTable[P[i].lCursor - 100][pt->rCursor] = i;
        }
      }
    }
  }
}
/*���������*/
void ShowAT()
{
  int i, j;
  printf("����Ԥ����������£�\n");
  printf("\t|\t");
  for (i = 0; i < vtNum; i++)
  {
    printf("%c\t", Vt[i]);
  }
  printf("#\t\n");
  printf("- - -\t|- - -\t");
  for (i = 0; i <= vtNum; i++)
    printf("- - -\t");
  printf("\n");
  for (i = 0; i < vnNum; i++)
  {
    printf("%c\t|\t", Vn[i]);
    for (j = 0; j <= vtNum; j++)
    {
      if (-1 != analyseTable[i][j])
        printf("R(%d)\t", analyseTable[i][j]);
      else
        printf("error\t");
    }
    printf("\n");
  }
}
//================= ���س��� =====================//
void Identify(char *st)
{
  int current, step, r; /*r��ʹ�õĲ���ʽ�����*/
  printf("\n%s�ķ������̣�\n", st);
  printf("����\t��������ջ\t��ǰָʾ�ַ�\tʹ�ò���ʽ���\n");

  step = 0;
  current = 0; /*���Ŵ�ָʾ��*/
  printf("%d\t", step);
  ShowStack();
  printf("\t\t%c\t\t- -\n", st[current]);
  while ('#' != st[current])
  {
    if (100 > analyseStack[topAnalyse]) /*��Ϊ�ս��ʱ*/
    {
      if (analyseStack[topAnalyse] == IndexCh(st[current]))
      {
        /*ƥ���ջ��ָʾ������*/
        Pop();
        current++;
        step++;
        printf("%d\t", step);
        ShowStack();
        printf("\t\t%c\t\t��ջ������\n", st[current]);
      }
      else
      {
        printf("%c-%c��ƥ�䣡", analyseStack[topAnalyse], st[current]);
        printf("�˴����Ǵ��ķ��ľ��ӣ�\n");
        return;
      }
    }
    else /*��Ϊ���ս��ʱ*/
    {
      r = analyseTable[analyseStack[topAnalyse] - 100][IndexCh(st[current])];
      if (-1 != r)
      {
        Push(r); /*����ʽ�Ҳ������󲿣�ָʾ�����ƶ�*/
        step++;
        printf("%d\t", step);
        ShowStack();
        printf("\t\t%c\t\t%d\n", st[current], r);
      }
      else
      {
        printf("�޿��ò���ʽ���˴����Ǵ��ķ��ľ��ӣ�\n");
        return;
      }
    }
  }
  if ('#' == st[current])
  {
    if (0 == topAnalyse && '#' == st[current])
    {
      step++;
      printf("%d\t", step);
      ShowStack();
      printf("\t\t%c\t\t�����ɹ���\n", st[current]);
      printf("%s�Ǹ����ķ��ľ��ӣ�\n", st);
    }
    else
    {
      while (topAnalyse > 0)
      {
        if (100 > analyseStack[topAnalyse]) /*��Ϊ�ս��ʱ*/
        {
          printf("�޿��ò���ʽ���˴����Ǵ��ķ��ľ��ӣ�\n");
          return;
        }
        else
        {
          r = analyseTable[analyseStack[topAnalyse] - 100][vtNum];
          if (-1 != r)
          {
            Push(r); /*����ʽ�Ҳ������󲿣�ָʾ�����ƶ�*/
            step++;
            printf("%d\t", step);
            ShowStack();
            if (0 == topAnalyse && '#' == st[current])
            {
              printf("\t\t%c\t\t�����ɹ���\n", st[current]);
              printf("%s�Ǹ����ķ��ľ��ӣ�\n", st);
            }
            else
              printf("\t\t%c\t\t%d\n", st[current], r);
          }
          else
          {
            printf("�޿��ò���ʽ���˴����Ǵ��ķ��ľ��ӣ�\n");
            return;
          }
        }
      }
    }
  }
}
/*��ʼ��ջ�����Ŵ�*/
void InitStack()
{
  int i;
  /*����ջ�ĳ�ʼ��*/
  for (i = 0; i < MaxStLength; i++)
    st[i] = '\0';
  analyseStack[0] = -1;  /*#(-1)��ջ*/
  analyseStack[1] = 100; /*��ʼ����ջ*/
  topAnalyse = 1;
}
/*��ʾ����ջ������*/
void ShowStack()
{
  int i;
  for (i = 0; i <= topAnalyse; i++)
  {
    if (100 <= analyseStack[i])
      printf("%c", Vn[analyseStack[i] - 100]);
    else
    {
      if (-1 != analyseStack[i])
        printf("%c", Vt[analyseStack[i]]);
      else
        printf("#");
    }
  }
}
/*ջ����ջ*/
void Pop()
{
  topAnalyse--;
}
/*ʹ�ò���ʽ��ջ����*/
void Push(int r)
{
  int i;
  struct pRNode *pt;
  Pop();
  pt = P[r].rHead;
  if (-1 == pt->rCursor) /*Ϊ�ղ���ʽʱ*/
    return;
  topAnalyse += P[r].rLength;
  for (i = 0; i < P[r].rLength; i++)
  {                                             /*��Ϊ�ղ���ʽʱ*/
    analyseStack[topAnalyse - i] = pt->rCursor; /*������ջ*/
    pt = pt->next;
  } /*ѭ��δ��ʱptΪ�գ���˵��rLength��¼�ȳ���*/
}
