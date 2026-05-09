/*
 * QUEUE MODULE IMPLEMENTATION (Token System)
 */

#include "queue.h"

static int token_counter = 100;

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    return q;
}

bool is_queue_empty(Queue *q) {
    return (q->size == 0);
}

bool is_queue_full(Queue *q) {
    return (q->size == MAX_QUEUE);
}

int enqueue(Queue *q, int account_number, char *name, char *service_type) {
    if (is_queue_full(q)) {
        print_error("Queue is full!");
        return -1;
    }
    
    int token = get_token_number();
    q->rear = (q->rear + 1) % MAX_QUEUE;
    
    q->items[q->rear].token_number = token;
    q->items[q->rear].account_number = account_number;
    strcpy(q->items[q->rear].customer_name, name);
    strcpy(q->items[q->rear].service_type, service_type);
    q->items[q->rear].arrival_time = time(NULL);
    
    q->size++;
    return token;
}

QueueNode* dequeue(Queue *q) {
    if (is_queue_empty(q)) {
        return NULL;
    }
    
    QueueNode *node = &q->items[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->size--;
    
    return node;
}

void display_queue(Queue *q) {
    printf("\n");
    print_header("QUEUE STATUS");
    
    if (is_queue_empty(q)) {
        printf("Queue is empty\n");
        print_line(70);
        return;
    }
    
    printf("%-10s %-20s %-20s %-15s\n", "Token", "Customer", "Service", "Wait Time");
    print_line(70);
    
    int current = q->front;
    for (int i = 0; i < q->size; i++) {
        time_t now = time(NULL);
        int wait_time = (int)difftime(now, q->items[current].arrival_time) / 60;
        
        printf("%-10d %-20s %-20s %-15d min\n",
               q->items[current].token_number,
               q->items[current].customer_name,
               q->items[current].service_type,
               wait_time);
        
        current = (current + 1) % MAX_QUEUE;
    }
    print_line(70);
}

void view_token_status(Queue *q, int token_number) {
    printf("\n");
    print_header("TOKEN STATUS");
    
    int current = q->front;
    int position = 0;
    bool found = false;
    
    for (int i = 0; i < q->size; i++) {
        if (q->items[current].token_number == token_number) {
            found = true;
            printf("%-40s: %d\n", "Token Number", token_number);
            printf("%-40s: %d / %d\n", "Position in Queue", position + 1, q->size);
            printf("%-40s: %s\n", "Customer Name", q->items[current].customer_name);
            printf("%-40s: %s\n", "Service Type", q->items[current].service_type);
            
            time_t now = time(NULL);
            int wait_time = (int)difftime(now, q->items[current].arrival_time) / 60;
            printf("%-40s: %d minutes\n", "Wait Time", wait_time);
            break;
        }
        current = (current + 1) % MAX_QUEUE;
        position++;
    }
    
    if (!found) {
        print_error("Token not found in queue!");
    }
    
    print_line(70);
}

int get_token_number() {
    return ++token_counter;
}

void process_next_customer(Queue *q) {
    if (is_queue_empty(q)) {
        print_error("Queue is empty!");
        return;
    }
    
    QueueNode *node = dequeue(q);
    printf("\n");
    print_header("CUSTOMER PROCESSED");
    printf("%-40s: %d\n", "Token Number", node->token_number);
    printf("%-40s: %s\n", "Customer Name", node->customer_name);
    printf("%-40s: %s\n", "Service Type", node->service_type);
    printf("%-40s: %s\n", "Status", "PROCESSED");
    print_line(70);
    
    printf("\nRemaining customers in queue: %d\n", q->size);
}

void print_token_receipt(int token_number, char *customer_name, char *service_type) {
    printf("\n");
    print_header("TOKEN RECEIPT");
    printf("%-40s: %d\n", "Token Number", token_number);
    printf("%-40s: %s\n", "Customer Name", customer_name);
    printf("%-40s: %s\n", "Service Type", service_type);
    printf("%-40s: %s\n", "Date", get_current_date());
    printf("%-40s: %s\n", "Time", get_current_time());
    printf("%-40s", "Instructions");
    printf("Please wait for your token to be called\n");
    print_line(70);
}
