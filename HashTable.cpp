
#include "HashTable.h"
#include "List.h"


HashTable::HashTable()
{
    list = new List[2];
    size = 2;
    elementsNumber = 0;
}

HashTable::~HashTable()
{
    for (int i = 0; i < size; ++i) {
        list[i].deleteData = true;
    }
    delete[] list;
}

PlayerData* *HashTable::find(int employee_id)
{
    return list[employee_id % size].find(employee_id);
}

void HashTable::insert(PlayerData *player)
{
    if (size == elementsNumber)
    {
        size *= 2;
        List* new_list = new List[size];
        List* tmp = list;
        for (int i = 0; i < (size/2); i++)
        {
            if (tmp[i].first)
            {
                Node* first = tmp[i].first;
                while (first)
                {
                    new_list[first->data->getPlayerId()%size].insert(first->data);
                    first = first->next;
                }
            }
        }
        list = new_list;
        delete[] tmp;
    }
    list[player->getPlayerId() % size].insert(player);
    elementsNumber++;
}
