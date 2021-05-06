#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//�����ֺ�������ĸ���
#define StayNum 13
#define CaclulateNum 9

//������  �����
char stayWord[StayNum][15] = {"const", "var", "procedure", "begin", "end", "odd", "if", "then", "call", "while", "do", "read", "write"};
char caculation[CaclulateNum] = {'+', '-', '*', '/', '>', '<', '=', '#', ':'};

//�жϴʷ�����
bool isNumber(char ch);
bool isCase(char ch);
bool isCaculationSymbol(char ch);
bool isBandSymbol(char ch);
bool isStayWord(char *str);

//�����ļ����PL����
void getInput(char *fileName, char *str);

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
    return ch >= '0' && ch <= '9';
}

//2.�ж��ַ��Ƿ�����ĸ
bool isCase(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

//3.�ж��ַ��Ƿ��������
bool isCaculation(char ch)
{
    int i;
    for (i = 0; i < CaclulateNum; i++)
    {
        if (ch == caculation[i])
        {
            return true;
        }
    }
    return false;
}

//4.�ж��ַ��Ƿ��ǽ��
bool isBoundary(char ch)
{
    return (ch == '(' || ch == ')' || ch == ',' || ch == ';' || ch == '.');
}

//5.�ж��ַ����Ƿ��Ǳ�����
bool isStayWord(char *str)
{
    int i;
    for (i = 0; i < StayNum; ++i)
    {
        if (strcmp(str, stayWord[i]) == 0)
        {
            return true;
        }
    }
    return false;
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

//�ʷ�����
void Analysis(char *filePath, char *source)
{
    //�������
    getInput(filePath, source);
    printf("�ʷ����������н����\n");
    //�ļ����ַ�������, Ҫ�������ַ����ĳ���, ĳ���ʷ��ַ����ĳ���
    int length = strlen(source), length1, length2;
    //�м����
    int i = 0, j, k, t;
    //�������,�м����
    char target[1000], temp[100];

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
            //���ַ�Ϊ��ĸ���������
            if (isCase(target[k]))
            {
                t = 0;
                while (isCase(target[k]) && k < length1)
                {
                    temp[t++] = target[k++];
                }
                temp[t] = '\0';
                //�ж��Ƿ��Ǳ�����
                if (isStayWord(temp))
                {
                    printf("%s  ������\n", temp);
                }
                else
                {
                    printf("%s  ��ʶ��\n", temp);
                }
                strcpy(temp, "");
            }
            //���ַ�Ϊ�������������
            else if (isNumber(target[k]))
            {
                t = 0;
                while (isNumber(target[k]) && k < length1)
                {
                    temp[t++] = target[k++];
                }
                temp[t] = '\0';
                printf("%s  ����\n", temp);

                strcpy(temp, "");
            }
            //�ж��ַ��Ƿ��ǽ��
            else if (isBoundary(target[k]))
            {
                printf("%c  ���\n", target[k++]);
                //�ж��Ƿ����
                if (target[k - 1] == '.')
                {
                    return;
                }
            }
            //�ж��ַ��Ƿ����������
            else if (isCaculation(target[k]))
            {
                if (target[k] == '<' || target[k] == '>' || target[k] == ':')
                {
                    if (target[k + 1] == '=')
                    {
                        printf("%c%c  �����\n", target[k], target[++k]);
                    }
                    else
                    {
                        printf("%c  �����\n", target[k++]);
                    }
                }
                else
                {
                    printf("%c  �����\n", target[k++]);
                }
            }
            else
            {
                printf("%c  �Ƿ�\n", target[k++]);
            }
        }
        strcpy(temp, "");
    }
}
