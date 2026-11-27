#ifndef ONEGINLIB_H
#define ONEGINLIB_H

typedef struct
{
    char* p;
    int size;
} ReadReturn;

void BubbleSort(int size, char* index[], int(* CompareFunc)(char* a, char* b));
void Change(char** a, char** b);
int min(int a,int b);
int CompareStrings(char* str1, char* str2);
int CompareStringsReverse(char* str1, char* str2);
ReadReturn ReadFromFile(const char* filename);
int CountStringsAndReplace(char* str, int size);
int ClearCompare(char* str1, char* str2);
void ShareStrings(char** data, char* onegin, int size_onegin);

#endif
