#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "pa3.h"

int main(int argc, char* argv[]) {

    if(argc == 5) {
        char* binaryGrid = argv[1];
        char* textGrid = argv[2];
        char* fastestTime = argv[3];
        char* fastestPath = argv[4];

        // Open the binary file and get the number of rows and columns
        FILE* file = fopen(binaryGrid, "rb");
        FILE* path = fopen(fastestPath, "wb");
        short m = 0;
        short n = 0; 
        
        fread(&m, sizeof(short), 1, file);
        fread(&n, sizeof(short), 1, file);

        int vertices = (m * n) + 1;
        
        Node** graph = (Node**)malloc(sizeof(Node*) * vertices);

        // Initialize each node to NULL
        for(int i = 0; i < vertices; i++) {
            graph[i] = NULL;
        }

        // Convert file 
        ConvertFile(file, textGrid, m, n);

        // Complete the graph by linking the adjacent nodes
        int* weightList = addAdjList(file, graph, m, n, vertices);
        // Initialize destination node
        weightList[vertices - 1] = 0;

        // Check adjacent nodes list
        //printAdjList(graph, m, n, vertices);

        // Create the fastest times file
        FastestTime(graph, weightList, fastestTime, m, n, vertices, path);

        // Free and close all file
        FreeGraph(graph, vertices);
        fclose(file);
        fclose(path);
        free(weightList);
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}