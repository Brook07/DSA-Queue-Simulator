// simulator.c: Simulates traffic management using queues
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10
#define PRIORITY_REDUCTION 5
#define MAX_QUEUE_SIZE 100

const char *lane_files[ROADS] = {"lanea.txt", "laneb.txt", "lanec.txt", "laned.txt"};

typedef struct {
    int vehicles[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

Queue lane_queues[ROADS][LANES];

void initialize_queues() {
    for (int road = 0; road < ROADS; road++) {
        for (int lane = 0; lane < LANES; lane++) {
            lane_queues[road][lane].front = -1;
            lane_queues[road][lane].rear = -1;
        }
    }
}

int is_empty(Queue *q) {
    return q->front == -1;
}

int is_full(Queue *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

void enqueue(Queue *q, int value) {
    if (is_full(q)) {
        printf("Queue is full!\n");
        return;
    }
    if (is_empty(q)) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->vehicles[q->rear] = value;
}

int dequeue(Queue *q) {
    if (is_empty(q)) {
        printf("Queue is empty!\n");
        return -1;
    }
    int value = q->vehicles[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }
    return value;
}

void load_lane_data() {
    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "r");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        for (int lane = 0; lane < LANES; lane++) {
            int vehicles;
            fscanf(file, "%d", &vehicles);
            lane_queues[road][lane].front = -1;
            lane_queues[road][lane].rear = -1;
            for (int i = 0; i < vehicles; i++) {
                enqueue(&lane_queues[road][lane], 1);
            }
        }
        fclose(file);
    }
}

void process_traffic() {
    int priority_lane_road = -1;
    for (int road = 0; road < ROADS; road++) {
        if (lane_queues[road][1].rear - lane_queues[road][1].front + 1 > PRIORITY_THRESHOLD) {
            priority_lane_road = road;
            break;
        }
    }

    if (priority_lane_road != -1) {
        printf("Priority lane detected at road %c!\n", 'A' + priority_lane_road);
        while (lane_queues[priority_lane_road][1].rear - lane_queues[priority_lane_road][1].front + 1 > PRIORITY_REDUCTION) {
            dequeue(&lane_queues[priority_lane_road][1]);
            printf("Processed a vehicle from priority lane at road %c.\n", 'A' + priority_lane_road);
        }
    } else {
        for (int road = 0; road < ROADS; road++) {
            for (int lane = 0; lane < LANES; lane++) {
                if (!is_empty(&lane_queues[road][lane])) {
                    dequeue(&lane_queues[road][lane]);
                    printf("Processed a vehicle from road %c, lane %d.\n", 'A' + road, lane + 1);
                }
            }
        }
    }
}

int main() {
    initialize_queues();
    while (1) {
        load_lane_data();
        process_traffic();
        sleep(5); // Process traffic every 5 seconds
    }
    return 0;
}
