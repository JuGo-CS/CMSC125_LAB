# Lab 3 Discussion

## Deadlock Strategy Choice

### Strategy chosen
We chose **deadlock prevention** using a strict global lock ordering policy.

### Why this strategy?
Prevention is simpler to reason about and eliminates the need for runtime cycle detection and abort logic. It also avoids aborting transactions under load, which improves predictability for this assignment's transactional workload.

### Proof that lock ordering eliminates circular wait
Every transaction acquires account locks in ascending order by account ID. Since all transactions follow the same global order, it is impossible for a cycle of waiting locks to form.

Suppose transaction `T1` holds lock on account `A` and waits for account `B`, while transaction `T2` holds lock on `B` and waits for `A`. For that to happen, `A` and `B` would need to appear in different order in the two acquisitions. Because we always sort account IDs and acquire in increasing order, such an inversion cannot occur.

### Coffman condition broken
This strategy breaks the **circular wait** condition. Since all locks are acquired in a consistent global order, no circular wait can arise.

## Buffer Pool Integration

### When accounts are loaded into the buffer pool
Accounts are loaded into the buffer pool when a transaction first accesses them. The access path reads the account record from disk into the in-memory buffer pool before applying any operation.

### When accounts are unloaded
Accounts are unloaded from the buffer pool when they are evicted to make room for other pages. Dirty accounts are written back to disk before eviction to preserve correctness.

### What happens if the pool is full when a transaction needs an account
If the buffer pool is full, the system evicts an existing page according to the pool's replacement policy, then loads the requested account. If the evicted page is dirty, it is flushed to disk first.

### Justification
This design keeps the working set of hot accounts in memory, which improves performance by reducing repeated disk reads. Correctness is preserved because page eviction only occurs after dirty pages are written back, and transactions acquire their locks before modifying pages.

## Reader-Writer Lock Performance

### Benchmark results
We compared `pthread_mutex_t` against `pthread_rwlock_t` using the provided trace files.

- `test-no-conflict.txt`: rwlock showed the biggest improvement because most operations are reads and multiple readers can proceed concurrently.
- `test-concurrent.txt`: rwlock had moderate improvement from concurrent read overlap.
- `test-deadlock.txt` and `test-pool-saturation.txt`: smaller or negligible improvement because write contention and pool pressure dominate.

### Why rwlock helps on read-heavy workloads
`pthread_rwlock_t` allows multiple readers to hold the lock simultaneously while still enforcing exclusive access for writers. On read-heavy traces, this reduces unnecessary blocking among reader threads and increases throughput compared to a plain mutex.

## Timer Thread Design

### Why a separate timer thread is necessary
A separate timer thread is necessary to enforce timeouts independently of transaction execution. It can expire locks and abort long-running operations without blocking or delaying normal transaction processing.

### What would break without the timer
If timer events were processed sequentially with transaction operations, a blocked or long-running transaction could prevent timeout handling entirely. That would allow deadlocks or stalls to persist longer and would prevent the system from enforcing concurrency time limits reliably.

### How the timer thread enables true concurrency testing
The timer thread runs concurrently with transaction execution, so timeouts and transaction progress are evaluated independently. This creates a more realistic concurrent environment and allows tests to exercise timing-dependent behaviors such as lock waits, aborts, and eviction under contention.
