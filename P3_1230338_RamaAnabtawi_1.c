//Rama Rani Said Anabtawi
//sec 1
//1230338
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAXOrder 1009 //maximum number of cities and towns is 650 so this is the suitable table size for the load factor to be 0.65

//---------Structs---------

//city struct for each city containing its index resulted from hashing
struct cityNode;
typedef struct cityNode* city;
struct cityNode
{
    char name[60];
    int index;
    int visited;
};

// struct for each node in the adj list and has a next pointer to form an edge
struct Node;
typedef struct Node* ptr;
struct Node
{
    city city;
    int distance;
    ptr next;
};

//struct node for an edge
struct node;
typedef struct node* edge;
struct node
{
    city destination;
    city source;
    int distance;
    edge next;
};

// Struct definition for Min Heap
struct MinHeap;
typedef struct MinHeap* Heap;
struct MinHeap
{
    int maxsize;
    int size;
    edge *heapArray;
} ;


int hash(char key[], unsigned int tableSize);
void insertToHeap(edge e, Heap H);
Heap CreateHeap(int maxSize);
void minHeapify(int pos, Heap H);
city insertCity(city table[], char name[], int tableSize);
void loadFile(city table[],ptr graph[]);
void insert(ptr L, int distance, city destination);
edge deleteFromHeap(Heap H);
ptr search_name(ptr L, city c);
void printGraph(ptr graph[]);
void prim(ptr graph[],int index);
Heap swap(int pos1, int pos2, Heap H);
int findIndex(int i,int arr[]);

int order = 0;      //number of nodes in a graph as a global variable
int graphSize=0;   //number of edges in a graph as a global variable
int loaded = 0;    //to make sure that the file loads once
double cpu_time_used_prim;
double cpu_time_used_krus;

int main()
{
    city hash_table[MAXOrder];  //hash table for cities
    ptr graph[MAXOrder];        //graph using adjacency list

    for (int i = 0; i < MAXOrder; i++)  //initializing the hash table and the adj list
    {
        hash_table[i] = NULL;
        graph[i] = NULL;
    }
    int op=0;
    do
    {
        printf("Enter a number from 1-5 to choose an operation as shown in the menu:\n");
        printf("\n----MENU----\n");
        printf("1. Load cities.\n");
        printf("2.Apply Prims Algorithm.\n");
        printf("3.Apply kruskals Algorithm.\n");
        printf("4.Compare the two algorithms.\n");
        printf("5.Exit.\n");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
        {
            if(loaded==1)
            {
                printf("The file is already loaded.\n");
                break;
            }
            loadFile(hash_table,graph);     //loading the input file into the hash table and graph
            printGraph(graph);
            printf("\nGraph order: %d\n",order);
            printf("Graph size: %d\n",graphSize);

            break;
        }

        case 2:
        {
            printf("Enter the name of the source city you want to construct the MST from.\n"); //reading the source city name from user

            char city[100];
            gets(city);
            gets(city);

            int flag=-1;
            int Pindex=0;
            for(int i=0; i<MAXOrder; i++)      //search for the city name in the hash table
            {
                if (hash_table[i] != NULL && strcmp(hash_table[i]->name, city) == 0)
                {
                    Pindex = hash_table[i]->index;
                    flag = 1;
                    break;
                }
            }
            if(flag == -1)
            {
                printf("City not found.\n");
                break;
            }

            prim(graph,Pindex);   //calling prim function

            for (int i = 0; i < MAXOrder; i++)  //reset visited for each node after calling prim
            {
                if (hash_table[i] != NULL)
                    hash_table[i]->visited = 0;
            }

            break;
        }
        case 3:
            kruskals(graph);
            break;
        case 4:
            if(cpu_time_used_krus>cpu_time_used_prim)
            {
                printf("Time consumed by kruskal:%f\n",cpu_time_used_krus);
                printf("Time consumed by Prim:%f\n",cpu_time_used_prim);
                printf("Kruskal's algorithm is faster than Prim's algorithm.");
            }
            else if(cpu_time_used_krus<cpu_time_used_prim)
            {
                printf("Time consumed by kruskal:%f\n",cpu_time_used_krus);
                printf("Time consumed by Prim:%f\n",cpu_time_used_prim);
                printf("Prim's algorithm is faster than Kruskal's algorithm.\n");
            }
            else printf("Both has the same time:%f\n",cpu_time_used_krus);
            break;
            case 5: break;


        default:
            printf("Invalid choice. Please try again.\n");
        }

    }
    while(op!=5);

    return 0;
}
int hash(char key[], unsigned int tableSize)
{
    unsigned int hash_val = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash_val = (hash_val << 5) + key[i];
    }
    return (hash_val % tableSize);
}
//insert city o hash table
city insertCity(city table[], char name[], int tableSize)
{
    int index = hash(name, tableSize);
    int i = 1;
    if ( order < MAXOrder )
    {
        while (table[index] != NULL && strcmp(table[index]->name, name) != 0) //handling collision
        {
            index = (index + i) % tableSize;
            i++;
        }

        if (table[index] == NULL || strcmp(table[index]->name, name) != 0)
        {
            table[index] = (city)malloc(sizeof(struct cityNode));
            strcpy(table[index]->name, name);
            table[index]->index = index;
            table[index]->visited=0;
            order++;     //incrementing order of the graph
        }
        return table[index];
    }
}
//insert the node to the graph
void insert(ptr L, int distance, city destination)
{
    if(L!=NULL)
    {
        ptr temp = (ptr) malloc(sizeof (struct Node));
        ptr p= L;
        while(p->next!= NULL)
        {
            p=p->next;
        }
        temp->distance = distance;
        temp->city = destination;
        p->next= temp;
        temp ->next = NULL;
    }
}
//search for a city in the neighbors of the other
ptr search_name(ptr L, city c)
{
    if (L == NULL || L->next == NULL)
    {
        return;
    }
    ptr temp= L->next;
    while(temp!=NULL && strcmp(temp->city->name,c->name)!=0 )
    {
        temp=temp ->next;
    }
    return temp;
}

void loadFile(city table[], ptr graph[])
{
    FILE *in = fopen("cities.txt", "r");
    if (in == NULL)
    {
        printf("Error opening file, make sure the file is available.\n");
        return;
    }

    char temp[300];
    if (fgets(temp, 300, in) == NULL)
    {
        printf("The file is empty.\n");
        fclose(in);
        return;
    }

    do
    {

        char *token;
        token = strtok(temp, "#");
        char source[50];
        char destination[50];
        char dis[10];

        if (token != NULL)
            strcpy(source, token);
        token = strtok(NULL, "#");
        if (token != NULL)
            strcpy(destination, token);
        token = strtok(NULL, "#\n");
        if (token != NULL)
            strcpy(dis, token);

        int distance = atoi(dis);

        city city1=insertCity(table, source, MAXOrder);        //initialize the first city and apply hashing on it
        city city2=insertCity(table, destination, MAXOrder);   //initialize the second city and apply hashing on it


        if (graph[city1->index] == NULL)    //if the slot is empty add city1
        {
            graph[city1->index] = (ptr) malloc(sizeof(struct Node));
            graph[city1->index]->city=city1;
            graph[city1->index]->distance=0;   //initialize the distance to 0
            graph[city1->index]->next = NULL;
        }


        if (graph[city2->index] == NULL)
        {
            graph[city2->index] = (ptr) malloc(sizeof(struct Node));
            graph[city2->index]->city=city2;
            graph[city2->index]->distance=0;
            graph[city2->index]->next = NULL;
        }

        // make sure if the cities are already neighbors
        ptr t= search_name(graph[city1->index],city2);
        if(t==NULL)insert(graph[city1->index],distance,city2);

        ptr t2= search_name(graph[city2->index],city1);
        if(t2==NULL)insert(graph[city2->index],distance,city1);

        graphSize++;
    }

    while (fgets(temp, 300, in) != NULL);

    loaded =1;
    fclose(in);
}


void prim(ptr graph[], int index)
{
    clock_t start, end;

    start = clock();

    Heap H = CreateHeap(graphSize);        //creating heap
    edge MST[order-1];                     //creating an array to store edges
    int totalCost = 0;
    int mstSize = 0;

    if (graph[index] == NULL)
    {
        printf("City is not found in graph.\n");
        return;
    }

    graph[index]->city->visited = 1;     //  setting the first node as visited
    ptr temp = graph[index]->next;

    //    First while loop to add the neighbors of the first node in heap
    while (temp != NULL)
    {
        if (temp->city != NULL && temp->city->visited == 0)   //if the city is not visited add its edge to the heap
        {
            edge e = (edge)malloc(sizeof(struct node));
            e->source = graph[index]->city;
            e->destination = temp->city;
            e->distance = temp->distance;
            e->next = NULL;
            insertToHeap(e, H);
        }
        temp = temp->next;
    }

    printf("-------MST built from city: %s------\n", graph[index]->city->name);
    while (H->size > 0 && mstSize < order - 1)  //conditions to make sure that the mst has true size
    {
        edge min = deleteFromHeap(H);         //get the minimum edge weight element
        if (min == NULL) break;

        if (min->destination->visited == 0)    //if its not visited add it to the MST and add its cost to the total
        {
            min->destination->visited = 1;
            MST[mstSize++] = min;
            totalCost += min->distance;

            ptr connected_node = graph[min->destination->index]->next;    //add all the non-visited neighbors of the minimum node to the heap
            while (connected_node != NULL)
            {
                if (connected_node->city != NULL && connected_node->city->visited == 0)
                {
                    edge e = (edge)malloc(sizeof(struct node));
                    e->source = min->destination;
                    e->destination = connected_node->city;
                    e->distance = connected_node->distance;
                    e->next = NULL;
                    insertToHeap(e, H);
                }
                connected_node = connected_node->next;
            }
        }
        else
        {
            free(min);
        }
    }

    end = clock();
    cpu_time_used_prim = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime consumed: %f seconds\n", cpu_time_used_prim);

    if (mstSize == order - 1)
    {
        printf("\n-----Complete MST found with %d edges:-----\n", mstSize);
        for (int j = 0; j < mstSize; j++)
        {
            printf("Edge %d: %s -- %s (cost: %d)\n", j + 1,
                   MST[j]->source->name,
                   MST[j]->destination->name,
                   MST[j]->distance);
            free(MST[j]);
        }

        printf("Total cost of MST: %d\n", totalCost);
    }
    else
    {
        printf("-----The resultant MST is not complete (not connected):------\n");
        for (int j = 0; j < mstSize; j++)
        {
            printf("Edge %d: %s -- %s (cost: %d)\n", j + 1,
                   MST[j]->source->name,
                   MST[j]->destination->name,
                   MST[j]->distance);
            free(MST[j]);
        }
        printf("Total cost of MST: %d\n", totalCost);
    }


    free(H->heapArray);
    free(H);
}


void printGraph(ptr graph[])
{
    for (int i = 0; i < MAXOrder; i++)
    {
        if (graph[i] != NULL)
        {
            printf(" %s connects to: ", graph[i]->city->name,graph[i]->city->index);
            ptr p = graph[i]->next;
            while (p != NULL)
            {
                printf("%s(%d) - ", p->city->name, p->distance);
                p = p->next;
            }
            printf("NULL\n");
        }
    }
}

// Kruskal's algorithm implementation
void kruskals(ptr graph[])
{

    clock_t start, end;
    start = clock();
    int indices[order];
    edge MST[order -1];
    ptr allNodes[order];
    int mstSize = 0;
    Heap H = CreateHeap(graphSize*2);
    int nodesCount = 0;
    int totalCost = 0;

    //Array for all nodes to easily map the index
    for(int i = 0; i < MAXOrder; i++)
    {
        if(graph[i] != NULL)
        {
            allNodes[nodesCount] = graph[i];
            nodesCount++;
        }
    }

    for(int i = 0; i < order; i++)
    {
        indices[i] = i;
    }
    // Add all edges to heap
    for(int i = 0; i < nodesCount; i++)
    {
        ptr source = allNodes[i];
        ptr temp = source->next;
        while(temp != NULL)
        {
            edge e = (edge)malloc(sizeof(struct node));
            e->source = source->city;
            e->destination = temp->city;
            e->distance = temp->distance;
            e->next = NULL;
            insertToHeap(e, H);
            temp = temp->next;
        }
    }
    while(H->size!=0 && mstSize < order-1)
    {
        edge min = deleteFromHeap(H);
        int sourceIndex= -1;
        int destIndex = -1;
        // Loop to find the initial index values for the cities
        for(int i=0; i<nodesCount; i++)
        {
            if(allNodes[i]->city == min->source)
                sourceIndex=i;
            if(allNodes[i]->city == min->destination)
                destIndex=i;

        }
        if(sourceIndex != -1 && destIndex != -1)    // Edge cities are found
        {
            int Source = findIndex(sourceIndex, indices);
            int Dest = findIndex(destIndex, indices);

            if(Source != Dest)    // No cycle would be created
            {
                MST[mstSize] = min;
                mstSize++;
                totalCost += min->distance;

                indices[Source] = Dest;

            }
            else
            {
                free(min);  // Edge would create a cycle
            }
        }

    }
    while(H->size > 0)
    {
        edge e = deleteFromHeap(H);
    }

    end = clock();
    cpu_time_used_krus = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime consumed: %f seconds\n", cpu_time_used_krus);
    if(mstSize == order - 1)
    {
        printf("-----Complete MST found with %d edges:-----\n", mstSize);
        for(int i = 0; i < mstSize; i++)
        {
            printf("Edge %d: %s - %s (weight: %d)\n",
                   i + 1, MST[i]->source->name, MST[i]->destination->name, MST[i]->distance);
        }
        printf("Total cost of MST: %d\n", totalCost);
    }
    else
    {
        printf("Incomplete MST - Graph may not be connected\n");
        for(int i = 0; i < mstSize; i++)
        {
            printf("Edge %d: %s - %s (weight: %d)\n",
                   i + 1, MST[i]->source->name, MST[i]->destination->name, MST[i]->distance);
        }
        printf("Total cost of MST: %d\n", totalCost);
    }


    for(int i = 0; i < mstSize; i++)
    {
        free(MST[i]);
    }
    free(H->heapArray);
    free(H);
}

int findIndex(int index, int arr[])
{
    if (arr[index] != index)
    {
        arr[index] = findIndex(arr[index], arr);
    }
    return arr[index];
}

Heap CreateHeap(int maxSize)
{
    Heap H = (Heap)malloc(sizeof(struct MinHeap));
    if (H == NULL)
    {
        printf("Out of memory.\n");
        return NULL;
    }

    H->heapArray = (ptr*)malloc(sizeof(ptr*) * (maxSize + 1));
    if (H->heapArray == NULL)
    {
        printf("Out of memory for heap array.\n");
        free(H);
        return NULL;
    }

    H->maxsize = maxSize + 1;
    H->size = 0;

    H->heapArray[0] = (ptr)malloc(sizeof(struct Node));
    H->heapArray[0]->destination = NULL;
    H->heapArray[0]->distance = -1;
    H->heapArray[0]->next = NULL;

    return H;
}
void minHeapify(int pos, Heap H)
{
    int left = leftChild(pos);
    int right = rightChild(pos);
    int smallest = pos;

    if (left <= H->size && H->heapArray[left]->distance < H->heapArray[smallest]->distance)
        smallest = left;

    if (right <= H->size && H->heapArray[right]->distance < H->heapArray[smallest]->distance)
        smallest = right;

    if (smallest != pos)
    {
        swap(pos, smallest, H);
        minHeapify(smallest, H);
    }
}
//function that deletes the minimum cost element in the heap
edge deleteFromHeap(Heap H)
{
    if (H->size == 0)
    {
        printf("The Heap is empty!!\n");
        return NULL;
    }

    edge minNode = H->heapArray[1];
    H->heapArray[1] = H->heapArray[H->size--];
    minHeapify(1, H);

    return minNode;
}
// swap function
Heap swap(int pos1, int pos2, Heap H)
{
    ptr temp = H->heapArray[pos1];
    H->heapArray[pos1] = H->heapArray[pos2];
    H->heapArray[pos2] = temp;
    return H;
}

//function to find the parent of a node in the heap
int parent(int pos)
{
    return (pos/2);
}

//function to find the left child in heap
int leftChild(int pos)
{
    return (pos*2);
}

//function to find the right child in heap
int rightChild(int pos)
{
    return ((pos*2) + 1);
}

//function that return true if the node in heap is leaf or false if not leaf
int isLeaf(Heap heap, int pos)
{
    return (pos > heap->size / 2 && pos <= heap->size);
}


//function to insert new node to the heap
void insertToHeap(edge e, Heap H)
{
    if (H->size >= H->maxsize) return;

    int current = ++H->size;
    H->heapArray[current] = e;

    while (current > 1 && H->heapArray[current]->distance < H->heapArray[parent(current)]->distance)
    {
        swap(current, parent(current), H);
        current = parent(current);
    }
}


