#!/usr/bin/env bash
set -euo pipefail

TEST_DIR="build/tests/tests"
if [[ ! -d "$TEST_DIR" ]]; then
    echo "Test directory '$TEST_DIR' does not exist"
    exit 1
fi

TEST_EXECUTABLES=$(find "$TEST_DIR" -type f -perm +111)

EXIT_CODE=0
for test_bin in $TEST_EXECUTABLES; do
    "$test_bin"
    if [[ $? -ne 0 ]]; then
        EXIT_CODE=1
    fi
done

exit $EXIT_CODE
