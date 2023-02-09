#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "pa3.h"
#define INFINITY INT_MAX

void printAdjList(Node** graph, short numRow, short numCol, int vertices) {
    
    // Print out the adjacent list
    printf("Adjacent List: \n");
    for(int i = 0; i < vertices; i++) {
        printf("Adjacent Nodes for Index %d: ", i);
        if(graph[i] == NULL) {
            printf("None");
        }
        else {
            Node* current = graph[i];
            while(current != NULL) {
                printf("%d ", current->index);
                current = current->next;
            }
        }
        printf("\n");
    }
}

void ConvertFile(FILE* binaryFile, char* textFile, short m, short n) {

    // Open the text file
    FILE* text = fopen(textFile, "w");
    short temp = 0; 

    // Write the size of the grid
    fprintf(text, "%hd %hd\n", m, n);

    // Write the grid values
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            fread(&temp, sizeof(short), 1, binaryFile);
            fprintf(text, "%hd", temp);
            if((j + 1) != n) {
                fprintf(text, " ");
            }
        }
        fprintf(text, "\n");
    }

    // Free variables and close all files
    fseek(binaryFile, 0, SEEK_SET);
    fclose(text);
}

int findNode(int* dist, bool* visited, Node** graph, int vertices) {

    // Find the minimum distance node
    int min = INFINITY;
    int minIndex = -1;

    for(int v = 0; v < vertices; v++) {
        if(visited[v] == false && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void updateDistancePath(Node** graph, int* dist, bool* visited, int vertices, int index, int* path) {
    
    // Update the distance of the nodes
    if(graph[index] != NULL) {
        int numAdj = 0;
        Node* current = graph[index];
        while(current != NULL) {
            numAdj += 1;
            current = current->next;
        }
        current = graph[index];
        for(int v = 0; v < numAdj; v++) {
            int adjNode = current->index;
            if(visited[adjNode] == false && dist[index] != INFINITY) {
                if(dist[index] + graph[index]->weight < dist[adjNode]) {
                    dist[adjNode] = dist[index] + graph[index]->weight;
                    path[adjNode] = index;
                }
            }
            current = current->next;
        }
    }
}

void DijkstraPath(Node** graph, int source, int vertices, int* path) {

    // Make an array for all node distances from souce and intialize all as INF
    int* dist = (int*)malloc(sizeof(int) * vertices);
    for(int i = 0; i < vertices; i++) {
        dist[i] = INFINITY;
    }

    // Make the source distance as zero
    dist[source] = 0;

    // Make an array to keep track of visited vertices
    bool* visited = (bool*)malloc(sizeof(bool) * vertices);
    for(int k = 0; k < vertices; k++) {
        visited[k] = false;
    }

    int u;
    for(int i = 0; i < vertices; i++) {
        u = findNode(dist, visited, graph, vertices);
        visited[u] = true;

        updateDistancePath(graph, dist, visited, vertices, u, path);
    }

    // Free all allocated memory
    free(visited);
    free(dist);
}

int Dijkstra(Node** graph, int source, int vertices) {

    // Make an array for all node distances from souce and intialize all as INF
    int* dist = (int*)malloc(sizeof(int) * vertices);
    for(int i = 0; i < vertices; i++) {
        dist[i] = INFINITY;
    }

    // Make the source distance as zero
    dist[source] = 0;

    // Make an array to keep track of visited vertices
    bool* visited = (bool*)malloc(sizeof(bool) * vertices);
    for(int k = 0; k < vertices; k++) {
        visited[k] = false;
    }

    int u;
    for(int i = 0; i < vertices; i++) {
        u = findNode(dist, visited, graph, vertices);
        visited[u] = true;

        updateDistance(graph, dist, visited, vertices, u);
    }

    int pathNode = dist[vertices - 1];

    // Free all allocated memory
    free(visited);
    free(dist);

    return pathNode;
}

void updateDistance(Node** graph, int* dist, bool* visited, int vertices, int index) {
    
    // Update the distance of the nodes
    if(graph[index] != NULL) {
        int numAdj = 0;
        Node* current = graph[index];
        while(current != NULL) {
            numAdj += 1;
            current = current->next;
        }
        current = graph[index];
        for(int v = 0; v < numAdj; v++) {
            int adjNode = current->index;
            if(visited[adjNode] == false && dist[index] != INFINITY) {
                if(dist[index] + graph[index]->weight < dist[adjNode]) {
                    dist[adjNode] = dist[index] + graph[index]->weight;
                }
            }
            current = current->next;
        }
    }
}

int findLength(int* path, int vertices) {
    int current = vertices - 1;
    int prev = path[current];
    int length = 0;

    while(prev != -1) {
        length++;
        current = prev;
        prev = path[current];
    }

    return length;
}

void writePath(int* path, int length, FILE* file, int vertices, short row, short column) {

    // Create an array to store indexes
    int* pathIndexes = (int*)malloc(sizeof(int) * (length + 1));

    // Set the array
    int current = vertices - 1;
    int prev = path[current];
    int i = 1;
    pathIndexes[0] = prev;

    while(prev != -1) {
        current = prev;
        prev = path[current];
        pathIndexes[i] = prev;
        i++;
    }

    for(int j = length - 1; j >= 0; j--) {
        short rowNum = pathIndexes[j] / column;
        short colNum = pathIndexes[j] % column;
        while(colNum > column) {
            colNum %= column;
        }
        //printf("Row %hd Column %hd\n", rowNum, colNum);
        fwrite(&rowNum, sizeof(short), 1, file);
        fwrite(&colNum, sizeof(short), 1, file);
    }

    free(pathIndexes);
}

void FastestTime(Node** graph, int* weights, char* filename, short m, short n, int vertices, FILE* pathFile) {

    // Open the binary file
    FILE* file = fopen(filename, "wb");

    // Write the number of columns to the file
    fwrite(&n, sizeof(short), 1, file);

    // Make an array to keep track of the path with different sources
    int* path = (int*)malloc(sizeof(int) * vertices);

    // Initialize path to none
    for(int i = 0; i < vertices; i++) {
        path[i] = -1;
    }

    // Create the array for the times with different sources
    int* times = (int*)malloc(sizeof(int) * n);

    for(int j = 0; j < n; j++) {
        int val = Dijkstra(graph, j, vertices);
        times[j] = val;
        //printf("Value from Dij %d\n", val);
        fwrite(&val, sizeof(int), 1, file);
    }

    // Find the shortest time from all sources
    int shortestTime = INT_MAX;
    int source = -1;
    for(int t = 0; t < n; t++) {
        if(times[t] < shortestTime) {
            shortestTime = times[t];
            source = t;
        }
    }

    // Find the path of the source with shortest path
    DijkstraPath(graph, source, vertices, path);

    // Write the shortest time
    fwrite(&shortestTime, sizeof(int), 1, pathFile);

    int pathLength = findLength(path, vertices);
    //printf("Path Length %d\n", pathLength);

    fwrite(&pathLength, sizeof(int), 1, pathFile);

    writePath(path, pathLength, pathFile, vertices, m, n);

    free(path);
    free(times);
    fclose(file);
}

void FreeList(Node* head)
{
   Node* tmp;
   while (head != NULL) {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

void FreeGraph(Node** graph, int vertices) {
    // Free the list of nodes
    for(int i = 0; i < vertices; i++) {
        FreeList(graph[i]);
    }
    free(graph);
}

int* addAdjList(FILE* file, Node** graph, short numRow, short numCol, int vertices) {

    // Set a temporary value for the weight
    short temp = 0; 

    // Move pointer by 2 shorts
    fread(&temp, sizeof(short), 2, file);

    // Create a weight array
    int* weight = (int*)malloc(sizeof(int) * vertices);

    // Get adjacent values 
    for(int i = 0; i < numRow; i++) {
        for(int j = 0; j < numCol; j++) {
            // Calculate which node we are at
            int r = i * numCol;
            int curIndex = r + j;

            // Get the weight of the current index
            fread(&temp, sizeof(short), 1, file);
            weight[curIndex] = temp;

            // Check if this node has top, bottom, left, right nodes
            // After checking if there exists a node, start or add to the adjacency list

            // Has top node
            if(i - 1 >= 0) {
                int topIndex = ((i - 1) * numCol) + j;
                Node* newNode = (Node*)malloc(sizeof(Node));
                *newNode = (Node) {.index = topIndex, .next = NULL, .weight = temp};

                // Check if there is a node in the index already
                if (graph[curIndex] == NULL) {
                    graph[curIndex] = newNode; 
                }
                // If not add to list
                else {
                    Node* current = graph[curIndex];
                    while(current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newNode;
                }
            }
            // Has bottom node
            if(i + 1 < numRow) {
                int bottomIndex = ((i + 1) * numCol) + j;
                Node* newNode = (Node*)malloc(sizeof(Node));
                *newNode = (Node) {.index = bottomIndex, .next = NULL, .weight = temp};

                // Check if there is a node in the index already
                if (graph[curIndex] == NULL) {
                    graph[curIndex] = newNode; 
                }
                // If not add to list
                else {
                    Node* current = graph[curIndex];
                    while(current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newNode;
                }
            }
            // Has left node
            if(j - 1 >= 0) {
                int leftIndex = (i * numCol) + (j - 1);
                Node* newNode = (Node*)malloc(sizeof(Node));
                *newNode = (Node) {.index = leftIndex, .next = NULL, .weight = temp};

                // Check if there is a node in the index already
                if (graph[curIndex] == NULL) {
                    graph[curIndex] = newNode; 
                }
                // If not add to list
                else {
                    Node* current = graph[curIndex];
                    while(current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newNode; 
                }
            }
            // Has right node
            if(j + 1 < numCol) {
                int rightIndex = (i * numCol) + (j + 1);
                Node* newNode = (Node*)malloc(sizeof(Node));
                *newNode = (Node) {.index = rightIndex, .next = NULL, .weight = temp};
                
                // Check if there is a node in the index already
                if (graph[curIndex] == NULL) {
                    graph[curIndex] = newNode; 
                }
                // If not add to list
                else {
                    Node* current = graph[curIndex];
                    while(current->next != NULL) {
                        current = current->next;
                    }
                    current->next = newNode;
                }
            }
        }
    }

    int destIndex = vertices - 1;
    // Make the bottom row adjacent to destination node
    for(int i = vertices - numCol - 1; i < vertices - 1; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        *newNode = (Node) {.index = destIndex, .next = NULL, .weight = weight[i]};
        if (graph[i] == NULL) {
            graph[i] = newNode; 
        }
        else {
            Node* current = graph[i];
            while(current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        } 
    }
    return weight;
}