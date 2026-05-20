# Lab 3: Concurrent Banking System 

## Team Members
* **Kenneth Garcia Mondejar**
* **Christian Jave Hulleza**

---

## Compilation & Execution Instructions

This repository features a robust, automated compilation and verification workflow via standard GNU `make`. 

* **Compile and Test:**
    To automatically compile all system binaries and sequentially evaluate the system against all standard banking trace scenarios
    ```bash
    make test
    ```

* **Debug Mode:**
    To compile the engine with fully optimized debugging symbols (`-g`) and active ThreadSanitizer flags for diagnosing potential race conditions or lock hierarchies, run:
    ```bash
    make debug
    ```

* **Clean Build Artifacts:**
    To safely remove generated binary executables, temporary structural object files, and memory dumps, run:
    ```bash
    make clean
    ```
---

## Implemented Features

As dictated by the course module, our implementation fully realizes a concurrent transaction execution model utilizing native POSIX threads and hardware-level synchronization primitives:

1.  **Multi-Threaded Transaction Execution Engine:** Instantiates an independent `pthread` for every incoming transactions ($T1 \dots Tn$). Threads dynamically block on condition variables waiting for a broadcast signal from a central background timer thread before execution.
2.  **Granular Reader-Writer Isolation:** Protects account records with independent `pthread_rwlock_t` structures. This design safely scales multi-threaded execution environments by permitting unbounded concurrent reads (`OP_BALANCE`) while ensuring mutual exclusion for write states (`OP_DEPOSIT`, `OP_WITHDRAW`).
3.  **Deadlock Prevention via Strict Global Lock Ordering:** Completely neutralizes the *Circular Wait* Coffman condition. For multi-resource operations (`OP_TRANSFER`), the transaction engine enforces an invariant locking sequence: resource locks are acquired deterministically in ascending order based on the unique `Account ID` (low ID mapped first, high ID mapped second).
4.  **Bounded Buffer Pool Control:** Implements a localized 5-slot memory architecture utilizing a classic producer-consumer counting semaphore matrix (`sem_t empty_slots` and `sem_t full_slots`) paired with a critical section mutex to manage hardware page mapping.
5.  **Runtime Analytics & Conservation Auditing:** Incorporates an immutable instrumentation layer capturing latencies (`WaitTicks`), engine throughput metrics (`tx/tick`), and executing an automated terminal invariant audit verifying that the global net centavos remain perfectly conserved.

---

## Known Limitations

Our concurrent engine achieves data conservation and passes standard runtime test cycles. It operates within the following constraints:

* **Single Deadlock Strategy — Prevention Only:** The system implements deadlock prevention exclusively via strict global lock ordering (ascending account ID). The `--deadlock detection` flag is accepted for CLI compatibility but always applies the prevention strategy. No wait-for-graph cycle detector is implemented.
* **Localized Page Directories & Cache Load Overhead:** The Buffer Pool Manager handles per-thread page requests independently instead of validating access against an atomic, global mapping registry. While this architecture guarantees strict data isolation during reading cycles, it can introduce concurrent cache misses and redundant load executions when multiple threads target identical account scopes simultaneously.
* **Absence of Fine-Grained Upgrades or Lock Downgrading:** The operational layer lacks support for advanced Multi-Version Concurrency Control (MVCC) or active transaction lock downgrading. Consequently, write actions execute over coarse, exclusive states, which can reduce maximal system throughput under dense write workloads (`DEPOSIT`/`WITHDRAW`) due to thread stalling.
* **Volatile In-Memory Storage Emulation:** The disk persistence subsystem is simulated within a volatile operational block. The system does not implement a Write-Ahead Logging (WAL) protocol, meaning transaction durability and absolute point-in-time recovery cannot be guaranteed if an unhandled process termination or hardware fault simulation occurs.
