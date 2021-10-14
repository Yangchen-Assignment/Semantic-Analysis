
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum symbol {
    nul,    atoken, btoken, lparen, rparen, comma
};


int token;    //��ǰ�ķ���
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

	printf("������������ļ���:");
	gets(filename);
	//scanf("%s",filename);
	do
	{
 		if((fin=fopen(filename,"r"))==NULL)
		{
			printf("���ܴ��ļ�.\n");
			return 0;
		}
		gettoken();            //����һ�����ʣ�������������token��
		parseP();                // ���ÿ�ʼ���Ŷ�Ӧ���ӳ���
		if (token==nul)
			{printf("�﷨��ȷ\n"); }
		else {printf("�﷨����\n");}
		fclose(fin);

		printf("���������������ļ���������س�");
		//scanf("%s",filename);
		gets(filename);
	} while (strlen(filename)>0);
	return 0;
}

/*
* �򻯵Ĵʷ���������ȡһ�����Ŵ浽token��
*/
void gettoken()
{
	ch=fgetc(fin);

	while (ch==' ' || ch==10 || ch==13 || ch==9)  /* ���Կո񡢻��С��س���TAB */
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
	else {printf("�ʷ�����\n"); exit(0);}
	return;
}

/*
P->S
*/
void parseP()
{
	int Si=0;
	int Snum=parseS(Si);
	printf("��ʶ��a��������%d\n",Snum);
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
		else {printf("�﷨����\n");exit(0);}
	}
	else if(token==atoken)
	{
		Snum=1,printf("���Ŷ�Ƕ����ȣ�%d\n", Si);
		gettoken();
	}
	else {printf("�﷨����\n");exit(0);}
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
	else {printf("�﷨����\n");exit(0);}
	return Lnum;
}


/* R-->,S R1 |�� */
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
	else {printf("�﷨����\n");exit(0);}
	return Rnum;
}