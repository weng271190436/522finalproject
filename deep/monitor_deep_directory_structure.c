#define _GNU_SOURCE
#include <sys/inotify.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define SUCCESS 0
#define SPIN 1
#define BUF_LEN 4096
#define MAX_NUM_FILES_WATCHED 100
#define MAX_PATH_LEN 100
#define BILLION 1000000000
#define MILLION 1000000
#define SIZE 2047

atomic_int creates;
atomic_int deletes;
unsigned long long total_diff;

void sigint_handler( int signum ){
	printf("In total, there have been %d creates and %d deletes\n.",
		creates, deletes);
	_exit(EXIT_FAILURE);
}

int main(int argc, char* args[]){
	creates=0;
	deletes=0;
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  // only allow it to run one cpu zero
  CPU_SET(1, &cpuset);
  sched_setaffinity(getpid(), 1, &cpuset);
  if (argc<2){
    printf("Usage: %s [path to watch]\n", args[0]);
    exit(EXIT_FAILURE);
  }
  char* path;
  int fd;
  fd = inotify_init1(0);
  if (fd==-1){
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }else{
    printf("Successfully initialize inotify.\n");
    printf("File descriptor number is %d.\n", fd);
  }
  int wd;
  int j;
  for (j = 1; j < argc; j++) {
		wd = inotify_add_watch(fd, args[j], IN_ALL_EVENTS);
		if (wd == -1){
			perror("inotify_add_watch_to_all_events");
			exit(EXIT_FAILURE);
		}else{
			printf("Add watch successful.\n");
			printf("File descriptor number is %d.\n",wd);
		}
	}

	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;

	sigaction( SIGINT, &sa, NULL );

	struct timespec spec;
	unsigned long long timestamp;
	unsigned long long start_nsec, end_nsec;

	total_diff = 0;

	while(SPIN){
    char buf[BUF_LEN] __attribute__(
			(aligned(__alignof__(struct inotify_event))));
		ssize_t len,i=0;

		/* read BUF_LEN bytes' worth of events*/
		len=read(fd,buf,BUF_LEN);
		if (creates == 0){
			clock_gettime(CLOCK_REALTIME, &spec);
			start_nsec = ((unsigned long long) spec.tv_sec) * BILLION + (unsigned long long) spec.tv_nsec;
			printf("start_nsec: %llu ns\n", start_nsec);
		}
		
		/* loop over every read event until none remain */
		while(i<len){
			struct inotify_event *event=
				(struct inotify_event *) &buf[i];
			/*
			printf("wd=%d mask=%d cookie=%d len=%d dir=%s\n",
				event->wd, event->mask,
				event->cookie, event->len,
				(event->mask & IN_ISDIR) ? "yes":"no");
			
			// if there is a name, print if
			
			if(event->len){
				printf("name=%s\n",event->name);
			}
			*/

			if(event->mask & IN_CREATE){
				atomic_fetch_add(&creates,1);
			}
			if(event->mask & IN_DELETE){
				atomic_fetch_add(&deletes,1);
				if (deletes == SIZE){
					clock_gettime(CLOCK_REALTIME, &spec);
					end_nsec = ((unsigned long long) spec.tv_sec) * BILLION + (unsigned long long) spec.tv_nsec;
					printf("end_nsec: %llu ns\n", end_nsec);
					total_diff = end_nsec - start_nsec;
					printf("From first create to last delete: %llu ms\n", total_diff/MILLION);
				}
					
			}
			if (event->mask & IN_MODIFY) {
				/*
				printf("Write detected\n");
				printf("%s\n", new_file_path);
 				FILE * rFile;
				rFile = fopen(new_file_path, "r");
				fscanf(rFile, "%llu", &timestamp);
				printf("Timestamp when creating file %llu \n", timestamp);
				printf("Diff: %llu\n", nsec - timestamp);
				total_diff += nsec - timestamp;
				*/
			}
			/* update the index to the start of the next event*/
			i+=sizeof(struct inotify_event)+event->len;
			//printf("There have been %d creates and %d deletes.\n", creates, deletes);
		}
	}

	printf("There were %d creates and %d deletes\n.",
		creates, deletes);
	return SUCCESS;
}
