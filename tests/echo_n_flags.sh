#!/bin/bash
set -e
BIN="$(dirname "$0")/../minishell"

run_test() {
  desc="$1"
  cmd="$2"
  expected="$3"
  expected_status="$4"
  output=$(printf "%b\nexit\n" "$cmd" | "$BIN" 2>&1)
  status=$?
  output=${output%exit}
  output=$(printf "%s" "$output")
  if [ "$output" = "$expected" ] && [ "$status" -eq "$expected_status" ]; then
    echo "PASS: $desc"
  else
    echo "FAIL: $desc" >&2
    echo "  expected: '$expected' (status $expected_status)" >&2
    echo "  got:      '$output' (status $status)" >&2
    exit 1
  fi
}

run_test "multiple -n" 'echo -nnn hello' 'hello' 0
run_test "quoted -n" 'echo "-n" world' 'world' 0
run_test "-n with quoted n" 'echo -n "n" "n" hi' 'hi' 0

