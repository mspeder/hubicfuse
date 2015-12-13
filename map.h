#ifndef _MAP_H
#define _MAP_H

typedef void stringMapDestructor(void* item);
typedef void stringMapIterator(const char* key, void* item, void* param);

typedef struct stringMapItem
{
  char* key;
  struct stringMapItem* next;
  struct stringMapItem* prev;
} stringMapItem;

typedef struct stringMap
{
  stringMapDestructor* destructor;
  struct stringMapItem* first;
  struct stringMapItem* last;
} stringMap;

stringMap* stringMap_new(stringMapDestructor* destructor);
void stringMap_delete(stringMap* map);
void stringMap_clear(stringMap* map);
void stringMap_put(stringMap* map, const char* key, void* item);
void* stringMap_get(stringMap* map, const char* key);
int stringMap_erase(stringMap* map, const char* key);
void stringMap_iterate(stringMap* map, stringMapIterator iter, void* param);

#endif
