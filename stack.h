#include <utility>
template <typename T> class Stack{
    public:
        Stack(){
            head=nullptr;
            size=0;
        }

        T& Top(){
            return head->item;
        }

        T& Peek(size_t index){
            StackItem* required=head;
            for(index; index>0; index--){
                required=required->next;
            }
            return required->item;
        }

        void Pop(){
            StackItem* temp=head;
            head=head->next;
            delete temp;   
            size--;                     
        }

        void Push(T& x_item){
            head = new StackItem(std::forward<T>(x_item), head);
            size++;
        }

        void Push(T&& x_item){
            head = new StackItem(std::forward<T>(x_item), head);
            size++;
        }

        bool Empty(){
            return !head;
        }

        size_t Size(){
            return size;            
        }

        void Clean(){
            while(head!=nullptr){
                StackItem* temp=head;
                head=head->next;
                delete temp;
            }
            size=0;
        }

        ~Stack(){
            while(head!=nullptr){
                StackItem* temp=head;
                head=head->next;
                delete temp;
            }
        }

        private:
            class StackItem{
                public:
                    StackItem(T& x_item, StackItem* x_next): item(std::forward<T>(x_item)), next(x_next){

                    }
                    StackItem(T&& x_item, StackItem* x_next): item(std::forward<T>(x_item)), next(x_next){

                    }
                    T item;
                    StackItem* next;
            };

            StackItem* head;
            size_t size;
};