#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10
#define PRIORITY_DROP 5
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

void process_lane(int road, int lane, int is_priority) {
    if (!is_empty(&lane_queues[road][lane])) {
        dequeue(&lane_queues[road][lane]);
        if (is_priority) {
            printf("Processed vehicle from PRIORITY lane %cL%d.\n", 'A' + road, lane + 1);
        } else {
            printf("Processed vehicle from road %c, lane %d.\n", 'A' + road, lane + 1);
        }
    }
}

void process_traffic() {
    static int normal_cycle = 0;
    int total_vehicles[ROADS] = {0};
    int priority_road = -1;

    // Calculate total vehicles for each road
    for (int road = 0; road < ROADS; road++) {
        for (int lane = 0; lane < LANES; lane++) {
            total_vehicles[road] += queue_size(&lane_queues[road][lane]);
        }
    }

    printf("\nTotal vehicles before processing:\n");
    for (int road = 0; road < ROADS; road++) {
        printf("  Road %c: %d vehicles\n", 'A' + road, total_vehicles[road]);
        if (total_vehicles[road] > PRIORITY_THRESHOLD) {
            priority_road = road;
        }
    }

    if (priority_road != -1) {
        printf("\nHigh-priority condition! Road %c is prioritized.\n", 'A' + priority_road);
        for (int lane = 1; lane < LANES; lane++) {
            while (queue_size(&lane_queues[priority_road][lane]) > PRIORITY_DROP) {
                process_lane(priority_road, lane, 1);
            }
        }
        int paired_road = (priority_road == 0 || priority_road == 2) ? 0 : 2;
        if (paired_road == priority_road) paired_road = (priority_road == 1 || priority_road == 3) ? 1 : 3;
        printf("Processing paired road %c along with priority road %c.\n", 'A' + paired_road, 'A' + priority_road);
        for (int lane = 1; lane < LANES; lane++) {
            process_lane(paired_road, lane, 0);
        }
        printf("Priority processing completed for road %c.\n", 'A' + priority_road);
    } else {
        printf("\nNormal condition: Processing roads in a cycle.\n");
        if (normal_cycle % 2 == 0) {
            printf("Processing roads A and C.\n");
            for (int lane = 1; lane < LANES; lane++) {
                process_lane(0, lane, 0); // Road A
                process_lane(2, lane, 0); // Road C
            }
        } else {
            printf("Processing roads B and D.\n");
            for (int lane = 1; lane < LANES; lane++) {
                process_lane(1, lane, 0); // Road B
                process_lane(3, lane, 0); // Road D
            }
        }
        normal_cycle++;
    }

    printf("\nTraffic cycle completed.\n");
}

int main() {
    initialize_queues();
    while (1) {
        load_lane_data();
        process_traffic();
        sleep(5);
    }
    return 0;
}
