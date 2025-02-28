# 🚦 DSA Queue Simulator

## Four-Way Junction Traffic Management System

This project implements a queue-based traffic management simulation for a four-way junction, developed as part of the Data Structure and Algorithms (COMP202) course.

![Traffic Simulation](https://github.com/Brook07/DSA-Queue-Simulator/blob/main/Demo.gif)

## 📋 Project Overview

The system manages normal and high-priority vehicle dispatch using queue data structures. The traffic flow is controlled by dynamically adjusting traffic lights based on queue sizes and priority conditions.

### Key Features

- 🚗 **Queue-based Traffic Management**: Vehicles are queued in each lane and processed according to traffic rules
- 🚨 **Priority Lane Handling**: Special handling for high-priority lanes when congestion occurs
- 🖥️ **Real-time Visualization**: Graphical representation using SDL2 library
- 📁 **File-based Communication**: Vehicle generation and processing via text files

## 🏗️ System Architecture

### Traffic Management Rules

1. **Normal Condition**: Traffic lights change based on the number of vehicles in each lane, ensuring fair service
2. **Priority Condition**: When lane AL2 exceeds 10 vehicles, it receives exclusive service until the count drops below 5
3. **Continuous Processing**: The system continuously monitors lane conditions and adjusts traffic flow

### Core Components

- **Generator**: Creates vehicle data and writes to text files
- **Simulator**: Reads vehicle data, manages queues, controls traffic lights, and provides visualization

## 📊 Data Structures

| Structure | Implementation | Purpose |
|-----------|----------------|---------|
| Queue | Array-based | Storing vehicles in each lane |
| Priority Queue | Modified queue with dynamic priority | Managing high-priority lanes |

## ⚙️ Algorithm Design

The traffic processing algorithm follows these steps:

1. Read vehicle data from files
2. Enqueue vehicles into respective lane queues
3. Check priority conditions (AL2 > 10 vehicles)
4. Process vehicles according to current conditions
5. Update traffic lights accordingly
6. Render the visualization
7. Repeat until simulation ends

### Time Complexity Analysis

- Queue Operations (enqueue, dequeue): O(1)
- Traffic Light Update: O(n) where n = number of lanes
- Vehicle Processing: O(n)
- Overall Time Complexity: O(n)

## 💻 Implementation Details

### Key Functions

```c
// Queue operations
void enqueue(Queue* q, Vehicle v);
Vehicle dequeue(Queue* q);
bool isEmpty(Queue* q);

// Traffic management
void updateTrafficLights(TrafficLight* lights);
void processTraffic(Lane* lanes);

// Visualization
void renderSimulation(SDL_Renderer* renderer, Lane* lanes);
void renderRoads(SDL_Renderer* renderer);
void renderVehicles(SDL_Renderer* renderer, Lane* lanes);

// File I/O
void readLaneData(Lane* lanes);
void writeVehicleData(Lane* lanes);
```

## 🚀 Installation and Setup

### Prerequisites

- C compiler (GCC or MinGW)
- SDL2 library
- Git (optional, for cloning the repository)

### Installation Steps

```bash
# Install SDL2 on Ubuntu/Debian
sudo apt-get install libsdl2-dev

# Install SDL2 on macOS using Homebrew
brew install sdl2

# Install SDL2 on Windows
# Download from https://www.libsdl.org/download-2.0.php
```

### Building the Project

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/dsa-queue-simulator.git
cd dsa-queue-simulator

# Compile the generator and Compile the simulator
	g++ -o bin/generator src/generator.c src/traffic_simulation.c -lSDL2 -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2

	g++ -Iinclude -Llib -o bin/main.exe src/main.c src/traffic_simulation.c -lmingw32 -lSDL2main -lSDL2

#Run the program
./bin/main.exe
