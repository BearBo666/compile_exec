#include "stdio.h"
#include "stdlib.h"

#define MaxRuleNum 8
#define MaxVnNum 5
#define MaxVtNum 5
#define MaxStackDepth 20
#define MaxPLength 20
#define MaxStLength 50

//==================== 0.���ݽṹ ======================
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
//Ԥ�������,���Ϊ����ʽ�ı��;+1���ڴ�Ž���������+1���ڴ��#(-1)
int analyseTable[MaxVnNum + 1][MaxVtNum + 1 + 1];
//����ջ,ջ��ָ��
int analyseStack[MaxStackDepth + 1];
int topAnalyse;

//======================= 1.��ʼ����ȡ ===========================
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

//================== 2.�����ӡfirst��follow�� ===================
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

//==================== 3.����Ԥ������� ========================
//����Ԥ�������;��ӡԤ�������
void CreateAT();
void ShowAT();

//==================== 4.�������� =============================
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

//==================== 1.��ʼ����ȡ ======================
//1.1��ʼ���ռ�
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
//1.2�����ַ���vn,vt�е��±�,��vn���򷵻��±�+100,��vt���򷵻��±�
int IndexCh(char ch)
{
  int n;
  n = 0;
  //�������ս��
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
//1.3��ӡVn��Vt������
void ShowChArray(char *collect)
{
  int k = 0;
  while ('\0' != collect[k])
  {
    printf(" %c ", collect[k++]);
  }
  printf("\n");
}
//1.4������ս��
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
//1.5�����ս�� ͬ���ս������
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
//1.6����ʽ����
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
    //��A->  ��A�Ƴ���,�˴�Ϊ\0,IndexCh����-1
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
//1.7�жϲ���ʽ��ȷ��
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

//================== 2.�����ӡfirst��follow�� ===================
//2.1������ս��U��first��
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
          //���˽ڵ���ӵ�U��first����
          AddFirst(U, pt->rCursor);
          //��first��û�п�������ѭ��
          if (!HaveEmpty(pt->rCursor))
          {
            break;
          }
          else
          {
            //�������������һ���ַ���first
            pt = pt->next;
          }
        }
        //��һ��ʾ�Ҳ��ַ���û��break��ѭ��
        j++;
      }
      //���ڵ����Ҳ��ַ����ĳ���˵��ÿ���Ҳ��ַ���û��break��ѭ��
      //����ÿһ���Ҳ��ַ������Ƴ���,������ӵ�U��first��
      if (j >= P[i].rLength)
        AddFirst(U, -1);
    }
  }
}
//2.2����first��
void AddFirst(int U, int nCh)
{
  struct collectNode *pt, *qt;
  int ch;
  pt = NULL;
  qt = NULL;
  //��nCh<100ʱ��ζ�����ս��
  if (nCh < 100)
  {
    //���U��first��ͷָ��
    pt = first[U - 100];
    //���ͷָ��ǿ�
    while (NULL != pt)
    {
      //������ս������first����
      if (pt->nVt == nCh)
        break;
      else
      {
        //��qtָ�����һ���ڵ�,ptָ��β��
        qt = pt;
        pt = pt->next;
      }
    }
    //����NULL˵���ս������first��
    if (NULL == pt)
    {
      //��β����Ӵ��ս��
      pt = (struct collectNode *)malloc(sizeof(struct collectNode));
      pt->nVt = nCh;
      pt->next = NULL;
      //���ͷָ���ǿյĻ�,��ͷָ��pt
      if (NULL == first[U - 100])
      {
        first[U - 100] = pt;
      }
      else
      {
        //ͷָ�벻Ϊ��,��pt��ӵ�β��
        qt->next = pt;
      }
      //ptָ���
      pt = pt->next;
    }
  }
  else
  {
    //�����ս��nCh��first����Ӹ�U
    pt = first[nCh - 100];
    //��nCh�ķ��ս������ǿ�
    while (NULL != pt)
    {
      //����nCh��first������
      ch = pt->nVt;
      if (-1 != ch)
      {
        //�ݹ����
        AddFirst(U, ch);
      }
      pt = pt->next;
    }
  }
}
//2.3�ж�ĳ�ַ���first�����Ƿ��п�(-1)
bool HaveEmpty(int nVn)
{
  //Ϊ�ս��ʱ
  if (nVn < 100)
    return false;
  struct collectNode *pt;
  //����nVn��first��
  pt = first[nVn - 100];
  while (NULL != pt)
  {
    if (-1 == pt->nVt)
      return true;
    pt = pt->next;
  }
  return false;
}
//2.4������ս��V��follow��
void Follow(int V)
{
  int i;
  struct pRNode *pt;
  //����ǿ�ʼ��,��#������follow
  if (100 == V)
    AddFollow(V, -1, 0);
  //��������ʽ
  for (i = 0; i < PNum; i++)
  {
    //��������ʽ�Ҳ�
    pt = P[i].rHead;
    //�ز���ʽ�Ҳ������ҵ�V
    while (NULL != pt && pt->rCursor != V)
      pt = pt->next;
    //�����ǰָ��V
    if (NULL != pt)
    {
      //�õ�V���ұ��ַ�
      pt = pt->next;
      //Ϊ��˵���� A->aV�����,��A��follow��Ӹ�V
      if (NULL == pt)
      {
        //���A��followΪ�� ����A!=V
        if (NULL == follow[P[i].lCursor - 100] && P[i].lCursor != V)
        {
          //����A��follow
          Follow(P[i].lCursor);
        }
        //��A��follow��Ӹ�V
        AddFollow(V, P[i].lCursor, 0);
      }
      else
      {
        //��Ϊ��ʱV->xVy,(ע�⣺y->)������AddFollow����Vt��y��first��
        while (NULL != pt && HaveEmpty(pt->rCursor))
        {
          AddFollow(V, pt->rCursor, 1); /*y��ǰ׺���п�ʱ������first��*/
          pt = pt->next;
        }
        if (NULL == pt) /*��������ַ������Ƴ���ʱ*/
        {
          //����˲���ʽ�󲿵�first��Ϊ�ղ����󲿲�����V
          if (NULL == follow[P[i].lCursor - 100] && P[i].lCursor != V)
          {
            //�������ʽ�󲿵�follow
            Follow(P[i].lCursor);
          }
          //������ʽ�󲿵�follow���뵽V��follow
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
//2.5����follow��
void AddFollow(int V, int nCh, int kind)
{
  /*����ֵС��100ʱnChΪVt*/
  /*#��-1��ʾ,kind���������ǲ�����ŵ�first��������follow��
  kind = 0�����follow����kind = 1����first��*/
  struct collectNode *pt, *qt;
  int ch;
  pt = NULL;
  qt = NULL;
  //Ϊ�ս��ʱ
  if (nCh < 100)
  {
    //�õ�����ӵķ��ս����follow
    pt = follow[V - 100];
    //��follow��Ѱ�Ҵ��ս��
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
    //����follow�����޴��ս��
    if (NULL == pt)
    {
      //�����ս����ӵ�V��follow��
      pt = (struct collectNode *)malloc(sizeof(struct collectNode));
      pt->nVt = nCh;
      pt->next = NULL;
      //��follow��Ϊ��,ֱ�ӵ���ͷָ��
      if (NULL == follow[V - 100])
      {
        follow[V - 100] = pt;
      }
      else
      {
        //qtָ�����һ��Ԫ��,��pt��ӵ�qt��next
        qt->next = pt;
      }
      //�ͷ�ָ��
      pt = pt->next;
    }
  }
  else //��nChΪ���ս��ʱ,Ҫ�ж�Ҫ����nCh��first����follow
  {
    //���Ҫ����nCh��follow
    if (0 == kind)
    {
      //����nCh��follow
      pt = follow[nCh - 100];
      while (NULL != pt)
      {
        //�õ�follow�е��ս��,����Ӹ�V
        ch = pt->nVt;
        AddFollow(V, ch, 0);
        pt = pt->next;
      }
    }
    else
    {
      //����nCh��first
      pt = first[nCh - 100];
      while (NULL != pt)
      {
        //�õ�first�еķǿ��ս��,����Ӹ�V
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
//2.6���first��follow��
void ShowCollect(struct collectNode **collect)
{
  int i;
  struct collectNode *pt;
  i = 0;
  //ѭ����������,����i���ս���ļ���ͷָ��ǿ�
  while (NULL != collect[i])
  {
    pt = collect[i];
    printf("\n%c:\t", Vn[i]);
    while (NULL != pt)
    {
      //���±겻����-1���ڵ㲻��ʾ��
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
//2.7����first��follow
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

//==================== 3.����Ԥ������� ========================
//3.1����Ԥ�������
void CreateAT()
{
  int i;
  struct pRNode *pt;
  struct collectNode *ct;
  //��������ʽ
  for (i = 0; i < PNum; i++)
  {
    //�õ��Ҳ�ͷ�ڵ�
    pt = P[i].rHead;
    //1.��һ���Ҳ��ַ�Ϊvn,��first�޿�
    while (NULL != pt && HaveEmpty(pt->rCursor)) //��Ϊ�ս��ʱfirst������,�������ѭ��
    {
      //ѭ������first��
      ct = first[pt->rCursor - 100];
      while (NULL != ct)
      {
        //����first���ַ���Ϊ��
        if (-1 != ct->nVt)
          //����ǰ����ʽ������Ӵ˲���ʽ���±�
          analyseTable[P[i].lCursor - 100][ct->nVt] = i;
        ct = ct->next;
      }
      pt = pt->next;
    }
    //2.��һ���Ҳ��ַ�Ϊ��
    if (NULL == pt)
    {
      //���󲿵�follow��Ӹ��󲿵�Ԥ�������
      ct = follow[P[i].lCursor - 100];
      while (NULL != ct)
      {
        //����follow���ַ���Ϊ��
        if (-1 != ct->nVt)
          //����ǰ����ʽ������Ӵ˲���ʽ���±�
          analyseTable[P[i].lCursor - 100][ct->nVt] = i;
        else
          //vtNum�±��ʾ��
          analyseTable[P[i].lCursor - 100][vtNum] = i;
        ct = ct->next;
      }
    }
    else
    {
      //3.��һ���Ҳ��ַ�Ϊ�ս��
      if (100 <= pt->rCursor)
      {
        ct = first[pt->rCursor - 100];
        while (NULL != ct)
        {
          analyseTable[P[i].lCursor - 100][ct->nVt] = i;
          ct = ct->next;
        }
      }
      //4.��һ���Ҳ��ַ�Ϊ���ս��
      else
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
//3.2���������
void ShowAT()
{
  int i, j;
  printf("����Ԥ����������£�\n\t|\t");
  //���һ���ս��
  for (i = 0; i < vtNum; i++)
  {
    printf("%c\t", Vt[i]);
  }
  //��ʽ��
  printf("#\t\n- - -\t|- - -\t");
  for (i = 0; i <= vtNum; i++)
    printf("- - -\t");
  printf("\n");
  //������ս��
  for (i = 0; i < vnNum; i++)
  {
    printf("%c\t|\t", Vn[i]);
    for (j = 0; j <= vtNum; j++)
    {
      if (-1 != analyseTable[i][j])
        printf("R(%d)\t", analyseTable[i][j]);
      else
        printf("empty\t");
    }
    printf("\n");
  }
}
//==================== 4.�������� =============================
//���س���
void Identify(char *st)
{
  //current:����ָʾ��; r:����ʽ���
  int current, step, r;
  printf("\n%s�ķ������̣�\n", st);
  printf("����\t��������ջ\t��ǰָʾ�ַ�\tʹ�ò���ʽ���\n");

  current = step = 0;
  printf("%d\t", step);
  ShowStack();

  //��0���������ַ�
  printf("\t\t%c\t\t- -\n", st[current]);
  //ֻҪδ����#
  while ('#' != st[current])
  {
    //��ǰ�ķ���ջ��Ϊ���ս�� ��ʼ��ʱtopָ��1����ʼ��
    if (100 > analyseStack[topAnalyse])
    {
      //�����ǰ�Ĵ����������ڷ���ջ����
      if (analyseStack[topAnalyse] == IndexCh(st[current]))
      {
        //ƥ���ջ
        Pop();
        //ָʾ������
        current++;
        //����+1
        step++;
        printf("%d\t", step);
        //��ӡ����ջ
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
    else //��Ϊ���ս��ʱ
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
        //Ԥ����Ƶ�����
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
//��ʼ��ջ�����Ŵ�
void InitStack()
{
  int i;
  //����ջ�ĳ�ʼ��*/
  for (i = 0; i < MaxStLength; i++)
    st[i] = '\0';
  analyseStack[0] = -1;  /*#(-1)��ջ*/
  analyseStack[1] = 100; /*��ʼ����ջ*/
  topAnalyse = 1;
}
//��ӡ����ջ������
void ShowStack()
{
  int i;
  //��ջ�ױ�������ջ
  for (i = 0; i <= topAnalyse; i++)
  {
    //����Ƿ��ս��
    if (100 <= analyseStack[i])
      printf("%c", Vn[analyseStack[i] - 100]);
    else //������ս��
    {
      //�����Ϊ��
      if (-1 != analyseStack[i])
        printf("%c", Vt[analyseStack[i]]);
      else
        printf("#");
    }
  }
}
//ջ����ջ
void Pop()
{
  topAnalyse--;
}
//��ջ:�洢��r������ʽ���Ҳ�
void Push(int r)
{
  int i;
  struct pRNode *pt;
  Pop();
  pt = P[r].rHead;
  if (-1 == pt->rCursor) /*Ϊ�ղ���ʽʱ*/
    return;
  //ջ���ȼ��ϲ���ʽ
  topAnalyse += P[r].rLength;
  //��ջ����ʼ������ջ
  for (i = 0; i < P[r].rLength; i++)
  {
    analyseStack[topAnalyse - i] = pt->rCursor;
    pt = pt->next;
  }
}
