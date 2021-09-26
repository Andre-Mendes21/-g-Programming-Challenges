#ifndef HASHST_H
#define HASHST_H

#include <stdbool.h>

typedef struct hash_entry_t
{
    const char *key;
    int *value;
} Entry;


typedef struct hashST_t
{
    Entry *entries;
    int prime_index;
    int p;
    int deleted;
    int size;
    int capacity;
    float load_factor;
    float deleted_factor;
} HashST;


HashST *hashST_create(void);

void hashST_destroy(HashST *st);

int *hashST_get(HashST *st, const char *key);

const char *hashST_put(HashST *st, const char *key, int *value);

void hashST_delete(HashST *st, const char *key);

int hashST_size(HashST *st);

typedef struct hashST_iter
{
    const char *key;
    int value;

    HashST *_st;
    int _index;
} HashST_iter;

HashST_iter hashST_iterator(HashST *st);

bool hashST_next(HashST_iter *iter);

#endif