#ifndef PA3_H_
#define PA3_H_

typedef struct _Node {
    int index;
    int weight;
    struct _Node *next;
} Node;

void ConvertFile(FILE*, char*, short m, short n);
void FastestTime(Node **, int *, char *, short m, short n, int vertices, FILE *);
void FreeList(Node *head);
void FreeGraph(Node **graph, int vertices);
int* addAdjList(FILE* binFile, Node** graph, short numRow, short numCol, int vertices);
void printAdjList(Node** graph, short numRow, short numCol, int vertices);
int findNode(int *dist, bool *visited, Node **graph, int vertices);
void updateDistancePath(Node **graph, int *dist, bool *visited, int vertices, int index, int *path);
void DijkstraPath(Node **graph, int source, int vertices, int *path);
int Dijkstra(Node **graph, int source, int vertices);
void updateDistance(Node **graph, int *dist, bool *visited, int vertices, int index);
int findLength(int *path, int vertices);
void writePath(int *path, int length, FILE *file, int vertices, short row, short column);

#endif