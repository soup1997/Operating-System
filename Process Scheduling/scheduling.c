#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct pNode
{
    int ID;
    int Priority;
    int Computing_Time; // 컴퓨팅 시간(변하면 안됨)
    int Remain;
    char state[10];
    struct pNode* Next; // 다음 노드 포인터
}Node;

typedef struct queue
{
    Node* Front;
    Node* Rear;
    int Count;
}Queue;

// 메인 뒤에 자세한 설명
void push(Queue*, Node*);
Node* newProcess(int, int, int);
Node* pop(Queue*);
void print_queue(Queue*);

// 조현습 작성
void sorted_queue();
void FCFS();
float get_sum(int*);
void remain_scheduling();
Queue q;

int main()
{
    int type = 0;
    int id, pri, computing_time;
    int scheduling_mode;

    FILE* fp = NULL;

    fp = fopen("Input.txt", "r");
    if (fp == NULL)  return 0; //Input.txt 파일이 없다면 0을 반환
    else {
        int flag = 1;
        printf("\nProcess_id    Priority    computing_time    turn_around_time\n");
        while (1)
        {
            fscanf(fp, "%d %d %d %d", &type, &id, &pri, &computing_time);

            if (type == 0) {
                Node* n = newProcess(id, pri, computing_time);
                push(&q, n);
            }

            else if (type == 1) // FCFS 스케줄링
            {
                FCFS();
            }

            else // 남은 프로세스 스케줄링
            {
                sorted_queue();
                FCFS();
            }
        } fclose(fp);
    } return 0;
}

// 가장 뒤의 노드에 인수로 받은 노드를 달아줌
void push(Queue* q, Node* n)
{
    memset(n->state, "\0", sizeof(n->state));
    strcpy(n->state, "READY"); // 준비 큐에 프로세스를 푸쉬하므로 상태를 Ready로 변경
    if (q->Count == 0)
    {
        q->Front = n;
        q->Rear = n;
        q->Count++;
    }
    else
    {
        q->Rear->Next = n;
        q->Rear = n;
        n->Next = NULL;
        q->Count++;
    }
}

//가장 앞 노드를 끊어내고 반환
Node* pop(Queue* q)
{
    if (q->Count == 0) return NULL;

    Node* n = q->Front;
    q->Front = n->Next;
    q->Count--;

    return n;
}

//새로운 노드(프로세스)를 만들어서 반환
Node* newProcess(int id, int priority, int computing_time)
{
    Node* n = (Node*)malloc(sizeof(Node)); // Node를 위한 새로운 메모리 공간을 할당해줌.
    n->Computing_Time = computing_time;
    n->Remain = computing_time;
    n->ID = id;
    n->Priority = priority;
    strcpy(n->state, "NEW"); // 프로세스 상태 NEW(새로 생성됨)로 설정
    return n;
}

// 배열에 존재하는 모든 값을 합산 하는 함수
float get_sum(int* arr)
{
    int size = sizeof(arr) / sizeof(int); // 배열 크기 구하기
    float total = 0;
    for (int i = 0; i < size; i++)
    {
        total += arr[i]; // 누적 합산
    }
    return total; // total 반환
}

void sorted_queue() {
    Node* tmp; // 임시 노드
    Node* head = q.Front; // 큐의 프론트를 헤드로 설정

    while (head->Next != NULL) // 노드의 next가 null이 될때까지 실행
    {
        if (head->Priority > head->Next->Priority) // 현재 헤드의 우선순위가 다음 노드의 우선순위보다 크다면
        {
            tmp = head->Next; // 앞으로 옮겨야할 노드를 tmp에 저장
            head->Next = head->Next->Next; // tmp에 저장한 노드를 제외하고 연결
            tmp->Next = q.Front; // temp의 next는 큐의 프론트를 가리킴
            q.Front = head = tmp; // 큐의 프론트와 head를 tmp로 초기화 시켜 모두 가장 첫자리로 이동
            continue; // 가장 앞으로 간 head가 head = head->next로 인해 앞으로 이동하지 못하게 처리
        }
        head = head->Next; // 다음 노드의 주소를 head로 지정
    }
    head = q.Front;
}

void FCFS() {
    float avg_tat = 0.0; // 평균 turn around time
    float avg_waittime = 0.0; // 평균 대기 시간

    float waittime_total = 0.0; // 대기시간 총합
    float tat_total = 0.0; // turn around time 총합
    float normalized_avg_tat = 0.0; // 정규화된 평균 tat
    int process_number = q.Count; // 총 프로세스 갯수

    int* wait_time = (int*)malloc(sizeof(int) * process_number); // 프로세스 당 wait time을 저장할 배열
    int* tat = (int*)malloc(sizeof(int) * process_number); // 프로세스 당 turn around time을 저장할 배열
    int prev_computing_time = 0; // 이전 계산 시간
    int i = 0; // 배열 인덱스 접근 변수

    // 두 배열 모두 0으로 초기화
    memset(wait_time, 0, sizeof(wait_time));
    memset(tat, 0, sizeof(tat));

    while (0 < q.Count) {
        Node* n = pop(&q); // 큐에서 프로세스를 pop
        memset(n->state, "\0", sizeof(n->state));
        strcpy(n->state, "RUNNING"); // 프로세스 상태를 RUNNING으로 변경
        // 비선점 방식 스케줄링
        if (i == 0)
        {
            wait_time[i] = n->Computing_Time;
            tat[i] = wait_time[i] + n->Computing_Time;
            prev_computing_time = n->Computing_Time;
        }

        else {
            wait_time[i] = wait_time[i - 1] + prev_computing_time; // 이전 wait time과 computing time을 합산하여 현재 프로세스의 wait time 계산
            tat[i] = wait_time[i] + n->Computing_Time; // 반환시간 = 대기시간 + 서비스 시간, wait time과 computing time을 합산하여 turn around time 계산
            prev_computing_time = n->Computing_Time; // 현재 프로세스의 computing time을 prev_computing_time에 대입하고, 다음 프로세스는 이를 토대로 wait time과 turn around time계산
        }

        printf("%d %14d %11d %18d\n", n->ID, n->Priority, n->Computing_Time, tat[i]);
        i++;
    }
    waittime_total = get_sum(wait_time);
    tat_total = get_sum(tat);

    avg_waittime = waittime_total / process_number; // 평균 대기시간
    avg_tat = tat_total / process_number; // 평균 반환시간

    // 평균 반환 시간
    normalized_avg_tat = avg_tat / avg_waittime; // 정규화된 평균 반환 시간 계산
    printf("Normalized average turn around time is %.2f\n", normalized_avg_tat);

    // 동적할당 해제
    free(wait_time);
    free(tat);
}

void remain_scheduling()
{
    sorted_queue();
    printf("\nScheduling remain process...\n");
    float avg_tat = 0.0; // 평균 turn around time
    float avg_waittime = 0.0; // 평균 대기 시간

    float waittime_total = 0.0; // 대기시간 총합
    float tat_total = 0.0; // turn around time 총합
    float normalized_avg_tat = 0.0; // 정규화된 평균 tat

    int* wait_time = NULL; // 프로세스 당 wait time을 저장할 배열
    int* tat = NULL; // 프로세스 당 turn around time을 저장할 배열
    int* arrival_time = NULL; // 프로세스 도착시간

    int prev_computing_time = 0; // 이전 계산 시간
    int i = 0; // 배열 인덱스 접근 변수

    int process_number = q.Count; // 총 프로세스 갯수

    wait_time = (int*)malloc(sizeof(int) * process_number); // 프로세스 갯수만큼 동적할당
    tat = (int*)malloc(sizeof(int) * process_number); // 프로세스 갯수만큼 동적할당

    // 두 배열 모두 0으로 초기화
    memset(wait_time, 0, sizeof(wait_time));
    memset(tat, 0, sizeof(tat));

    while (0 < q.Count) {
        Node* n = pop(&q); // 큐에서 프로세스를 pop
        memset(n->state, "\0", sizeof(n->state));
        strcpy(n->state, "RUNNING"); // 프로세스 상태를 RUNNING으로 변경
        // 비선점 방식 스케줄링
        if (i == 0)
        {
            wait_time[i] = n->Computing_Time;
            tat[i] = wait_time[i] + n->Computing_Time;
            prev_computing_time = n->Computing_Time;
        }

        else {
            wait_time[i] = wait_time[i - 1] + prev_computing_time; // 이전 wait time과 computing time을 합산하여 현재 프로세스의 wait time 계산
            tat[i] = wait_time[i] + n->Computing_Time; // 반환시간 = 대기시간 + 서비스 시간, wait time과 computing time을 합산하여 turn around time 계산
            prev_computing_time = n->Computing_Time; // 현재 프로세스의 computing time을 prev_computing_time에 대입하고, 다음 프로세스는 이를 토대로 wait time과 turn around time계산
        }

        printf("%d %14d %11d %18d\n", n->ID, n->Priority, n->Computing_Time, tat[i]);
        i++;
    }
    waittime_total = get_sum(wait_time);
    tat_total = get_sum(tat);

    avg_waittime = waittime_total / process_number; // 평균 대기시간
    avg_tat = tat_total / process_number; // 평균 반환시간

    // 평균 반환 시간
    normalized_avg_tat = avg_tat / avg_waittime; // 정규화된 평균 반환 시간 계산
    printf("Normalized average turn around time is %.2f\n", normalized_avg_tat);

    // 동적할당 해제
    free(wait_time);
    free(tat);
}