#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) {
    // Se llama a funcion hash para saber el indice de la casilla en donde se deberia insertar
    unsigned long indice = hash(key, map->capacity);

    // SONDEO LINEAL
    // Mientras que no se encuentre con un valor NULL y 
    // que tampoco la clave sea igual a la clave que se encuentra en la casilla
    // Osea, mientras no se encuentre con una casilla vácia ni se encuentre con la misma clave
    // El indice avanzara uno más 
    while(map->buckets[indice] != NULL && strcmp(map->buckets[indice]->key, key) != 0){
        indice = (indice + 1) % map->capacity;
    }

    // Si no ocurre colison, osea, la casilla esta vacia, se inserta
    if (map->buckets[indice] == NULL) {
        map->buckets[indice] = malloc(sizeof(Pair));
        map->buckets[indice]->key = strdup(key); // copiamos la clave
        map->size++;
    }
    // Actualizamos el valor
    map->buckets[indice]->value = value;
}

void enlarge(HashMap * map) {
    // Se duplica la capacidad del mapa
    long new_capacity = map->capacity * 2;
    // Se reserva memoria para el nuevo mapa
    HashMap * new_map = createMap(new_capacity);
    if (new_map == NULL) {
        return;
    }
    // Se copian los datos del mapa original al nuevo mapa
    for (long i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            insertMap(new_map, map->buckets[i]->key, map->buckets[i]->value);
        }
    }
    // Se libera la memoria del mapa original
    free(map->buckets);
    // Se actualiza el mapa original con el nuevo mapa
    map->buckets = new_map->buckets;
    map->capacity = new_capacity;
    free(new_map);
    // Se actualiza el tamaño del mapa original
}


HashMap * createMap(long capacity) {
    // Reservamos memoria para el mapa
    HashMap*mapa = (HashMap*)malloc(sizeof(HashMap));
    if(mapa == NULL) {
        return NULL;
    }
    // Reservamos memoria para los buckets
    mapa->buckets = (Pair**)malloc(sizeof(Pair*)*capacity);
    if(mapa->buckets == NULL) {
        return NULL;
    }
    // Inicializamos todos los buckets en NULL
    for(long i = 0; i < capacity; i++){
        mapa->buckets[i] = NULL;
    }
    // Dejamos declarada la capacidad, la cantidad de datos y el indice del ultimo dato accedido
    // es -1 porque no hay datos en el mapa
    mapa->capacity = capacity;
    mapa->size = 0;
    mapa->current = -1;
    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    

    return;
}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}

// END