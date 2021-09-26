#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "HashST.h"

#define LOAD_LIMIT 0.5f
#define MIN_LOAD_LIMIT 0.125f
#define DELETED_LIMIT 0.2f
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static const int primes[] = {
            17, 37, 79, 163, 331,
            673, 1361, 2729, 5471, 10949,
            21911, 43853, 87719, 175447, 350899,
            701819, 1403641, 2807303, 5614657,
            11229331, 22458671, 44917381, 89834777, 179669557
    };

static const int prime_len = 24;


static HashST *hashST_init(int prime_index)
{
    HashST *st = malloc(sizeof(HashST));

    if(st == NULL)
    {
        return NULL;
    }

    st->prime_index = prime_index;
    st->capacity = primes[prime_index];
    st->size = 0;
    st->deleted = 0;
    st->load_factor = 0.0f;
    st->deleted_factor = 0.0f;
    st->p = primes[prime_index - 1];
    
    st->entries = calloc(st->capacity, sizeof(Entry));
    if(st->entries == NULL)
    {
        free(st);
        return NULL;
    }

    return st;
}

HashST *hashST_create(void)
{
    return hashST_init(1);
}

void hashST_destroy(HashST *st)
{
    for(int i = 0; i < st->capacity; i++)
    {
        if(st->entries[i].key != NULL)
        {
            free((void *) st->entries[i].key);
        }
    }

    free(st->entries);
    free(st);
}

static uint64_t hash_code(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for(const char *p = key; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

static uint64_t hash(int capacity, const char *key)
{
    return hash_code(key) % capacity;
}

static uint64_t double_hash(int p, const char *key)
{
    return 1 + (hash_code(key) % p);
}

int *hashST_get(HashST *st, const char *key)
{
    uint64_t hash1 = hash(st->capacity, key);
    uint64_t hash2 = double_hash(st->p, key);

    for(uint64_t i = hash1; st->entries[i].key != NULL; i = (i + hash2) % st->capacity)
    {
        if(strcmp(key, st->entries[i].key) == 0)
        {
            return st->entries[i].value;
        }
    }

    return NULL;
}

static bool resize(HashST *st, int prime_index)
{
    if(prime_index < 1 || prime_index >= prime_len)
    {
        return false;
    }

    st->p = st->prime_index;
    st->prime_index = prime_index;

    HashST *aux = hashST_init(st->prime_index);

    for(int i = 0; i < st->capacity; i++)
    {
        if(st->entries[i].value != NULL)
        {
            hashST_put(aux, st->entries[i].key, st->entries[i].value);
        }
    }

    st->entries = aux->entries;
    st->capacity = aux->capacity;
    st->p = aux->p;
    st->size = aux->size;
    st->deleted = aux->deleted;
    st->load_factor = (float) aux->size / aux->capacity;
    st->deleted_factor = (float) aux->deleted / aux->capacity;

    hashST_destroy(aux);

    return true;
}

const char *hashST_put(HashST *st, const char *key, int *value)
{
    if(value == NULL)
    {
        return NULL;
    }

    if(st->load_factor >= LOAD_LIMIT)
    {
        resize(st, st->prime_index + 1);
    }

    else if(st->deleted_factor > DELETED_LIMIT)
    {
        resize(st, st->prime_index);
    }

    uint64_t i = hash(st->capacity, key);
    uint64_t hash2 = double_hash(st->p, key);

    for(; st->entries[i].key != NULL; i = (i + hash2) % st->capacity)
    {
        if(strcmp(key, st->entries[i].key) == 0)
        {
            if(st->entries[i].value == NULL)
            {
                *st->entries[i].value = *value;
                st->deleted--;
                st->size++;
                st->load_factor = (float) st->size / st->capacity;
                st->deleted_factor = (float) st->deleted / st->capacity;
                return st->entries[i].key;
            }

            else
            {
                *st->entries[i].value = *value;
                return st->entries[i].key;
            }
        }
    }

    st->entries[i].key = key;
    *st->entries[i].value = *value;
    st->size++;
    st->load_factor = (float) st->size / st->capacity;
    st->deleted_factor = (float) st->deleted / st->capacity;

    return key;
}

void hashST_delete(HashST *st, const char *key)
{
    uint64_t i = hash(st->capacity, key);
    uint64_t hash2 = double_hash(st->p, key);

    while(true)
    {
        if(st->entries[i].key == NULL)
        {
            return;
        }

        if(strcmp(st->entries[i].key, key) && st->entries[i].value != NULL)
        {
            break;
        }
        i = (i + hash2) % st->capacity;
    }

    st->entries[i].value = NULL;
    st->size--;
    st->deleted++;

    st->load_factor = (float) st->size / st->capacity;
    st->deleted_factor = (float) st->deleted / st->capacity;

    if(st->load_factor < MIN_LOAD_LIMIT)
    {
        resize(st, st->prime_index - 1);
    }

    else if(st->deleted_factor > DELETED_LIMIT)
    {
        resize(st, st->prime_index);
    }
}

int hashST_size(HashST *st)
{
    return st->size;
}

HashST_iter hashST_iterator(HashST *st)
{
    HashST_iter iter;
    iter._st = st;
    iter._index = 0;

    return iter;
}

bool hashST_next(HashST_iter *iter)
{
    HashST *st = iter->_st;

    while(iter->_index < st->capacity)
    {
        int i = iter->_index;
        iter->_index++;

        if(st->entries[i].key != NULL)
        {
            Entry entry = st->entries[i];
            iter->key = entry.key;
            iter->value = entry.value;
            return true;
        }
    }

    return false;
}