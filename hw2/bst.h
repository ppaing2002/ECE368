#ifndef BST_H_   
#define BST_H_

// Functions to build BST
void BST_Read(char* input, char* output, bool* error);
void BST_Write(Tnode* node, FILE* filename, int* num, char* children);
void Delete_Pred_Node(Tnode* node, int key);
Tnode* Insert_Node(Tnode* node, int val);
Tnode* Delete_Node(Tnode* node, int val);
Tnode* Delete_Pred(Tnode* node, int key, Tnode* prev);
Tnode* Rebalance(Tnode* node, int bal);
Tnode* Predecessor(Tnode* node);
void Set_BST_Balance(Tnode* node);
int Get_BST_Height(Tnode* node);
Tnode* CC_Rotation(Tnode* old_root);
Tnode* CR_Rotation(Tnode* old_root);

// Functions to evaluate BST
int Valid_File(char* filename);
int Valid_BST(char* filename, int* hbt);
void Valid_HBT(Tnode** node, int* valid);
void Free_BST(Tnode* node);

void Print_Preorder_Balance(Tnode *root);
void Print_Preorder(Tnode *root);

#endif