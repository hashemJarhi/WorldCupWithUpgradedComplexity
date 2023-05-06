#ifndef LIST_H_
#define LIST_H_

#include "PlayerData.h"


class Node {
public:
    Node *next;
    PlayerData* data;
    Node( Node *next, PlayerData* data);
    ~Node() {
       // delete data;
    };
};


class List {
public:
    Node *first;

    Node *last;

    bool deleteData;

    List();

    ~List();

    PlayerData** find(int id) const;

    void insert( PlayerData *player);

};


#endif //LIST_H_
