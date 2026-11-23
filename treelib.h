#ifndef TREELIB_H
#define TREELIB_H

#include <stdint.h>
#include <stdio.h>
#include <time.h>


#define FILE_NAME_LENGTH 26

enum {
    ERROR_OUTPUT = -1,
    NORMAL_OUTPUT = 0
};

typedef struct TreeNode_t TreeNode_t;

struct TreeNode_t{
    double value;
    struct TreeNode_t *left;
    struct TreeNode_t *right;
};

typedef struct{
    TreeNode_t *root;
}Tree;

TreeNode_t *CreateNode(double value);
void SetRoot(Tree *tree, TreeNode_t *node);
void DumpNode(TreeNode_t *node);
void DestroyTree(Tree *tree);
void LinkLeft(TreeNode_t *node, TreeNode_t *l_node);
void LinkRight(TreeNode_t *node, TreeNode_t *r_node);
void NodeGraphicDump(TreeNode_t *node, char *file_name);
void GetSubTreeDepth(TreeNode_t *node, int32_t depth, int32_t* max_depth);
void CreateTestTree(TreeNode_t *node, int32_t curr_depth, int32_t depth);
void CreateFileForNodeGraphicDump(TreeNode_t *node);
void SetDOTFileName(char *file_name, time_t now);
char *SystemCallArg(char *dot_file, char *svg_file);
void PrefixOutFile(const TreeNode_t *node, char *output_file);
void InfixOutFile(const TreeNode_t *node, char *output_file);
void PostfixOutFile(const TreeNode_t *node, char *output_file);
Tree *ReadPrefixFromFile(Tree *tree, char *output_file);
void BlanckedScanf(FILE *fp, char *buff);

#endif