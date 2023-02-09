#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "hbt.h"
#include "bst.h"

void BST_Read(char* input, char* output, bool* error) {

    FILE* file;
    file = fopen(input, "rb");

    fseek(file, 0, SEEK_END);
    int length = ftell(file);

    int size = length / (sizeof(int) + sizeof(char));
    fseek(file, 0, SEEK_SET);

    // Making array to keep track of operations
    int* temp_int = (int*)malloc(sizeof(int));
    char* temp_char = (char*)malloc(sizeof(char));
    int* int_array = (int*)malloc((size * sizeof(int)));
    char* char_array = (char*)malloc((size * sizeof(char)));

    // Reading the information into an array
    for(int i = 0; i < size; i++) {
        fread(temp_int, sizeof(int), 1, file);
        int_array[i] = temp_int[0];
        fread(temp_char, sizeof(char), 1, file);
        char_array[i] = temp_char[0];
    }

    Tnode* root = NULL; 
  
    for(int j = 0; j < size; j++) {
        if(char_array[j] == 'i') {
            root = Insert_Node(root, int_array[j]);
            //printf("Root Key %d\n", root->key);
            Set_BST_Balance(root);
        }
        else if(char_array[j] == 'd') {
            Delete_Node(root, int_array[j]);
            Set_BST_Balance(root);
        }
        else {
            *error = true;
        }
        //printf("Preorder: ");
        //Print_Preorder(root);
        //printf("\n\n");
    }

    /*printf("\n");
    printf("Preorder ");
    Print_Preorder(root);
    printf("\n");
    printf("Balance ");
    Print_Preorder_Balance(root);
    printf("\n");*/

    FILE* outFile;
    outFile = fopen(output, "wb");
    int* num = malloc(sizeof(int));
    char* children = malloc(sizeof(char)); 
    BST_Write(root, outFile, num, children);

    // Freeing all the arrays and closing the file
    free(num);
    free(children);
    free(temp_int);
    free(temp_char);
    free(int_array);
    free(char_array);
    Free_BST(root);
    fclose(file);
    fclose(outFile);
}

void BST_Write(Tnode* node, FILE* filename, int* num, char* children) {
    if(node != NULL) {
        num[0] = node->key;
        fwrite(num, sizeof(int), 1, filename);
        if(node->right == NULL && node->left == NULL) {
            //printf("Node %d\n", node->key);
            children[0] = 0;
            //printf("Children for this Node %c\n", children[0]);
            fwrite(children, sizeof(char), 1, filename); 
        }
        else if(node->right != NULL && node->left == NULL) {
            //printf("Node %d\n", node->key);
            children[0] = 1;
            //printf("Children for this Node %c\n", children[0]); 
            fwrite(children, sizeof(char), 1, filename);
        }
        else if(node->right == NULL && node->left != NULL) {
            //printf("Node %d\n", node->key);
            children[0] = 2;
            //printf("Children for this Node %c\n", children[0]);
            fwrite(children, sizeof(char), 1, filename);
        }
        else {
            //printf("Node %d\n", node->key);
            children[0] = 3;
            //printf("Children for this Node %c\n", children[0]);
            fwrite(children, sizeof(char), 1, filename);
        }
        BST_Write(node->left, filename, num, children);
        BST_Write(node->right, filename, num, children);
    }
}

static int Get_Node_Balance(Tnode* node) {
    if(node != NULL) {
        int balance = node->balance;
        return balance;
    } 
    else {
        return 0;
       }
}

void Set_BST_Balance(Tnode* node) { 
    if(node != NULL) {
        int balance = Get_BST_Height(node->left) - Get_BST_Height(node->right);
        node->balance = balance;
        Set_BST_Balance(node->left);
        Set_BST_Balance(node->right); 
    }
}

int Get_BST_Height(Tnode* node) {
    if(node == NULL) {
        return 0;
    }
    else {
        int left = Get_BST_Height(node->left);
        int right = Get_BST_Height(node->right);

        if (left > right) {
            return (left + 1);
        }
        else if(right > left) {
            return (right + 1);
        }
        else {
            int same = right + 1;
            return same;
        }
    }
}

Tnode* Rebalance(Tnode* node, int bal_of_Current) {

    //printf("Node: %d Bal: %d \n", node->key, bal_of_Current);
    
    if(bal_of_Current > 1) {
        if((node->left->balance == 1)) {
            //printf("Rotated! \n");
            Tnode* new_root = CR_Rotation(node);
            Set_BST_Balance(node);
            return new_root;
        }
        else if(node->left->balance == -1) {
            //printf("Rotated! \n");
            node->left = CC_Rotation(node->left);
            Tnode* new_root = CR_Rotation(node);
            Set_BST_Balance(node);
            return new_root; 
        }
    }
    else if(bal_of_Current < -1) {
        if(node->right->balance == 1) {
            //printf("Rotated! \n");
            node->right = CR_Rotation(node->right);
            Tnode* new_root = CC_Rotation(node);
            Set_BST_Balance(node);
            return new_root;  
        }
        else if(node->right->balance == -1) {
            //printf("Rotated! \n");
            Tnode* new_root = CC_Rotation(node);
            Set_BST_Balance(node);
            return new_root; 
        }
    }

    return node;
}

Tnode* Insert_Node(Tnode* node, int val) {

    if(node == NULL) {
        //printf("Made new node: %d\n", val);
        node = malloc(sizeof(*node));
        *node = (Tnode) {.key = val, .balance = 0, .left = NULL, .right = NULL};
        return(node);
    }
    else if(val > node->key) {
        //printf("Going right ");
        node->right = Insert_Node(node->right, val);
    }
    else if(val <= node->key) {
        //printf("Going left ");
        node->left = Insert_Node(node->left, val); 
    }


    Set_BST_Balance(node);
    //printf("After Node: %d, Bal: %d", node->key, node->balance);
    int bal_of_Current = Get_Node_Balance(node);
    node = Rebalance(node, bal_of_Current);

    // Return unchanged root if balance is not > 1 or < -1
    return node;
}

Tnode* Delete_Node(Tnode* node, int val) {
    
    Tnode* temp = NULL;

    if(node == NULL) {
        //printf("Node not found\n");
        return(node);
    }
    else if(val > node->key) {
        //printf("Searching Right ");
        node->right = Delete_Node(node->right, val);
    }
    else if(val < node->key) {
        //printf("Searching Left ");
        node->left = Delete_Node(node->left, val);
    }
    else if(val == node->key) {
        //printf("Key Found!\n");
        //printf("Node Del Key: %d ", val);
        int numChild;
        // no child
        if(node->left == NULL && node->right == NULL) {
            numChild = 0;
        }
        // right child only
        else if(node->left == NULL && node->right != NULL) {
            numChild = 1;
        }
        // left child only
        else if(node->left != NULL && node->right == NULL) {
            numChild = 2;
        }
        // both left and right child
        else {
            numChild = 3;
        }

        if(numChild == 0) {
            node = NULL;
        }
        else if(numChild == 1) {
            temp = node->right;
            free(node);
            return(temp);
        }
        else if(numChild == 2) {
            temp = node->left;
            free(node);
            return(temp);
        }
        else if(numChild == 3) {
            temp = Predecessor(node->left);
            node->key = temp->key;
            int new_key = temp->key;
            Tnode* prev = node;

            //Delete_Pred_Node(node->left, new_key);
            node->left = Delete_Pred(node->left, new_key, prev);
        }
        Set_BST_Balance(node);
    }

    Set_BST_Balance(node);
    int bal_of_Current = Get_Node_Balance(node);

    node = Rebalance(node, bal_of_Current);

    // Return unchanged root if balance is not > 1 or < -1
    return node; 
}

Tnode* Delete_Pred(Tnode* node, int key, Tnode* prev) {
   if(node->key == key && node->right == NULL && node->left == NULL) {
        free(node);
        return(NULL);
   } 
   else if(node->key == key && node->right == NULL && node->left != NULL) {
        //prev->right = node->left;
        Tnode* temp = node->left;
        free(node);
        return(temp);
   }
   else {
        prev = node;
        node->right = Delete_Pred(node->right, key, prev);
   }

    Set_BST_Balance(node);
    int bal_of_Current = Get_Node_Balance(node);

    node = Rebalance(node, bal_of_Current);

    // Return unchanged root if balance is not > 1 or < -1
    return node; 
}

Tnode* Predecessor(Tnode* node) {
    while(node->right != NULL) {
        node = node->right;
    }
    return node;
}

Tnode* CC_Rotation(Tnode* old_root) {
    Tnode* new_root = old_root->right;
    old_root->right = new_root->left;
    new_root->left = old_root;
    
    return new_root;
}

Tnode* CR_Rotation(Tnode* old_root) {
    Tnode* new_root = old_root->left;
    old_root->left = new_root->right;
    new_root->right = old_root;

    return new_root;
}