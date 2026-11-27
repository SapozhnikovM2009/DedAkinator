#include "treelib.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Wrong arguments amount, need 3, got %d\n", argc);
        return 0;
    }
    Tree tree = {};
    ReadPrefixFromFile(&tree, "out.txt");

    DiffWays(&tree, argv[1], argv[2]);

    DestroyTree(&tree);
    return 0; 
}
