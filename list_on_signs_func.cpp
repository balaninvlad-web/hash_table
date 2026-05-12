#include "list_on_signs_func.h"
#include "hash_table.h"

#define MAX_COMMAND_LENGTH 200

LinkedList* ListCtor ()
{
    LinkedList* list = (LinkedList*) calloc (1, sizeof(LinkedList));

    if (!list)
    {
        fprintf (stderr, "ERORR: in Ctor - memory allocation\n");
        return NULL;
    }

    list->dummy = (Node*) calloc (1, sizeof(Node));
    if (!list->dummy) 
    {
        free (list);
        fprintf (stderr, "ERROR: ListCtor - dummy allocation failed\n");
        return NULL;
    }

    list->dummy->next = list->dummy;

    list->dummy->prev = list->dummy;

    list->size = 0;

    #ifndef NDEGUG
        //CreateLogFile (list, "list_dump.dot");
    #endif
    
    return list;
}

void ListDtor (LinkedList* list)
{
    if(!list) return;

    Node* current = list->dummy->next;

    while (current != list->dummy)
    {
        Node* next = current->next;
        free (current);
        current = next;
    }
    free (list->dummy);
    free (list);
}

Node* InsertAfterNode (LinkedList* list, Node* after_node, const char* key)
{
    if (!list || !key) return NULL;

    Node* new_node = (Node*) calloc (1, sizeof(Node));
    if (!new_node)
    {
        fprintf (stderr, "ERORR: in InsertNode - memory allocation\n");
        return NULL;
    }
    new_node->key = key;
    new_node->count = 1;

    if (after_node == NULL)
    {
        after_node = list->dummy;
    }

    new_node->next = after_node->next;
    new_node->prev = after_node;
    after_node->next->prev = new_node;
    after_node->next = new_node;

    list->size++;

    #ifndef NDEGUG
        //VerifyList (list);
        //CreateLogFile (list, "list_dump.dot");
    #endif

    return new_node;
}

Node* ListFindNode (LinkedList* list, const char* key)
{
    if (!list || !key) return NULL;
    Node* current = list->dummy->next;
    Node* dummy = list->dummy;

    uint64_t key_prefix = 0;
    __asm__ ("movq (%1), %0" : "=r" (key_prefix) : "r" (key) : "memory");

    while (current != dummy) 
    {
        uint64_t node_prefix = 0;
        __asm__ ("movq (%1), %0" : "=r" (node_prefix) : "r" (current->key) : "memory");
        int equal = 0;
        __asm__ ("cmp %2, %1; sete %0" : "=r" (equal) : "r" (node_prefix), "r" (key_prefix) : "cc");

        if (equal && MyAvxStrcmp (current->key, key) == 0)  return current;
        current = current->next;
    }
    return NULL;
}

void PrintList (LinkedList* list)
{
    if (!list) return;
    printf ("List size: %zu\n", list->size);
    Node* curr = GetHead (list);
    int idx = 0;
    while (curr != list->dummy) 
    {
        printf("[%d] '%s' : %d\n", idx++, curr->key, curr->count);
        curr = curr->next;
    }
    printf("\n\n");
}

int DeleteNode (LinkedList* list, Node* node)
{
    if (!list || !node || node == list->dummy) return -1;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free (node);
    list->size--;

    //CreateLogFile (list, "list_dump.dot");

    return 0;
}

void CreateLogFile (LinkedList* list, const char* filename)
{
    #ifndef NDEBUG
        assert(list);
    #endif

    FILE* dot_file = fopen (filename, "w");

    Create_head_log_file (dot_file);

    Create_graph_node (list, dot_file);

    Make_ranking (list, dot_file);

    Make_arrow (list, dot_file);

    Make_service_signs (list, dot_file);

    Create_picture ();

    fclose (dot_file);
}

void Create_head_log_file (FILE* dot_file)
{
    #ifndef NDEBUG
        assert(dot_file);
    #endif

    printf("START Writing TO file.....\n");

    fprintf(dot_file,"digraph list{\n");

    fprintf(dot_file, "    bgcolor=\"#001f29\"\n");

    fprintf(dot_file, "    rankdir = HR\n");
    fprintf(dot_file, "    nodesep = 0.5;\n");
    fprintf(dot_file, "    ranksep = 0.7;\n");

    fprintf(dot_file, "    node [shape=plaintext, style=filled, fontname=\"Arial\"];\n");

    fprintf(dot_file, "    edge [fontname=\"Arial\"];\n\n");

    fprintf(dot_file, "    {\n");
}

void Create_graph_node (LinkedList* list, FILE* dot_file)
{
    #ifndef NDEBUG
        assert(list);
        assert(dot_file);
    #endif

    fprintf(dot_file, "dummy [label=<<TABLE BORDER='1' CELLBORDER='1' CELLSPACING='0'>"
                      "<TR><TD COLSPAN='2'><B>DUMMY</B></TD></TR>"
                      "<TR><TD COLSPAN='2'>Addr: %p</TD></TR>"
                      "<TR><TD>Next: %p</TD><TD>Prev: %p</TD></TR></TABLE>>, "
                      "fillcolor=\"#691236\", color=\"#fdfdfd\", fontcolor=\"#fdfdfd\"];\n",
            (void*)list->dummy, (void*)list->dummy,
            (void*)list->dummy->next, (void*)list->dummy->prev);

    Node* current = GetHead(list);
    Node* head = GetHead(list);
    Node* tail = GetTail(list);

    for (size_t i = 0; i < list->size; i++)
    {
        printf("START WRITE BLOCKS Writing TO file.....\n");

        const char* fillcolor = "";
        const char* color = "black";

        if (current == head && current == tail)
        {
            fillcolor = "#edb1f1";  // ������������ �������
            color = "#fdfdfd";
        }
        else if (current == head)
        {
            fillcolor = "#5f3035";  // ������
            color = "#fdfdfd";
        }
        else if (current == tail)
        {
            fillcolor = "#305551";  // �����
            color = "#fdfdfd";
        }
        else
        {
            fillcolor = "#2799a0";  // ������� ����
            color = "#fdfdfd";
        }
        /*if ( (unsigned)list->next[i] > list->capacity || list->next[i] < 0 || list->prev[i] > (int)list->capacity || list->prev[i] < -1)
        {
            fillcolor = "red";

            fprintf(dot_file, "     node%d [shape=tripleoctagon, label = \"ERORR NEXT %d\", fillcolor=red, color= white, fontcolor= white];\n", (int)list->next[i], (int)list->next[i]);
        }*/
        fprintf(dot_file, "        node%p [label=<<TABLE BORDER='1' CELLBORDER='1' CELLSPACING='0'>"
                          "<TR><TD COLSPAN='2'><B>Addr: %p</B></TD></TR>"
                          "<TR><TD COLSPAN='2'>Key: %s</TD></TR>"          // было Data: %d
                          "<TR><TD>Next: %p</TD><TD>Prev: %p</TD></TR></TABLE>>, "
                          "fillcolor=\"%s\", color=\"%s\", fontcolor=\"%s\"];\n",
               (void*)current, (void*)current, current->key,
               (void*)current->next, (void*)current->prev, fillcolor, color, color);

        printf ("BLOCKS Writing TO file.....\n");

        current = current->next;
    }

    fprintf(dot_file,"}\n");
}

void Make_ranking (LinkedList* list, FILE* dot_file)
{
    #ifndef NDEGUG
        assert(list);
        assert(dot_file);
    #endif

    if (list->size == 0)
    {
        fprintf(dot_file, "    // Empty list - no ranking needed\n");
        return;
    }

    fprintf(dot_file,"{ rank = same; dummy; ");

    Node* current = GetHead(list);

    for (size_t i = 0; i < list->size; i++)
    {
        fprintf(dot_file,"node%p; ", (void*)current);
        current = current->next;
    }

    fprintf(dot_file,"}\n");

    //fprintf(dot_file, "{rank = same; head; tail}\n");
}

void Make_arrow (LinkedList* list, FILE* dot_file)
{
    #ifndef NDEGUG
        assert(list);
        assert(dot_file);
    #endif

    printf("Make_arrow: creating connections...\n");

    Node* current = GetHead(list);

    int connection_count = 0;

    for (size_t i = 0; i < list->size; i++)
    {
        fprintf(dot_file, "node%p -> node%p [color = \"transparent\"; weight=100];\n", (void*)current, (void*)current->next);

        current = current->next;
    }

    current = GetHead(list);

    while (current != list->dummy)
    {
        if (current->next != NULL && current->next != list->dummy)
        {
        // ������ ������� ����� ��
            fprintf(dot_file, "    node%p -> node%p [color=\"#adebff\", penwidth=3];\n",
                    (void*)current, (void*)current->next);
            connection_count++;

            printf("  NEXT: %p -> %p\n", (void*)current, (void*)current->next);
        }

        if (current->prev != NULL && current->prev != list->dummy)
        {
        // �� ������ ������� ����� ��
            fprintf(dot_file, "    node%p -> node%p [color=\"#ffadb1\", style=dashed, penwidth=2];\n",
                   (void*)current, (void*)current->prev);

            connection_count++;

            printf("  PREV: %p -> %p\n", (void*)current, (void*)current->prev);
        }

        current = current->next;
    }
    //int free_pos = Get_first_free_pos(list);
}

void Make_service_signs (LinkedList* list, FILE* dot_file)
{
    #ifndef NDEGUG
        assert(list);
        assert(dot_file);
    #endif

    fprintf(dot_file, "    head [shape=Mrecord, color=\"#fdfdfd\", fillcolor=\"#5f3035\", fontcolor=white, style=filled, label=\"HEAD\"];\n");
    fprintf(dot_file, "    tail [shape=Mrecord, color=\"#fdfdfd\", fillcolor=\"#305551\", fontcolor=white, style=filled, label=\"TAIL\"];\n");

    Node* head = GetHead(list);
    Node* tail = GetTail(list);

    if (head != list->dummy)
    {
        printf("HEAD Writing TO file.....\n");
        fprintf(dot_file, "    head -> node%p [color=\"#5f3035\", weight=100];\n", (void*)head);
    }

    if (tail != list->dummy)
    {
        printf("TAIL Writing TO file.....\n");
        fprintf(dot_file, "    tail -> node%p [color=\"#305551\", weight=100];\n", (void*)tail);
    }

    fprintf(dot_file, "}\n");
    fclose(dot_file);

    printf("End Writing TO file.....\n");
}

void Create_picture (void)
{
    static int image_counter = 1;

    char command[MAX_COMMAND_LENGTH] = "";

    snprintf(command, sizeof(command), "dot -Tpng list_dump.dot -o imagesDump/list_dump%d.png", image_counter);

    system(command);

    printf("dot -Tpng list_dump.dot -o imagesDump/list_dump%d.png", image_counter);

    printf("Graph generated: list_dump%d.png\n", image_counter);

    image_counter++;
}

Node* GetHead (LinkedList* list)
{
    return list->dummy->next;
}

Node* GetTail (LinkedList* list)
{
    return list->dummy->prev;
}

void VerifyList (LinkedList* list)
{
    if (!list) 
    {
        printf("VerifyList: list is NULL\n");
        return;
    }

    size_t count = 0;
    Node* curr = list->dummy->next;
    while (curr != list->dummy) 
    {
        count++;
        curr = curr->next;
    }
    if (count != list->size) printf ("VerifyList: size mismatch! stored=%zu, actual=%zu\n", list->size, count);
    else                     printf ("VerifyList: OK (%zu nodes)\n", count);
}
