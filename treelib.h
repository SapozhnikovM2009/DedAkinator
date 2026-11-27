#ifndef TREELIB_H
#define TREELIB_H

#include <stdint.h>
#include <stdio.h>
#include <time.h>


#define FILE_NAME_LENGTH 26
#define MAX_VAL_SIZE 10000
#define MAX_SP_SIZE "9999"

#define MAX_ANS_LEN 6

#define GAME_IN_PROCESS 1
#define ANS_NOT_GET 1

#define ONE (TreeNode_t*)1

enum {
    NOT_FOUND = -1,
    FOUND = 1
};

enum {
    ERROR_OUTPUT = -1,
    NORMAL_OUTPUT = 0
};

enum {
    NEGATIVE = -1,
    POSITIVE = 1
};

typedef struct TreeNode_t TreeNode_t;

struct TreeNode_t{
    char *value;
    struct TreeNode_t *left;
    struct TreeNode_t *right;
};

typedef struct{
    TreeNode_t *root;
}Tree;

TreeNode_t *CreateNode(char *value);
void SetRoot(Tree *tree, TreeNode_t *node);
void DumpNode(TreeNode_t *node);
void DestroyTree(Tree *tree);
void LinkLeft(TreeNode_t *node, TreeNode_t *l_node);
void LinkRight(TreeNode_t *node, TreeNode_t *r_node);
void NodeGraphicDump(TreeNode_t *node, char *file_name);
void GetSubTreeDepth(TreeNode_t *node, int32_t depth, int32_t* max_depth);
void TreeGDToNewFile(TreeNode_t *node);
void SetDOTFileName(char *file_name, time_t now);
char *SystemCallArg(char *dot_file, char *svg_file);
void PrefixOutFile(const TreeNode_t *node, char *output_file);
void InfixOutFile(const TreeNode_t *node, char *output_file);
void PostfixOutFile(const TreeNode_t *node, char *output_file);
void ReadPrefixFromFile(Tree *tree, char *output_file);
void BlanckedScanf(FILE *fp, char *buff);
TreeNode_t *UnlinckLeft(TreeNode_t *node);
TreeNode_t *UnlinckRight(TreeNode_t *node);
void StartGame(Tree *tree);
void SplitAkinator(char** data, char* str, int size_str);
int CountStringsAndSetEnds(char* str, int size);
void FindWay(Tree *tree, const char *object);
void DiffWays(Tree *tree, const char *object1, const char *object2);


#endif