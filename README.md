# 522 Final Project

# Introduction
The purpose is to investigate the performance of inotify.
Hopefully, we can provide useful guidance and advice for developers who would like to use inotify.

# Experiments
1.2000 creates and deletes in a single directory
Usage:

Purpose: 
Investigate whether inotify can keep up with fast creates and deletes

Result: 
Inotify reported 2000 creates and deletes.
Inotify uses 195ms while the program that creates and deletes file used 194ms

Conclusion:
Inotify worked correctly and fast enough.

2.2000 creates and deletes in a flat root directory with 2000 subdirectories
Usage:
Purpose:
Result:
