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
    unsigned long posicion = hash(key, map->capacity);

    // SONDEO LINEAL
    // Mientras que no se encuentre con un valor NULL y 
    // que tampoco la clave sea igual a la clave que se encuentra en la casilla
    // Osea, mientras no se encuentre con una casilla vácia ni se encuentre con la misma clave
    // El indice avanzara uno más 
    while(map->buckets[posicion] != NULL && strcmp(map->buckets[posicion]->key, key) != 0){
        posicion = (posicion + 1) % map->capacity;
    }

    // Si no ocurre colison, osea, la casilla esta vacia, se inserta
    if (map->buckets[posicion] == NULL) {
        map->buckets[posicion] = malloc(sizeof(Pair));
        map->buckets[posicion]->key = strdup(key); // copiamos la clave
        map->size++;
    }
    // Actualizamos el valor
    map->buckets[posicion]->value = value;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    // guardamos la antigua capacidad y los buckets
    Pair ** old_buckets = map->buckets;
    long old_capacity = map->capacity;

    // duplicamos la capacidad
    map->capacity *= 2;
    // reservamos memoria para los nuevos buckets
    map->buckets = (Pair **)malloc(sizeof(Pair *) * map->capacity);
    if (map->buckets == NULL) return;

    // iniciamos los buckets en NULL
    for (long i = 0; i < map->capacity; i++) {
        map->buckets[i] = NULL;
    }
    // el tamaño lo dejamos en cero
    map->size = 0;

    // insertamos los antiguos pair en el mapa
    for (long i = 0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }
    // liberamos la memoria de los antiguos buckets
    free(old_buckets);
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
    // Dejamos declarada la capacidad, la cantidad de pair y el indice del ultimo pair accedido
    // es -1 porque no hay pair en el mapa
    mapa->capacity = capacity;
    mapa->size = 0;
    mapa->current = -1;
    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
    // Buscamos el Pair con la funcion searchMap
    Pair * eliminar = searchMap(map, key);

    // Solo si se encontro el Pair, se INVALIDA la clave y se actualiza size -1.
    if(eliminar != NULL){
        eliminar->key = NULL;
        map->size--;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    // Con funcion hash encontramos la posicion en donde debe estar el pair con esa clave
    unsigned long posicion = hash(key, map->capacity);

    // mientras el Pair que se accede no sea NULL,     
    while(map->buckets[posicion] != NULL){
        // Si la clave es distinta de NULL y a su vez la clave que esta en la posicion, es igual a clave
        if (map->buckets[posicion]->key != NULL && strcmp(map->buckets[posicion]->key, key) == 0){
            // Actualizamos al currente debido a que sera el ultimo pair al que accedemos y retornamos
            map->current = posicion;
            return map->buckets[posicion];
        }
        // Si no cumple la condicion, avanza de posicion
        posicion = (posicion + 1) % map->capacity;
    }
    // Si se sale del bucle retornara NULL, debido a que no lo encontro
    return NULL;
}

Pair * firstMap(HashMap * map) {
    // partimos desde posicion 0, y mientras posicion sea menor a la capacidad del mapa, preguntara:
    for(long posicion = 0; posicion < (map->capacity -1); posicion++){
        // Solo si el pair es distinto de NULL y su clave TAMBIEN...
        if(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
        {
            // ACTUALIZAMOS current debido a que accederemos a este pair y retornamos el pair
            map->current = posicion;
            return map->buckets[posicion];    
        }
    }
    // Si nunca cumple la condicion retornara NULL, osea si nunca encuentra un primer pair valido
    return NULL;
}

Pair * nextMap(HashMap * map) {
    // Para encontar el siguiente utilizaremos el current +1, de ahi partimos buscando, hasta capacidad -1
    for(long posicion = (map->current +1); posicion < (map->capacity -1); posicion++){
        // Solo si nos encontramos un pair que no es NULL y que su clave tampoco sea NULL
        if(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
        {
            // Actualizamos el current y retornamos este pair
            map->current = posicion;
            return map->buckets[posicion];
        }
    }
    // Si nunca cumple la condicion retornara NULL, osea que no hay un siguiente pair valido.
    return NULL;
}

// END