// vehicle_generator.c: Refined vehicle generator for traffic simulation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10

const char *lane_files[ROADS] = {"lanea.txt", "laneb.txt", "lanec.txt", "laned.txt"};

void generate_vehicles() {
    srand(time(0)); // Seed for randomization

    int priority_road = rand() % ROADS;   // Randomly select priority road
    int priority_lane = (rand() % 2) + 1; // Randomly select priority lane (2 or 3)

    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "w");
        if (!file) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        for (int lane = 0; lane < LANES; lane++) {
            int vehicles;
            if (road == priority_road && lane == priority_lane) {
                vehicles = rand() % 6 + 11; // 11-15 vehicles for the priority lane
            } else {
                vehicles = rand() % 10 + 1; // 1-10 vehicles for non-priority lanes
            }
            fprintf(file, "%d\n", vehicles);
        }
        fclose(file);
    }

    printf("🚦 Vehicles generated successfully! Priority road is %c, lane %d.\n", 'A' + priority_road, priority_lane + 1);
}

int main() {
    generate_vehicles();
    return 0;
}
