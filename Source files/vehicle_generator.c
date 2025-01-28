#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROADS 4
#define LANES 3
#define PRIORITY_THRESHOLD 10

const char *lane_files[ROADS] = {"lanea.txt", "laneb.txt", "lanec.txt", "laned.txt"};

void generate_vehicles() {
    srand(time(0));

    int priority_road = rand() % ROADS;

    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "w");
        if (!file) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        int lane_1, lane_2, lane_3;

        if (road == priority_road) {
            // Priority road: one lane exceeds the threshold
            int priority_lane = rand() % LANES;
            lane_1 = (priority_lane == 0) ? rand() % 5 + 11 : rand() % 10 + 1; // 11-15 for priority
            lane_2 = (priority_lane == 1) ? rand() % 5 + 11 : rand() % 10 + 1; // 11-15 for priority
            lane_3 = (priority_lane == 2) ? rand() % 5 + 11 : rand() % 10 + 1; // 11-15 for priority
        } else {
            // Non-priority roads: all lanes under the threshold
            lane_1 = rand() % 10 + 1; // 1-10 vehicles
            lane_2 = rand() % 10 + 1; // 1-10 vehicles
            lane_3 = rand() % 10 + 1; // 1-10 vehicles
        }

        fprintf(file, "%d %d %d\n", lane_1, lane_2, lane_3);
        fclose(file);
    }

    printf("Vehicles generated successfully! Priority road is %c.\n", 'A' + priority_road);
}

int main() {
    generate_vehicles();
    return 0;
}