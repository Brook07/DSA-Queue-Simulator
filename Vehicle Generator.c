#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_VEHICLES 100 // Max vehicles per road

typedef struct Queue {
    int vehicles[MAX_VEHICLES];
    int front, rear, size;
} Queue;

// Initialize a queue
void initQueue(Queue* q) {
    q->front = q->rear = -1;
    q->size = 0;
}

// Enqueue a vehicle
void enqueue(Queue* q, int vehicle) {
    if (q->size == MAX_VEHICLES) {
        printf("Queue full! Cannot add more vehicles.\n");
        return;
    }
    if (q->rear == -1) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_VEHICLES;
    }
    q->vehicles[q->rear] = vehicle;
    q->size++;
}

// Simulate random vehicle generation
void generateVehicles(Queue* roads, int roadCount) {
    srand(time(NULL));
    int vehicleID = 1;

    while (1) {
        int road = rand() % roadCount; // Random road (0-3)
        enqueue(&roads[road], vehicleID++);
        printf("Vehicle %d added to road %d. Total vehicles: %d\n",
               vehicleID - 1, road + 1, roads[road].size);
        sleep(rand() % 3 + 1); // Random interval between 1-3 seconds
    }
}

int main() {
    Queue roads[4]; // 4 roads
    for (int i = 0; i < 4; i++) {
        initQueue(&roads[i]);
    }

    generateVehicles(roads, 4); // Start generating vehicles

    return 0;
}
