
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum symbol {
    nul,    atoken, btoken, lparen, rparen, comma
};


int token;    //当前的符号
char ch;

FILE* fin;

void gettoken();
void parseP();
int parseS(int Si);
int parseL(int Li);
int parseR(int Ri);

int main()
{
    char filename[20];

	printf("请输入分析的文件名:");
	gets(filename);
	//scanf("%s",filename);
	do
	{
 		if((fin=fopen(filename,"r"))==NULL)
		{
			printf("不能打开文件.\n");
			return 0;
		}
		gettoken();            //读第一个单词，将单词类别放入token中
		parseP();                // 调用开始符号对应的子程序
		if (token==nul)
			{printf("语法正确\n"); }
		else {printf("语法错误\n");}
		fclose(fin);

		printf("继续分析则输入文件名，否则回车");
		//scanf("%s",filename);
		gets(filename);
	} while (strlen(filename)>0);
	return 0;
}

/*
* 简化的词法分析，获取一个符号存到token中
*/
void gettoken()
{
	ch=fgetc(fin);

	while (ch==' ' || ch==10 || ch==13 || ch==9)  /* 忽略空格、换行、回车和TAB */
		ch=fgetc(fin);

	if (ch==EOF)  token=nul;

    else if (ch=='a' )
	    token = atoken;
	else if(ch == 'b')
		token = btoken;
    else if (ch == '(')
		token = lparen;
    else if (ch == ')')
		token = rparen;
    else if (ch == ',')
		token = comma;
	else {printf("词法错误\n"); exit(0);}
	return;
}

/*
P->S
*/
void parseP()
{
	int Si=0;
	int Snum=parseS(Si);
	printf("标识符a的数量：%d\n",Snum);
}

/*
S->(L)|a
*/
int parseS(int Si)
{
	int Snum;
	if(token==lparen)
	{
		gettoken();
		int Lnum=parseL(Si+1);
		if(token==rparen) 
		{
			gettoken();
			Snum=Lnum;
		}
		else {printf("语法错误\n");exit(0);}
	}
	else if(token==atoken)
	{
		Snum=1,printf("括号对嵌套深度：%d\n", Si);
		gettoken();
	}
	else {printf("语法错误\n");exit(0);}
	return Snum;
}


/* L-->SR */
int parseL(int Li)
{
	int Lnum;
	if(token==lparen || token==atoken)
	{
		int Snum=parseS(Li);
		int Rnum=parseR(Li);
		Lnum=Snum+Rnum;
	}
	else {printf("语法错误\n");exit(0);}
	return Lnum;
}


/* R-->,S R1 |空 */
int parseR(int Ri)
{
	int Rnum;
	if(token==comma)
	{
		gettoken();
		int Snum=parseS(Ri);
		int R1num=parseR(Ri);
		Rnum=Snum+R1num;
	}
	else if(token==rparen) {Rnum=0;}
	else {printf("语法错误\n");exit(0);}
	return Rnum;
}