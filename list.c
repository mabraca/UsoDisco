#include "list.h"

/*AQUI SE ENCUENTRAN LAS DISTINTAS FUNCIONES PARA UTILIZAR DURANTE EL PROGRAMA. 
    SE CREA LISTA
    SE VERIFICA SI ESTA VACIA
    SE OBTIENE EL NOMBRE DEL DIRECTORIO QUE SE ENCUENTRA LA LISTA
    SE AGREGA EL DIRECTORIO A LA LISTA
    SE AGREGA AL PRINCIPIO DE LA LISTA
    SE DESTRUYE LA LISTA Y LA LISTA DE RESULTADOS
    SE VERIFICA LA LISTA DE RESULTADOS SI ESTA VACIA
    SE AGREGA INFORMACION A LA LISTA DE RESULTADOS
    SE OBTIENE INFORMACION DE LAS DIRECCIONES EN LA LISTA DE RESULTADOS. 

    PD: hay otras funciones que se encuentran en este archivo que fueron para comprobar
        datos y que no se borraron poque pueden ser util para verificar informacion 
        en casos de errores. 

    */

directory_list * create_list(){
    directory_list * l;
    l = (directory_list*)malloc(sizeof(directory_list));
    l->start = NULL;
    l->last = NULL;
    return l; 
}

int is_empty(directory_list * l){
    return l->start == NULL;
}

int get_directory_name(directory_list * l, char * name){
    int len;
    directory * aux;
    if(is_empty(l)){
        strcpy(name, "");
        return;
    } 
    strcpy(name, l->start->name);
    aux = l->start;
    l->start = l->start->next;
    if(l->start == NULL){
        l->last = l->start;
    }
    free(aux);
    return 0; 
}

int add_directory(directory_list * l, char * name){
    directory * d;
    d = (directory*)malloc(sizeof(directory));
    strcpy(d->name, name);
    if(is_empty(l)){
        d->next = NULL;
        l->last = d;
    }else{
        d->next = l->start;
    }
    l->start = d; 
    if(DEBUG) {
        print_directory(l->start);
    }
    return 0;
}

int print_directory(directory * d){
    printf("\nDirectory name: %s\n", d->name);
    return 0;
}

int print_list(directory_list * l){
    directory * aux;
    aux = l->start;
    printf("\nImprimiendo Lista..\n");
    while(aux != NULL){
        print_directory(aux);
        aux = aux->next;
    }
    return 0;
}

int add_start_list(directory_list * destiny, directory_list * start_list){
    if(is_empty(start_list)){
        return 0;
    }
    start_list->last->next = destiny->start;
    if(is_empty(destiny)){
        destiny->last = start_list->last;
    }
    destiny->start = start_list->start;
    return 0; 
}

int destroy_list(directory_list * l){
    l->start = NULL;
    l->last = NULL;
    free(l);
    l = NULL;
    return 0;
}

int destroy_result_list(result_list * rl){
    rl->start =NULL;
    free(rl);
    rl=NULL;
    return 0;
}

result_list * create_result_list(){
    result_list * rl;
    rl = (result_list*)malloc(sizeof(result_list));
    rl->start = NULL;
    return rl;
}

int is_empty_result_list(result_list * rl){
    return rl->start == NULL;
}

int print_node(node * n){
    printf("Node name: %s\n", n->name);
    printf("Node blocks: %ld\n", n->blocks);
}

int add_result_node(result_list * rl, char * name, long blocks){
    node * aux;
    aux = (node*)malloc(sizeof(node));
    strcpy(aux->name, name);
    aux->blocks = blocks;
    aux->next = rl->start;
    rl->start = aux;
    if(DEBUG){
        print_node(aux);
    }
    return 0;
}

int get_result_node_information(result_list * rl, char * name, long * blocks){
    int len;
    node * aux;
    if(is_empty_result_list(rl)){
        len = strlen(name);
        memset(name, '\0', len);
        strcpy(name, "");
        *blocks = 0;
        return 0;
    }
    aux = rl->start;
    strcpy(name, aux->name);
    *blocks = aux->blocks;
    rl->start = rl->start->next;
    free(aux);
    return 0; 
}

