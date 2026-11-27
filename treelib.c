#include "oneginlib.h"
#include "treelib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "AddressStacklib.h"

// FIXME: segfault on EOF (почитать про то, что делает ctrl-d в консоли) DONE
// FIXME: memory leaks on getline DONE

// FIXME: если не найден объект сказать об этом DONE
// TODO: сделать прогу, где можно ввести через argv ввести 2 имени и сравнить их пути DONE

// +- TODO: если указатель не меняю, то его надо делать const

// FIXME: fix bugs (по коду)

// - TODO: verificator

// NOTE: man fork + exec
// - TODO*: озвучка текста (espeak) + флаг отключения 6
// - TODO**: графический интерфейс к акинатору 7

int CountStringsAndSetEnds(char* str, int size)
{
    int count_strings = 1;
    int node_val_open = 0;
    for (int i = 0;i < size;i++) {
        if (str[i] == '(' || str[i] == ')') {
            if (i == size - 1 || str[i + 1] != ' ') {
                fprintf(stderr, "Wrong akinator tree input format\n");
                return ERROR_OUTPUT;
            }
            else {
                count_strings++;
                str[i + 1] = '\0';
            }
        }
        if (str[i] == '"' && node_val_open == 0) {
            node_val_open = 1;
        }
        else {
            if (str[i] == '"' && node_val_open == 1) {
                if (i == size - 1 || str[i + 1] != ' ') {
                    fprintf(stderr, "Wrong akinator tree input format\n");
                    return ERROR_OUTPUT;
                }
                else {
                    count_strings++;
                    str[i + 1] = '\0';
                }
                node_val_open = 0;
            }
        }
    }
    return count_strings;
}

void SplitStrings(char** data, char* onegin, int size_onegin)
{
    data[0] = onegin;
    int j = 1;
    for (int i = 0;i < size_onegin - 1;i++)
    {
        if (onegin[i] == '\0')
        {
            data[j] = &onegin[i] + 1;
            j++;
        }
    }
}


TreeNode_t *CreateNode(char *value) {
    TreeNode_t *nodep = calloc(1, sizeof(TreeNode_t));
    if (nodep == NULL) {
        fprintf(stderr, "Can't create node\n");
        return NULL;
    }
    char *nodep_val = calloc(strlen(value) + 1, sizeof(char));
    if (nodep_val == NULL) {
        fprintf(stderr, "Can't copy value to node\n");
        return NULL;
    }
    strcpy(nodep_val, value);
    nodep->value = nodep_val;
    return nodep;
}

void SetRoot(Tree *tree, TreeNode_t *node) {
    tree->root = node;
}


void DumpNode(TreeNode_t *node) {
    for (int i = 0; i < 30; i++) printf("-");
    if (node == NULL) {
        printf("\n");
        printf("You printed NULL node\n");
        for (int i = 0; i < 30; i++) printf("-");
        printf("\n");
        return;
    }
    printf("\n");
    printf("Node:\n");
    printf("Address: %p\n", node);
    printf("Value: %s\n", node->value);
    printf("Left: %p\n", node->left);
    printf("Right: %p\n", node->right);
    for (int i = 0; i < 30; i++) printf("-");
    printf("\n");
}

static void DestroyNode(TreeNode_t *node) {
    if (node == NULL) return;
    DestroyNode(node->left);
    DestroyNode(node->right);
    free(node->value);
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

TreeNode_t *UnlinckLeft(TreeNode_t *node) {
    if (node->left == NULL) {
        fprintf(stderr, "You are trying to unclay left Node from Node, that already haven't left Node\n");
        return NULL;
    }
    TreeNode_t *deleting_node = node->left;
    node->left = NULL;
    return deleting_node;
}

TreeNode_t *UnlinckRight(TreeNode_t *node) {
    if (node->right == NULL) {
        fprintf(stderr, "You are trying to unclay left Node from Node, that already haven't right Node\n");
        return NULL;
    }
    TreeNode_t *deleting_node = node->right;
    node->right = NULL;
    return deleting_node;
}

void LinkRight(TreeNode_t *node, TreeNode_t *r_node) {
    if (node->right != NULL) {
        fprintf(stderr, "You are trying to clay Node to Node, that already have right Node\n");
        return;
    }
    node->right = r_node;
}

static void GraphicInfoDump(TreeNode_t *node, FILE* out_file, uint32_t depth, uint32_t number) {
    if (node->left != NULL || node->left != NULL) {
        fprintf(out_file, "\t%d[shape = record, fillcolor = \"lightgreen\", color = \"green\", style = \"filled, rounded\", ", number);
    }
    else {
        fprintf(out_file, "\t%d[shape = record, fillcolor = \"yellow\", color = \"black\", style = \"filled, rounded\", ", number);
    }
    fprintf(out_file, "label = \" {value : %s | {address : %p} | {left : %p | right : %p}}\"];\n", node->value, node, node->left, node->right);

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


void TreeGDToNewFile(TreeNode_t *node) {
    char dot_file_name[FILE_NAME_LENGTH] = {};
    time_t now = time(NULL);
    
    SetDOTFileName(dot_file_name, now);
    
    FILE* svg_out_file = fopen(dot_file_name, "w");
    if (svg_out_file == NULL) {
        fprintf(stderr, "Can't open file %s\n", dot_file_name);
        return;
    }
    
    fprintf(svg_out_file, "digraph structs\n");
    fprintf(svg_out_file, "{\n");
    fprintf(svg_out_file, "\trankdir = UD;\n");
    fprintf(svg_out_file, "\t");
    if (node == NULL) {
        fprintf(svg_out_file, "\n}\n");
        fclose(svg_out_file);
        return;
    }
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
    
    // FIXME: use snprintf
    strcpy(file_name, ctime(&now)); // use localtime
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
    // TODO: snprintf
    int32_t size_d = strlen(dot_file);
    int32_t size_s = strlen(svg_file);
    int32_t size_arg = size_d + size_s + 15;
    char *arg = calloc(size_arg, sizeof(char));

    char buf_str[11] = "dot -Tsvg ";
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
    if (node != NULL) fprintf(out_file, "\"%s\" ", node->value);
    if (node->left != NULL) {
        PrefixPrintFile(node->left, out_file);
    }
    if (node->right != NULL) {
        PrefixPrintFile(node->right, out_file);
    }
    fprintf(out_file, ") ");
}

void PrefixOutFile(const TreeNode_t *node, char *output_file) {
    if (node == NULL) {
        return;
    }
    FILE *out_file = fopen(output_file, "w");
    if (out_file == NULL) {
        fprintf(stderr, "Can't open file <%s> for prefix output\n", output_file);
        return;
    }
    PrefixPrintFile(node, out_file);
    fclose(out_file);
}

static TreeNode_t *ReadPrefixMakeTree(char **data, uint32_t *index) {
    
    if (data[*index][0] == ' ') {
        uint32_t succesfull_readed = sscanf(data[*index], "%" MAX_SP_SIZE "s", data[*index]);
        if (succesfull_readed != 1) {
            fprintf(stderr, "Can't read spliting elem\n");
            return NULL;
        }
    }
    if (strcmp(data[*index], ")") == 0) {
        return NULL;
    }
    if (strcmp(data[*index], "(") == 0) {
        (*index)++;
        uint32_t size_di = strlen(data[*index]);
        if (data[*index][size_di - 1] == ' ' || data[*index][0] == ' ') {
            uint32_t succesfull_readed = sscanf(data[*index], "%" MAX_SP_SIZE "s", data[*index]);
            size_di = strlen(data[*index]);
            if (succesfull_readed != 1) {
                fprintf(stderr, "Can't read value for node\n");
                return NULL;
            }
        }

        if (data[*index][size_di - 1] != '"' || data[*index][0] != '"') {
            fprintf(stderr, "Wrong node value format, must be in \"\", but got <%s>\n", data[*index]);
            return NULL;
        }

        data[*index][strlen(data[*index]) - 1] = '\0';
        TreeNode_t *node = CreateNode(data[*index] + 1);
        (*index)++;

        if (data[*index][0] == ' ') {
            uint32_t succesfull_readed = sscanf(data[*index], "%" MAX_SP_SIZE "s", data[*index]);
            if (succesfull_readed != 1) {
                fprintf(stderr, "Can't read spliting elem\n");
                return NULL;
            }
        }

        if (strcmp(data[*index], ")") == 0) {
            return node;
        }
        
        TreeNode_t *l_node = ReadPrefixMakeTree(data, index);
        
        if (l_node == NULL) {
             return NULL;
        }
        LinkLeft(node, l_node);
        
        (*index)++;
        if (data[*index][0] == ' ') {
            uint32_t succesfull_readed = sscanf(data[*index], "%" MAX_SP_SIZE "s", data[*index]);
            if (succesfull_readed != 1) {
                fprintf(stderr, "Can't read spliting elem\n");
                return NULL;
            }
        }
        if (strcmp(data[*index], ")") == 0) {
            return node;
        }
        if (strcmp(data[*index], "(") == 0) {
            TreeNode_t *r_node = ReadPrefixMakeTree(data, index);
            if (r_node == NULL) {
                return NULL;
            }
            LinkRight(node, r_node);
            (*index)++;
            if (strcmp(data[*index], ")") == 0) {
                return node;
            }
        }
    }
    fprintf(stderr, "File with tree is wrong\n");
    return NULL;
}

 void ReadPrefixFromFile(Tree *tree, char *output_file) {
    ReadReturn RR = ReadFromFile(output_file);
    // FIXME: check return
    int words_amount = CountStringsAndSetEnds(RR.p, RR.size);
    printf("%d\n", words_amount);
    if (words_amount == -1) {
        fprintf(stderr, "Can't count strings (file is wrong)\n");
        free(RR.p);
        return;
    }
    char** data = calloc(words_amount, sizeof(char*));
    if (data == NULL) {
        fprintf(stderr, "Can't calloc for prefix read\n");
        return;
    }
    SplitStrings(data, RR.p, RR.size);
    uint32_t index = 0;
    TreeNode_t *node = ReadPrefixMakeTree(data, &index);
    printf("[%p]\n", node);
    if (node == NULL) {
        free(data);
        free(RR.p);
        return;
    }
    free(data);
    free(RR.p);
    //printf("--OKEY--, %p\n", node);
    //free(node);
    
    SetRoot(tree, node);
    //return tree;
    //return tree;
}



static void InfixPrintFile(const TreeNode_t *node, FILE *out_file) {

    fprintf(out_file, "( ");
    if (node->left != NULL) {
        InfixPrintFile(node->left, out_file);
    }
    fprintf(out_file, "%s ", node->value);
    if (node->right != NULL) {
        InfixPrintFile(node->right, out_file);
    }
    fprintf(out_file, ") ");
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
    fprintf(out_file, "%s ", node->value);
    fprintf(out_file, ") ");
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

static uint32_t PositiveAnswer(char *ans) {
    return strcmp(ans, "y") == 0 || strcmp(ans, "yes") == 0 || strcmp(ans, "true") == 0 || strcmp(ans, "True") == 0 || strcmp(ans, "д") == 0 || strcmp(ans, "да") == 0;
}

static uint32_t NegativeAnswer(char *ans) {
    return strcmp(ans, "n") == 0 || strcmp(ans, "no") == 0 || strcmp(ans, "false") == 0 || strcmp(ans, "False") == 0 || strcmp(ans, "н") == 0 || strcmp(ans, "нет") == 0;
}

static void AkinatorGame (TreeNode_t *node, TreeNode_t *parent, int last_answer) {
    if (node->left != NULL && node->right != NULL) {
        printf("%s?\n", node->value);
    }
    if (node->left == NULL && node->right == NULL) {
        printf("Is it %s?\n", node->value);
    }
    if ((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL)) {
        printf("Tree error: there is an elem that has left node or right node, but don't have another\n");
        DumpNode(node);
        return;
    }
    printf("True or false [y/n]: ");
    char *ans = NULL;
    size_t len = 0;
    int32_t size_a = 0;
    while (GAME_IN_PROCESS) {
        size_a = getline(&ans, &len, stdin);
        if (size_a <= 0 ) {
            fprintf(stderr, "Can't read user's answer\n");
            free(ans);
            return;
        }
        ans[size_a - 1] = '\0';
        //scanf("%5s", ans);
        //printf("[%s]\n", ans);
        if (PositiveAnswer(ans)) {
            if (node->left != NULL) {
                AkinatorGame(node->left, node, POSITIVE);
                free(ans);
            }
            else {
                printf("I guessed :)\n");
                free(ans);
            }
            break;
        }
        if (NegativeAnswer(ans)) {
            if (node->right != NULL) {
                AkinatorGame(node->right, node, NEGATIVE);
                free(ans);
            }
            else {
                printf("Oh, so who it was?\n");
                size_a = getline(&ans, &len, stdin);
                if (size_a <= 0 ) {
                    fprintf(stderr, "Can't read user's answer\n");
                    free(ans);
                    return;
                }
                ans[size_a - 1] = '\0';
                //scanf("%" MAX_SP_SIZE "s", buf);
                TreeNode_t *new_node = CreateNode(ans);
                printf("How is %s different from %s? (Or: how is %s different from %s?)\n", node->value, new_node->value, new_node->value, node->value);
                printf("He ");
                size_a = getline(&ans, &len, stdin);
                if (size_a <= 0 ) {
                    fprintf(stderr, "Can't read user's answer\n");
                    free(ans);
                    return;
                }
                ans[size_a - 1] = '\0';
                //scanf("%" MAX_SP_SIZE "s", buf);
                
                //scanf("%" MAX_SP_SIZE "s", buf);

                TreeNode_t *diff_node = CreateNode(ans);
                
                TreeNode_t *old_leave = NULL;
                printf("Difference you wrote is simply right for %s? ", node->value);
                if (last_answer == POSITIVE) {
                    old_leave = UnlinckLeft(parent);
                    LinkLeft(parent, diff_node);
                }
                if (last_answer == NEGATIVE) {
                    old_leave = UnlinckRight(parent);
                    LinkRight(parent, diff_node);
                }
                while (ANS_NOT_GET) {
                    size_a = getline(&ans, &len, stdin);
                    if (size_a <= 0 ) {
                        fprintf(stderr, "Can't read user's answer\n");
                        free(ans);
                        return;
                    }
                    ans[size_a - 1] = '\0';
                    
                    if (PositiveAnswer(ans)) {
                        LinkRight(diff_node, new_node);
                        LinkLeft(diff_node, old_leave);
                        free(ans);
                        break;
                    }
                    if (NegativeAnswer(ans)) {   
                        LinkLeft(diff_node, new_node);
                        LinkRight(diff_node, old_leave);
                        free(ans);
                        break;
                    }
                    printf("Sorry, I can't understand you. Write [y / n / yes / no / true / True / false / False], you wrote <%s>\n", ans);
                }  
            }
            break;
        }
        printf("Sorry, I can't understand you. Write [y / n / yes / no / true / True / false / False], you wrote <%s>\n", ans);
    }
}

void StartGame(Tree *tree) {
    if (tree->root == NULL) {
        fprintf(stderr, "You can't start game with NULL tree\n");
        return;
    }
    AkinatorGame(tree->root, NULL, POSITIVE);
}

static int32_t GetWay(TreeNode_t *node, const char *object, stack *stk, stack *characters) {
    assert(node);
    if (strcmp(node->value, object) == 0) {
        return FOUND;
    }
    else {
        if (node->left == NULL && node->right == NULL) return NOT_FOUND;
        if ((node->left == NULL) ^ (node->right == NULL)) {
            fprintf(stderr, "Found incorrect node:\n");
            DumpNode(node);
            return NOT_FOUND;
        }
        if (GetWay(node->left, object, stk, characters) == FOUND) {
            StackPush(stk, node);
            StackPush(characters, ONE);
            return FOUND;
        }
        if (GetWay(node->right, object, stk, characters) == FOUND) {
            StackPush(stk, node);
            StackPush(characters, NULL);
            return FOUND;
        }
        return NOT_FOUND;
    }
}

void FindWay(Tree *tree, const char *object) {
    if (tree->root == NULL) {
        printf("Not found <%s>\n", object);
        return;
    }
    stack stk, characters;
    StackInit(&stk, 1);
    StackInit(&characters, 1);
    GetWay(tree->root, object, &stk, &characters);
    if (stk.size == 0) {
        printf("Can't find <%s> in tree\n", object);
    }
    else {
        while (stk.size != 0) {
            if (StackPop(&characters) == ONE) printf("<%s>\n", StackPop(&stk)->value);
            else printf("not <%s>\n", StackPop(&stk)->value);
        }
    }
    StackDestroy(&characters);
    StackDestroy(&stk);
}

static void PrintDiff(stack *stk1, stack *characters1, stack *stk2, stack *characters2) {
    uint32_t diff_flag = 1;
    while (stk1->size != 0 && stk2->size != 0) {
        TreeNode_t *obj1 = StackPop(stk1);
        TreeNode_t *obj2 = StackPop(stk2);
        TreeNode_t *obj_ch1 = StackPop(characters1);
        TreeNode_t *obj_ch2 = StackPop(characters2);
        if (diff_flag == 1) {
            if (obj_ch1 == obj_ch2) {
                if (strcmp(obj1->value, obj2->value) == 0) {
                    if (obj_ch1 == ONE) {
                        printf(GREEN "\t\t<%s>\n" BLACK, obj1->value);
                    }
                    else {
                        printf(GREEN "\t\tnot <%s>\n" BLACK, obj1->value);
                    }
                }
                else {
                    diff_flag = 0;
                    if (obj_ch1 == ONE) {
                        printf(PURPLE "<%s>\t\t" RED "<%s>\n" BLACK, obj1->value, obj2->value);
                    }
                    else {
                        printf(PURPLE "not <%s>\t\t" RED "not <%s>\n" BLACK, obj1->value, obj2->value);
                    }
                }
            }
            else {
                diff_flag = 0;
                if (obj_ch1 == ONE) {
                    printf(PURPLE "<%s>\t\t" RED "not <%s>\n" BLACK, obj1->value, obj2->value);
                }
                else {
                    printf(PURPLE "not <%s>\t\t" RED "<%s>\n" BLACK, obj1->value, obj2->value);
                }
            }
        }
        else {
            if (obj_ch1 == ONE && obj_ch2 == ONE) {
                printf(PURPLE "<%s>\t\t" RED "<%s>\n" BLACK, obj1->value, obj2->value);
            }
            else if (obj_ch1 == ONE) {
                printf(PURPLE "<%s>\t\t" RED "not <%s>\n" BLACK, obj1->value, obj2->value);
            }
            else if (obj_ch2 == ONE) {
                printf(PURPLE "not <%s>\t\t" RED "<%s>\n" BLACK, obj1->value, obj2->value);
            }
            else {
                printf(PURPLE "not <%s>\t\t" RED "not <%s>\n" BLACK, obj1->value, obj2->value);
            }
        }
    }
    while (stk1->size != 0) {
        if (StackPop(characters1) == ONE) printf(PURPLE "<%s>\n" BLACK, StackPop(stk1)->value);
        else printf(PURPLE "not <%s>\n" BLACK, StackPop(stk1)->value);
    }
    while (stk2->size != 0) {
        if (StackPop(characters2) == ONE) printf(RED "\t\t\t\t<%s>\n" BLACK, StackPop(stk2)->value);
        else printf(RED "\t\t\t\tnot <%s>\n" BLACK, StackPop(stk2)->value);
    }
} 

void DiffWays(Tree *tree, const char *object1, const char *object2) {
    stack stk1, characters1;
    StackInit(&stk1, 1);
    StackInit(&characters1, 1);

    GetWay(tree->root, object1, &stk1, &characters1);
    if (stk1.size == 0) {
        printf("Can't find <%s> in tree\n", object1);
    }

    stack stk2, characters2;
    StackInit(&stk2, 1);
    StackInit(&characters2, 1);
    GetWay(tree->root, object2, &stk2, &characters2);
    if (stk2.size == 0) {
        printf("Can't find <%s> in tree\n", object2);
    }
    if (stk1.size != 0 && stk2.size != 0) {
        printf(PURPLE "<%s>\t\t\t" RED "<%s>\n" BLACK, object1, object2);
        PrintDiff(&stk1, &characters1, &stk2, &characters2);
    }

    StackDestroy(&characters1);
    StackDestroy(&stk1);
    StackDestroy(&characters2);
    StackDestroy(&stk2);
}
