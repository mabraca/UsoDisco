#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define DIRLEN 1024
#define LINELEN 1500

/*Se definen las distintas estructuras a utilizar durante el programa. y se inicializan las distintas funciones */

typedef struct directorys{
    char name[DIRLEN];
    struct directorys * next;
}directory;

typedef struct directorys_list{
    directory * start;
    directory * last;
}directory_list;

typedef struct nodes{
    char name[DIRLEN];
    long blocks;
    struct nodes * next;
}node;

typedef struct results_list{
    node * start;
}result_list;

typedef struct threads_param{
    directory_list * l;
    result_list * rl;
    pthread_mutex_t * add_mutex;
    pthread_mutex_t * get_mutex;
    pthread_mutex_t * add_node_mutex;
    pthread_mutex_t * modify_busy; 
}thread_param;

extern directory_list * create_list(void);
extern int is_empty(directory_list*);
extern int get_directory_name(directory_list *, char *);
extern int add_directory(directory_list*, char*);
extern int print_directory(directory*);
extern int print_list(directory_list*);
extern int add_start_list(directory_list*, directory_list*);
extern int destroy_list(directory_list*);
extern result_list * create_result_list(void);
extern int is_empty_result_list(result_list*);
extern int print_node(node*);
extern int add_result_node(result_list*, char*, long);
extern int get_result_node_information(result_list*, char*, long*);



