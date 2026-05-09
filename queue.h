/*
 * QUEUE MODULE HEADER
 * Token system for bank visit simulation using array-based queue
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "banking_system.h"
#include "utils.h"

// Queue structure
typedef struct {
    QueueNode items[MAX_QUEUE];
    int front;
    int rear;
    int size;
} Queue;

// Function prototypes
Queue* create_queue();
bool is_queue_empty(Queue *q);
bool is_queue_full(Queue *q);
int enqueue(Queue *q, int account_number, char *name, char *service_type);
QueueNode* dequeue(Queue *q);
void display_queue(Queue *q);
void view_token_status(Queue *q, int token_number);
int get_token_number();
void process_next_customer(Queue *q);
void print_token_receipt(int token_number, char *customer_name, char *service_type);

#endif // QUEUE_H
