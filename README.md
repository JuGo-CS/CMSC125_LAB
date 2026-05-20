# CMSC125 — Course Labs (CMSC 125)

This repository collects the laboratory assignments and supporting code for CMSC 125
(Operating Systems). The labs demonstrate core OS concepts through small, focused
projects: a user shell, a set of CPU-scheduling simulators, and a concurrent banking
transaction engine. Each lab directory contains source code, headers, a `Makefile`, and
an individual `README.md` with detailed build and usage instructions.

Purpose and learning goals
--------------------------
- Provide hands-on experience with process creation, I/O redirection, and basic job
	control (Lab 1).
- Explore CPU scheduling policies, metrics collection, and discrete-event simulation
	design (Lab 2).
- Implement concurrency control, synchronization primitives, and deadlock avoidance in a
	realistic transaction workload (Lab 3).

Prerequisites
-------------
- POSIX-compatible environment (Linux or WSL on Windows)
- `gcc` (or `clang`) and `make`
- Basic familiarity with C and `pthread`s for Lab 3

Repository layout (top-level)
----------------------------
- `lab_1/`       — Simple Unix-like shell (`MYSH`) implemented in C
- `lab_2/`       — CPU scheduler simulators (`schedsim` and `schedsim-2` implementations)
- `lab_3/`       — Concurrent banking system with transaction engine and buffer pool

Lab 1 — MYSH (Unix shell)
--------------------------
Location: `lab_1/`

Summary
- A basic command-line shell written in C that provides: command execution with
	arguments, input/output redirection (`<`, `>`, `>>`), background execution using
	`&`, and a few built-in commands (`pwd`, `cd`, `exit`). It includes lightweight
	background-job tracking and cleanup.

Key files
- `mysh.c`        — main REPL loop, prompt, and coordination between parser and interpreter
- `parser.c`      — lexical splitting, token handling, and validation of redirections
- `interpreter.c` — built-in command handling, forking, redirection setup, and exec
- `mysh.h`        — data structures, constants, and function prototypes

Build & run
```bash
cd lab_1
make          # compile mysh
make run      # run shell
make clean    # remove binaries
```

Examples
- Run a command in background and list files while it runs:
```text
mysh> sleep 20 &
mysh> ls -la
```
- Redirect output and append:
```text
mysh> echo hello > out.txt
mysh> echo world >> out.txt
```

Notes
- Piping (`|`) is intentionally not implemented for this iteration.
- Only one input and one output redirection are supported per command invocation.

Lab 2 — CPU Scheduler Simulators
--------------------------------
Location: `lab_2/` (contains `schedsim/` and `schedsim-2/`)

Summary
- Implements a discrete-event simulation engine to evaluate multiple CPU scheduling
	algorithms: FCFS, SJF, STCF (preemptive SJF), Round-Robin (RR), and a configurable
	Multi-Level Feedback Queue (MLFQ). The simulator loads workloads, processes events
	(arrivals, completions, quantum expirations), and reports per-process metrics and
	aggregated statistics for comparison.

Important pieces (see `lab_2/schedsim-2/README.md` for details)
- `includes/` — interfaces for scheduler, simulator, metrics, and data-structure ADTs
- `src/`      — DES engine, scheduler implementations, and metric reporting
- `workloads/`— example input files and `mlfq_config.txt` to tune MLFQ behavior
- `docs/`     — design notes for the MLFQ implementation

Build & run examples
```bash
cd lab_2/schedsim-2
make
# Run FCFS on workload-1
./schedsim --algorithm=fcfs --input=workloads/workload-1.txt
# Run MLFQ with specified config
./schedsim --algorithm=mlfq --mlfq-config=workloads/mlfq_config.txt --input=workloads/workload-2.txt
```

What you can learn
- Differences between preemptive and non-preemptive policies
- How RR quantum affects response time vs. throughput
- How MLFQ adapts to unknown workloads via promotion/demotion and priority boost

Lab 3 — Concurrent Banking System
---------------------------------
Location: `lab_3/`

Summary
- A multi-threaded transaction processing system that demonstrates concurrency control
	and synchronization. The engine uses POSIX threads, per-account reader-writer locks,
	a bounded buffer pool for transaction objects, and enforces a deterministic lock
	acquisition ordering (ascending account ID) to prevent circular wait and deadlocks.

Key files
- `src/`       — core implementation (`bank.c`, `transaction.c`, `buffer_pool.c`, etc.)
- `include/`   — header files describing public APIs and data structures
- `tests/`     — prepared workloads to validate behavior (concurrency, deadlock, invalid)

Build & test
```bash
cd lab_3
make test      # compile and run the repository's test suite
make debug     # build with debug flags (ThreadSanitizer recommended)
make clean
```

Design notes & limitations
- Deadlock prevention is achieved via strict global lock ordering (ascending Account ID).
	This avoids circular wait but does not implement deadlock detection or dynamic lock
	reordering.
- Persistence is simulated; there is no write-ahead logging (WAL) or durable storage.

How to get started
------------------
1. Pick the lab folder you want to explore: `lab_1`, `lab_2/schedsim-2`, or `lab_3`.
2. Read the lab's own `README.md` for deeper details, design notes, and examples.
3. Build with `make` inside the chosen directory and run the example commands shown.

Testing guidance
----------------
- Use the provided `workloads/` and `tests/` inputs to reproduce typical scenarios.
- For Lab 2, try `--compare` to run multiple algorithms on the same input and compare
	average turnaround, waiting, and response times.
- For Lab 3, run `make debug` with ThreadSanitizer to catch potential data races.

Authors
-------
- Christian Jave S. Hulleza  — cshulleza@up.edu.ph
- Kenneth G. Mondejar        — kgmondejar@up.edu.ph



This repository is provided for course use. See individual lab READMEs for any
additional attribution.
