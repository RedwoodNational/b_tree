#include<string.h>
#include"b_tree_node.h"
TNode::TNode(){
}

TNode::TNode (TNode& old_root){
    weight = 1;
    capacity = 1;
    words = new char* [capacity];
    keys = new unsigned long long [capacity];
    children = new TNode* [capacity+1];
    const auto median = old_root.weight>>1;
    words[0] = old_root.words[median];
    keys[0] = old_root.keys[median];
    old_root.weight = median; 
    children[0] = &old_root;
    children[1] = new TNode;
    children[1]->weight = median;
    children[1]->capacity = median;
    children[1]->words = new char* [median];
    children[1]->keys = new unsigned long long [median];
    for(size_t i = 0; i < median; i++){
        children[1]->words[i] = old_root.words[i + median + 1];
        children[1]->keys[i] = old_root.keys[i + median + 1];
    }
    if(old_root.children){
        children[1]->children=new TNode* [median + 1];
        for(size_t i = 0; i <= median; i++){
            children[1]->children[i] = old_root.children[i + median + 1];
        }
    }else{
        children[1]->children = nullptr;
    }

}

TNode::TNode(char const* word, unsigned long long key){
    weight = 1;
    capacity = 1;
    words=new char* [capacity];
    words[0]=new char[strlen(word) + 1];
    strcpy(words[0], word);
    keys=new unsigned long long [capacity];
    keys[0] = key;
    children = nullptr;
}

void TNode::InsertLeaf(char const* word, unsigned long long key, size_t index){
    if(weight >= capacity){
        capacity = capacity<<1;
        char** extended_words=new char* [capacity];
        for(size_t i = 0; i < index; i++){
            extended_words[i] = words[i];
        }
        for(size_t i = index; i < weight; i++){
            extended_words[i + 1] = words[i];
        }
        delete[] words;
        words=extended_words;
        unsigned long long* extended_keys=new unsigned long long [capacity];
        for(size_t i = 0; i < index; i++){
            extended_keys[i] = keys[i];
        }
        for(size_t i = index; i < weight; i++){
            extended_keys[i + 1] = keys[i];
        }
        delete[] keys;
        keys = extended_keys;
    }else{
        for(size_t i = weight; i > index; i--){
            keys[i] = keys[i - 1];
            words[i] = words[i - 1];
        }
    }
    ++weight;
    words[index]=new char[strlen(word) + 1];
    strcpy(words[index], word);
    keys[index] = key;
}

void TNode::RemoveLeaf(size_t index){
    delete[] words[index];
    for(size_t i = index + 1; i < weight; ++i){
        words[i - 1]=words[i];
        keys[i - 1]=keys[i];
    }
    --weight;
}

void TNode::Split(size_t child_index){
    if(weight >= capacity){
        capacity = capacity<<1;
        unsigned long long* extended_keys=new unsigned long long [capacity];
        for(size_t i = 0; i < child_index; i++){
            extended_keys[i] = keys[i];
        }
        for(size_t i = child_index; i < weight; i++){
            extended_keys[i + 1] = keys[i];
        }
        delete[] keys;
        keys=extended_keys;
        char** extended_words=new char* [capacity];
        for(size_t i = 0; i < child_index; i++){
            extended_words[i] = words[i];
        }
        for(size_t i = child_index; i < weight; i++){
            extended_words[i + 1] = words[i];
        }
        delete[] words;
        words = extended_words;
        TNode** extended_children = new TNode* [capacity + 1];
        for(size_t i = 0; i <= child_index; i++){
            extended_children[i] = children[i];
        }
        for(size_t i = child_index + 1; i <= weight; i++){
            extended_children[i + 1] = children[i];
        }
        delete[] children;
        children=extended_children;           
    }else{
        for(size_t i = weight; i > child_index; i--){
            keys[i]=keys[i - 1];
            words[i]=words[i - 1];
        }
        for(size_t i=weight + 1; i > child_index + 1; i--){
            children[i]=children[i - 1];
        }
    }
    ++weight;
    const auto median(children[child_index]->weight>>1);
    keys[child_index] = children[child_index]->keys[median];
    words[child_index] = children[child_index]->words[median];
    children[child_index + 1] = new TNode;
    children[child_index + 1]->weight = median;
    children[child_index + 1]->capacity = median;
    children[child_index + 1]->words = new char*[median];
    children[child_index + 1]->keys = new unsigned long long[median];
    for(size_t i = 0; i < median; i++){
        children[child_index + 1]->keys[i]=children[child_index]->keys[i + median + 1];
        children[child_index + 1]->words[i]=children[child_index]->words[i + median + 1];
    }
    children[child_index]->weight = median;
    if(children[child_index]->children){
        children[child_index + 1]->children=new TNode*[median + 1];
        for(size_t i = 0; i <= median; i++){
            children[child_index + 1]->children[i]=children[child_index]->children[i + median + 1];
        }
    }else{
        children[child_index + 1]->children=nullptr;
    }
}

void TNode::Merge(size_t child_index){
    bool synchronized = true;
    auto left_child = children[child_index];
    auto right_child = children[child_index + 1];
    auto new_weight = left_child->weight + right_child->weight + 1;
    if(left_child->capacity < new_weight){
        synchronized = false;
        left_child->capacity = new_weight;
        char** extended_words = new char* [left_child->capacity];
        for(size_t i = 0; i < left_child->weight; i++){
            extended_words[i] = left_child->words[i];
        }
        delete [] left_child->words;
        left_child->words = extended_words;
        unsigned long long* extended_keys = new unsigned long long [left_child->capacity];
        for(size_t i = 0; i < left_child->weight; i++){
            extended_keys[i] = left_child->keys[i];
        }
        delete [] left_child->keys;
        left_child->keys = extended_keys;                
    }
    left_child->words[left_child->weight] = words[child_index];
    left_child->keys[left_child->weight] = keys[child_index];
    for(size_t i = 0; i < right_child->weight; i++){
        left_child->keys[i + left_child->weight + 1]=right_child->keys[i];
        left_child->words[i + left_child->weight + 1]=right_child->words[i];
    }
    if(left_child->children){
        if(!synchronized){
            TNode** extended_children=new TNode* [left_child->capacity + 1];
            for(size_t i = 0; i <= left_child->weight; i++){
                extended_children[i] = left_child->children[i];
            }
            delete [] left_child->children;
            left_child->children = extended_children;
        }
        for(size_t i = 0; i <= right_child->weight; i++){
            left_child->children[i + left_child->weight + 1] = right_child->children[i];
        }
        delete [] right_child->children;
        right_child->children = nullptr;
    }
    right_child->weight = 0;
    delete right_child;
    left_child->weight = new_weight;
    weight--;
    for(size_t i = child_index; i < weight; i++){
        words[i]=words[i + 1];
        keys[i]=keys[i + 1];
    }
    for(size_t i = child_index + 1; i <= weight; i++){
        children[i]=children[i + 1];
    }                    
}

void TNode::TransferLeft(size_t child_index){
    bool synchronized = true;
    auto left_child = children[child_index];
    auto right_child = children[child_index+1];
    if(left_child->weight >= left_child->capacity){
        synchronized = false;
        left_child->capacity = left_child->capacity<<1;
        char** extended_words = new char* [left_child->capacity];
        for(size_t i = 0; i < left_child->weight; i++){
            extended_words[i] = left_child->words[i];
        }
        delete [] left_child->words;
        left_child->words = extended_words;
        unsigned long long* extended_keys = new unsigned long long [left_child->capacity];
        for(size_t i = 0; i < left_child->weight; i++){
            extended_keys[i] = left_child->keys[i];
        }
        delete [] left_child->keys;
        left_child->keys = extended_keys;
    }
    left_child->keys[left_child->weight] = keys[child_index];
    left_child->words[left_child->weight] = words[child_index];
    keys[child_index] = right_child->keys[0];
    words[child_index] = right_child->words[0];
    for(size_t i = 1; i < right_child->weight; i++){
        right_child->keys[i - 1] = right_child->keys[i];
        right_child->words[i - 1] = right_child->words[i];
    }
    if(left_child->children){
        if(!synchronized){
            TNode** extended_children = new TNode* [left_child->capacity + 1];
            for(size_t i = 0; i <= left_child->weight; i++){
                extended_children[i] = left_child->children[i];
            }
            delete [] left_child->children;
            left_child->children = extended_children;
        }
        left_child->children[left_child->weight+1] = right_child->children[0];
        for(size_t i = 1; i <= right_child->weight; i++){
            right_child->children[i - 1] = right_child->children[i];
        }
    }
    ++left_child->weight;
    --right_child->weight;
}

void TNode::TransferRight(size_t child_index){
    bool synchronized = true;
    auto right_child = children[child_index];
    auto left_child = children[child_index - 1];
    if(right_child->weight >= right_child->capacity){
        synchronized = false;
        right_child->capacity = right_child->capacity<<1;
        char** extended_words = new char* [right_child->capacity];
        for(size_t i = 0; i < right_child->weight; i++){
            extended_words[i + 1] = right_child->words[i];
        }
        delete [] right_child->words;
        right_child->words = extended_words;
        unsigned long long* extended_keys = new unsigned long long [right_child->capacity];
        for(size_t i = 0; i < right_child->weight; i++){
            extended_keys[i + 1]=right_child->keys[i];
        }
        delete [] right_child->keys;
        right_child->keys = extended_keys;
    }else{
        for(size_t i = right_child->weight; i > 0; i--){
            right_child->keys[i]=right_child->keys[i - 1];
            right_child->words[i]=right_child->words[i - 1];
        }
    }
    right_child->words[0] = words[child_index - 1];
    right_child->keys[0] = keys[child_index - 1];
    words[child_index - 1] = left_child->words[left_child->weight - 1];
    keys[child_index - 1] = left_child->keys[left_child->weight - 1];
    if(right_child->children){                
        if(!synchronized){
            TNode** extended_children = new TNode* [right_child->capacity + 1];
            for(size_t i = 0; i <= right_child->weight; i++){
                extended_children[i + 1] = right_child->children[i];
            }
            delete [] right_child->children;
            right_child->children = extended_children;
        }else{
            for(size_t i = right_child->weight + 1; i > 0; i--){
                right_child->children[i] = right_child->children[i - 1];
            }
        }
        right_child->children[0] = left_child->children[left_child->weight];
    }
    ++right_child->weight;
    --left_child->weight;
}

bool TNode::StealLeft(size_t child_index, size_t min_count){
    return (child_index > 0 && children[child_index - 1]->weight >= min_count); 
}

bool TNode::StealRight(size_t child_index, size_t min_count){
    return (child_index < weight && children[child_index + 1]->weight >= min_count);
}

void TNode::Search(char const* word, bool &status, size_t& index){
    char** front = words;
    char** back = words + weight - 1;
    if(!words || strcasecmp(word, *front) < 0){
        index = 0;
        status = false;
        return;
    }else if(strcasecmp(word, *back) > 0){
        index = weight;
        status = false;
        return;
    }
    while(front < back){
        char** middle = front + ((back - front)>>1);
        if(strcasecmp(word, *middle) <= 0){
            back = middle;
        }else{
            front = middle + 1;
        }
    }
    if(strcasecmp(word, *back) != 0){
        status = false;
    }else{
        status = true;
    }
    index = back - words;
}

void TNode::Untie(){
    delete [] children;
    children = nullptr;
}

TNode* TNode::Child(size_t index){
    return children[index];
}

char const* TNode::Word(size_t index){
    return words[index];
}

unsigned long long const * TNode::Key(size_t index){
    return keys + index;
}

bool TNode::Leaf(){
    return !children;
}

size_t TNode::Weight(){
    return weight;
}

TNode::~TNode(){
    delete[] keys;
    for(size_t i = 0; i < weight; i++){
        delete[] words[i];
    }
    delete[] words;
    if(children){
        for(size_t i = 0; i <= weight; i++){
            delete children[i];
        }
        delete[] children;
    }
}