#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> // BUFSIZ
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
struct List {
    struct Node *first;
    struct Node *last;
};

struct Node {
    struct Node* next;
    int val;
};

struct List* List_init(){
    struct List* ret = malloc(sizeof(struct List));
    if(ret){ ret->first = ret->last = NULL;}
    return ret;
}

int List_empty(struct List *list){
    return list->first == NULL;
}

// ret 1 on success
int List_append(struct List *list, int val) {
    int ret=0;
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if(node){
        node->val = val;
        node->next = NULL;
        if(!list->first) {list->first = node;}
        if(list->last) {list->last->next=node;}
        list->last = node;
        ret=1;
    }
    return ret;
}

// run f for each node value in list
void foreach(struct List *list, void (*f)(int)) {
    struct Node *it;
    for(it = list->first; it; it=it->next) f(it->val);
}

// run f for each node value in list return the new list
struct List* map(struct List *list, int (*f)(int)) {
    struct Node *it;
    struct List* ret = List_init();
    for(it = list->first; it && List_append(ret, f(it->val)); it=it->next);
    return ret;
}

struct List* map_mut(struct List *list, int (*f)(int)) {
    struct Node *it;
    for(it = list->first; it; it=it->next) it->val = f(it->val);
    return list;
}

// return list with n elements: [s, f(s), f(f(s)), ..]
struct List* iterate(int s, size_t n, int (*f)(int)) {
    struct List* ret = List_init();
    if(ret){
        for(;n>0 && List_append(ret, s);n--){  
            s = f(s);
        }
    }
    return ret;
}

int fold1(struct List *list, int (*f_val_acc)(int, int), int acc) {
    struct Node *it;
    for(it = list->first; it; it=it->next) acc = f_val_acc(it->val,acc);
    return acc;
}

bool save(struct List* list, const char* filename) {
    bool ret = false;
    FILE *f = fopen(filename, "wt+");
    if(f){
        struct Node *it;
        for(it = list->first; it; it=it->next){
            if(0>fprintf(f,"%d ",it->val)) goto err1;
        }
        ret = true;
err1:
        fclose(f);    
    }
    return ret;
}

bool load(struct List** list, const char* filename){
    int val, n;
    *list = List_init();
    FILE *f = fopen(filename, "rt");
    if(f){
        while((n=fscanf(f,"%d",&val)) && n!=EOF && n>0 && List_append(*list, val));
        fclose(f);
        return true;
    }
    return false;
}

bool serialize(struct List* list, const char* filename){
    bool ret = false;
    FILE *f = fopen(filename, "wb+");
    if(f){
        struct Node *it;
        for(it = list->first; it; it=it->next){   
            if(1!=fwrite(&it->val, sizeof(it->val), 1, f)) goto err1;
        }
        ret = true;
err1:
        fclose(f);    
    }
    return ret;
}
bool deserialize(struct List** list, const char* filename){
    int val, n;
    *list = List_init();
    FILE *f = fopen(filename, "rb");
    if(f){      
        while((n=fread(&val,sizeof(val), 1, f)) && n==1 && List_append(*list, val));
        fclose(f);
        return true;
    }
    return false;
}

void List_clean(struct List* list){
    if(list){
        struct Node *it = list->first, *tmp;
        while(it){
            tmp = it;
            it = it->next;
            free(tmp);
        }
        list->first = list->last = NULL;
    }
}

void List_free(struct List** list){
    if(list && *list){
        List_clean(*list);
        free(*list);
        *list = NULL;
    }
}


void print_int_space(int val){printf("%d ",val);}
void print_int_cr(int val){printf("%d\n",val);}
int f2(int x){return x*x;}
int f3(int x){return x*x*x;}
int sum(int x, int acc) {return x+acc;}
int min(int x, int acc) {return (x<acc)?x:acc;}
int max(int x, int acc) {return (x>acc)?x:acc;}
int mod2(int x){return x%2;}
int mul2(int x){return x*2;}

int main(){
    
    // scan ints into list
    int i;
    int ret;
    struct List list= {NULL,NULL};
    while((ret = scanf("%d",&i)) && ret != EOF && ret == 1 && List_append(&list,i));

    foreach(&list, &print_int_space);
    puts("");
    foreach(&list, &print_int_cr);

    struct List *list2 = map(&list, &f2);
    struct List *list3 = map(&list, &f3);
    foreach(list2, &print_int_space);
    foreach(list3, &print_int_space);
    puts("");
    printf("sum=%d\n",fold1(&list, sum,0));

    if(!List_empty(&list)){
        printf("max=%d\n",fold1(&list, max, list.first->val));
        printf("min=%d\n",fold1(&list, min, list.first->val));
    }
    map_mut(&list, mod2);
    foreach(&list, &print_int_space);
    puts("");
    struct List *list4 = iterate(1,10,mul2);
    foreach(list4, &print_int_space);
    puts("");

    save(&list, "1.txt");
    struct List *list5;
    load(&list5, "1.txt");
    foreach(list5, &print_int_space);
    puts("");
 
    serialize(&list, "1.bin");
    struct List *list6;
    deserialize(&list6, "1.bin");
    foreach(list6, &print_int_space);
    puts("");

    List_clean(&list);
    List_free(&list2);
    List_free(&list3);
    List_free(&list4);
    List_free(&list5);

    return 0;
}