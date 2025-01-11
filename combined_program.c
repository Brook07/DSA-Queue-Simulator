#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep()

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

// Function to manage traffic lights with round-robin fairness
void manageTraffic(Queue* roads, int roadCount, int* lastRoad) {
    int maxRoad = getMaxQueue(roads, roadCount); // Road with most vehicles

    // Ensure fairness by giving priority to roads cyclically
    for (int i = 0; i < roadCount; i++) {
        int road = (*lastRoad + i + 1) % roadCount;
        if (!isEmpty(&roads[road])) {
            maxRoad = road;
            break;
        }
    }

    *lastRoad = maxRoad; // Update the last served road
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
}

// Function to generate vehicles randomly
void generateVehicles(Queue* roads, int roadCount, int* vehicleID) {
    int road = rand() % roadCount; // Random road (0-3)
    enqueue(&roads[road], (*vehicleID)++);
    printf("Vehicle %d added to road %d. Total vehicles: %d\n",
           *vehicleID - 1, road + 1, roads[road].size);
}

int main() {
    Queue roads[4]; // 4 roads
    int vehicleID = 1; // Vehicle ID counter
    int lastRoad = -1; // Track the last road that got the green light

    // Initialize the random number generator
    srand(time(NULL));

    // Initialize queues for the roads
    for (int i = 0; i < 4; i++) {
        initQueue(&roads[i]);
    }

    // Main loop for traffic management
    while (1) {
        // Step 1: Generate vehicles randomly
        generateVehicles(roads, 4, &vehicleID);
        sleep(rand() % 3 + 1); // Random interval between 1-3 seconds

        // Step 2: Manage traffic lights
        manageTraffic(roads, 4, &lastRoad);
    }

    return 0;
}
