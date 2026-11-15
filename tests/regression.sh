#!/bin/sh
set -eu
fcfs=$(./campuscompute fcfs jobs.csv)
rr=$(./campuscompute rr 2 jobs.csv)
priority=$(./campuscompute priority jobs.csv)
printf '%s\n' "$fcfs" | grep -F 'Average waiting=5.25 | average turnaround=9.50' >/dev/null
printf '%s\n' "$rr" | grep -F 'Average waiting=7.00 | average turnaround=11.25' >/dev/null
printf '%s\n' "$priority" | grep -F 'Average waiting=4.00 | average turnaround=8.25' >/dev/null
./campuscompute banker safe | grep -F 'SAFE STATE. Safe sequence:' >/dev/null
./campuscompute banker unsafe | grep -F 'UNSAFE STATE.' >/dev/null
./campuscompute logger 100 >/dev/null
[ "$(wc -l < campuscompute.log | tr -d ' ')" = 100 ]
[ "$(sort -u campuscompute.log | wc -l | tr -d ' ')" = 100 ]
./campuscompute worker normal | grep -F 'status 0' >/dev/null
./campuscompute worker fail | grep -F 'status 7' >/dev/null
./campuscompute limit | grep -F 'Observed EMFILE' >/dev/null
./campuscompute ipc | grep -F 'parent read it after semaphore notification' >/dev/null
if ./campuscompute rr 0 jobs.csv >/dev/null 2>&1; then echo 'Invalid quantum accepted' >&2; exit 1; fi
echo 'CampusCompute regression tests passed.'
