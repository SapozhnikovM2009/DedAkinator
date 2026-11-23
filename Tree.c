#include <stdio.h>
#include "treelib.h"

int main() {
    Tree tree = {.root = NULL};
    TreeNode_t *tnt = CreateNode(50);
    SetRoot(&tree, tnt);
    
    TreeNode_t *tnt1 = CreateNode(20);
    TreeNode_t *tnt2 = CreateNode(10);
    TreeNode_t *tnt3 = CreateNode(30);
    TreeNode_t *tnt4 = CreateNode(60);
    TreeNode_t *tnt5 = CreateNode(52);
    TreeNode_t *tnt6 = CreateNode(67);

    LinkLeft(tnt, tnt1);
    LinkLeft(tnt1, tnt2);
    LinkRight(tnt1, tnt3);
    LinkRight(tnt, tnt4);
    LinkLeft(tnt4, tnt5);
    //LinkRight(tnt4, tnt6);

    
    
    PrefixOutFile(tnt, "out.txt");
    
    Tree readed_tree = {.root = NULL};
    ReadPrefixFromFile(&readed_tree, "out.txt");
    //printf("\n%p\n", readed_tree.root);
    //DumpNode(readed_tree.root);
    //DumpNode(readed_tree.root->left);
    //DumpNode(readed_tree.root->right);
    CreateFileForNodeGraphicDump(readed_tree.root);
    DestroyTree(&tree);
    return 0; 
}