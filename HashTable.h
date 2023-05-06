
#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "List.h"
class PlayerData;

class HashTable {
    List* list;
    long long size;
    long long elementsNumber;
public:
    HashTable();
    ~HashTable();
    void insert( PlayerData* player);
    PlayerData** find(int id);
};

#endif //HASHTABLE_H_
