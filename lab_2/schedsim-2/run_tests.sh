#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

OUTPUT_FILE="test_output.txt"
> "$OUTPUT_FILE"

# Build the simulator
echo "=== Building simulator ===" | tee -a "$OUTPUT_FILE"
make clean all 2>&1 | tee -a "$OUTPUT_FILE"

tests=(
  "fcfs_input-1"
  "fcfs_input-2"
  "fcfs_input-3"
  "sjf_input-1"
  "sjf_input-2"
  "sjf_input-3"
  "rr_input-1"
  "rr_input-2"
  "rr_input-3"
  "stcf_input-1"
  "stcf_input-2"
  "stcf_input-3"
  "mlfq_input-1"
  "mlfq_input-2"
  "mlfq_input-3"
)

for t in "${tests[@]}"; do
  echo "" | tee -a "$OUTPUT_FILE"
  echo "=== Test: $t ===" | tee -a "$OUTPUT_FILE"
  # `make` target may not stop on fail due to -k; stop on first fail here
  if ! make "$t" 2>&1 | tee -a "$OUTPUT_FILE"; then
    echo "ERROR: target $t failed" | tee -a "$OUTPUT_FILE"
    exit 1
  fi
  echo "=== Completed: $t ===" | tee -a "$OUTPUT_FILE"
done

echo "" | tee -a "$OUTPUT_FILE"
echo "All tests completed successfully." | tee -a "$OUTPUT_FILE"
