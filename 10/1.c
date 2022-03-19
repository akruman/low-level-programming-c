#include <stdio.h>
#include <stdlib.h>
struct Node;
struct Node {
    struct Node* next;
    int val;
};

struct Node* Node_push_front(struct Node *prev, int val) {
    struct Node* front = malloc(sizeof(struct Node));
    if(front){
        front->val = val;
        front->next = prev;
    }
    return front;
}

// add node after last, put val inside, return new last
struct Node* Node_push_back(struct Node *last, int val) {
    last->next = malloc(sizeof(struct Node));
    if(last->next){
        last->next->val = val;
        last->next->next = NULL;
    }
    return last->next;
}

void Node_free(struct Node *list) {
    while(list) {
        struct Node* t = list;
        list = list->next;
        free(t);
    }
}
int Node_sum(struct Node *list) {
    int sum = 0;
    for(;list;list=list->next){
        sum += list->val;
    }
    return sum;
}
// on success return 0, and val returns value 
int Node_at(struct Node *list, size_t ix, int *val) {
    for(;ix && list;list=list->next)ix--;
    if(list && ix==0){
        *val = list->val;
        return 0;
    }
    return -1;
    
}
int main(){
    int x;
    struct Node* list = NULL;
    while(1==scanf("%d",&x)){
        struct Node* t = Node_push_front(list, x); // list is reversed, if we picky should add push_back
        if(!t){perror("MEM\n"); exit(1);}
        list = t;
    }

    if(!Node_at(list, 2, &x))
        printf("list[2]=%d\n",x);
    else
        printf("list[2] not exists\n");

    printf("sum = %d\n", Node_sum(list));
    Node_free(list);
    return 0;
}