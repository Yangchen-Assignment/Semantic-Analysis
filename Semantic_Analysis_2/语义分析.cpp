/*
һ���򵥵ķ���ģʽ
P�� DS.
D��B; D
D����
B��int L | real L
L��id | L��id
S�� V := E H
H����S | ��
E��E+T | T
T��( E )
T��id
V��id


start->   DS.
D->B; D
D->��
B->int L    { L.type := int }|real L    { L.type := real }
L->id   { A.Type := L.type  enter(v.entry,L.type)}  A
A-> ��idA   { A1.Type := A.type  enter(v.entry,A.type)}
A->��

S�� V := E { gen( ":=", E.place,0,V.place) } H
H����S | ��
E->T    { R.i:=T.place}      R     {E.place:=R.s}
R->+T    { R1.i:= newtemp; gen( "*", R.i, T.place , R1.i) }     R   {R.s:= R1.s; }
R-> ��    {Rs=R.i}
T->( E )  { T.place := E.place}
T->id     {T.place:=found (id)}
V->id     {V.place:=found(id)}
*/
#include "�������.h"

int main()
{
    char filename[20];
	printf("������������ļ���:");
	gets(filename);
	if((fin=fopen(filename,"r"))==NULL)
	{
		printf("���ܴ��ļ�.\n");
		exit(0);
	}

    init();                  //��ʼ��
    getsym();                //����һ�����ʣ�������������sym�У����ִ�ֵ����id��
	start();                //��ʼ��start->DS.  ����

    if (sym==eeof)
	{
		printf("�﷨��ȷ\n\n���м���뱣�浽�ļ��������ļ���������س�");
		gets(filename);
		if(strlen(filename)<=0) return 0;
		if((fout=fopen(filename,"w"))==NULL)
		{
			printf("���ܴ��ļ�.\n");
			exit(0);
		}
		for (int cx1=0;cx1<cx;cx1++)
			fprintf(fout,"(%s,%s,%s,%s)\n",code[cx1].f,code[cx1].l,code[cx1].r,code[cx1].t);
		return 0;
	}
	else {printf("�﷨��1:  . �������о���"); exit(0);}
	fclose(fin);
	fclose(fout);
	return 0;
}

/*��ʼ������*/
void init()
{
	int i;

	/* ���õ��ַ����� */
	for (i=0; i<=255; i++)
	{
		ssym[i] = nul;            //����ȷ�ĵ��ַ�����Ϊnul����Ԥ�ó�ֵnul
	}
	ssym['-'] = minus;
	ssym['+'] = plus;
	ssym['*'] = times;
	ssym['/'] = divide;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['.'] = period;
	ssym[','] = comma;
	ssym[';'] = semicolon;

	/* ���ñ��������� */
	strcpy(&(word[0][0]), "real");
	strcpy(&(word[1][0]), "int");

	/* ���ñ����ַ��� */
	wsym[0] = realsym;
	wsym[1] = intsym;

	tv=100;           //��ʱ����ָ���ֵ����Tx��tv��ȡֵû�н��������𵽵�����ʱ������������ı���
	tx=0;           //��ָ���ֵ
	cx=0;     //ָ�������

}

/*�ʷ���������ȡһ������*/
int getsym()
{
	int i,k;
	ch=fgetc(fin);

	if (ch==EOF) {sym=eeof; return 0;}         //�ļ�����

	while (ch==' ' || ch==10 || ch==13 || ch==9)  /* ���Կո񡢻��С��س���TAB */
		ch=fgetc(fin);

	if (ch>='a' && ch<='z')
	{           /* ���ֻ�������a..z��ͷ ��*/
		k = 0;
		do
		{
			if(k<al)                  /* ���ŵ���󳤶�Ϊal �������ͽ�β����*/
			{
				a[k] = ch;
				k++;
			}
			ch=fgetc(fin);
		} while (ch>='a' && ch<='z' || ch>='0' && ch<='9');

		a[k] = 0;
		strcpy(id, a);
		fseek(fin,-1,1);

		for (i=0;i<norw;i++)        /* ������ǰ�����Ƿ�Ϊ������ */
			 if (strcmp(id,word[i]) == 0)
					 break;
		if (i <norw) {sym = wsym[i];}
		else {sym = ident;}          /* ����ʧ���������Ǳ�ʶ�� */
		
	}
	else if(ch == ':') /* ��⸳ֵ���� */
	{
		ch=fgetc(fin);
		if (ch == '=') {sym = becomes;}
		else {sym = nul;}  /* ����ʶ��ķ��� */
	}
	else sym = ssym[ch];     /* �����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ��� */
	return 0;
}

/*�ڷ��ű��м���һ��*/
void enter(enum symbol type)
{
	tx=tx+1;
	if (tx > txmax)
    {
		printf(" ���ű�Խ�� ");           /* ���ű�Խ�� */
		return;
	}
	if(found(id)>0) {printf("�﷨���󣺱�ʶ��%s�����ظ�����",id);exit(0);}
	strcpy(table[tx].name, id); /* ȫ�ֱ���id���Ѵ��е�ǰ��ʶ��������,TxΪ���뵱ǰ����֮ǰ��βָ�� */
	table[tx].type = type;
}

/*
* �������ֵ�λ��.
* �ҵ��򷵻������ֱ��е�λ��,���򷵻�0.
*
* idt:    Ҫ���ҵ�����
* tx:     ��ǰ���ֱ�βָ�룬ȫ�ֱ���
*/
int found(char* idt)
{
	int i;
	strcpy(table[0].name, idt);
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}

/* �м����*/
int gen(enum symbol op, int arg1, int arg2,int result )
{
	char temp1[al+1],temp2[al+1],temp3[al+1];
	if(arg1>=100)                            //ģ��������ʱ����
	{
		wsprintf(temp1,"T%d",arg1);
	}
	else
	{
		strcpy(temp1, table[arg1].name);
	}

	if(arg2>=100)
	{
		wsprintf(temp2,"T%d",arg2);
	}
	else
	{
		strcpy(temp2, table[arg2].name);
	}

	if(result>=100)
	{
		wsprintf(temp3,"T%d",result);
	}
	else
	{
		strcpy(temp3,table[result].name);
	}

	if (op==becomes)
	{
		printf("(:=,%s,%s,%s)\n",temp1,temp2,temp3);
		writecode(":=",temp1,temp2,temp3);
	}
	else if (op==plus)                          // +����
	{
		writecode("+",temp1,temp2,temp3);
		printf("(+,%s,%s,%s)\n",temp1,temp2,temp3);
	}
	else if (op==minus)                          // -����
	{
		writecode("-",temp1,temp2,temp3);
		printf("(-,%s,%s,%s)\n",temp1,temp2,temp3);
	}
	else if (op==times)                          // *����
	{
		writecode("-",temp1,temp2,temp3);
		printf("(*,%s,%s,%s)\n",temp1,temp2,temp3);
	}
	else if (op==divide)                          // /����
	{
		writecode("-",temp1,temp2,temp3);
		printf("(/,%s,%s,%s)\n",temp1,temp2,temp3);
	}

	return 0;
}

/*��������*/
void writecode(char op[al+1], char arg1[al+1], char arg2[al+1],char result[al+1] )
{
	if (cx >= cxmax)
	{
		printf("Program too long"); /* ������� */
		return ;
	}
	strcpy(code[cx].f, op);
	strcpy(code[cx].l,arg1);
	strcpy(code[cx].r,arg2);
	strcpy(code[cx].t,result);
	cx++;
	return ;
}

/* 
P->DS. 
*/
void  start()
{
	if (sym==intsym ||sym==realsym)
	{
		D();
		S();
		if (sym==period)
		{
			getsym();
			return;
		}
		else {printf("�﷨��2�� ȱ�ٳ������."); exit(0);}
	}
	else {printf("�﷨��3: ����ֻ����int,��real��ʼ���������ִ�Сд"); exit(0);}
}

/*
D->B;D
D->��
*/
void  D()
{
	if (sym==intsym ||sym==realsym)
	{
		B();
		if (ch=';')
		{
			getsym();
			D();
		}
		else {printf("�﷨��4"); exit(0);}
	}
	else if(sym==ident || sym==period)  return;
	else {printf("�﷨��4"); exit(0);}
}

/*
B->int L { L.type := int } | real L { L.type := real }
*/
void  B()
{
	if (sym==intsym )
	{
		getsym();
		L(intsym);
	}
	else if (sym==realsym)
	{
		getsym();
		L(realsym);
	}else {printf("�﷨��5"); exit(0);}
}

/*
L->id { A.Type:=L.type; enter(v.entry,L.type) } A 
V.entryͨ��ȫ�ֱ���tx���Դ���
*/
void L(enum symbol type)
{
	if (sym==ident)
	{
		enter(type);
		getsym();
		A(type);
	}
	else {printf("�﷨��6"); exit(0);}
}

/*
A->��id A { A1.Type:=A.type; enter(v.entry,A.type) }
A->��
*/
void A(enum symbol type)
{
	if (sym==comma)          //��ǰ����Ϊ��
	{
		getsym();
		if (sym==ident)
		{
			enter(type);
			getsym();
			A(type);
		}
		else {printf("�﷨��7"); exit(0);}
	}
	else if (sym== semicolon)   return ; //��ǰ����Ϊ����A��follow��Ԫ�أ��൱�ڽ���A->��
	else {printf("�﷨��7"); exit(0);}
}

/*
S->V:=E { gen( ":=", E.place,0,V.place) } H
*/
void S()
{
	int Vplace,Eplace;
	if (sym==ident)
	{
		Vplace=V();
		jtype=table[Vplace].type;
		//getsym();
		if (sym==becomes)     //��ǰ����Ϊ:=
		{
			getsym();
			Eplace=E();
			gen(becomes,Eplace,-1,Vplace);
			H();
		}
		else
		{printf("�﷨��8"); exit(0);}
	}
	else {printf("�﷨��8"); exit(0);}
}

/*
H->��S | ��
*/
void H()
{
	if (sym==semicolon)          //��ǰ����Ϊindent����
	{
		getsym();
		S();
	}
	else if (sym==period)   return ;
	else {printf("�﷨����9�����û��������β"); exit(0);}
}

/*
E->T { R.i:=T.place} R { E.place:=R.s }
*/
int E()
{
	int ri,tplace,Rs;
	if (sym==ident || sym== lparen)
	{
		tplace=T();
		ri=tplace;
		Rs=R(ri);
	}
	else {printf("�﷨��10"); exit(0);}
	return Rs;
}

/*
T->F { P.i:=T.place} P { T.place:=R.s }
*/
int T()
{
	int pi,fplace,Ps;
	if (sym==ident || sym== lparen)
	{
		fplace=F();
		pi=fplace;
		Ps=P(pi);
	}
	else {printf("�﷨��11"); exit(0);}
	return Ps;
}

/*
R-> +T { R1.i:= newtemp; gen( "*", R.i, T.place , R1.i) } R { R.s:= R1.s; }
R-> -T { R1.i:= newtemp; gen( "*", R.i, T.place , R1.i) } R { R.s:= R1.s; }
R-> �� { R.s=R.i }
*/
int R(int Ri)
{
	int Rs,tplace;
	if (sym==plus || sym==minus)
	{
		if(Ri<=100 && table[Ri].type!=jtype) {printf("�﷨���󣺱���%s���Ͳ�ƥ��",table[Ri].name); exit(0);}
		enum symbol tsym=sym;
		getsym();
		tplace=T();
		if(tplace<=100 && table[tplace].type!=jtype) {printf("�﷨���󣺱���%s���Ͳ�ƥ��",table[tplace].name); exit(0);}
		tv=tv+1;            //������ʱ����
		gen(tsym,Ri,tplace,tv);
		Rs=R(tv);
	}
	else if (sym== semicolon || sym==rparen|| sym==period)
	{
		Rs=Ri;
	}
	else {printf("�﷨��12"); exit(0);}
	return Rs;
}

/*
P-> *F { P1.i:= newtemp; gen( "*", P.i, F.place , P1.i) } P { P.s:= P1.s; }
P-> /F { P1.i:= newtemp; gen( "*", P.i, F.place , P1.i) } P { P.s:= P1.s; }
R-> �� { P.s=P.i }
*/
int P(int Pi)
{
	int Ps,fplace;
	if (sym==times || sym==divide)
	{
		if(Pi<=100 && table[Pi].type!=jtype){printf("�﷨���󣺱���%s���Ͳ�ƥ��",table[Pi].name); exit(0);}
		enum symbol tsym=sym;
		getsym();
		fplace=F();
		if(table[fplace].type!=jtype){printf("�﷨���󣺱���%s���Ͳ�ƥ��",table[fplace].name); exit(0);}
		tv=tv+1;            //������ʱ����
		gen(tsym,Pi,fplace,tv);
		Ps=P(tv);
	}
	else if (sym== semicolon || sym==rparen || sym==period || sym==plus || sym==minus)
	{
		Ps=Pi;
	}
	else {printf("�﷨��13"); exit(0);}
	return Ps;
}



/*
F->( E )  { F.place:=E.place}
F->id     { F.place:=found(id)}
*/
int F()
{
	int Fplace;
	if (sym==ident)
	{
		Fplace=found (id);
		if (Fplace==0)  {printf("����%sû������",id); exit(0);}
		getsym();
	}
	else if (sym== lparen)
	{
		getsym();
		Fplace=E();
		if (sym== rparen)
			getsym();
		else
		{
			printf("�﷨��ȱ)"); exit(0);
		}
	}
	else{printf("�﷨��,ȱ("); exit(0);}
	return Fplace;
}


/*
V->id { V.place:=found(id) }
*/
int V()
{
	int Vplace;
	if (sym==ident)
	{
		Vplace=found (id);
		if (Vplace==0)  {printf("����%sû������",id); exit(0);}
		getsym();
	}
	else{printf("�﷨��14"); exit(0);}
	return Vplace;
}
