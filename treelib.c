#include "treelib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


    
// FIXME: AKINATOR 5
// TODO*: озвучка текста (espeak) + флаг отключения 6
// TODO**: графический интерфейс к акинатору 7

// TODO: посчитать глубину дерева 1 DONE
// TODO: man system, man fork + exec 3 DONE
// TODO: dated dumps 2 DONE
// TODO: verificator 4

TreeNode_t *CreateNode(double value) {
    TreeNode_t *nodep = calloc(1, sizeof(TreeNode_t));
    if (nodep == NULL) {
        fprintf(stderr, "Can't create node\n");
        return NULL;
    }
    nodep->value = value;
    return nodep;
}

void SetRoot(Tree *tree, TreeNode_t *node) {
    tree->root = node;
}


void DumpNode(TreeNode_t *node) {
    for (int i = 0; i < 30; i++) printf("-");
    printf("\n");
    printf("Node:\n");
    printf("Address: %p\n", node);
    printf("Left: %p\n", node->left);
    printf("Right: %p\n", node->right);
    for (int i = 0; i < 30; i++) printf("-");
    printf("\n");
}

static void DestroyNode(TreeNode_t *node) {
    if (node == NULL) return;
    DestroyNode(node->left);
    DestroyNode(node->right);
    free(node);
}

void DestroyTree(Tree *tree) {
    DestroyNode(tree->root);
}


void LinkLeft(TreeNode_t *node, TreeNode_t *l_node) {
    if (node->left != NULL) {
        fprintf(stderr, "You are trying to clay Node to Node, that already have left Node\n");
        return;
    }
    node->left = l_node;
}

void LinkRight(TreeNode_t *node, TreeNode_t *r_node) {
    if (node->right != NULL) {
        fprintf(stderr, "You are trying to clay Node to Node, that already have right Node\n");
        return;
    }
    node->right = r_node;
}

static void GraphicInfoDump(TreeNode_t *node, FILE* out_file, uint32_t depth, uint32_t number) {
    fprintf(out_file, "\t%d[shape = record, fillcolor = \"lightgreen\", color = \"green\", style = \"filled, rounded\", ", number);
    fprintf(out_file, "label = \" {value : %lg | {address : %p} | {left : %p | right : %p}}\"];\n", node->value, node, node->left, node->right);

    if (node->left != NULL) {
        fprintf(out_file, "\t%d->%d;\n", number, 2 * number + 1);
        GraphicInfoDump(node->left, out_file, depth + 1, 2 * number + 1);
    }
    if (node->right != NULL) {
        fprintf(out_file, "\t%d->%d;\n", number, 2 * number + 2);
        GraphicInfoDump(node->right, out_file, depth + 1, 2 * number + 2);
    }

    fprintf(out_file, "\t{rank = same; %d; a%d}\n", number, depth);
}

void NodeGraphicDump(TreeNode_t *node, char *file_name) {
    FILE* out_file = fopen(file_name, "w");
    if (out_file == NULL)
    {
        fprintf(stderr, "Can't open file %s\n", file_name);
        return;
    }
    fprintf(out_file, "digraph structs\n");
    fprintf(out_file, "{\n");
    fprintf(out_file, "\trankdir = UD;\n");
    fprintf(out_file, "\t");

    int32_t depth = -1;
    GetSubTreeDepth(node, 0, &depth);
    printf("%d", depth);
    for (int i = 0; i < depth + 1; i++) {
        fprintf(out_file, "\ta%d[fillcolor = transparent, color = transparent, label = \"\"]\n", i);
    }
    fprintf(out_file, "\t");
    for (int i = 0; i < depth; i++) {
        fprintf(out_file, "a%d->", i);
    }
    fprintf(out_file, "a%d[color = transparent];\n", depth);
    GraphicInfoDump(node, out_file, 0, 0);

    fprintf(out_file, "\n}\n");
    fclose(out_file);
}



void GetSubTreeDepth(TreeNode_t *node, int32_t depth, int32_t* max_depth) {
    if (depth > *max_depth) *max_depth = depth;
    if (node->left != NULL) {
        GetSubTreeDepth(node->left, depth + 1, max_depth);
    }
    if (node->right != NULL) {
        GetSubTreeDepth(node->right, depth + 1, max_depth);
    }
}

void CreateTestTree(TreeNode_t *node, int32_t curr_depth, int32_t depth) {
    if (curr_depth < depth) {
        TreeNode_t* l_node = CreateNode(curr_depth * 2 + 1);
        LinkLeft(node, l_node);
        TreeNode_t* r_node = CreateNode(curr_depth * 2 + 2);
        LinkRight(node, r_node);
        CreateTestTree(l_node, curr_depth + 1, depth);
        CreateTestTree(r_node, curr_depth + 1, depth);
    }
}

void CreateFileForNodeGraphicDump(TreeNode_t *node) {
    char dot_file_name[FILE_NAME_LENGTH] = {};
    time_t now = time(NULL);
    
    SetDOTFileName(dot_file_name, now);
    
    FILE* svg_out_file = fopen(dot_file_name, "w");
    if (svg_out_file == NULL)
    {
        fprintf(stderr, "Can't open file %s\n", dot_file_name);
        return;
    }
    
    fprintf(svg_out_file, "digraph structs\n");
    fprintf(svg_out_file, "{\n");
    fprintf(svg_out_file, "\trankdir = UD;\n");
    fprintf(svg_out_file, "\t");

    int32_t depth = -1;
    
    GetSubTreeDepth(node, 0, &depth);
    for (int i = 0; i < depth + 1; i++) {
        fprintf(svg_out_file, "\ta%d[fillcolor = transparent, color = transparent, label = \"\"]\n", i);
    }
    fprintf(svg_out_file, "\t");
    for (int i = 0; i < depth; i++) {
        fprintf(svg_out_file, "a%d->", i);
    }
    fprintf(svg_out_file, "a%d[color = transparent];\n", depth);
    GraphicInfoDump(node, svg_out_file, 0, 0);

    fprintf(svg_out_file, "\n}\n");
    fclose(svg_out_file);

    char *arg = SystemCallArg(dot_file_name, "graph.svg");
    //printf("<%s>\n", arg);
    system(arg);
    free(arg);
}

void SetDOTFileName(char *file_name, time_t now) {
    
    
    strcpy(file_name, ctime(&now));
    for (int i = 8; file_name[i] != '\0'; i++) {
        if (file_name[i] == ' ' || file_name[i] == ':') {
            file_name[i - 8] = '_';
        }
        else {
            file_name[i - 8] = file_name[i];
        }
        if (file_name[i] == '\n') {
            file_name[i - 8] = '.';
            file_name[i - 7] = 'd';
            file_name[i - 6] = 'o';
            file_name[i - 5] = 't';
            file_name[i - 4] = '\0';
        }
    }
}

char *SystemCallArg(char *dot_file, char *svg_file) {
    int32_t size_d = strlen(dot_file);
    int32_t size_s = strlen(svg_file);
    int32_t size_arg = size_d + size_s + 15;
    char *arg = calloc(size_arg, sizeof(char));

    char buf_str[10] = "dot -Tsvg ";
    for (int i = 0; i < 10; i++) {
        arg[i] = buf_str[i];
    }
    for (int i = 10; i < 10 + size_d; i++) {
        arg[i] = dot_file[i - 10];
    }
    arg[10 + size_d] = ' ';
    char buf_str2[4] = "-o ";
    for (int i = 11 + size_d; i < 11 + 3 + size_d; i++) {
        arg[i] = buf_str2[i - 11 - size_d];
    }
    for (int i = 11 + 3 + size_d; i < 11 + 3 + size_d + size_s; i++) {
        arg[i] = svg_file[i - 11 - 3 - size_d];
    }

    return arg;
}

static void PrefixPrintFile(const TreeNode_t *node, FILE *out_file) {

    fprintf(out_file, "( ");
    if (node != NULL) fprintf(out_file, "%lg ", node->value);
    if (node->left != NULL) {
        PrefixPrintFile(node->left, out_file);
    }
    if (node->right != NULL) {
        PrefixPrintFile(node->right, out_file);
    }
    fprintf(out_file, ") ");
}

void PrefixOutFile(const TreeNode_t *node, char *output_file) {
    FILE *out_file = fopen(output_file, "w");
    if (out_file == NULL) {
        fprintf(stderr, "Can't open file <%s> for prefix output\n", output_file);
        return;
    }
    PrefixPrintFile(node, out_file);
    fclose(out_file);
}

static TreeNode_t *ReadPrefixMakeTree(FILE *out_file) {
    double val = 0;
    fscanf(out_file, "%lg", &val);
    TreeNode_t *node = CreateNode(val);
    char s[100];
    fscanf(out_file, "%99s", s);
    if (strcmp(s, ")") == 0) {
        return node;
    }
    if (strcmp(s, "(") == 0) {
        TreeNode_t *l_node = ReadPrefixMakeTree(out_file);
        LinkLeft(node, l_node);
        fscanf(out_file, "%99s", s);
        if (strcmp(s, ")") == 0) {
            return node;
        }
        if (strcmp(s, "(") == 0) {
            TreeNode_t *r_node = ReadPrefixMakeTree(out_file);
            LinkRight(node, r_node);
            fscanf(out_file, "%99s", s);
            if (strcmp(s, ")") == 0) {
                return node;
            }
        }
    }
}

Tree *ReadPrefixFromFile(Tree *tree, char *output_file) {
    FILE *out_file = fopen(output_file, "r");
    if (out_file == NULL) {
        fprintf(stderr, "Can't open file %s for reading prefix notation\n", output_file);
        return NULL;
    }
    char s[100];
    fscanf(out_file, "%99s", s);
    TreeNode_t *node = ReadPrefixMakeTree(out_file);
    SetRoot(tree, node);
    fclose(out_file);
    return tree;
}



static void InfixPrintFile(const TreeNode_t *node, FILE *out_file) {

    fprintf(out_file, "( ");
    if (node->left != NULL) {
        InfixPrintFile(node->left, out_file);
    }
    fprintf(out_file, "%lg", node->value);
    if (node->right != NULL) {
        InfixPrintFile(node->right, out_file);
    }
    fprintf(out_file, ")");
}

void InfixOutFile(const TreeNode_t *node, char *output_file) {
    FILE *out_file = fopen(output_file, "w");
    if (out_file == NULL) {
        fprintf(stderr, "Can't open file <%s> for prefix output\n", output_file);
        return;
    }
    InfixPrintFile(node, out_file);
    fclose(out_file);
}

static void PostfixPrintFile(const TreeNode_t *node, FILE *out_file) {

    fprintf(out_file, "( ");
    if (node->left != NULL) {
        PostfixPrintFile(node->left, out_file);
    }
    
    if (node->right != NULL) {
        PostfixPrintFile(node->right, out_file);
    }
    fprintf(out_file, "%lg", node->value);
    fprintf(out_file, ")");
}

void PostfixOutFile(const TreeNode_t *node, char *output_file) {
    FILE *out_file = fopen(output_file, "w");
    if (out_file == NULL) {
        fprintf(stderr, "Can't open file <%s> for prefix output\n", output_file);
        return;
    }
    PostfixPrintFile(node, out_file);
    fclose(out_file);
}

void BlanckedScanf(FILE *fp, char *buff) {
    //Legendary function
    char c = '\0';
    for (int i = 0; c != ')' && c != '('; i++) {
        c = getc(fp);
        if (c == EOF) {
            fprintf(stderr, "Strange behaviour: BlanckedScanf readed EOF, succesfully readed: <%s>\n", buff);
            return;
        }
        buff[i] = c;
    }
    ungetc(c, fp);
}
