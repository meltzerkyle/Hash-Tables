#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = key;
  pair->value = value;
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
    free(pair);
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair *linked_pair = create_pair(key, value);

  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] != NULL)
  {
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      ht->storage[index] = linked_pair;
    }
    else
    {
      LinkedPair *stored_pair = ht->storage[index];
      while (stored_pair->next != NULL)
      {
        if (strcmp(stored_pair->next->key, key) == 0)
        {
          linked_pair->next = stored_pair->next->next;
          destroy_pair(stored_pair->next);
          stored_pair->next = linked_pair;
          return;
        }
        stored_pair = stored_pair->next;
      }
      stored_pair->next = linked_pair;
    }
  }
  else
  {
    ht->storage[index] = linked_pair;
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  //store the parent bc you need to delete it
  //linkedpairparent->next = linkedpair->next
  //destroypair(linkedpair)
  LinkedPair *stored_pair = ht->storage[index];
  if (stored_pair == NULL)
  {
    printf("Warning: no item with that key exists!");
  }

  if (strcmp(stored_pair->key, key) == 0)
  {
    ht->storage[index] = stored_pair->next;
    destroy_pair(stored_pair);
  }
  else
  {
    while (stored_pair->next != NULL)
    {
      if (strcmp(stored_pair->next->key, key) == 0)
      {
        stored_pair->next = stored_pair->next->next;
        destroy_pair(stored_pair->next);
        return;
      }
      stored_pair = stored_pair->next;
    }
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] != NULL)
  {
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      return ht->storage[index]->value;
    }
    else
    {
      LinkedPair *stored_pair = ht->storage[index];
      while (stored_pair->next != NULL)
      {
        if (strcmp(stored_pair->next->key, key) == 0)
        {
          return stored_pair->next->value;
        }
        stored_pair = stored_pair->next;
      }
    }
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity - 1; i++)
  {
    LinkedPair *stored_pair = ht->storage[i];
    while (stored_pair != NULL)
    {
      LinkedPair *temp = stored_pair->next;
      destroy_pair(temp);
      stored_pair = temp;
    }
  }
  free(ht->storage);
  free(ht);
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);
  for (int i = 0; i < ht->capacity - 1; i++)
  {
    LinkedPair *linked_pair = ht->storage[i];
    while (linked_pair != NULL)
    {
      hash_table_insert(new_ht, linked_pair->key, linked_pair->value);
      linked_pair = linked_pair->next;
    }
  }
  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
