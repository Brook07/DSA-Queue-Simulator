#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_VEHICLES 100 // Max vehicles per road
#define GREEN_DURATION 5 // Duration for green light in seconds

typedef struct Queue {
    int vehicles[MAX_VEHICLES];
    int front, rear, size;
} Queue;

// Initialize a queue
void initQueue(Queue* q) {
    q->front = q->rear = -1;
    q->size = 0;
}

// Check if queue is empty
int isEmpty(Queue* q) {
    return q->size == 0;
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

// Dequeue a vehicle
int dequeue(Queue* q) {
    if (isEmpty(q)) return -1;
    int vehicle = q->vehicles[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1; // Reset queue
    } else {
        q->front = (q->front + 1) % MAX_VEHICLES;
    }
    q->size--;
    return vehicle;
}

// Function to find the road with the most vehicles
int getMaxQueue(Queue* roads, int roadCount) {
    int maxIndex = 0;
    for (int i = 1; i < roadCount; i++) {
        if (roads[i].size > roads[maxIndex].size) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

int main() {
    Queue roads[4]; // 4 roads
    for (int i = 0; i < 4; i++) {
        initQueue(&roads[i]);
    }

    while (1) {
        int maxRoad = getMaxQueue(roads, 4); // Get road with max vehicles
        printf("\nRoad %d has the green light.\n", maxRoad + 1);

        // Simulate clearing vehicles from the selected road
        for (int i = 0; i < GREEN_DURATION; i++) {
            if (!isEmpty(&roads[maxRoad])) {
                int vehicle = dequeue(&roads[maxRoad]);
                printf("Clearing vehicle %d from road %d\n", vehicle, maxRoad + 1);
            } else {
                printf("Road %d is empty now.\n", maxRoad + 1);
                break;
            }
            sleep(1);
        }

        // Ensure fairness by checking other roads
        for (int i = 0; i < 4; i++) {
            if (i != maxRoad && !isEmpty(&roads[i])) {
                printf("Road %d will get the green light next.\n", i + 1);
            }
        }
    }

    return 0;
}
