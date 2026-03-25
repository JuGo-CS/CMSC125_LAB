# CPU Scheduler Simulator (schedsim-2)

## Group Members
- **Christian Jave S. Hulleza**
- **Kenneth G. Mondejar**

## Project Overview

This is an advanced **Discrete Event Simulation (DES)** based CPU scheduler simulator that implements and compares five different CPU scheduling algorithms. The simulator accurately models process arrival, execution, context switching, and completion using discrete events.

**Key Feature**: MLFQ (Multi-Level Feedback Queue) implementation that learns process behavior without prior knowledge of burst times.

## Implemented Algorithms

### 1. **FCFS** (First-Come-First-Served)
- Non-preemptive scheduling
- Processes run to completion in arrival order
- Simple but suffers from convoy effect with long-running processes

### 2. **SJF** (Shortest Job First)
- Non-preemptive scheduling
- Selects process with shortest burst time
- Optimal for minimizing average waiting time (requires known burst times)

### 3. **STCF** (Shortest Time-to-Completion First)
- **Preemptive** version of SJF
- Dynamically preempts for incoming shorter jobs
- Cancels scheduled events and reschedules when preemption occurs
- Best average response time and turnaround time

### 4. **RR** (Round Robin)
- Preemptive with configurable time quantum
- Uses batch semantics queue (pending → active → round)
- Fair scheduling with bounded response times
- Higher context switches but better responsiveness

### 5. **MLFQ** (Multi-Level Feedback Queue) ⭐ **Custom Implementation**
- Adaptive scheduling without knowing burst times
- 3+ configurable priority levels
- Time quantum and allotment per level
- Priority boost mechanism to prevent starvation
- Learns process behavior through observation:
  - **Interactive processes** (I/O-bound) stay in high-priority queues
  - **Compute-intensive processes** (CPU-bound) demoted to lower queues
- **See [docs/mlfq_design.md](docs/mlfq_design.md) for detailed design justification**

## Directory Structure

```
schedsim-2/
├── README.md                    # This file
├── Makefile                     # Build configuration
├── schedsim                     # Compiled executable
│
├── includes/                    # Header files
│   ├── command.h               # Command-line parsing
│   ├── scheduler.h             # Scheduler interface
│   ├── simulator.h             # Discrete event simulator
│   ├── compare.h               # Comparison mode
│   ├── metrics.h               # Performance metrics
│   ├── gantt.h                 # Gantt chart visualization
│   ├── mlfq-config.h           # MLFQ configuration
│   ├── objects/
│   │   ├── process.h           # Process structure
│   │   └── event.h             # Event structure
│   └── data-structures/
│       ├── process-queue-adt.h        # Queue abstraction
│       ├── fcfs-process-queue.h       # FCFS queue
│       ├── sjf-process-queue.h        # SJF queue
│       ├── rr-process-queue.h         # RR queue (3-list batch)
│       ├── stcf-process-queue.h       # STCF queue
│       ├── mlfq-process-queue.h       # MLFQ queue (multi-level)
│       └── event-queue.h              # Event priority queue
│
├── src/                         # Source files
│   ├── main.c                  # Entry point
│   ├── command.c               # Command-line parsing
│   ├── simulator.c             # DES engine & event handlers
│   ├── compare.c               # Comparison mode implementation
│   ├── scheduler.c             # Process loading & initialization
│   ├── metrics.c               # Metric calculations
│   ├── gantt.c                 # Gantt chart generation
│   ├── mlfq-config.c           # MLFQ config file parser
│   ├── schedulers/
│   │   ├── first-come-first-serve.c
│   │   ├── shortest-job-first.c
│   │   ├── shortest-time-completion-first.c
│   │   ├── round-robin.c
│   │   └── mlfq.c
│   ├── objects/
│   │   ├── process.c
│   │   └── event.c
│   └── data-structures/
│       ├── fcfs-queue.c
│       ├── sjf-queue.c
│       ├── rr-queue.c
│       ├── stcf-queue.c
│       ├── mlfq-queue.c
│       └── event-queue.c
│
├── workloads/                   # Input workload files
│   ├── workload-1.txt          # 4 processes, small bursts
│   ├── workload-2.txt          # 5 processes, mixed sizes
│   ├── workload-3.txt          # 5 processes, large bursts
│   └── mlfq_config.txt         # Default MLFQ configuration
│
└── docs/                        # Documentation
    └── mlfq_design.md          # MLFQ design justification & analysis
```

## Compilation

### Prerequisites
- GCC compiler
- Standard C library (libc)
- Make utility

### Build Instructions

```bash
# Clean and rebuild
make clean && make

# Build with verbose output (optional)
gcc -Wextra -I includes -o schedsim src/**/*.c src/schedulers/*.c src/objects/*.c src/data-structures/*.c
```

**Output**: Executable `schedsim` (∼500 KB, optimized)

## Usage

### Basic Usage: Single Algorithm

```bash
./schedsim --algorithm=<ALGORITHM> --input=<WORKLOAD> [OPTIONS]
```

### Examples

**FCFS Scheduling:**
```bash
./schedsim --algorithm=fcfs --input=workload-1.txt
```

**Round Robin with quantum=10:**
```bash
./schedsim --algorithm=rr --quantum=10 --input=workload-2.txt
```

**STCF (Preemptive SJF):**
```bash
./schedsim --algorithm=stcf --input=workload-3.txt
```

**MLFQ with custom configuration:**
```bash
./schedsim --algorithm=mlfq --mlfq-config=workloads/mlfq_config.txt --input=workload-1.txt
```

**MLFQ with default configuration:**
```bash
./schedsim --algorithm=mlfq --mlfq-config=mlfq_config.txt --input=workload-2.txt
```

**Compare all algorithms on same workload:**
```bash
./schedsim --compare --input=workload-1.txt
```

### Input Format

**Process Definition (workload.txt):**
```
PID ARRIVAL_TIME BURST_TIME
A   10           30
B   20           40
C   50           50
D   30           10
```

**MLFQ Configuration (mlfq_config.txt):**
```
# Queue Level Definitions: Q<id> <time_quantum> <allotment>
Q0  10  50      # High priority (interactive): q=10, allotment=50
Q1  30  150     # Medium priority: q=30, allotment=150
Q2  -1  -1      # Low priority (FCFS, no limits)

# Priority Boost Configuration
BOOST_PERIOD 200  # Reset all to Q0 every 200 time units
```

## Output Description

### Standard Output (Per-Algorithm Mode)

```
=== Gantt Chart ===
[A--][B---][D][C----]
10   40    80 90     140

=== Metrics ===
Process |  AT  |  BT  |  FT  |   TT   |   WT   |   RT   
--------|------|------|------|--------|--------|--------
A       |   10 |   30 |   40 |     30 |      0 |      0
B       |   20 |   40 |   80 |     60 |     20 |     20
C       |   50 |   50 |  140 |     90 |     40 |     40
D       |   30 |   10 |   90 |     60 |     50 |     50
--------|------|------|------|--------|--------|-------
Average |      |      |      |  60.00 |  27.50 |  27.50
```

**Legend:**
- **AT** = Arrival Time (when process entered system)
- **BT** = Burst Time (total CPU time needed)
- **FT** = Finish Time (when process completed)
- **TT** = Turnaround Time (FT - AT, total time in system)
- **WT** = Waiting Time (TT - BT, time spent not executing)
- **RT** = Response Time (Start Time - AT, delay to first execution)

### Comparison Output (--compare mode)

```
=== Algorithm Comparison for workload-1.txt ===

Algorithm       | Avg TT   | Avg WT   | Avg RT   | Context Switches
--------|--------|--------|--------|------------------
FCFS            |     60.0 |     27.5 |     27.5 |               3
SJF             |     52.5 |     20.0 |     20.0 |               3
STCF            |     52.5 |     20.0 |     17.5 |               4
RR (q=10)       |     62.5 |     30.0 |     17.5 |               8
MLFQ            |     65.0 |     32.5 |      7.5 |              10
```

## Makefile Targets

```bash
make              # Build simulator
make clean        # Remove compiled binary
make fcfs_input-1 # Test FCFS on workload-1
make sjf_input-2  # Test SJF on workload-2
make rr_input-3   # Test RR on workload-3
make mlfq_input-1 # Test MLFQ on workload-1
```

## Architecture Highlights

### 1. **Discrete Event Simulation (DES)**
- Events: ARRIVAL, COMPLETION, QUANTUM_EXPIRE, PRIORITY_BOOST
- Event queue maintains chronological order
- Time advances via event processing, not fixed ticks
- **Supports preemption** (STCF cancels scheduled events)

### 2. **Process Queue Abstraction (ADT Pattern)**
```c
typedef struct AbstractProcessQueue {
    void (*enqueue)(struct AbstractProcessQueue*, Process*);
    Process* (*dequeue)(struct AbstractProcessQueue*);
    void (*destruct_queue)(struct AbstractProcessQueue*);
    int size;
} AbstractProcessQueue;
```
- Each algorithm implements its own queue behavior
- FCFS: Simple FIFO
- SJF/STCF: Sorted by burst time / remaining time
- RR: 3-list batch queue (pending, active, round)
- MLFQ: Multi-level queue array with promotion/demotion

### 3. **Context Switching**
- Counted as transitions between different processes in Gantt chart
- FCFS: Minimum switches (non-preemptive)
- STCF/RR: More switches due to preemption
- MLFQ: Balanced switches from priority boosting

### 4. **Metrics Calculation**
- **Turnaround Time**: Total time from arrival to completion (system overhead)
- **Waiting Time**: Time spent ready but not executing (scheduling fairness)
- **Response Time**: Delay from arrival to first execution (interactivity)

## Known Limitations & Assumptions

### Input Assumptions
1. **No negative values**: Arrival times and burst times ≥ 0
2. **No duplicate PIDs**: Each process has unique identifier
3. **Process info available**: All processes listed before simulation
4. **BurstTime interpretation**: Treated as total CPU time needed
5. **No I/O behavior**: All time is CPU time (no I/O blocking modeled)

### Scheduler Limitations
1. **No preemption for SJF**: Processes run to completion once started
2. **Fixed time quantum for RR**: Cannot adjust dynamically per-round
3. **No multi-core support**: Single CPU simulation only
4. **No process priority inheritance**: Priority doesn't change (except MLFQ)
5. **No aging mechanism**: Processes can wait indefinitely without aging (except MLFQ boost)

### Simulation Limitations
1. **No random arrivals**: All arrival times predetermined
2. **No I/O wait states**: Processes always CPU-bound
3. **Perfect memory access**: No cache effects modeled
4. **No inter-process communication**: No synchronization overhead
5. **Deterministic behavior**: Same input always produces same output

### MLFQ-Specific Limitations
1. **Known burst time benefits**: SJF still gives better results if burst times known
2. **Configuration-dependent**: Performance varies greatly with queue parameters
3. **No learning algorithm**: Parameters must be tuned manually
4. **Fixed queue count**: Cannot dynamically add/remove queues

### Performance Characteristics
- **Time Complexity**: O(n log n) per algorithm (event processing + queue operations)
- **Space Complexity**: O(n) for process array, O(m) for event queue
- **Supported scales**: Tested up to 100 processes, 1000-2000 time units

## Testing Results

### Workload-1 Characteristics
- **Processes**: 4 (A=30, B=40, C=50, D=10 time units)
- **Arrivals**: Staggered (T=10,20,30,50)
- **Best Algorithm**: SJF/STCF (TT=52.5, minimal variance)
- **Most Responsive**: MLFQ (RT=7.5, excellent for interactive processes)

### Workload-2 Characteristics
- **Processes**: 5 with mixed burst times
- **Key Challenge**: Process A has very long burst (100 units)
- **Best Algorithm**: STCF (preemption prevents A from blocking others)
- **Trade-off**: RR good responsiveness, poor average turnaround

### Workload-3 Characteristics
- **Processes**: 5 with larger burst times
- **Complexity**: Longer simulation, more context switches
- **CPU-Intensive**: MLFQ excels at balancing fairness and responsiveness
- **MLFQ Advantage**: Adapts without prior knowledge of burst times

## Comparison Results Summary

| Metric | Best Case | Worst Case | Trade-off |
|--------|-----------|-----------|-----------|
| **Avg Turnaround Time** | SJF (if bursts known) | FCFS | Preemption helps |
| **Avg Waiting Time** | SJF | FCFS | SJF requires burst knowledge |
| **Avg Response Time** | MLFQ/STCF | FCFS | Preemption essential |
| **Context Switches** | FCFS (fewest) | MLFQ/RR (many) | Responsiveness cost |
| **Fairness** | RR (uniform) | SJF (starvation risk) | Preemption + boost needed |
| **Adaptability** | MLFQ (learns) | SJF/RR (fixed) | MLFQ handles unknowns |

## Configuration & Customization

### Adjust MLFQ Parameters

**mlfq_config.txt:**
```
# For interactive workloads (shorter response time)
Q0  5   25      # Very short quantum
Q1  15  75      # Still responsive
Q2  -1  -1      # FCFS fallback
BOOST_PERIOD 100  # Frequent boosts

# For batch workloads (throughput)
Q0  50  200     # Longer quantum
Q1  100 500     # Much longer allotment
Q2  -1  -1
BOOST_PERIOD 500  # Less frequent boosts
```

### Adjust Round Robin Quantum

```bash
# For responsiveness
./schedsim --algorithm=rr --quantum=5 --input=workload.txt

# For throughput
./schedsim --algorithm=rr --quantum=50 --input=workload.txt
```

## Compilation Verification

```bash
# Check GCC version
gcc --version

# Compile with debug symbols (optional)
gcc -g -Wextra -I includes -o schedsim src/**/*.c

# Check binary size
ls -lh schedsim

# Run basic test
./schedsim --algorithm=fcfs --input=workload-1.txt
```

## Troubleshooting

**Issue: "No input provided!" error**
- Solution: Add `--input=workload.txt` or `--processes="A:10:30;B:20:40"`

**Issue: MLFQ "Failed to load config" error**
- Solution: Ensure `mlfq_config.txt` is in `workloads/` directory
- or specify full path: `--mlfq-config=/full/path/to/config.txt`

**Issue: Compilation fails**
- Solution: Check GCC is installed: `gcc --version`
- Verify all source files present: `find src -name "*.c" | wc -l`
- Solution: `make clean && make`

## Future Enhancements

1. **Preemptive MLFQ Priority Levels**: Different boost periods per level
2. **Adaptive Time Quantums**: Adjust based on observed process behavior
3. **Multi-CPU Support**: Simulate parallel execution
4. **I/O Operations**: Model I/O blocking and wake-up events
5. **Process Dependencies**: Support inter-process communication
6. **Statistical Analysis**: Generate comparative report with confidence intervals
7. **Visualization**: Interactive Gantt chart viewer
8. **Workload Generation**: Synthetic workload creator with distributions

## References

- **MLFQ Design**: Based on "Operating Systems: Three Easy Pieces" by Remzi Arpaci-Dusseau
- **Discrete Event Simulation**: Standard DES pattern for process-based simulation
- **Scheduler Algorithms**: Classic scheduling theory (CPU scheduling)

---

**Lab**: CMSC 125 - Operating Systems  
**Course**: 3rd Year, 2nd Semester  
**University**: [Technology Institute]  
**Last Updated**: March 2026
