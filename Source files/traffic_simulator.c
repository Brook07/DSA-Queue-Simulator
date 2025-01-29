#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // For Sleep() on Windows

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10
#define MAX_QUEUE_SIZE 100
#define VEHICLE_PASS_TIME 1 // 1 second per vehicle
#define MAX_VEHICLES 10 // Maximum vehicles per lane except priority lane

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

void enqueue(Queue *q, int value) {
    if ((q->rear + 1) % MAX_QUEUE_SIZE == q->front) return; // Queue full
    if (is_empty(q)) q->front = 0;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->vehicles[q->rear] = value;
}

int dequeue(Queue *q) {
    if (is_empty(q)) return -1;
    int value = q->vehicles[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }
    return value;
}

int queue_size(Queue *q) {
    if (is_empty(q)) return 0;
    return (q->rear >= q->front) ? q->rear - q->front + 1 : MAX_QUEUE_SIZE - q->front + q->rear + 1;
}

void generate_traffic() {
    srand(time(NULL));
    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "w");
        if (!file) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        for (int lane = 0; lane < LANES; lane++) {
            int vehicles = (lane == 1) ? rand() % (MAX_VEHICLES + 11) : rand() % (MAX_VEHICLES + 1);
            if (lane != 1 && vehicles > MAX_VEHICLES) vehicles = MAX_VEHICLES;
            fprintf(file, "%d\n", vehicles);
        }
        fclose(file);
    }
    printf("\U0001F697 Traffic data updated!\n");
}

void load_lane_data() {
    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "r");
        if (!file) {
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

void process_road(int road) {
    while (!is_empty(&lane_queues[road][1])) {
        dequeue(&lane_queues[road][1]);
    }
}

void process_traffic() {
    static int normal_cycle = 0;
    int priority_road = -1;
    int max_priority_count = 0;
    for (int road = 0; road < ROADS; road++) {
        int lane_count = queue_size(&lane_queues[road][1]);
        if (lane_count > PRIORITY_THRESHOLD && lane_count > max_priority_count) {
            priority_road = road;
            max_priority_count = lane_count;
        }
    }

    printf("\n\U0001F6A6 Total vehicles before processing:\n");
    for (int road = 0; road < ROADS; road++) {
        printf("  Road %c (Lane 2): %d vehicles\n", 'A' + road, queue_size(&lane_queues[road][1]));
    }

    if (priority_road != -1) {
        int clear_time = queue_size(&lane_queues[priority_road][1]) * VEHICLE_PASS_TIME;
        printf("\n⚠️ Priority road detected at Road %c! Clearing all vehicles (Estimated time: %d sec).\n", 'A' + priority_road, clear_time);
        process_road(priority_road);
    } else {
        printf("\nRunning normal condition.\n");
        if (normal_cycle % 2 == 0) {
            printf("Processing Roads A and C.\n");
            process_road(0);
            process_road(2);
        } else {
            printf("Processing Roads B and D.\n");
            process_road(1);
            process_road(3);
        }
        normal_cycle++;
    }
}

int main() {
    initialize_queues();
    while (1) {
        generate_traffic();
        load_lane_data();
        process_traffic();
        Sleep(5000);
    }
    return 0;
}
