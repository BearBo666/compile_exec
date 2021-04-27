#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//保留字
char stayWord[13][15] = {"const", "var", "procedure", "begin", "end", "odd", "if", "then", "call", "while", "do", "read", "write"};
//运算符
char CaculationSymbol[8] = {'+', '-', '/', '>', '<', '=', '#', ';'};
//分析结果
char result[100][100];

//判断词法类型
bool isNumber(char ch);
bool isCase(char ch);
bool isCaculationSymbol(char ch);
bool isBandSymbol(char ch);
bool isStayWord(char *str);

//接受文件里的PL语言
void getInput(char *fileName, char *str);
//
void calulationString(char *str);
bool bandString(char *str);

//词法分析
void Analysis(char *InputFileName, char *str);

int main()
{
    //缓冲区
    char buffer[10000];
    //文件路径
    char inputPath[100] = "input.txt";
    //开始分析
    Analysis(inputPath, buffer);
    return 0;
}

//1.判断字符是否是数字
bool isNumber(char ch)
{
    return ch >= '0' && ch <= '9' ? true : false;
}

//2.判断字符是否是字母
bool isCase(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) ? true : false;
}

//3.判断字符是否是运算符
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

//4.判断字符是否是界符
bool isBandSymbol(char ch)
{
    return (ch == '(' || ch == ')' || ch == ',' || ch == ';' || ch == '.') ? true : false;
}

//5.判断字符串是否是保留字
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

//从文件中读取输入流
void getInput(char *fileName, char *str)
{
    char ch;
    int k = 0;
    FILE *f;
    //以读方式打开文件
    f = fopen(fileName, "r");
    while ((ch = fgetc(f)) != EOF)
    {
        //以空格间隔每一行
        ch == '\n' ? str[k++] = ' ' : str[k++] = ch;
    }
    str[k] = '\0';
    fclose(f);
}

//打印运算符
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

//打印界符,并返回是否
bool bandString(char *str)
{
    int len, i;
    len = strlen(str);
    for (i = 0; i < len; ++i)
        switch (str[i])
        {

        case '(':
            printf("  (  界符\n");
            return false;
        case ')':
            printf("  )  界符\n");
            return false;
        case ',':
            printf("  ,  界符\n");
            return false;
        case ';':
            printf("  ;  界符\n");
            return false;
        case '.':
            printf("  .  界符\n");
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

//词法分析
void Analysis(char *filePath, char *source)
{
    //获得输入
    getInput(filePath, source);
    printf("词法分析器运行结果：\n");
    //文件流字符串长度, 要分析的字符串的长度, 某个词法字符串的长度
    int length = strlen(source), length1, length2;
    //中间变量
    int i = 0, j, k, t, index;
    //分析结果,中间变量
    char target[100], temp[100];

    //循环读取文件流字符串
    while (i < length)
    {
        j = k = 0;
        //滤过开头空格
        while (source[i] == ' ' && i < length)
        {
            ++i;
        }

        //滤过source字符串中间的空格
        while (source[i] != ' ' && i < length)
        {
            target[j++] = source[i++];
        }
        target[j] = '\0';

        //要分析的字符串的长度
        length1 = strlen(target);
        //循环要分析的字符串
        while (k < length1)
        {
            //判断是否是是字母
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

                    //赋给中间
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
            //?鍗???????????
            else if (isNumber(target[k]))
            {
                while (isNumber(target[k]) && k < length1)
                    temp[t++] = target[k++];
                temp[t] = '\0';
                printf("  %s  ????\n", temp);
                strcpy(temp, "");
                t = 0;
            }
            //?鍗????????????
            else if (isCaculationSymbol(target[k]))
            {
                while (isCaculationSymbol(target[k]) && k < length1)
                    temp[t++] = target[k++];
                temp[t] = '\0';
                calulationString(temp);
                strcpy(temp, "");
                t = 0;
            }
            //?鍗?????????
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
