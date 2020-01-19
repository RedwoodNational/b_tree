#ifndef B_TREE_NODE_H
#define B_TREE_NODE_H
#include<cstddef>
class TTree;
class TNode{
    public:
        friend TTree;
        TNode();
        TNode (TNode& old_root);
        TNode(char const* word, unsigned long long key);
        void InsertLeaf(char const* word, unsigned long long key, size_t index);
        void RemoveLeaf(size_t index);
        void Split(size_t child_index);
        void Merge(size_t child_index);
        void TransferLeft(size_t child_index);
        void TransferRight(size_t child_index);
        bool StealLeft(size_t child_index, size_t min_count);
        bool StealRight(size_t child_index, size_t min_count);
        void Search(char const* word, bool &status, size_t& index);
        void Untie();
        TNode* Child(size_t index);
        char const* Word(size_t index);
        unsigned long long const * Key(size_t index);
        bool Leaf();
        size_t Weight();
        ~TNode();
        private:
            char** words;
            unsigned long long* keys;
            TNode** children;
            size_t weight;
            size_t capacity;
};
#endif