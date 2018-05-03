# 522 Final Project

# Introduction
The purpose is to investigate the performance of inotify.
Hopefully, we can provide useful guidance and advice for developers who would like to use inotify.

# Experiments
1.2000 creates and deletes in a single directory.

Usage:
run inotify program in one terminal: ./single/monitor_create_delete_in_single_directory single/test_directory
run 2000 creates and deletes in another terminal: ./single/create_delete

Purpose: 
Investigate whether inotify can keep up with fast creates and deletes if they happen in a single directory.
A hypothetical use case: a database where records are frequently created and deleted

Technical details:
Use clock_gettime(CLOCK_REALTIME, &spec) to get time.

Result (on Raspberry Pi):
Inotify reported 2000 creates and deletes so inotify reported the exact number of creates and deletes that
it is supposed to reported
Inotify uses 195ms (from first create detected to last delete detected) while the program that 
creates and deletes file used 194ms (from immediately before first create to immediately after last delete).

Conclusion:
Inotify worked correctly and fast enough.

2.2000 creates and deletes in a flat root directory with 2000 subdirectories.

Usage:
create 2000 flat directories and run inotify program in one terminal: ./flat/flat_directory.sh
run 2000 creates and deletes in another terminal: ./flat/monitor_flat_directory_structure

Purpose:
Investigate whether inotify can keep up with creates and deletes in a large number of subdirectories in a flat
root directory
A hypothetical use case: a database where a large number of users can create and delete records in a
subdirectory that is assigned to her

Result:
Inotify reported 2000 creates and deletes so inotify reported the exact number of creates and deletes that
it is supposed to reported
Inotify uses 165ms (from first create detected to last delete detected) while the program that 
creates and deletes file used 166ms (from immediately before first create to immediately after last delete).

Conclusion:
Inotify worked correctly and fast enough.

3.Concurrently run three programs, each making 2000 creates and deletes in a single directory.

Usage:
run inotify program in one terminal: ./three_processes/monitor_create_delete_in_single_directory three_processes/test_directory
Concurrently run three programs, each making 2000 creates and deletes in another terminal: bash three_processes/run_three_cores.sh

Purpose: 
Investigate whether inotify can keep up with fast creates and deletes run concurrently if they happen in a single directory.
A hypothetical use case: a database where records are frequently created and deleted

Technical details:
Use clock_gettime(CLOCK_REALTIME, &spec) to get time.

Result (on Raspberry Pi):
Inotify reported 6000 creates and deletes so inotify reported the exact number of creates and deletes that
it is supposed to reported
Inotify uses 114ms (from first create detected to last delete detected) while the program that 
creates and deletes programs used 267, 303, 305ms respectively (from immediately before first create to immediately after last delete).
