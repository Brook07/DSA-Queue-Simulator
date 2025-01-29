// traffic_simulator.c: Refined traffic simulator without <sys/wait.h> or <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // For Sleep() on Windows

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10
#define PRIORITY_RESET 5
#define MAX_QUEUE_SIZE 100
#define VEHICLE_PASS_TIME 1 // 1 second per vehicle
#define MAX_VEHICLES 20 // Maximum vehicles per lane

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

// Generate random traffic and write to files
void generate_traffic() {
    srand(time(NULL)); // Seed random number generator
    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "w");
        if (!file) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        for (int lane = 0; lane < LANES; lane++) {
            int vehicles = rand() % (MAX_VEHICLES + 1); // Generate 0 to MAX_VEHICLES vehicles
            fprintf(file, "%d\n", vehicles);
        }
        fclose(file);
    }
    printf("🚗 Traffic data updated!\n");
}

// Load data from files into queues
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

// Process vehicles for a specific road
void process_road(int road, int *total_time) {
    for (int lane = 0; lane < LANES; lane++) {
        while (!is_empty(&lane_queues[road][lane])) {
            dequeue(&lane_queues[road][lane]);
            (*total_time)++;
        }
    }
}

// Traffic management logic
void process_traffic() {
    static int normal_cycle = 0; // To alternate between A&C and B&D
    int total_time = 0;
    int total_vehicles[ROADS] = {0};
    int priority_road = -1;

    // Calculate total vehicles and detect priority road
    for (int road = 0; road < ROADS; road++) {
        for (int lane = 0; lane < LANES; lane++) {
            total_vehicles[road] += queue_size(&lane_queues[road][lane]);
        }
        if (total_vehicles[road] > PRIORITY_THRESHOLD && (priority_road == -1 || total_vehicles[road] > total_vehicles[priority_road])) {
            priority_road = road;
        }
    }

    printf("\n🚦 Total vehicles before processing:\n");
    for (int road = 0; road < ROADS; road++) {
        printf("  Road %c: %d vehicles\n", 'A' + road, total_vehicles[road]);
    }

    if (priority_road != -1) {
        // Serve only the priority road
        printf("\n⚠️ Priority road detected at Road %c! Processing exclusively.\n", 'A' + priority_road);
        process_road(priority_road, &total_time);
    } else {
        // Normal condition: Alternate between A&C or B&D
        printf("\nNo priority road detected. Running normal condition.\n");
        if (normal_cycle % 2 == 0) {
            printf("Processing Roads A and C.\n");
            process_road(0, &total_time); // Road A
            process_road(2, &total_time); // Road C
        } else {
            printf("Processing Roads B and D.\n");
            process_road(1, &total_time); // Road B
            process_road(3, &total_time); // Road D
        }
        normal_cycle++;
    }

    printf("\n⏱️ Combined Countdown: %d seconds required to clear all processed vehicles.\n", total_time * VEHICLE_PASS_TIME);
}

int main() {
    initialize_queues();
    while (1) {
        generate_traffic();    // Generate new vehicle data
        load_lane_data();      // Load updated data into queues
        process_traffic();     // Process traffic based on updated data
        Sleep(5000);           // Wait 5 seconds before the next cycle
    }
    return 0;
}
