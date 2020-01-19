#ifndef B_TREE_H
#define B_TREE_H
#include"b_tree_node.h"
class TTree{
    public:
        TTree(size_t branching_factor);
        bool Remove(char const* word);
        unsigned long long const* Search(const char* word);
        bool Insert(char const* word, unsigned long long key);
        bool Save(char const* filename);
        bool Load(char const* filename);
        ~TTree();
    private:
        size_t t;
        TNode* root;
};
#endif