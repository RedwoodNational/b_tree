#include <iostream>
#include <string.h>
#include "b_tree.h"

int main(){
    const size_t TREE_DEGREE = 2;
    const short MAX_WORD_LENGTH = 257;
    char mode;
    unsigned long long key;
    TTree wordlist(TREE_DEGREE);
    char string[MAX_WORD_LENGTH];
    while(std::cin>>mode){
        if(mode == '+'){
            std::cin>>string;
            std::cin>>key;
            if(wordlist.Insert(string, key)){
                std::cout<<"OK"<<std::endl;
            }else{
                std::cout<<"Exist"<<std::endl;
            }
            continue;            
        }
        if(mode == '-'){
            std::cin>>string;
            if(wordlist.Remove(string)){
                std::cout<<"OK"<<std::endl;
            }else{
                std::cout<<"NoSuchWord"<<std::endl;
            }
            continue;
        }
        if(mode == '!'){
            std::cin>>string;
            if(strcmp("Save", string) == 0){
                std::cin>>string;
                if(wordlist.Save(string)){
                    std::cout<<"OK"<<std::endl;
                }else{
                    std::cout<<"ERROR: Couldn't create file"<<std::endl;
                }
                continue;
            }
            if(strcmp("Load", string) == 0){
                std::cin>>string;
                if(wordlist.Load(string)){
                    std::cout<<"OK"<<std::endl;
                }else{
                    std::cout<<"ERROR: Couldn't open file"<<std::endl;
                }
                continue;
            }
            continue;                        
        }
        std::cin.unget();
        std::cin>>string;
        auto key_ptr = wordlist.Search(string);
        if(key_ptr){
            std::cout<<"OK: "<<*key_ptr<<std::endl;
        }else{
            std::cout<<"NoSuchWord"<<std::endl;
        }
    }
    return 0;
}


