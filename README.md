# **Traffic Simulator 🚦**

A real-time traffic simulation system for managing and optimizing road traffic flow.

## **Overview**

Traffic Simulator is a C-based application that simulates a real-world traffic system by generating vehicles on different roads and lanes. It processes and clears traffic using an intelligent queue-based approach to prioritize high-traffic areas.

This project aims to model dynamic traffic behavior and optimize vehicle movement by implementing priority-based traffic management.
## **Key Features**

- 🚗 **Real-Time Traffic Generation**: Simulates vehicle arrivals dynamically.
- 📊 **Queue-Based Traffic Processing**: Implements queue structures for managing lanes.
- ⚠️ **Priority-Based Traffic Flow**: Roads with high congestion get priority clearance.
- 🕒 **Automated Traffic Updates**: Continuously updates traffic data every 5 seconds.
- 🔄 **Crossroad Traffic Handling**: Simulates multiple intersecting roads.

## **How It Works**

### **Vehicle Generation**

- Vehicles are generated randomly for different lanes on 4 roads.
- Data is stored in text files (`lanea.txt`, `laneb.txt`, etc.).

### **Traffic Processing**

- Reads vehicle data from files and stores it in queue structures.
- Roads with excessive traffic (above a set threshold) are given priority clearance.
- Normal cycle alternates between different roads if no priority case exists.

### **Continuous Simulation**

- Runs in an infinite loop with periodic updates.
- Clears vehicles from lanes based on defined rules.

## **Requirements**

- 🖥️ **OS**: Windows/Linux (Windows uses `Sleep()`, Linux uses `sleep()`).
- 💾 **Memory**: 2GB RAM (Minimum), 4GB+ (Recommended).
- 📜 **Compiler**: GCC (MinGW for Windows, GCC for Linux/Mac).

## **Installation & Usage**

### **Clone the Repository**

```bash
git clone https://github.com/yourusername/traffic-simulator.git
```
This will download the project files into a folder.

### **Navigate to the Project Folder**

Navigate to the Project Folder
```bash
cd traffic-simulator
```
### **Compile and Run**
For Windows:
```bash
gcc traffic_simulator.c -o simulator.exe
simulator.exe
```
For Linux/Mac:
```bash
gcc traffic_simulator.c -o simulator
./simulator
```
To Run the Vehicle Generator (Optional but Recommended)
```bash
gcc vehicle_generator.c -o generator
./generator
```
### **Process to Run the Program**
1.Run simulator.exe (Windows) or ./simulator (Linux/Mac) to start the traffic simulation.

2.Optionally, run generator.exe (Windows) or ./generator (Linux/Mac) to dynamically update vehicle data.

3.Observe real-time traffic updates and priority-based vehicle clearance.

4.Press CTRL + C to stop execution.

### **Demo (GIF/Video)**


For a full demonstration, check out this video.

### **References**
Queue-based traffic management: GeeksforGeeks

Traffic simulation concepts: Research Paper on Traffic Flow

C Programming for File Handling: Tutorial

### **Contributions**
Contributions are welcome! Fork the repository, make changes, and submit a pull request.

Happy coding! 🚦🚗
