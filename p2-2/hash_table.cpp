/*
 Student Name: James Lehman
 Date: 9/9/18

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "MapItem"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_MapItem"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _MapItem". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "MapItem myNewMapItem;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"
#include "map.h"

/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
  /** The array of pointers to the head of a singly linked list, whose nodes
      are MapItem objects */
  MapItem **buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};


void *value;
void* tempVal;


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createMapItem
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/

//node = hashTable -> buckets[hashTable -> hash(key)]

static MapItem *createMapItem(HashTable *hashTable, int index, unsigned int key, void* value)
{
  MapItem *this_node = hashTable -> buckets[index];
  if(this_node != NULL) {
    if(this_node != NULL && this_node -> next != NULL) {
      this_node = this_node -> next;
    }
    MapItem *next_node = (MapItem *)malloc(sizeof(MapItem));
    this_node -> next = next_node;
    
    //next_node -> data = value;
    next_node -> key = key; // stores the values
    next_node -> next = NULL;
    return(next_node);

  } else {
    MapItem *next_node = (MapItem *)malloc(sizeof(MapItem));

    //next_node -> data = value;
    next_node -> key = key; // stores the values
    next_node -> next = NULL;
    return(next_node);
  }
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/

static MapItem *findItem(HashTable *hashTable, unsigned int key)
{
  int index = hashTable -> hash(key);
  MapItem *this_node = hashTable -> buckets[index];
  
  while(this_node != NULL && this_node -> key != key) {
    this_node = this_node -> next;
  }
  return(this_node);
}

// Freedom - frees the correct node and corrects the next pointers 
static void *freedom(MapItem *this_node, HashTable *hashTable, int index) 
{
  //value = this_node -> value;
  MapItem* temp = this_node -> next;

  MapItem *check_node = hashTable -> buckets[index]; // Create new node to index with bucket with
  while(check_node != NULL && check_node != this_node && check_node -> next != this_node) {
    check_node = check_node -> next;
  }

  if (check_node == this_node) {
    hashTable->buckets[index] = this_node->next;
    printf("removing head thing\n");
  } else if (this_node -> next != NULL) {
    check_node -> next = check_node -> next -> next;
  }
  
  free(this_node);
  
  this_node = temp;
  return(value);
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets == 0)
  {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (MapItem **)malloc(numBuckets * sizeof(MapItem *));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i = 0; i < numBuckets; ++i)
  {
    newTable->buckets[i] = NULL;
  }
  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
  int count = 0;
  MapItem *this_node = hashTable -> buckets[count];
  
  while(count <= hashTable -> num_buckets){
    while (this_node != NULL) {
      //printf("going to free now\n");
      MapItem *temp = this_node->next;
      freedom(this_node, hashTable, count);
      this_node = temp;
    }
    count++;
  }
}

void* insertItem(HashTable *hashTable, unsigned int key, MapItem *value)
{
  MapItem *this_node = findItem(hashTable, key);
  int index = hashTable -> hash(key);

  if (this_node != NULL) {
    tempVal = this_node -> next;
    this_node -> next = value;
    return (tempVal);
  } else {
    //printf("Node does not exist for given key, creating node.\n");
    
    hashTable -> buckets[index] = createMapItem(hashTable, index, key, value);

    return(0);
  }
}

int getItem(HashTable *hashTable, unsigned int key)
{
  MapItem *this_node = findItem(hashTable, key);

  if(this_node != NULL && this_node -> key == key) {
    return(this_node -> data);
  } else {
    //printf("Node does not exist for given key, returned NULL.\n");
    return(0);
  }
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
  MapItem *this_node = findItem(hashTable, key);

  if(this_node != NULL) {
    int index = hashTable -> hash(key);
    value = freedom(this_node, hashTable, index);
    return(value);
  } else {
    //printf("Node does not exist for given key, nothing deleted/returned.\n");
    return(0);
  }

}

void deleteItem(HashTable *hashTable, unsigned int key)
{
  MapItem *this_node = findItem(hashTable, key);
  
  if(this_node != NULL) {
    int index = hashTable -> hash(key);
    freedom(this_node, hashTable, index);
  }
}
