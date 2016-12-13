#include "list.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

extern int get_main_directory_blocks(directory_list*, result_list*, char*, 
    pthread_mutex_t*, pthread_mutex_t*);
extern int write_file(result_list*, char*);
void * get_directory_blocks(void*);
