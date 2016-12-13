    /*******************************************************/
    /*USO DISCO      CREADO POR:                            /
    /                Carlos Noria                           /
    /                Maria Bracamonte                       /
    /                                                       /
    /       Lab. Sistema de operacion                       /
    /       Prof.Angela Di Serio.                           /
    /       Prof. Carlos Gomez.                             /
    /                                                       /
    /*******************************************************/

#include "command.h"
#include <pthread.h>

/*Funcion principal:
    Se inicializan todos los parametros a utilizar.
    Se crean todos los mutex a utilizar
    Con un ciclo for se le da nombre a cada parametro de entrada, de lo contrariose les da uno por defecto.
    Se genera el hilo maestro y los hilos trabajadores.
    Una vez realizado el trabajo de cada hilo, se eliminan los mutex y se imprime el resultado.
    Se libera el espacio utilizado. 
*/



int main(int argc, char * argv[]){
    char base_dir[DIRLEN], filename[LINELEN];
    int num_threads, i,error, j;
    char buffer[DIRLEN];
    pthread_t * threads;
    pthread_attr_t attr;
    int ocupados = 0;
    int dir_=0;
    int file_=0;
    int thr_=0;

    getcwd(buffer, sizeof(buffer));
    thread_param * param;
    param = (thread_param*)malloc(sizeof(thread_param));
    param->l = create_list();
    param->rl = create_result_list();
    pthread_mutex_t add_mutex;
    pthread_mutex_t get_mutex;
    pthread_mutex_t add_node_mutex;
    pthread_mutex_t modify_busy;

    pthread_mutex_init(&add_mutex, NULL);
    pthread_mutex_init(&get_mutex, NULL);
    pthread_mutex_init(&add_node_mutex, NULL);
    pthread_mutex_init(&modify_busy, NULL);

    param->add_mutex = &add_mutex;
    param->get_mutex = &get_mutex;
    param->add_node_mutex = &add_node_mutex;
    param->modify_busy = &modify_busy;

    //Seccion de seteo de parametros    
    //   ./UsoDisco -h
  
     //   ./UsoDisco distintas formas de -o salida / -n i entero / -d direccion
    for(j=1;j<argc;j=j+1){
        if(strcmp(argv[j],"-h")==0){
            printf("--------- Sintaxis--------- \n");
            printf("UsoDisco [-h] | [-n i] [-d directorio] [-o salida ]\n\n");
            printf("-h               Muestra por pantalla un mensaje de ayuda\n");
            printf("-n i             Nivel de concurrencia solicitado. El argumento i es un entero\n");
            printf("-d <dir>         Directorio a calcular.\n");
            printf("-o <salida.txt>  Nombre del archivo de salida. \n");
            printf("----------------------------\n");
            sleep(1);
            exit(1);
        }
        if(strcmp(argv[j],"-o")==0){
            strcpy(filename, argv[j+1]);
            file_+=1;
        }
        else if(strcmp(argv[j],"-n")==0){
            num_threads=atoi(argv[j+1]);
            thr_+=1;
        }
        else if(strcmp(argv[j],"-d")==0){
            strcpy(base_dir, argv[j+1]);
            dir_+=1;
        }
    }
    //Default
    if (thr_==0){           
        num_threads=1;
    }
    if(dir_==0){           
        strcpy(base_dir, buffer);
    }
    if(file_==0){
        strcpy(filename, "");
    }

    threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    

    get_main_directory_blocks(param->l, param->rl, base_dir, param->add_mutex, 
        param->add_node_mutex);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for(i = 0; i < num_threads; i++){
        error= pthread_create(&threads[i], &attr, get_directory_blocks, (void *)param);
        if (error != 0)
            {
                perror ("No se pudo crear los threads");
                exit (-1);
            }
    }
    for (i = 0; i < num_threads; i++) {
        int errorr;
        void* b=NULL;
        errorr=pthread_join(threads[i], &b);
        if(errorr!=0){
            printf("error en el thread,%i\n",i );
        }
    }
    //destruir_semaforos
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&add_mutex);
    pthread_mutex_destroy(&get_mutex);
    pthread_mutex_destroy(&add_node_mutex);
    pthread_mutex_destroy(&modify_busy);

    //imprimir resultados
    write_file(param->rl, filename);
    destroy_list(param->l);
    destroy_result_list(param->rl);
    free(param);
    free(threads);
    pthread_exit(NULL); 

}
