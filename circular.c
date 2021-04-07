#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;                // 배열에 들어갈 요소 char 형 변수 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType* createQueue();                     // 큐 생성
int freeQueue(QueueType* cQ);                 // 동적할당 해제  
int isEmpty(QueueType* cQ);                   //큐가 비었는지 확인
int isFull(QueueType* cQ);                    //큐가 차있는지 확인
void enQueue(QueueType* cQ, element item);    // 큐에 원소 추가
void deQueue(QueueType* cQ, element* item);   // 큐에 원소 삭제
void printQ(QueueType* cQ);                   //큐 원소 출력 
void debugQ(QueueType* cQ);                   // front, rear, 원소 출력 
element getElement();                         // 원소 입력받음 

int main(void)
{
	QueueType* cQ = createQueue(); // 큐 동적 할당 후 *cQ에 큐 배열 연결 
	element data;
	char command;

	do {
		printf("---------------[Seojin LEE] [2020039034]------------\n");
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':                      // i일 때 데이터 입력받고 enQueue함수에 cQ와 data 전달 (원소 넣음)
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':                      // d일때, deQueue함수에 cQ와 data 전달(원소 제거)
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':                      // p일때 원소 출력
			printQ(cQ);
			break;
		case 'b': case 'B':                      // b일 때 큐의 현재 상태 출력
			debugQ(cQ);
			break;
		case 'q': case 'Q':                      //q일 때 동적하당 해제하는 cQ에 전달
			freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');  //q 또는 Q 입력 될때까지 반복


	return 1;
}

QueueType* createQueue()                            // 큐 생성
{
	QueueType* cQ;                                                  // 구조체 포인터 cQ선언
	cQ = (QueueType*)calloc(MAX_QUEUE_SIZE, sizeof(QueueType));     //구조체 QueueType의 크기만큼 cQ 동적할당
	cQ->front = 0;
	cQ->rear = 0;
	return cQ;                                                      //구조체 포인터의 주소를 return함
}

int freeQueue(QueueType* cQ)
{
	if (cQ == NULL) return 1;                                       // cQ가 비었으면 1을 리턴
	free(cQ);                                                       // cQ 동적할당을 해제 
	return 1;
}

element getElement()                                                // 문자 한 개 입력받음
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;                                                    //문자 반환
}


/* complete the function */
int isEmpty(QueueType* cQ)                                          //큐가 비어있는지 확인
{
	if (cQ->front == cQ->rear)
	{
		printf("큐가 비어있습니다.!\n");                           //오류메세지 출력
		return -1;
	}
	else
		return 0;
}

/* complete the function */
int isFull(QueueType* cQ)                                          // 큐가 차있는지 확인
{
	cQ->rear = ((cQ->rear) + 1) % MAX_QUEUE_SIZE;
	if (cQ->front == cQ->rear) // 
	{
		printf("큐가 가득 찼습니다.\n");                        // 큐가 가득차있으면 메세지 출력
		cQ->rear = ((cQ->rear) + (MAX_QUEUE_SIZE - 1)) % MAX_QUEUE_SIZE;              // rear 원래대로 돌려놓기
		return -1;
	}
	else
		return 0;                                                 //가득차 있지 않으면 return 0
}


/* complete the function */
void enQueue(QueueType* cQ, element item)                            //큐에 item 추가
{
	if (isFull(cQ) == 0) // 
	{
		cQ->rear = (cQ->rear) % MAX_QUEUE_SIZE;                      //큐가 가득 차 있지 않으면 rear값 증가
		cQ->queue[cQ->rear] = item;                                 // 큐에 데이터를 삽입
	}

	return 0;
}

/* complete the function */
void deQueue(QueueType* cQ, element* item)                 // 큐 안에 있는 데이터 삭제
{
	if (isEmpty(cQ) == 0)
	{
		cQ->front = ((cQ->front) + 1) % MAX_QUEUE_SIZE;    //큐가 비어있지 않으면 front를 하나 증가시키고 %MAX_QUEUE_SIZE 
		cQ->queue[cQ->front] = '\0';                       //증가한 front가 가르키는 곳의 값을 null로 초기화
	}
	return 0;
}


void printQ(QueueType* cQ)                          // 큐에 들어있는 데이터 출력
{
	int i, first, last;

	first = (cQ->front + 1) % MAX_QUEUE_SIZE;       //first 값 하나 증가 
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;          // rear값 하나 증가 

	printf("Circular Queue : [");

	i = first;
	while (i != last) {                             //first == last 까지 원소 출력 
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE;               //i 를 하나 씩 증가 

	}
	printf(" ]\n");
}


void debugQ(QueueType* cQ) // 큐의 전체 내역(상황) 출력
{

	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if (i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;                                                  //cQ->front 있는지 확인 후 어디에있는지 출력하고 계속 진행
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);                      //큐 원소 순서대로 출력 

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);             //front, rear 출력 
}
