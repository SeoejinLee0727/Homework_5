/*
 * postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

 /* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;


char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);                        // 스택에 Push하는 함수
char postfixPop();                               // 스택에 Pop하는 함수
void evalPush(int x);                            // 계산하는 스택에 Push하는 함수
int evalPop();                                   // 계산하는 스택에 Pop하는 함수
void getInfix();                                 // infix을 입력하는 함수
precedence getToken(char symbol);                // 연산자 분류하는 함수
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();                                // postfix로 변환하늖 함수
void debug();                                    // 출력하는 함수
void reset();
void evaluation();                               // 계산하늖 마수

int main()
{
	char command;
	
	printf("------------------[Seojin Lee] [2020039034]---------------------\n");

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':       // i 또는 I면, infix형태로 식을 입력받음
			getInfix();
			break;
		case 'p': case 'P':       // p 또는 P면, 이미 입력된 infix를 postfix형태로 바꿔줌
			toPostfix();
			break;
		case 'e': case 'E':       // e 또는 E면, 식을 계산g함
			evaluation();
			break;
		case 'd': case 'D':       // 식에 대한 정보를 나열하여 보여줌
			debug();
			break;
		case 'r': case 'R':       // 배열에 들어있는 것을 다 리셋시킴
			reset();
			break;
		case 'q': case 'Q':       // 프로그램 종료
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)                    // postfix스택에 원소 삽입
{
	postfixStack[++postfixStackTop] = x;    // 먼저 Top의 크기를 하나 증가시킨 후에 Top+1 자리에 x값을 넣음
}

char postfixPop()                           
{
	char x;
	if (postfixStackTop == -1)               //값이 -1이면 널값 반환
		return '\0';
	else {
		x = postfixStack[postfixStackTop--]; //-1이 아니면 값을 x에 넣은 후에, Top에서 1을 뺌
	}
	return x;
}

void evalPush(int x)
{
	evalStack[++evalStackTop] = x;             // 계산스택에 값을 넣음
}

int evalPop()
{
	if (evalStackTop == -1)
		return -1;
	else
		return evalStack[evalStackTop--];     // 계산스택에서 out시킴
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);              // infixExp에 입력된 값 저장함
}

precedence getToken(char symbol)       // symbol을 받아 기호를 판단하여 리턴을 알려줌
{
	switch (symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	default: return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);               // x를 받아서, symbol 판단 함수 호출함
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)     
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);       // 문자열이 없으면 c를 postfixExp에 c복사해서 붙여넣음
	else
		strncat(postfixExp, c, 1);       // 문자열이 있으면 c를 뒤에 붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	char* exp = infixExp; //infiXExp를 가리키는 포인터 변수 선언 

	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	char y;          //pop값을 저장할 변수 

	while (*exp != '\0')
	{
		x = *exp;

		if (getPriority(x) == 1)         //x가 피연산자면  
		{
			charCat(&x);                // x를 postfixExp에 저장 
		}

		//피연산자가 아니면  
		else if (getPriority(x) == 9)       //닫는 괄호를 만날 경우 
		{
			while (postfixStack[postfixStackTop] != '(')        //왼쪽 괄호가 아니면 반복
			{
				y = postfixPop();          // pop해서 y에 저장해줌
				charCat(&y);               //p값을 postfixExp에 push 해줌
			}
			postfixPop();                  // (를 pop해주기
		}

		else if (getPriority(x) == 0)      // 왼쪽 괄호를 만났을 경우
		{
			postfixPush(x++);
		}

		// 연산자를 만났을 경우 

		else if (getPriority(x) <= getPriority(postfixStack[postfixStackTop])) //infixexp 연산자가 postfixstacktop보다 작거나 같을 때
		{
			y = postfixPop();              // pop해서p에 저장 
			charCat(&y);
			postfixPush(x);                //postfixExp에 push 해줌
		}

		else
		{
			postfixPush(x);
		}

		exp++; // 다음 문자 읽기
	}

	while (postfixStackTop != -1) {      //스택의 top이 -1이 아니면 ( 비어있지 않으면 )
		y = postfixPop();              // pop해줌 
		charCat(&y);                   //변환하고 있는 식에 알맞게 적용
	}

}


void debug()        // 각종 상태를 보여줌
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);      // 어떻게 입력받았는지 보여주기
	printf("postExp =  %s\n", postfixExp);     // 후위변환한 식 출력
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);      ///postfixStack안에 있는 인덱스 출력

	printf("\n");

}

void reset()                                   // 초기화해줌
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)     // //스택 값도 다 없애줌
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()   //변환된 식 계산
{
	char* ptr = postfixExp;      // postfixExp 가리키는 포인터 선언 
	char X;
	char Y;                     // 피연산자 변수 선언 
	while (*ptr != '\0')
	{
		if (getPriority(*ptr) == operand)          
			evalPush((*ptr) - '0');             // 숫자일 때 스택에 저장
		else                                     // 연산자이면
		{
			Y = evalPop();
			X = evalPop();
			switch (getPriority(*ptr))
			{
			case plus:
				evalPush(X + Y);
				break;
			case minus:
				evalPush(X - Y);
				break;
			case times:
				evalPush(X * Y);
				break;
			case divide:
				evalPush(X / Y);
				break;
			}
		}
		ptr++;  // 다음 원소 읽기
	}

	evalResult = evalStack[0];              // 결과 저장

}
