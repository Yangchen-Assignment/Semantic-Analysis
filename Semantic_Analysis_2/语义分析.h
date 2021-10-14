#include <stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
//#include <iostream.h>
#include<ctype.h>
#include <windows.h>

#define txmax 100    /* ���ű�������� */
#define al 10        /* ���ŵ���󳤶� */
#define tvmax 500    /* ����ܹ��������ʱ����ȡֵ��Χ[100, tvmax] */
#define norw 2       /* �ؼ��ָ��� */
#define cxmax 500   /* ��������������� */

int tx;              //���ֱ�ָ��, ȡֵ��Χ[0, txmax-1]
int tv ;             //��ʱ��������


/* ���� */
enum symbol {
    nul,    eeof,   ident,      minus,      plus,   times,  lparen,divide,
	rparen, comma,  semicolon,  becomes,    period, realsym,    intsym,
};

enum symbol jtype;
enum symbol sym;    /* ��ǰ�ķ��� */
char ch;            /* ��ȡ�ַ��Ļ�������getch ʹ�� */
char id[al+1];      /* ��ǰident, �����һ���ֽ����ڴ��0 */
char a[al+1];       /* ��ʱ����, �����һ���ֽ����ڴ��0 */

/* ���ű�ṹ */
struct tablestruct
{
    char name[al];      /* ���� */
    enum symbol type;   // ����
};

struct tablestruct table[txmax]; /* ���ű� */

char word[norw][al];        /* ������ */
enum symbol wsym[norw];     /* �����ֶ�Ӧ�ķ���ֵ */
enum symbol ssym[256];      /* ���ַ��ķ���ֵ��ɢ�б� */

int cx;             /* ��Ԫʽ����ָ��, ȡֵ��Χ[0, cxmax-1]*/
struct instruction
{
    char f[al+1]; /* ������ */
    char l[al+1];     /* ������� */
    char r[al+1];     /* �Ҳ�����*/
	char t[al+1];     /* ��� */
};
struct instruction code[cxmax]; /* ����������������� */

FILE* fin;
FILE* fout;

int getsym();
void enter(enum symbol type);
void init();
int found(char* idt);
int gen(enum symbol op, int arg1, int arg2,int result );     //�м�������
void writecode(char *op, char *arg1, char *arg2,char *result );    //д����
void start();
void D();
void B();
void L(enum symbol type);
void A(enum symbol type);
void S();
void H();
int E();
int R(int Ri);
int T();
int P(int Pi);
int F();
int V();