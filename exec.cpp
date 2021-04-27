#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//������
char stayWord[13][15] = {"const", "var", "procedure", "begin", "end", "odd", "if", "then", "call", "while", "do", "read", "write"};
//�����
char CaculationSymbol[8] = {'+', '-', '/', '>', '<', '=', '#', ';'};
//�������
char result[100][100];

//�жϴʷ�����
bool isNumber(char ch);
bool isCase(char ch);
bool isCaculationSymbol(char ch);
bool isBandSymbol(char ch);
bool isStayWord(char *str);

//�����ļ����PL����
void getInput(char *fileName, char *str);
//
void calulationString(char *str);
bool bandString(char *str);

//�ʷ�����
void Analysis(char *InputFileName, char *str);

int main()
{
    //������
    char buffer[10000];
    //�ļ�·��
    char inputPath[100] = "input.txt";
    //��ʼ����
    Analysis(inputPath, buffer);
    return 0;
}

//1.�ж��ַ��Ƿ�������
bool isNumber(char ch)
{
    return ch >= '0' && ch <= '9' ? true : false;
}

//2.�ж��ַ��Ƿ�����ĸ
bool isCase(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) ? true : false;
}

//3.�ж��ַ��Ƿ��������
bool isCaculationSymbol(char ch)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (ch == CaculationSymbol[i])
        {
            return true;
        }
    }
    return false;
}

//4.�ж��ַ��Ƿ��ǽ��
bool isBandSymbol(char ch)
{
    return (ch == '(' || ch == ')' || ch == ',' || ch == ';' || ch == '.') ? true : false;
}

//5.�ж��ַ����Ƿ��Ǳ�����
bool isStayWord(char *str)
{
    int i;
    for (i = 0; i < 13; ++i)
    {
        if (!strcmp(str, stayWord[i]))
            break;
    }
    return i;
}

//���ļ��ж�ȡ������
void getInput(char *fileName, char *str)
{
    char ch;
    int k = 0;
    FILE *f;
    //�Զ���ʽ���ļ�
    f = fopen(fileName, "r");
    while ((ch = fgetc(f)) != EOF)
    {
        //�Կո���ÿһ��
        ch == '\n' ? str[k++] = ' ' : str[k++] = ch;
    }
    str[k] = '\0';
    fclose(f);
}

//��ӡ�����
void calulationString(char *str)
{
    int len = strlen(str);
    int i;
    for (i = 0; i < len; ++i)
    {
        if (str[i] == ':')
        {
            if (i + 1 < len && str[i + 1] == '=')
            {
                printf("  :=  ?????\n");
                i++;
            }
            else
            {
                printf("  %c  ????????\n", str[i]);
            }
        }
        else if (str[i] == '>')
        {
            if (i + 1 < len && str[i + 1] == '=')
            {
                printf("  >=  ????????\n");
                i++;
            }
            else
            {
                printf("  >  ????????\n");
            }
        }
        else if (str[i] == '<')
        {
            if (i + 1 < len && str[i + 1] == '=')
            {
                printf("  <=  ????????\n");
                i++;
            }
            else
                printf("  <  ????????\n");
            ;
        }
        else
        {
            printf("  %c  ?????\n", str[i]);
        }
    }
}

//��ӡ���,�������Ƿ�
bool bandString(char *str)
{
    int len, i;
    len = strlen(str);
    for (i = 0; i < len; ++i)
        switch (str[i])
        {

        case '(':
            printf("  (  ���\n");
            return false;
        case ')':
            printf("  )  ���\n");
            return false;
        case ',':
            printf("  ,  ���\n");
            return false;
        case ';':
            printf("  ;  ���\n");
            return false;
        case '.':
            printf("  .  ���\n");
            return true;
        default:
            return false;
        }
}

//????????????
void tokenWord(char *str)
{
    int length = strlen(str);
    int i;
    for (i = 1; i < length; i++)
    {
    }
}

//�ʷ�����
void Analysis(char *filePath, char *source)
{
    //�������
    getInput(filePath, source);
    printf("�ʷ����������н����\n");
    //�ļ����ַ�������, Ҫ�������ַ����ĳ���, ĳ���ʷ��ַ����ĳ���
    int length = strlen(source), length1, length2;
    //�м����
    int i = 0, j, k, t, index;
    //�������,�м����
    char target[100], temp[100];

    //ѭ����ȡ�ļ����ַ���
    while (i < length)
    {
        j = k = 0;
        //�˹���ͷ�ո�
        while (source[i] == ' ' && i < length)
        {
            ++i;
        }

        //�˹�source�ַ����м�Ŀո�
        while (source[i] != ' ' && i < length)
        {
            target[j++] = source[i++];
        }
        target[j] = '\0';

        //Ҫ�������ַ����ĳ���
        length1 = strlen(target);
        //ѭ��Ҫ�������ַ���
        while (k < length1)
        {
            //�ж��Ƿ�������ĸ
            if (isCase(target[k]))
            {
                t = 0;
                //
                while ((!isCaculationSymbol(target[k])) && (!isBandSymbol(target[k])) && k < length1)
                {
                    temp[t++] = target[k++];
                }
                temp[t] = '\0';
                index = isStayWord(temp);
                if (index < 13)
                {
                    printf("  %s  ??????\n", stayWord[index]);

                    //�����м�
                    strcpy(temp, "");
                    t = 0;
                }
                else
                {
                    printf("  %s  ?????\n", temp);
                    strcpy(temp, "");
                    t = 0;
                }
            }
            //?�???????????
            else if (isNumber(target[k]))
            {
                while (isNumber(target[k]) && k < length1)
                    temp[t++] = target[k++];
                temp[t] = '\0';
                printf("  %s  ????\n", temp);
                strcpy(temp, "");
                t = 0;
            }
            //?�????????????
            else if (isCaculationSymbol(target[k]))
            {
                while (isCaculationSymbol(target[k]) && k < length1)
                    temp[t++] = target[k++];
                temp[t] = '\0';
                calulationString(temp);
                strcpy(temp, "");
                t = 0;
            }
            //?�?????????
            else if (isBandSymbol(target[k]))
            {
                while (isBandSymbol(target[k]) && k < length1)
                {
                    temp[t++] = target[k++];
                }
                temp[t] = '\0';
                bool flag = bandString(temp);
                if (flag == true)
                {
                    printf("????");
                    goto end;
                }
                strcpy(temp, "");
                t = 0;
            }
            else
            {
                printf(" %c ???\n", target[k]);
                k++;
            }
        }
        strcpy(temp, "");
    }
end:
    fclose(stdout);
}
