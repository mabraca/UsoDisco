#include "command.h"
int busy = 0;


/* Funcion del hilo maestro  get_main_directory_blocks
    Los parametros que necesita la funcion son: 
        l       Lista donde se guardara las direcciones
        rl      Lista de resultado donde se guardara informacion del directorio
        name    Nombre del directorio a buscar informacion
        Los distitons mutex
    Busca en el directorio principal la informacion requerida.
    Funcionamiento:
    Se inicializan los valores a utilizar
    Se inicia la lectura del directorio
    Se comprueba si hay archivos regulares para guardar su informacion, de lo contrario lo a;ade en el directorio para que sus trabajadores hagan el trabajo.
    Se utilzia los distintos mutex para guardar la informacion para que no haya ninguna problema de escritura

*/
int get_main_directory_blocks(directory_list * l, result_list * rl,
    char * name, pthread_mutex_t * add_mutex, pthread_mutex_t * add_node_mutex){
    DIR *dir;
    struct dirent *dp;
    struct stat s;
    directory_list * temp = create_list();
    long result = 0;
    char complete_name[DIRLEN];
    dir = opendir(name);
    if(dir == NULL){
        perror("opendir:");
        return (-1);
    }

    while((dp = readdir(dir)) != NULL){
        int er;
        sprintf(complete_name, "%s/%s", name, dp->d_name);
        er=(stat(complete_name, &s));
        if (er==-1){
            perror(complete_name);
        }
        else{
            if(S_ISDIR(s.st_mode)){
                if((strcmp(dp->d_name, ".") != 0) && 
                    (strcmp(dp->d_name, "..") != 0)){
                    add_directory(temp, complete_name);
                }
            }else if(S_ISREG(s.st_mode)){
                result += s.st_blocks;
            }

        }
    }
    closedir(dir);
    pthread_mutex_lock(add_mutex);
    add_start_list(l, temp);
    pthread_mutex_unlock(add_mutex);
    destroy_list(temp);
    pthread_mutex_lock(add_node_mutex);
    add_result_node(rl, name, result);
    pthread_mutex_unlock(add_node_mutex);
    return 0;
}

/*Funcion de resultado write_file
    Los parametros a necesitar:
        rl Lista de resultados
        filename nombre del Archivo de salida 
    Da los resultados de la busqueda, ya sea por consola o por escritura de archivo.
    Funcionamiento:
    Se inicializan los valores a utilizar.
    Se comprueba si el nombre del archivo esta vacio, indicando que el usuarion
    quiere que se imprima por pantalla, de lo contrario se imprimira en un archivo.
    Se abre el archivo donde se guarda los resultados, si el usuario indico el nombre. 
    Se va leyendo la lista de resultados y se van guardando en el archivo o imprimiendo en pantalla en su defecto.
    System clear se hace para limpiar cualquier cosa que tengamos en nuestra entrada de comandos (consola)



*/
int write_file(result_list * rl, char * filename){
    FILE * f;
    long blocks;
    char dir_name[DIRLEN], line[LINELEN];
    int dir_name_len, line_len;
    if(strcmp(filename, "") != 0){
        f = fopen(filename, "w");
        if(f == NULL){
            printf("\nError al abrir el archivo para escritura.. Cerrando\n");
            return (-1);
        }
        while(!is_empty_result_list(rl)){
            get_result_node_information(rl, dir_name, &blocks);
            sprintf(line,"%ld          %s \n", blocks, dir_name);
            fputs(line, f);
        }
        fclose(f);
    }else{system("clear");
         while(!is_empty_result_list(rl)){
            get_result_node_information(rl, dir_name, &blocks);
            sprintf(line,"%ld          %s \n", blocks, dir_name);
            printf("%s",line);
        }  
    }
    return 0;
}



/*Funcion de los hilos trabajadores get_directory_blocks
    Tiene un solo parametro param donde de obtedra la informacion del directorio
    Funcionamiento:
    Se inicializan los valoes a utilizar
    Se crea un busy wait para estar al tanto si hay mas directorios en la la lista, si no lo hay mata al hilo.
    Se va leyendo cada informacion de la lista obtenida por el parametro de entrada y se coloca como ocupado mientras lee
    Se lee el directorio obtenido y se va guardando informacion de los archivos regulares. Se desoculpa una vez terminado
    Se utiliza los distintos mutex para que no hay problema con la escritura.



*/
void * get_directory_blocks(void * param){
    thread_param * p = (thread_param*)param;
    directory_list * l = p->l;
    result_list * rl = p->rl;
    DIR *dir;
    struct dirent *dp;
    struct stat s;
    directory_list * temp;
    char name[DIRLEN], complete_name[DIRLEN];
    long result = 0;
    int er;
 
    while(1){
        temp = create_list();
        //busy wait
        while(is_empty(l) && busy > 0){
            sleep(1);
        }

        if(is_empty(l) && busy <= 0){
            destroy_list(temp);
            pthread_exit(NULL);
        }
        //extraer elemento de la fila
        pthread_mutex_lock(p->modify_busy);        
        busy++;
        pthread_mutex_unlock(p->modify_busy);
        pthread_mutex_lock(p->get_mutex);
        get_directory_name(l, name);
        pthread_mutex_unlock(p->get_mutex);
        
        if(strcmp(name, "\0") != 0){
            dir = opendir(name);
            if(dir == NULL){
                pthread_exit(NULL);
            }
            while((dp = readdir(dir)) != NULL){
                sprintf(complete_name, "%s/%s", name, dp->d_name);
                er=(stat(complete_name, &s));
                if (er==-1){
                    perror(complete_name);
                }
                else{
                    stat(complete_name, &s);
                    if(S_ISDIR(s.st_mode)){
                        if((strcmp(dp->d_name, ".") != 0) && 
                            (strcmp(dp->d_name, "..") != 0)){
                            add_directory(temp, complete_name);
                        }
                    }else if(S_ISREG(s.st_mode)){
                        result += s.st_blocks;
                    }
                }
            }
            closedir(dir);
            pthread_mutex_lock(p->add_mutex);
            add_start_list(l, temp);
            pthread_mutex_unlock(p->add_mutex);
            destroy_list(temp);
            pthread_mutex_lock(p->add_node_mutex);
            add_result_node(rl, name, result);
            pthread_mutex_unlock(p->add_node_mutex);
        }
        pthread_mutex_lock(p->modify_busy);        
        busy--;
        pthread_mutex_unlock(p->modify_busy);
    }
}




