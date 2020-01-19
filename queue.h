#include <utility>
template <typename T> class Queue{
    public:
        Queue(){
            head=nullptr;
            tail=nullptr;
            size=0;
        }

        T& Front(){
            return head->item;
        }

        T& Back(){
            return tail->item;
        }

        void Pop(){
            if(head!=tail){
                head=head->next;
                delete head->back;
                head->back=nullptr;
            }else{
                delete head;
                head=nullptr;
                tail=nullptr;
            }
            size--;                     
        }

        void Push(T& x_item){
            if(head==nullptr){
                tail=new QueueItem(std::forward<T>(x_item), nullptr, nullptr);
                head=tail;
            }else{
                tail=new QueueItem(std::forward<T>(x_item), tail, nullptr);
                tail->back->next=tail;
            }
            size++;
        }

        void Push(T&& x_item){
            if(head==nullptr){
                tail=new QueueItem(std::forward<T>(x_item), nullptr, nullptr);
                head=tail;
            }else{
                tail=new QueueItem(std::forward<T>(x_item), tail, nullptr);
                tail->back->next=tail;
            }
            size++;
        }

        bool Empty(){
            return !head;
        }

        size_t Size(){
            return size;            
        }

        void Clean(){
            while(head!=tail){
                head=head->next;
                delete head->back;
            }
            delete head;
            head=nullptr;
            size=0;
        }

        ~Queue(){
            while(head!=tail){
                head=head->next;
                delete head->back;
            }
            delete head;
        }

        private:
            class QueueItem{
                public:
                    QueueItem(T& x_item, QueueItem* x_back, QueueItem* x_next): item(std::forward<T>(x_item)), back(x_back), next(x_next){

                    }
                    QueueItem(T&& x_item, QueueItem* x_back, QueueItem* x_next): item(std::forward<T>(x_item)), back(x_back), next(x_next){

                    }
                    T item;
                    QueueItem* back;
                    QueueItem* next;
            };

            QueueItem* head;
            QueueItem* tail;
            size_t size;
};