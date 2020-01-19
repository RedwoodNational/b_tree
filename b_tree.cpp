#include<cstddef>
#include<fstream>
#include<string.h>
#include"stack.h"
#include"queue.h"
#include"b_tree.h"
TTree::TTree(size_t branching_factor){
    root = nullptr;
    t = branching_factor;
}

bool TTree::Remove(char const* word){
    bool status;
    size_t index;
    TNode* location = root;
    if(!root){
        return false;
    }
    if(root->Weight() == 1){
        if(root->Leaf()){
            root->Search(word, status, index);
            if(status){
                delete root;
                root = nullptr;
                return true;
            }
            return false;
        }
        if(root->Child(0)->Weight() < t && root->Child(1)->Weight() < t){
            root->Merge(0);
            location = root->Child(0);
            root->Untie();
            delete root;
            root = location;
        }
    }
    while(true){
        location->Search(word, status, index);
        if(status){
            if(location->Leaf()){
                location->RemoveLeaf(index);
            }else{
                size_t s_index;
                TNode* s_location;;
                while(true){
                    ++index;
                    if(location->Child(index)->Weight() >= t){
                        s_index = index - 1;
                        s_location = location;
                        location = location->Child(index);
                        break;
                    }else if(location->StealRight(index, t)){
                        s_index = index - 1;
                        s_location = location;
                        location->TransferLeft(index);
                        location = location->Child(index);
                        break;
                    }else if(location->StealLeft(index, t)){
                        location->TransferRight(index);
                        location = location->Child(index);
                        index = 0;               
                    }else if(index == location->Weight()){
                        location->Merge(index - 1);
                        location=location->Child(index - 1);
                        index = t - 1;
                    }else{
                        s_index = index - 1;
                        s_location = location;
                        location->Merge(index);
                        location = location->Child(index);
                        break;
                    }
                    if(location->Leaf()){
                        location->RemoveLeaf(index);
                        return true;
                    }
                }
                while(!location->Leaf()){
                    if(location->Child(0)->Weight() < t){
                        if(location->StealRight(0, t)){
                            location->TransferLeft(0);                 
                        }else{
                            location->Merge(0);
                        }
                    }                   
                    location = location->Child(0); 
                }
                std::swap(location->words[0], s_location->words[s_index]);
                std::swap(location->keys[0], s_location->keys[s_index]);
                location->RemoveLeaf(0);
            }
            return true;
        }
        if(location->Leaf()){
            return false;
        }
        if(location->Child(index)->Weight() < t){
            if(location->StealLeft(index, t)){
                location->TransferRight(index);
                location = location->Child(index);
                continue;                    
            }
            if(location->StealRight(index, t)){
                location->TransferLeft(index);
                location = location->Child(index);
                continue;
            }
            if(index == location->Weight()){
                --index;
            }
            location->Merge(index);
        }                   
        location = location->Child(index);  
    }
}

unsigned long long const* TTree::Search(const char* word){
    auto location = root;
    while(root){
        bool status;
        size_t index;
        location->Search(word, status, index);
        if(status){
            return location->Key(index);
        }
        if(location->Leaf()){
            break;
        }
        location = location->Child(index);
    }
    return nullptr;
}

bool TTree::Insert(char const* word, unsigned long long key){
    if(root == nullptr){
        root=new TNode (word, key);
        return true;
    }

    if(root->Weight() == 2 * t - 1){
        root=new TNode (*root);
    }
    bool status;
    size_t index;
    TNode* location = root;
    while(true){
        location->Search(word, status, index);
        if(status){
            return false;
        }
        if(location->Leaf()){
            location->InsertLeaf(word, key, index);
            return true;
        }
        if(location->Child(index)->Weight()==(2*t-1)){
            location->Split(index);
            int compare = strcasecmp(word, location->Word(index));
            if(compare > 0){
                ++index;
            }else if(compare == 0){
                return false;
            }
        }
        location = location->Child(index);
    }
}

bool TTree::Save(char const* filename){
    std::ofstream dump(filename, std::ios_base::out|std::ios_base::trunc);
    if(!dump.is_open()){
        return false;
    }
    if(root){
        Stack<TNode*> route;
        route.Push(root);
        Stack<size_t> locate;
        locate.Push(0);
        while(locate.Top() <= route.Top()->Weight()){
            while(!route.Top()->Leaf()){
                route.Push(route.Top()->Child(locate.Top()));
                locate.Push(0);
            }
            while(locate.Top()<route.Top()->Weight()){
                dump<<strlen(route.Top()->Word(locate.Top()))<<" "<<route.Top()->Word(locate.Top())<<" "<<*route.Top()->Key(locate.Top())<<std::endl;
                ++locate.Top();
            }
            while(route.Size()>1 && locate.Top()>=route.Top()->Weight()){
                route.Pop();
                locate.Pop();                            
            }
            if(locate.Top()<route.Top()->Weight()){
                dump<<strlen(route.Top()->Word(locate.Top()))<<" "<<route.Top()->Word(locate.Top())<<" "<<*route.Top()->Key(locate.Top())<<std::endl;
            }
            ++locate.Top();
        }
    }
    dump.close();
    return !dump.goodbit;
}

bool TTree::Load(char const* filename){
    std::ifstream dump(filename, std::ios_base::in);
    if(!dump.is_open()){
        return false;
    }
    char key[256];
    Queue<char*> words;
    Queue<unsigned long long>keys;
    while(dump>>key){
        words.Push(new char [atoll(key)+1]);
        dump>>words.Back();
        dump>>key;
        keys.Push(atoll(key));
    }
    dump.close();
    size_t full = 2 * t - 1;
    size_t total = words.Size();
    if(!total){
        delete root;
        root=nullptr;

    }else if(total <= full){
        delete root;
        root = new TNode;
        root->weight = total;
        root->capacity = total;
        root->children = nullptr;
        root->words = new char* [total];
        root->keys = new unsigned long long [total];
        for(size_t i = 0; i < total; i++){
            root->words[i] = words.Front();
            words.Pop();
            root->keys[i] = keys.Front();
            keys.Pop();
        }
    }else{
        total = total / (full + 1) - 1;
        Queue<TNode*> transporter;
        Queue<TNode*> node_collector;
        Queue<char*> words_collector;
        Queue<unsigned long long> keys_collector;
        for(size_t i = 0; i < total; i++){
            transporter.Push(new TNode);
            transporter.Back()->weight = full;
            transporter.Back()->capacity = full;
            transporter.Back()->children = nullptr;
            transporter.Back()->words = new char* [full];
            transporter.Back()->keys = new unsigned long long [full];
            for(size_t j = 0; j < full; j++){
                transporter.Back()->words[j] = words.Front();
                words.Pop();
                transporter.Back()->keys[j] = keys.Front();
                keys.Pop();
            }
            words_collector.Push(words.Front());
            words.Pop();
            keys_collector.Push(keys.Front());
            keys.Pop();
        }
        total=words.Size() / 2;
        transporter.Push(new TNode);
        transporter.Back()->weight = total;
        transporter.Back()->capacity = total;
        transporter.Back()->children = nullptr;
        transporter.Back()->words = new char* [total];
        transporter.Back()->keys = new unsigned long long [total];
        for(size_t i = 0; i < total; i++){
            transporter.Back()->words[i] = words.Front();
            words.Pop();
            transporter.Back()->keys[i] = keys.Front();
            keys.Pop();
        }
        words_collector.Push(words.Front());
        words.Pop();
        keys_collector.Push(keys.Front());
        keys.Pop();
        total = words.Size();
        transporter.Push(new TNode);
        transporter.Back()->weight = total;
        transporter.Back()->capacity = total;
        transporter.Back()->children = nullptr;
        transporter.Back()->words = new char* [total];
        transporter.Back()->keys = new unsigned long long [total];
        for(size_t i = 0; i < total; i++){
            transporter.Back()->words[i] = words.Front();
            words.Pop();
            transporter.Back()->keys[i] = keys.Front();
            keys.Pop();
        }
        std::swap(words, words_collector);
        std::swap(keys, keys_collector);
        while(words.Size() > full){
            total=words.Size() / (full + 1) - 1;
            for(size_t i = 0; i < total; i++){
                node_collector.Push(new TNode);
                node_collector.Back()->weight = full;
                node_collector.Back()->capacity = full;
                node_collector.Back()->words = new char* [full];
                node_collector.Back()->children = new TNode* [full + 1];
                node_collector.Back()->keys=new unsigned long long [full];
                for(size_t j = 0; j < full; j++){
                    node_collector.Back()->children[j] = transporter.Front();
                    transporter.Pop();
                    node_collector.Back()->words[j] = words.Front();
                    words.Pop();
                    node_collector.Back()->keys[j] = keys.Front();
                    keys.Pop();
                }
                node_collector.Back()->children[full] = transporter.Front();
                transporter.Pop();
                words_collector.Push(words.Front());
                words.Pop();
                keys_collector.Push(keys.Front());
                keys.Pop();
            }
            total = words.Size() / 2;
            node_collector.Push(new TNode);
            node_collector.Back()->weight = total;
            node_collector.Back()->capacity = total;
            node_collector.Back()->words = new char* [total];
            node_collector.Back()->children = new TNode* [total + 1];
            node_collector.Back()->keys = new unsigned long long [total];
            for(size_t i = 0; i < total; i++){
                node_collector.Back()->children[i] = transporter.Front();
                transporter.Pop();
                node_collector.Back()->words[i] = words.Front();
                words.Pop();
                node_collector.Back()->keys[i] = keys.Front();
                keys.Pop();
            }
            node_collector.Back()->children[total] = transporter.Front();
            transporter.Pop();
            words_collector.Push(words.Front());
            words.Pop();
            keys_collector.Push(keys.Front());
            keys.Pop();
            total = words.Size();
            node_collector.Push(new TNode);
            node_collector.Back()->weight = total;
            node_collector.Back()->capacity = total;
            node_collector.Back()->words = new char* [total];
            node_collector.Back()->children = new TNode* [total + 1];
            node_collector.Back()->keys = new unsigned long long [total];
            for(size_t i = 0; i < total; i++){
                node_collector.Back()->children[i] = transporter.Front();
                transporter.Pop();
                node_collector.Back()->words[i] = words.Front();
                words.Pop();
                node_collector.Back()->keys[i] = keys.Front();
                keys.Pop();
            }
            node_collector.Back()->children[total] = transporter.Front();
            transporter.Pop();
            std::swap(transporter, node_collector);
            std::swap(words, words_collector);
            std::swap(keys, keys_collector);
        }
        total = words.Size();
        delete root;
        root = new TNode;
        root->weight = total;
        root->capacity = total;
        root->words = new char* [total];
        root->children = new TNode* [total+1];
        root->keys = new unsigned long long [total];
        for(size_t i = 0; i < total; i++){
            root->children[i] = transporter.Front();
            transporter.Pop();
            root->words[i] = words.Front();
            words.Pop();
            root->keys[i] = keys.Front();
            keys.Pop();
        }
        root->children[total] = transporter.Front();
        transporter.Pop();
    }
    return true;
}

TTree::~TTree(){
    delete root;
}