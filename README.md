# 🚆 Railway Reservation System using Multithreading (C++)

## 📌 Project Overview

The **Railway Reservation System** is a joint collaborative project developed in C++ using the Standard Thread Library.

The project simulates multiple users performing railway reservation operations simultaneously while managing concurrent access to train seat availability.

It demonstrates the practical implementation of **Multithreading, Mutex Locks, Condition Variables, and Thread Synchronization** to coordinate multiple booking requests and handle concurrent operations.

## 🤝 Collaborative Project

This project was developed collaboratively by:

| Contributor                   | GitHub Profile                                 | Repository                                                                          |
| ----------------------------- | ---------------------------------------------- | ----------------------------------------------------------------------------------- |
| **Bharti Upkar**              | [@bhartiupkar](https://github.com/bhartiupkar) | [Train_Reservation_system](https://github.com/bhartiupkar/Train_Reservation_system) |
| **Tuli Shruti (Shruti Guba)** | [@TuliShruti](https://github.com/TuliShruti)   | [Train_Reservation_System](https://github.com/TuliShruti/Train_Reservation_System)  |

Both repositories represent the collaborative development of the same project.

## ⚙️ Features

- **Multithreaded Execution:** Multiple threads simulate different users interacting with the reservation system simultaneously.
- **Seat Inquiry:** Users can check the availability of seats on a particular train.
- **Seat Booking:** Users can book a random number of seats, subject to availability.
- **Booking Cancellation:** Users can cancel existing bookings, making seats available again.
- **Thread Synchronization:** Mutex locks are used to coordinate access to shared resources.
- **Condition Variables:** Threads wait when the maximum number of active queries is reached or when conflicting booking operations occur.
- **Concurrent Queries:** Supports up to 5 active queries simultaneously.
- **Reservation Chart:** Displays the final seat availability of all trains after execution.

## 🛠️ Technologies & Concepts Used

- **Programming Language:** C++
- **Libraries:**
  - `iostream`
  - `thread`
  - `mutex`
  - `condition_variable`
  - `chrono`

**Core Concepts:**

- Multithreading and Thread Management
- Mutual Exclusion using Mutex
- Thread Synchronization
- Condition Variables
- Concurrent Resource Access
- Critical Section Management

## 📊 System Configuration

| Parameter                       |    Value |
| ------------------------------- | -------: |
| Maximum Number of Trains        |      100 |
| Initial Seat Capacity per Train |      500 |
| Maximum Number of Threads       |       20 |
| Maximum Active Queries          |        5 |
| Minimum Booking Size            |        5 |
| Maximum Booking Size            |       10 |
| Simulation Duration             | 1 Minute |

## 🔄 Working of the System

1. The system initializes 100 trains, each with a capacity of 500 seats.
2. It creates 20 worker threads, each representing a user.
3. Each thread generates random reservation requests.
4. Three types of operations are supported:
   - **Inquiry:** Checks available seats.
   - **Booking:** Books seats if sufficient availability exists.
   - **Cancellation:** Cancels previously booked seats.
5. Mutex locks and condition variables coordinate concurrent operations and manage access to shared resources.
6. The simulation runs for one minute.
7. Once all threads finish execution, the final reservation chart displays the remaining seats for every train.

## 📂 Project Structure

```text
Train_Reservation_System/
│
├── README.md
└── main.cpp
```

## 🚀 Getting Started

### Prerequisites

- C++ compiler (G++ recommended)
- C++11 or later
- Support for POSIX threads

### Compilation

Clone the repository:

```bash
git clone https://github.com/bhartiupkar/Train_Reservation_system.git
```

Navigate to the project directory:

```bash
cd Train_Reservation_system
```

Compile the program:

```bash
g++ -std=c++11 main.cpp -o train_reservation -pthread
```

### Run the Program

```bash
./train_reservation
```

## 🎯 Learning Outcomes

Through this collaborative project, we explored:

- Practical implementation of multithreading in C++.
- Managing simultaneous user requests.
- Synchronizing access to shared resources.
- Using mutex locks and condition variables.
- Understanding concurrency and race conditions.
- Implementing a basic real-world reservation simulation.

---

**Developed collaboratively by Bharti Upkar and Tuli Shruti.**

*An academic project demonstrating Multithreading and Synchronization concepts in C++.*
