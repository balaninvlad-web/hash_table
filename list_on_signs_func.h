#ifndef LIST_ON_SIGNS_FUNC_H
#define LIST_ON_SIGNS_FUNC_H

#include <string.h> 
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int ListErr_t;

typedef struct Node
{
    const char* key;
    int count;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct
{
    Node* dummy;
    size_t size;
} LinkedList;

LinkedList* ListCtor ();
void ListDtor (LinkedList* list);

Node* InsertAfterNode (LinkedList* list, Node* after_node, const char* key);
int DeleteNode (LinkedList* list, Node* node);
Node* ListFindNode (LinkedList* list, const char* key);

Node* GetHead (LinkedList* list);
Node* GetTail (LinkedList* list);

void VerifyList (LinkedList* list);
void PrintList (LinkedList* list);

void CreateLogFile (LinkedList* list, const char* filename);
void Create_head_log_file (FILE* dot_file);
void Create_graph_node (LinkedList* list, FILE* dot_file);
void Make_ranking (LinkedList* list, FILE* dot_file);
void Make_arrow (LinkedList* list, FILE* dot_file);
void Make_service_signs (LinkedList* list, FILE* dot_file);
void Create_picture ();

#endif
