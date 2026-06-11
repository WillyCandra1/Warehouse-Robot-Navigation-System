# 🤖 Warehouse Robot Navigation System

> A console-based warehouse management and robot navigation simulator built in C++.

![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![Standard](https://img.shields.io/badge/standard-C%2B%2B11-blue)
![Build](https://img.shields.io/badge/build-g%2B%2B-success)
![STL Containers](https://img.shields.io/badge/STL%20containers-none-critical)

---

## 📋 Overview

The Warehouse Robot Navigation System simulates how an automated warehouse coordinates orders, robots, inventory, and physical layout. It is organised into **five independent modules**, each demonstrating a different data structure and its associated algorithms.

---

## ✨ Modules & Features

| # | Module | Responsibility | Core Data Structure |
|---|--------|----------------|---------------------|
| 1 | **Order Management** | Queue incoming orders and process them in arrival order | Circular Queue (array-based) |
| 2 | **Robot Assignment** | Assign tasks to robots in round-robin fashion, track status & history | Circular Linked List |
| 3 | **Robot Navigation & Path Tracking** | Record a robot's movements and reverse them to return to base | Stack |
| 4 | **Item Search & Management** | Store and look up inventory items efficiently by ID | Binary Search Tree |
| 5 | **Warehouse Layout & Navigation** | Model the warehouse map and compute shortest routes between locations | Weighted Graph (Adjacency List) |

---

## 🧠 Data Structures & Algorithms

Everything below is implemented from scratch — no STL containers.

- **Circular Queue** — efficient FIFO order processing with wraparound.
- **Circular Linked List** — round-robin task distribution across robots.
- **Stack** — last-in-first-out movement history enabling exact path reversal.
- **Binary Search Tree** — ordered storage giving fast item lookup by key.
- **Weighted Graph (Adjacency List)** — vertices in a self-built dynamic array, each owning a linked list of weighted edges.
  - **Dijkstra's Algorithm** — shortest path between two locations.
  - **Breadth-First Search (BFS)** — full traversal of all reachable sections, using a self-built queue.

---

## 🗂️ Project Structure

```
Warehouse-Robot-Navigation-System/
├── src/
│   ├── main.cpp                 # Program entry point & all menus
│   ├── order.h / order.cpp      # Task 1 — Order Management (queue)
│   ├── Robot.hpp                # Shared Robot struct
│   ├── RobotAssignment.*        # Task 2 — Robot Assignment (circular linked list)
│   ├── Stack.hpp / Stack.cpp    # Self-built stack used by navigation
│   ├── Navigation.*             # Task 3 — Navigation & Path Tracking (stack)
│   ├── ItemManager.*            # Task 4 — Item Search & Management (BST)
│   └── WarehouseGraph.*         # Task 5 — Warehouse Layout & Navigation (graph)
│
└── data/
    ├── orders.csv               # Saved orders
    ├── robots.csv               # Saved robots
    ├── items.csv                # Saved inventory
    └── layout.csv               # Saved warehouse map (locations + connections)
```

---

## 🚀 Getting Started

### Prerequisites

- A C++ compiler supporting C++11 or later (e.g. **g++**, MinGW, or MSVC).

### Build

Compile from inside the `src` directory:

```bash
cd Warehouse-Robot-Navigation-System/src

g++ -o warehouse main.cpp order.cpp Navigation.cpp ItemManager.cpp RobotAssignment.cpp Stack.cpp WarehouseGraph.cpp
```

### Run

```bash
./warehouse        # Linux / macOS
warehouse.exe      # Windows
```

> **Note:** Run the program from inside the `src` folder. It reads from and writes to `../data`, so the `data` directory must sit alongside `src`.

---

## 🕹️ Usage

On launch, the program loads saved data and presents the main menu:

```
====================================
 Warehouse Robot System
====================================
1. Order Management (Task 1)
2. Robot Assignment (Task 2)
3. Robot Navigation (Task 3)
4. Item Management (Task 4)
5. Warehouse Layout (Task 5)
0. Exit
```

Each option opens that module's own sub-menu. On exit, all data is saved back to the CSV files so your work persists between sessions.

### Example: dispatching a robot (Tasks 2, 3 & 5 working together)

1. **Robot Assignment** → add a robot (e.g. `R1`).
2. **Warehouse Layout** → *Find route* to preview a path, then *Dispatch a robot* to send `R1` from the dock to a destination.
3. **Robot Navigation** → *Return to base* to send the robot back along the reversed path.

This showcases the cross-module integration: Task 5 computes the route and verifies the robot exists (Task 2), then drives the navigation system (Task 3), which reverses the path for the return trip.

---

## 💾 Data Files

All state is stored as human-readable CSV in `data/`.

**`layout.csv`** uses two sections — locations and connections:

```
#LOCATIONS
DOCK,Packing Station,DOCK,0,4
ZA,Zone A,ZONE,2,4
A1,Aisle A-1,AISLE,2,2
SA1,Shelf A-1,SHELF,2,0
#CONNECTIONS
DOCK,ZA,2
ZA,A1,2
A1,SA1,2
```

Locations are `id,name,type,x,y`; connections are `from,to,weight`. The loader validates every line, so malformed data is skipped rather than crashing the program.

---
