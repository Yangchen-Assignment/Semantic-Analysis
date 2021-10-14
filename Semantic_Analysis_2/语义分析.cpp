/*
一个简单的翻译模式
P→ DS.
D→B; D
D→ε
B→int L | real L
L→id | L，id
S→ V := E H
H→；S | ε
E→E+T | T
T→( E )
T→id
V→id


start->   DS.
D->B; D
D->ε
B->int L    { L.type := int }|real L    { L.type := real }
L->id   { A.Type := L.type  enter(v.entry,L.type)}  A
A-> ，idA   { A1.Type := A.type  enter(v.entry,A.type)}
A->ε

S→ V := E { gen( ":=", E.place,0,V.place) } H
H→；S | ε
E->T    { R.i:=T.place}      R     {E.place:=R.s}
R->+T    { R1.i:= newtemp; gen( "*", R.i, T.place , R1.i) }     R   {R.s:= R1.s; }
R-> ε    {Rs=R.i}
T->( E )  { T.place := E.place}
T->id     {T.place:=found (id)}
V->id     {V.place:=found(id)}
*/
#include "语义分析.h"

int main()
{
    char filename[20];
	printf("请输入分析的文件名:");
	gets(filename);
	if((fin=fopen(filename,"r"))==NULL)
	{
		printf("不能打开文件.\n");
		exit(0);
	}

    init();                  //初始化
    getsym();                //读第一个单词，将单词类别放入sym中，单字词值放入id中
	start();                //开始按start->DS.  分析

    if (sym==eeof)
	{
		printf("语法正确\n\n将中间代码保存到文件请输入文件名，否则回车");
		gets(filename);
		if(strlen(filename)<=0) return 0;
		if((fout=fopen(filename,"w"))==NULL)
		{
			printf("不能打开文件.\n");
			exit(0);
		}
		for (int cx1=0;cx1<cx;cx1++)
			fprintf(fout,"(%s,%s,%s,%s)\n",code[cx1].f,code[cx1].l,code[cx1].r,code[cx1].t);
		return 0;
	}
	else {printf("语法错1:  . 后不能再有句子"); exit(0);}
	fclose(fin);
	fclose(fout);
	return 0;
}

/*初始化函数*/
void init()
{
	int i;

	/* 设置单字符符号 */
	for (i=0; i<=255; i++)
	{
		ssym[i] = nul;            //不正确的单字符符号为nul，先预置初值nul
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

	/* 设置保留字名字 */
	strcpy(&(word[0][0]), "real");
	strcpy(&(word[1][0]), "int");

	/* 设置保留字符号 */
	wsym[0] = realsym;
	wsym[1] = intsym;

	tv=100;           //临时变量指针初值，让Tx和tv的取值没有交集，区别到底是临时变变量和声明的变量
	tx=0;           //表指针初值
	cx=0;     //指令计数器

}

/*词法分析，获取一个符号*/
int getsym()
{
	int i,k;
	ch=fgetc(fin);

	if (ch==EOF) {sym=eeof; return 0;}         //文件结束

	while (ch==' ' || ch==10 || ch==13 || ch==9)  /* 忽略空格、换行、回车和TAB */
		ch=fgetc(fin);

	if (ch>='a' && ch<='z')
	{           /* 名字或保留字以a..z开头 ，*/
		k = 0;
		do
		{
			if(k<al)                  /* 符号的最大长度为al ，超长就截尾处理*/
			{
				a[k] = ch;
				k++;
			}
			ch=fgetc(fin);
		} while (ch>='a' && ch<='z' || ch>='0' && ch<='9');

		a[k] = 0;
		strcpy(id, a);
		fseek(fin,-1,1);

		for (i=0;i<norw;i++)        /* 搜索当前符号是否为保留字 */
			 if (strcmp(id,word[i]) == 0)
					 break;
		if (i <norw) {sym = wsym[i];}
		else {sym = ident;}          /* 搜索失败则，类型是标识符 */
		
	}
	else if(ch == ':') /* 检测赋值符号 */
	{
		ch=fgetc(fin);
		if (ch == '=') {sym = becomes;}
		else {sym = nul;}  /* 不能识别的符号 */
	}
	else sym = ssym[ch];     /* 当符号不满足上述条件时，全部按照单字符符号处理 */
	return 0;
}

/*在符号表中加入一项*/
void enter(enum symbol type)
{
	tx=tx+1;
	if (tx > txmax)
    {
		printf(" 符号表越界 ");           /* 符号表越界 */
		return;
	}
	if(found(id)>0) {printf("语法错误：标识符%s不得重复申明",id);exit(0);}
	strcpy(table[tx].name, id); /* 全局变量id中已存有当前标识符的名字,Tx为插入当前符号之前表尾指针 */
	table[tx].type = type;
}

/*
* 查找名字的位置.
* 找到则返回在名字表中的位置,否则返回0.
*
* idt:    要查找的名字
* tx:     当前名字表尾指针，全局变量
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

/* 中间代码*/
int gen(enum symbol op, int arg1, int arg2,int result )
{
	char temp1[al+1],temp2[al+1],temp3[al+1];
	if(arg1>=100)                            //模拟申请临时变量
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
	else if (op==plus)                          // +运算
	{
		writecode("+",temp1,temp2,temp3);
		printf("(+,%s,%s,%s)\n",temp1,temp2,temp3);
	}
	else if (op==minus)                          // -运算
	{
		writecode("-",temp1,temp2,temp3);
		printf("(-,%s,%s,%s)\n",temp1,temp2,temp3);
	}
	else if (op==times)                          // *运算
	{
		writecode("-",temp1,temp2,temp3);
		printf("(*,%s,%s,%s)\n",temp1,temp2,temp3);
	}
	else if (op==divide)                          // /运算
	{
		writecode("-",temp1,temp2,temp3);
		printf("(/,%s,%s,%s)\n",temp1,temp2,temp3);
	}

	return 0;
}

/*处理代码段*/
void writecode(char op[al+1], char arg1[al+1], char arg2[al+1],char result[al+1] )
{
	if (cx >= cxmax)
	{
		printf("Program too long"); /* 程序过长 */
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
		else {printf("语法错2： 缺少程序结束."); exit(0);}
	}
	else {printf("语法错3: 程序只能用int,和real开始，而且区分大小写"); exit(0);}
}

/*
D->B;D
D->ε
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
		else {printf("语法错4"); exit(0);}
	}
	else if(sym==ident || sym==period)  return;
	else {printf("语法错4"); exit(0);}
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
	}else {printf("语法错5"); exit(0);}
}

/*
L->id { A.Type:=L.type; enter(v.entry,L.type) } A 
V.entry通过全局变量tx隐性传递
*/
void L(enum symbol type)
{
	if (sym==ident)
	{
		enter(type);
		getsym();
		A(type);
	}
	else {printf("语法错6"); exit(0);}
}

/*
A->，id A { A1.Type:=A.type; enter(v.entry,A.type) }
A->ε
*/
void A(enum symbol type)
{
	if (sym==comma)          //当前单词为，
	{
		getsym();
		if (sym==ident)
		{
			enter(type);
			getsym();
			A(type);
		}
		else {printf("语法错7"); exit(0);}
	}
	else if (sym== semicolon)   return ; //当前单词为；即A的follow集元素，相当于进行A->ε
	else {printf("语法错7"); exit(0);}
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
		if (sym==becomes)     //当前单词为:=
		{
			getsym();
			Eplace=E();
			gen(becomes,Eplace,-1,Vplace);
			H();
		}
		else
		{printf("语法错8"); exit(0);}
	}
	else {printf("语法错8"); exit(0);}
}

/*
H->；S | ε
*/
void H()
{
	if (sym==semicolon)          //当前单词为indent类型
	{
		getsym();
		S();
	}
	else if (sym==period)   return ;
	else {printf("语法错误9：语句没有正常结尾"); exit(0);}
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
	else {printf("语法错10"); exit(0);}
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
	else {printf("语法错11"); exit(0);}
	return Ps;
}

/*
R-> +T { R1.i:= newtemp; gen( "*", R.i, T.place , R1.i) } R { R.s:= R1.s; }
R-> -T { R1.i:= newtemp; gen( "*", R.i, T.place , R1.i) } R { R.s:= R1.s; }
R-> ε { R.s=R.i }
*/
int R(int Ri)
{
	int Rs,tplace;
	if (sym==plus || sym==minus)
	{
		if(Ri<=100 && table[Ri].type!=jtype) {printf("语法错误：变量%s类型不匹配",table[Ri].name); exit(0);}
		enum symbol tsym=sym;
		getsym();
		tplace=T();
		if(tplace<=100 && table[tplace].type!=jtype) {printf("语法错误：变量%s类型不匹配",table[tplace].name); exit(0);}
		tv=tv+1;            //生成临时变量
		gen(tsym,Ri,tplace,tv);
		Rs=R(tv);
	}
	else if (sym== semicolon || sym==rparen|| sym==period)
	{
		Rs=Ri;
	}
	else {printf("语法错12"); exit(0);}
	return Rs;
}

/*
P-> *F { P1.i:= newtemp; gen( "*", P.i, F.place , P1.i) } P { P.s:= P1.s; }
P-> /F { P1.i:= newtemp; gen( "*", P.i, F.place , P1.i) } P { P.s:= P1.s; }
R-> ε { P.s=P.i }
*/
int P(int Pi)
{
	int Ps,fplace;
	if (sym==times || sym==divide)
	{
		if(Pi<=100 && table[Pi].type!=jtype){printf("语法错误：变量%s类型不匹配",table[Pi].name); exit(0);}
		enum symbol tsym=sym;
		getsym();
		fplace=F();
		if(table[fplace].type!=jtype){printf("语法错误：变量%s类型不匹配",table[fplace].name); exit(0);}
		tv=tv+1;            //生成临时变量
		gen(tsym,Pi,fplace,tv);
		Ps=P(tv);
	}
	else if (sym== semicolon || sym==rparen || sym==period || sym==plus || sym==minus)
	{
		Ps=Pi;
	}
	else {printf("语法错13"); exit(0);}
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
		if (Fplace==0)  {printf("变量%s没有声明",id); exit(0);}
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
			printf("语法错，缺)"); exit(0);
		}
	}
	else{printf("语法错,缺("); exit(0);}
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
		if (Vplace==0)  {printf("变量%s没有声明",id); exit(0);}
		getsym();
	}
	else{printf("语法错14"); exit(0);}
	return Vplace;
}
