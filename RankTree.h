
#ifndef WET2_RANKTREE_H
#define WET2_RANKTREE_H

template <class Key , class Data>
class nodeInRankTree
{
private:
public:

    int height;
    long long int sonsNumber;
    Key *key;
    Data *data;
    nodeInRankTree *leftSon;
    nodeInRankTree *rightSon;
    nodeInRankTree *father;

    nodeInRankTree(): height(0), sonsNumber(1), key(nullptr), data(nullptr), leftSon(nullptr), rightSon(nullptr), father(nullptr)
    {}

    nodeInRankTree(Key &new_key, Data &new_data, nodeInRankTree *father = nullptr)
            : height(0), sonsNumber(1), leftSon(nullptr), rightSon(nullptr), father(father)
    {
        this->key = new Key(new_key);
        this->data = new Data(new_data);
    }

    ~nodeInRankTree()
    {
        leftSon = nullptr;
        rightSon = nullptr;
        father = nullptr;
    }

    void updateHeight()
    {
        int right_height = (rightSon == nullptr ? -1 : this->rightSon->height);
        int left_height = (leftSon == nullptr ? -1 : leftSon->height);
        this->height = (right_height > left_height ? right_height : left_height) + 1;
    }
    long long int sonsNum(nodeInRankTree* node)
    {
        return node == NULL ? 0 : node->sonsNumber;
    }

    int getHeight(nodeInRankTree *node)
    {
        if(node == nullptr)
            return -1;
        node->updateHeight();
        return node->height;
    }

    int balanceFactor()
    {
        return ((getHeight(leftSon)) - (getHeight(rightSon)));
    }

    void swap(nodeInRankTree* to_swap)
    {
        auto tmp= to_swap->key;
        auto tmpData= to_swap->data;

        (to_swap->data) = (this->data);
        (to_swap->key) = (this->key);

        (this->data) = (tmpData);
        (this->key) = (tmp);

    }

};


template<class Key , class Data>
class RankTree
{
private:
    int size;
    nodeInRankTree<Key,Data> *root;
    nodeInRankTree<Key,Data> *max;
public:

    RankTree()
    {
        size = 0;
        root = nullptr;
        max= nullptr;
    }

    ~RankTree()
    {
        treeClear();
    }
    void insert(Key &new_key, Data &new_data)
    {
        nodeInRankTree<Key,Data> *itr = this->root;
        nodeInRankTree<Key,Data> *father = nullptr;

        while (itr != nullptr)
        {
            if(new_key == (*itr->key))
            {
                return;
            }
            father = itr;

            if(new_key > *(itr->key))
            {
                itr = itr->rightSon;
            }
            else
            {
                itr = itr->leftSon;
            }
        }

        nodeInRankTree<Key,Data> *new_node = new nodeInRankTree<Key,Data>(new_key, new_data, father);

        if (father == nullptr)
        {
            this->root = new_node;
        } else if(new_key > *(father->key))
        {
            father->rightSon = new_node;
        }
        else
        {
            father->leftSon = new_node;
        }

        nodeInRankTree<Key,Data> *itr2 = father;
        while (itr2 != nullptr)
        {
            itr2->sonsNumber = itr2->sonsNum(itr2->leftSon) + itr2->sonsNum(itr2->rightSon) + 1;
            itr2 = itr2->father;
        }


        fixTheTree(new_node);
        if (max == nullptr || new_key > (*max->key) )
        {
            max = new_node;
        }
        size = size + 1;
    }
    void remove(Key &key) {     nodeInRankTree<Key,Data> *to_delete = find(key);
        if(to_delete == nullptr)
        {
            return;
        }

        nodeInRankTree<Key,Data> *temp_father = to_delete->father;
        if(to_delete->rightSon && to_delete->leftSon)
        {
            nodeInRankTree<Key,Data> *itr = to_delete->rightSon;
            while(itr->leftSon != nullptr)
            {
                itr=itr->leftSon;
            }

            to_delete->swap(itr);
            to_delete=itr;

        }
        if (to_delete->rightSon)
        {
            to_delete->swap(to_delete->rightSon);
            to_delete = to_delete->rightSon;
            to_delete->father->rightSon = nullptr;

        }
        else if (to_delete->leftSon)
        {
            to_delete->swap(to_delete->leftSon);
            to_delete = to_delete->leftSon;
            to_delete->father->leftSon = nullptr;

        }

        temp_father = to_delete->father;
        to_delete->father= nullptr;

        if(temp_father)
        {
            if (temp_father->rightSon && temp_father->rightSon->key == to_delete->key)
            {
                temp_father->rightSon = nullptr;
            }

            if (temp_father->leftSon && *(temp_father->leftSon->key) == *(to_delete->key))
            {
                temp_father->leftSon = nullptr;
            }
        }

        else
        {
            root = nullptr;
            max = nullptr;
        }

        if (temp_father && (*to_delete->key) == (*max->key))
        {
            max = temp_father;
        }

        nodeInRankTree<Key,Data> *itr = temp_father;
        while (itr != nullptr)
        {
            itr->sonsNumber = itr->sonsNum(itr->leftSon) + itr->sonsNum(itr->rightSon) + 1;
            itr = itr->father;
        }
        delete to_delete->key;
        delete to_delete->data;
        delete to_delete;
        size = size - 1;
        fixTheTree(temp_father);
        return;
    }
    nodeInRankTree<Key,Data>* find(Key &key)
    {
        nodeInRankTree<Key,Data> *itr = this->root;
        while (itr != nullptr  )
        {
            if ((*itr->key) != key)
            {
                if (*(itr->key) > key)
                    itr = itr->leftSon;
                else
                    itr = itr->rightSon;
            } else
            {
                return itr;
            }
        }
        return nullptr;
    }

    nodeInRankTree<Key,Data>* getRoot()
    {
        return this->root;
    }
    nodeInRankTree<Key,Data>* getIndex(nodeInRankTree<Key,Data> *node , long long int rank);
    void updateRanksForTree(nodeInRankTree<Key,Data> *node);
    void rotateRightRight(nodeInRankTree<Key,Data> *node);
    void rotateLeftLeft(nodeInRankTree<Key,Data> *node);
    void rotateRightLeft(nodeInRankTree<Key,Data> *node);
    void rotateLeftRight(nodeInRankTree<Key,Data> *node);

    class Update
    {
    public:
        void operator()( nodeInRankTree<Key,Data> *node)
        {
            node->updateHeight();
        }
    };
    template <class Func>
    void postOrderSearch( nodeInRankTree<Key,Data> *node, Func function);


    void fixTheTree(nodeInRankTree<Key,Data> *node)
    {
        postOrderSearch(this->root , Update());
        nodeInRankTree<Key,Data>* itr= node;
        while (itr != nullptr)
        {
            if (itr->balanceFactor() > 1)
            {
                if (itr->leftSon->balanceFactor() >= 0)
                {
                    rotateLeftLeft(itr); //LL
                }
                else
                {
                    rotateLeftRight(itr); //LR
                }
            }
            else if (itr->balanceFactor() < -1)
            {
                if (itr->rightSon->balanceFactor() == 1)
                {
                    rotateRightLeft(itr); //RL
                } else
                {
                    rotateRightRight(itr); //RR
                }
            }
            itr->updateHeight();
            itr = itr->father;
        }
    }
    void treeClear()
    {
        if (this->root != nullptr)
        {
            remove(*this->root->key);
            treeClear();
        }
        max = nullptr;
    }
};

template<class Key, class Data>
nodeInRankTree<Key,Data> *RankTree<Key, Data>::getIndex(nodeInRankTree<Key,Data> *node , long long int rank)
{
    if(node == nullptr)
        return nullptr;
    if(node->sonsNum(node->leftSon) == rank - 1)
        return node;
    else if(node->sonsNum(node->leftSon) > rank - 1)
        return getIndex(node->leftSon, rank);
    else
        return getIndex(node->rightSon, rank - (node->sonsNum(node->leftSon)) - 1);
}

template<class Key,class Data>
void RankTree<Key,Data>::updateRanksForTree(nodeInRankTree<Key,Data> *node)
{
    if (node == nullptr)
        return;
    updateRanksForTree(node->leftSon);
    updateRanksForTree(node->rightSon);
    node->sonsNumber = node->sonsNum(node->leftSon) + node->sonsNum(node->rightSon) + 1;
    node->updateHeight();
}

template<class Key , class Data>
void RankTree<Key,Data>::rotateLeftLeft(nodeInRankTree<Key,Data> *node)
{
    nodeInRankTree<Key,Data> *temp_root = node->leftSon;

    node->leftSon = temp_root->rightSon;
    if(node->leftSon)
    {
        node->leftSon->father = node;
    }

    node->sonsNumber = node->sonsNum(node->leftSon) + node->sonsNum(node->rightSon) + 1;

    // set the original node as left son of the temp/new root
    temp_root->father = node->father;
    node->father = temp_root;
    temp_root->rightSon = node;

    // set the original root father as the new root father

    if(temp_root->father)
    {
        if(temp_root->father->rightSon == node){
            temp_root->father->rightSon = temp_root;
        } else
        {
            temp_root->father->leftSon = temp_root;
        }
    } else
    {
        this->root = temp_root;
    }
    node->updateHeight();
    temp_root->updateHeight();

    if(temp_root->father)
    {
        temp_root->updateHeight();
    }
    temp_root->sonsNumber = temp_root->sonsNum(temp_root->leftSon) + temp_root->sonsNum(temp_root->rightSon) + 1;

}

template<class Key , class Data>
void RankTree<Key,Data>::rotateRightRight(nodeInRankTree<Key,Data> *node)
{
    nodeInRankTree<Key,Data> *temp_root = node->rightSon;

    // set original node/root sons... (set its right son)
    node->rightSon = temp_root->leftSon;
    if(node->rightSon)
    {
        node->rightSon->father = node;
    }

    node->sonsNumber = node->sonsNum(node->leftSon) + node->sonsNum(node->rightSon) + 1;

    // set the original node as left son of the temp/new root
    temp_root->father = node->father;
    node->father = temp_root;
    temp_root->leftSon = node;

    // set the original root father as the new root father

    if(temp_root->father)
    {
        if(temp_root->father->leftSon == node){
            temp_root->father->leftSon = temp_root;
        } else
        {
            temp_root->father->rightSon = temp_root;
        }
    } else
    {
        this->root = temp_root;
    }

    node->updateHeight();
    temp_root->updateHeight();

    if(temp_root->father)
    {
        temp_root->updateHeight();
    }
    temp_root->sonsNumber = temp_root->sonsNum(temp_root->leftSon) + temp_root->sonsNum(temp_root->rightSon) + 1;
}

template<class Key , class Data>
void RankTree<Key,Data>::rotateRightLeft(nodeInRankTree<Key,Data> *node)
{
    rotateLeftLeft(node->rightSon);
    return rotateRightRight(node);
}

template<class Key , class Data>
void RankTree<Key,Data>::rotateLeftRight(nodeInRankTree<Key,Data> *node)
{
    rotateRightRight(node->leftSon);
    return rotateLeftLeft(node);
}


template<class Key,class Data>
template<class Func>
void RankTree<Key,Data>::postOrderSearch( nodeInRankTree<Key,Data> *node, Func function)
{
    if (node == nullptr)
        return;
    postOrderSearch(node->leftSon, function);
    postOrderSearch(node->rightSon, function);
    function(node);
}


#endif //WET2_RANKTREE_H