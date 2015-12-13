/*
   This code implements a basic string to struct class.
   Right now it is implemented by a simple double linked list.

   @file map.c
   @brief This code implements a map class
   @author Matthieu Speder
*/

#include <stdlib.h>
#include <string.h>
#include "map.h"

stringMap* stringMap_new(stringMapDestructor* destructor)
{
  stringMap* map = malloc(sizeof(stringMap));
  map->destructor = destructor;
  map->first = NULL;
  map->last = NULL;
  return map;
}

void stringMap_delete(stringMap* map)
{
  stringMap_clear(map);
  free(map);
}

void stringMap_clear(stringMap* map)
{
  stringMapItem* it = map->first;
  while (it != NULL)
  {
    stringMapItem* nxt = it->next;
    map->destructor(it);
    free(it->key);
    free(it);
    it = nxt;
  }
  map->first = NULL;
  map->last = NULL;
}

void stringMap_put(stringMap* map, const char* key, void* item)
{
  if (key == NULL || item == NULL) return;
  stringMap_erase(map, key);
  stringMapItem* it = (stringMapItem*)item;
  it->key = strdup(key);
  it->prev = map->last;
  it->next = NULL;
  map->last = it;
  if (it->prev == NULL)
    map->first = it;
  else
    it->prev->next = it;
}

void* stringMap_get(stringMap* map, const char* key)
{
  stringMapItem* it = map->first;
  while (it != NULL)
  {
    if (strcmp(key, it->key) == 0)
      return it;
    it = it->next;
  }
  return NULL;
}

int stringMap_erase(stringMap* map, const char* key)
{
  stringMapItem* it = stringMap_get(map, key);
  if (it == NULL) return -1;
  if (it->prev == NULL)
    map->first = it->next;
  else
    it->prev->next = it->next;
  if (it->next == NULL)
    map->last = it->prev;
  else
    it->next->prev = it->prev;
  map->destructor(it);
  free(it->key);
  free(it);
  return 0;
}

void stringMap_iterate(stringMap* map, stringMapIterator iter, void* param)
{
  stringMapItem* it = map->first;
  while (it != NULL)
  {
    stringMapItem* nxt = it->next;
    iter(it->key, it, param);
    it = nxt;
  }
}
