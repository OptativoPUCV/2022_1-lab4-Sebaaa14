#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

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
  long pos =hash(key,map->capacity);

  while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL){
    if(is_equal(map->buckets[pos]->key, key) == 1) break; 
    pos = ((pos+1) % map->capacity); //para ir avanzando
  }
  
  if (map->buckets[pos] != NULL){
    map->buckets[pos]->key = key;
    map->buckets[pos]->value = value;
  }
  else map->buckets[pos] =createPair(key,value);

  map->size ++;
  
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  //////////////////////////////////////////// 
  
  //Lo hice con lo que explico la clase, pro 
  int auxCapacidad = map->capacity;
  Pair ** auxArreglo = map->buckets;
  map -> capacity *= 2;
  map->buckets=(Pair**) malloc (sizeof(Pair*)*map->capacity);
  map->size=0;

  for (int i=0; i < auxCapacidad; i++){
    if (auxArreglo[i]!=NULL){
      insertMap(map,auxArreglo[i]->key,auxArreglo[i]->value);
    }
  }
  free (auxArreglo);
}


HashMap * createMap(long capacity) {
  HashMap * miau = (HashMap*) malloc (sizeof(HashMap));
  miau->buckets= (Pair**) calloc (capacity,sizeof(Pair*));
  miau->size = 0 ;
  miau->capacity = capacity;
  miau->current = -1 ;
  return miau;
}

void eraseMap(HashMap * map,  char * key) {    
  long pos =hash(key,map->capacity);
  
  while (map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key,key) == 0){
    pos = ((pos+1)%(map->capacity));
  }
  if (map->buckets[pos] == NULL) return;
  map->buckets[pos]->key = NULL;
  map->size--;
}

Pair * searchMap(HashMap * map,  char * key) {   
  long pos = hash(key,map->capacity);
  
  while (map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key,key) == 0){
    pos = ((pos+1)%(map->capacity)); //Para avanzar de posicion como un nextList
     if(map->buckets[pos]==NULL || map->buckets[pos]->value == NULL) return NULL;
  }
  
  map->current=pos;
  
  return map->buckets[pos];
}

Pair * firstMap(HashMap * map) {
  int i = 0;

  while (i < map->capacity){
   if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
    map->current = i;
    return map->buckets[i];
    } 
    i++;
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for (int i = (map->current + 1); i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      map->current = i;
      return map->buckets[i];
   }
  }
  return NULL;
}
