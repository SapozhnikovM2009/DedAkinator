#include <stdio.h>
#include "treelib.h"

int main() {
    Tree tree = {};
    // TreeNode_t *tnt = CreateNode("Animal");
    // SetRoot(&tree, tnt);
    
    // TreeNode_t *tnt1 = CreateNode("1.5");
    // TreeNode_t *tnt2 = CreateNode("Teaching calculus");
    // TreeNode_t *tnt3 = CreateNode("Redkozubov");
    // TreeNode_t *tnt4 = CreateNode("Dedinskiy");

    // LinkLeft(tnt, tnt1);
    // LinkRight(tnt, tnt2);
    // LinkLeft(tnt2, tnt3);
    // LinkRight(tnt2, tnt4);

    ReadPrefixFromFile(&tree, "out.txt");

    //FindWay(&tree, "Kalinich");
    //StartGame(&tree);
    //DiffWays(&tree, "Varya", "Sanya");

    //PrefixOutFile(tree.root, "out.txt");
    //TreeGDToNewFile(tree.root);
    DestroyTree(&tree);
    return 0; 
}
