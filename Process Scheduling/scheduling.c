#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct pNode
{
    int ID;
    int Priority;
    int Computing_Time; // ��ǻ�� �ð�(���ϸ� �ȵ�)
    int Remain;
    char state[10];
    struct pNode* Next; // ���� ��� ������
}Node;

typedef struct queue
{
    Node* Front;
    Node* Rear;
    int Count;
}Queue;

// ���� �ڿ� �ڼ��� ����
void push(Queue*, Node*);
Node* newProcess(int, int, int);
Node* pop(Queue*);
void print_queue(Queue*);

// ������ �ۼ�
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
    if (fp == NULL)  return 0; //Input.txt ������ ���ٸ� 0�� ��ȯ
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

            else if (type == 1) // FCFS �����ٸ�
            {
                FCFS();
            }

            else // ���� ���μ��� �����ٸ�
            {
                sorted_queue();
                FCFS();
            }
        } fclose(fp);
    } return 0;
}

// ���� ���� ��忡 �μ��� ���� ��带 �޾���
void push(Queue* q, Node* n)
{
    memset(n->state, "\0", sizeof(n->state));
    strcpy(n->state, "READY"); // �غ� ť�� ���μ����� Ǫ���ϹǷ� ���¸� Ready�� ����
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

//���� �� ��带 ����� ��ȯ
Node* pop(Queue* q)
{
    if (q->Count == 0) return NULL;

    Node* n = q->Front;
    q->Front = n->Next;
    q->Count--;

    return n;
}

//���ο� ���(���μ���)�� ���� ��ȯ
Node* newProcess(int id, int priority, int computing_time)
{
    Node* n = (Node*)malloc(sizeof(Node)); // Node�� ���� ���ο� �޸� ������ �Ҵ�����.
    n->Computing_Time = computing_time;
    n->Remain = computing_time;
    n->ID = id;
    n->Priority = priority;
    strcpy(n->state, "NEW"); // ���μ��� ���� NEW(���� ������)�� ����
    return n;
}

// �迭�� �����ϴ� ��� ���� �ջ� �ϴ� �Լ�
float get_sum(int* arr)
{
    int size = sizeof(arr) / sizeof(int); // �迭 ũ�� ���ϱ�
    float total = 0;
    for (int i = 0; i < size; i++)
    {
        total += arr[i]; // ���� �ջ�
    }
    return total; // total ��ȯ
}

void sorted_queue() {
    Node* tmp; // �ӽ� ���
    Node* head = q.Front; // ť�� ����Ʈ�� ���� ����

    while (head->Next != NULL) // ����� next�� null�� �ɶ����� ����
    {
        if (head->Priority > head->Next->Priority) // ���� ����� �켱������ ���� ����� �켱�������� ũ�ٸ�
        {
            tmp = head->Next; // ������ �Űܾ��� ��带 tmp�� ����
            head->Next = head->Next->Next; // tmp�� ������ ��带 �����ϰ� ����
            tmp->Next = q.Front; // temp�� next�� ť�� ����Ʈ�� ����Ŵ
            q.Front = head = tmp; // ť�� ����Ʈ�� head�� tmp�� �ʱ�ȭ ���� ��� ���� ù�ڸ��� �̵�
            continue; // ���� ������ �� head�� head = head->next�� ���� ������ �̵����� ���ϰ� ó��
        }
        head = head->Next; // ���� ����� �ּҸ� head�� ����
    }
    head = q.Front;
}

void FCFS() {
    float avg_tat = 0.0; // ��� turn around time
    float avg_waittime = 0.0; // ��� ��� �ð�

    float waittime_total = 0.0; // ���ð� ����
    float tat_total = 0.0; // turn around time ����
    float normalized_avg_tat = 0.0; // ����ȭ�� ��� tat
    int process_number = q.Count; // �� ���μ��� ����

    int* wait_time = (int*)malloc(sizeof(int) * process_number); // ���μ��� �� wait time�� ������ �迭
    int* tat = (int*)malloc(sizeof(int) * process_number); // ���μ��� �� turn around time�� ������ �迭
    int prev_computing_time = 0; // ���� ��� �ð�
    int i = 0; // �迭 �ε��� ���� ����

    // �� �迭 ��� 0���� �ʱ�ȭ
    memset(wait_time, 0, sizeof(wait_time));
    memset(tat, 0, sizeof(tat));

    while (0 < q.Count) {
        Node* n = pop(&q); // ť���� ���μ����� pop
        memset(n->state, "\0", sizeof(n->state));
        strcpy(n->state, "RUNNING"); // ���μ��� ���¸� RUNNING���� ����
        // ���� ��� �����ٸ�
        if (i == 0)
        {
            wait_time[i] = n->Computing_Time;
            tat[i] = wait_time[i] + n->Computing_Time;
            prev_computing_time = n->Computing_Time;
        }

        else {
            wait_time[i] = wait_time[i - 1] + prev_computing_time; // ���� wait time�� computing time�� �ջ��Ͽ� ���� ���μ����� wait time ���
            tat[i] = wait_time[i] + n->Computing_Time; // ��ȯ�ð� = ���ð� + ���� �ð�, wait time�� computing time�� �ջ��Ͽ� turn around time ���
            prev_computing_time = n->Computing_Time; // ���� ���μ����� computing time�� prev_computing_time�� �����ϰ�, ���� ���μ����� �̸� ���� wait time�� turn around time���
        }

        printf("%d %14d %11d %18d\n", n->ID, n->Priority, n->Computing_Time, tat[i]);
        i++;
    }
    waittime_total = get_sum(wait_time);
    tat_total = get_sum(tat);

    avg_waittime = waittime_total / process_number; // ��� ���ð�
    avg_tat = tat_total / process_number; // ��� ��ȯ�ð�

    // ��� ��ȯ �ð�
    normalized_avg_tat = avg_tat / avg_waittime; // ����ȭ�� ��� ��ȯ �ð� ���
    printf("Normalized average turn around time is %.2f\n", normalized_avg_tat);

    // �����Ҵ� ����
    free(wait_time);
    free(tat);
}

void remain_scheduling()
{
    sorted_queue();
    printf("\nScheduling remain process...\n");
    float avg_tat = 0.0; // ��� turn around time
    float avg_waittime = 0.0; // ��� ��� �ð�

    float waittime_total = 0.0; // ���ð� ����
    float tat_total = 0.0; // turn around time ����
    float normalized_avg_tat = 0.0; // ����ȭ�� ��� tat

    int* wait_time = NULL; // ���μ��� �� wait time�� ������ �迭
    int* tat = NULL; // ���μ��� �� turn around time�� ������ �迭
    int* arrival_time = NULL; // ���μ��� �����ð�

    int prev_computing_time = 0; // ���� ��� �ð�
    int i = 0; // �迭 �ε��� ���� ����

    int process_number = q.Count; // �� ���μ��� ����

    wait_time = (int*)malloc(sizeof(int) * process_number); // ���μ��� ������ŭ �����Ҵ�
    tat = (int*)malloc(sizeof(int) * process_number); // ���μ��� ������ŭ �����Ҵ�

    // �� �迭 ��� 0���� �ʱ�ȭ
    memset(wait_time, 0, sizeof(wait_time));
    memset(tat, 0, sizeof(tat));

    while (0 < q.Count) {
        Node* n = pop(&q); // ť���� ���μ����� pop
        memset(n->state, "\0", sizeof(n->state));
        strcpy(n->state, "RUNNING"); // ���μ��� ���¸� RUNNING���� ����
        // ���� ��� �����ٸ�
        if (i == 0)
        {
            wait_time[i] = n->Computing_Time;
            tat[i] = wait_time[i] + n->Computing_Time;
            prev_computing_time = n->Computing_Time;
        }

        else {
            wait_time[i] = wait_time[i - 1] + prev_computing_time; // ���� wait time�� computing time�� �ջ��Ͽ� ���� ���μ����� wait time ���
            tat[i] = wait_time[i] + n->Computing_Time; // ��ȯ�ð� = ���ð� + ���� �ð�, wait time�� computing time�� �ջ��Ͽ� turn around time ���
            prev_computing_time = n->Computing_Time; // ���� ���μ����� computing time�� prev_computing_time�� �����ϰ�, ���� ���μ����� �̸� ���� wait time�� turn around time���
        }

        printf("%d %14d %11d %18d\n", n->ID, n->Priority, n->Computing_Time, tat[i]);
        i++;
    }
    waittime_total = get_sum(wait_time);
    tat_total = get_sum(tat);

    avg_waittime = waittime_total / process_number; // ��� ���ð�
    avg_tat = tat_total / process_number; // ��� ��ȯ�ð�

    // ��� ��ȯ �ð�
    normalized_avg_tat = avg_tat / avg_waittime; // ����ȭ�� ��� ��ȯ �ð� ���
    printf("Normalized average turn around time is %.2f\n", normalized_avg_tat);

    // �����Ҵ� ����
    free(wait_time);
    free(tat);
}