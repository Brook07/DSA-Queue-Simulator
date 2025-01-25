// traffic_generator.c: Generates vehicles for each lane and writes to files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_VEHICLES 20
#define ROADS 4
#define LANES 3

const char *lane_files[ROADS] = {"lanea.txt", "laneb.txt", "lanec.txt", "laned.txt"};

void generate_vehicles() {
    srand(time(NULL));
    for (int road = 0; road < ROADS; road++) {
        FILE *file = fopen(lane_files[road], "w");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        for (int lane = 0; lane < LANES; lane++) {
            int vehicles = rand() % MAX_VEHICLES;
            fprintf(file, "%d\n", vehicles);
        }
        fclose(file);
    }
}

int main() {
    while (1) {
        generate_vehicles();
        printf("Generated vehicles for all lanes.\n");
        sleep(5); // Generate new vehicles every 5 seconds
    }
    return 0;
}
