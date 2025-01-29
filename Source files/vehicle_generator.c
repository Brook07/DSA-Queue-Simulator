// vehicle_generator.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ROADS 4
#define LANES 3
#define MAX_VEHICLES 20  // Maximum vehicles per lane

const char *lane_files[ROADS] = {"lanea.txt", "laneb.txt", "lanec.txt", "laned.txt"};

void generate_traffic() {
    srand(time(NULL)); // Seed random number generator

    while (1) {
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
        sleep(5); // Wait before generating new data
    }
}

int main() {
    generate_traffic();
    return 0;
}
