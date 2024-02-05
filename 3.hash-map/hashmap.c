#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CAPACITY 50000

// Hash the string
int hash_fn(char* str) {
    int i = 0;

    for (int j = 0; str[j]; j++)
        i += str[j];
    
    return i % CAPACITY;
}

// 2 Words in memory
typedef struct item {
    char* key;
    char* value;
} Hitem;

typedef struct LNode {
    Hitem* item;
    struct LNode* next;
} LNode;

// 3 Words in Memory
typedef struct HashTable {
    Hitem** items; // Pointer an array of pointers which point to a Hitem
    LNode** overflow_buckets;
    int size;
    int count;
} HashTable;

void free_list(LNode* list) {
    LNode* current = list;
    while(list) {
        current = list;
        list = list->next;
        free(current->item->key);
        free(current->item->value);
        free(current->item);
        free(current);
    }
}
LNode* allocate_node() {
    LNode* node = (LNode*) malloc(sizeof(LNode));
    return node;
}

LNode* list_insert(LNode* list, Hitem* item) {
    LNode* node = allocate_node();
    if (!list) {
        node->item = item;
        node->next = NULL;
        list = node;
        return list;
    }
    else if (list->next == NULL) {
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LNode* current = list;
    while(current->next) {
        current = current->next;
    }
    node->item = item;
    node->next = NULL;
    current->next = node;
    return list;
}

Hitem* list_remove(LNode* list) {
    if (!list)
        return NULL;

    if (!list->next)
        return NULL;
    
    LNode* node = list->next;
    LNode* current = list;
    current->next = NULL;
    list = node;
    Hitem* item = NULL;
    memcpy(current->item, item, sizeof(Hitem));
    free(current->item->key);
    free(current->item->value);
    free(current->item);
    free(current);
    return item;
}

LNode** create_overflow_buckets(HashTable* table) {
    LNode** buckets = (LNode**) calloc(table->size, sizeof(LNode*));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

void free_overflow_buckets(HashTable* table) {
    LNode** buckets = table->overflow_buckets;

    for (int i =0; i < table->size; i++)
        free_list(buckets[i]);
    
    free(buckets);
}
// Allocate memory for key, value and the Hitem itself
Hitem* create_item(char* key, char* value) {
    Hitem* item = (Hitem*) malloc(sizeof(Hitem));
    item->key = (char*) malloc(strlen(key) + 1);
    item->value = (char*) malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

// Allocated memory for size, count and the array of pointer
HashTable* create_table(int size) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Hitem**) calloc(table->size, sizeof(Hitem*));
    
    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;
    
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}


void free_item(Hitem* item) {
    free(item->key);
    free(item->value);
    free(item);
}

void free_table(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        Hitem* item = table->items[i];
        if (item != NULL) {
            free_item(item);
        }
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}


void handle_collision (HashTable* table, int index, Hitem* item) {
    LNode* head = table->overflow_buckets[index];
    if (head == NULL) {
        head = allocate_node();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else {
        table->overflow_buckets[index] = list_insert(head, item);
        return;
    }
}

void ht_insert(HashTable* table, char* key, char* value) {
    Hitem* item = create_item(key, value);
    int index = hash_fn(key);

    Hitem* cur_item = table->items[index];

    if (cur_item == NULL) 
    {
        if (table->count == table->size) 
        {
            printf("Error: HashTable is Full\n");
            free_item(item);
            return;
        }

        table->items[index] = item;
        table->count++;
    } else {
        if (strcmp(cur_item->key, key) == 0) {
            free_item(cur_item);
            table->items[index] = item;
            return;
        } else {
            handle_collision(table, index,item);
            return;
        }
    }
}

char* ht_search(HashTable* table, char* key) {
    int index = hash_fn(key);

    Hitem* item = table->items[index];
    LNode* head = table->overflow_buckets[index];
    
    if (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item->value;

        if (head==NULL)
            return NULL;

        // I feel like something is wrong here
        item = head->item;
        head = head->next;
        }
    }
    // Item not Found
    return NULL;
}

void ht_delete(HashTable* table, char* key) {
    int index = hash_fn(key);
    Hitem* item = table->items[index];
    LNode* head = table->overflow_buckets[index];

    if (item == NULL) {
        return;
    }
    else {
        if (head == NULL && strcmp(item->key, key) == 0)
        {
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            if (strcmp(item->key, key) == 0)
            {
                free_item(item);
                LNode* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_list(node);
                table->overflow_buckets[index] = head;
                return;
            }
            
            LNode* current = head;
            LNode* prev = NULL;

            while (current)
            {
                if(strcmp(current->item->key, key) == 0) 
                {
                    if (prev == NULL) 
                    {
                        // First element of the chian
                        // Remove Chain
                        free_list(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else
                    {
                        prev->next = current->next;
                        current->next = NULL;
                        free_list(current);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }
                current = current->next;
                prev = current;
            }
        }
    }
}

void print_search(HashTable* table, char* key) {
    char* val;
    if ((val = ht_search(table, key)) == NULL) {
        printf("Item not found");
        return;
    } else {
        printf("Key: %s, Value:%s", key, val);
    }
}

void print_table(HashTable *table)
{
    printf("\nHash Table\n-------------------\n");

    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i])
        {
            printf("Index:%d, Key:%s, Value:%s\n", i, table->items[i]->key, table->items[i]->value);
        }
    }

    printf("-------------------\n\n");
}







int main()
{
    HashTable* ht = create_table(CAPACITY);
    ht_insert(ht, (char *)"1", (char *)"First address");
    ht_insert(ht, (char *)"2", (char *)"Second address");
    ht_insert(ht, (char *)"Hel", (char *)"Third address");
    ht_insert(ht, (char *)"Cau", (char *)"Fourth address");
    print_search(ht, (char *)"1");
    print_search(ht, (char *)"2");
    print_search(ht, (char *)"3");
    print_search(ht, (char *)"Hel");
    print_search(ht, (char *)"Cau"); // Collision!
    print_table(ht);
    ht_delete(ht, (char *)"1");
    ht_delete(ht, (char *)"Cau");
    print_table(ht);
    free_table(ht);
    return 0;
}