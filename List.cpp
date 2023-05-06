#include "List.h"


Node::Node(Node *next, PlayerData* data) : next(next), data(data){}

List::List() : first(nullptr),last(nullptr),deleteData(false) {}

List::~List()
{
    while (first)
    {
        Node *tmp = first;
        first = first->next;
        if(deleteData == true)
        {
           if(tmp->data) delete tmp->data;
        }
        delete tmp;
    }
    first = nullptr;
    last = nullptr;
}


PlayerData** List::find(int id) const
{
    if (this)
    {
        Node *tmp = first;
        while (tmp)
        {
            if (tmp->data->getPlayerId() == id)
            {
                return &tmp->data;
            }
            tmp = tmp->next;
        }
    }
    return nullptr;
}
void List::insert( PlayerData *player)
{
    Node *node = new Node( nullptr,player);
    if (!first)
    {
        first = node;
        last = node;
        return;
    }
    last->next = node;
    last = node;
}