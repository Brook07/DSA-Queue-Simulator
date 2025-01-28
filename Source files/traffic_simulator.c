// traffic_simulator.c: Refined traffic simulator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10
#define MAX_QUEUE_SIZE 100
#define VEHICLE_PASS_TIME 1 // 1 second per vehicle

const char *lane_files[ROADS] = {"lanea.txt", "laneb.txt", "lanec.txt", "laned.txt"};

typedef struct {
    int vehicles[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

Queue lane_queues[ROADS][LANES];


void run_vehicle_generator() {
    system("./vehicle_generator"); // Call the vehicle generator
}
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

void process_lanes(int road, int *total_time, int is_priority) {
    for (int lane = 1; lane < LANES; lane++) { // Process only Lane 2 and Lane 3
        while (!is_empty(&lane_queues[road][lane])) {
            dequeue(&lane_queues[road][lane]);
            (*total_time)++;
        }
    }
}

void process_traffic() {
    int total_time = 0;
    int total_vehicles[ROADS] = {0};
    int priority_road = -1;

    // Calculate total vehicles and detect priority road
    for (int road = 0; road < ROADS; road++) {
        for (int lane = 0; lane < LANES; lane++) {
            total_vehicles[road] += queue_size(&lane_queues[road][lane]);
        }
        if (total_vehicles[road] > PRIORITY_THRESHOLD && priority_road == -1) {
            priority_road = road;
        }
    }

    printf("\n🚦 Total vehicles before processing:\n");
    for (int road = 0; road < ROADS; road++) {
        printf("  Road %c: %d vehicles\n", 'A' + road, total_vehicles[road]);
    }

    if (priority_road != -1) {
        printf("\n⚠️ Priority lane detected at Road %c! Processing Road %c and its pair.\n", 'A' + priority_road, 'A' + priority_road);
        int paired_road = (priority_road % 2 == 0) ? 2 : 3; // Pair: A & C, B & D
        process_lanes(priority_road, &total_time, 1);
        process_lanes(paired_road, &total_time, 0);
    } else {
        static int normal_cycle = 0;
        printf("\nNo priority lane detected. Running normal condition.\n");
        if (normal_cycle % 2 == 0) {
            printf("Processing Roads A and C.\n");
            process_lanes(0, &total_time, 0);
            process_lanes(2, &total_time, 0);
        } else {
            printf("Processing Roads B and D.\n");
            process_lanes(1, &total_time, 0);
            process_lanes(3, &total_time, 0);
        }
        normal_cycle++;
    }

    printf("\n⏱️ Combined Countdown: %d seconds required to clear all processed vehicles.\n", total_time * VEHICLE_PASS_TIME);
}

int main() {
    initialize_queues();
    while (1) {
        run_vehicle_generator();    // Generate new vehicle data for each cycle
        load_lane_data();          // Load updated data into queues
        process_traffic();         // Process traffic based on updated data
        sleep(5);                  // Wait before the next cycle
    }
    return 0;
}