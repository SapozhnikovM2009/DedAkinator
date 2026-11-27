#include "oneginlib.h"

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


void ShareStrings(char** data, char* onegin, int size_onegin)
{
    data[0] = onegin;
    int j = 1;
    for (int i = 0;i < size_onegin - 1;i++)
    {
        if (onegin[i] == '\0')
        {
            data[j] = &onegin[i]+1;
            j++;
        }
    }
}

int CountStringsAndReplace(char* str, int size)
{
    int count_strings = 1;
    for (int i = 0;i < size;i++)
    {
        if (str[i] == '\n')
        {
            count_strings++;
            str[i] = '\0';
        }
    }
    return count_strings;
}

ReadReturn ReadFromFile(const char* filename)
{
    // TODO: error check
    int succesfully_opened = open(filename, O_RDONLY);
    if (succesfully_opened < 0)
    {
        ReadReturn RR = {NULL, 0};
        return RR;
    }
    struct stat st;
    fstat(succesfully_opened, &st);

    char* p = calloc(st.st_size + 1, sizeof(char));

    int size_onegin = read(succesfully_opened, p, st.st_size);
    p[size_onegin] = '\0';
    size_onegin++;
    close(succesfully_opened);
    ReadReturn RR = {p, size_onegin};
    return RR;
}

void BubbleSort(int size, char* index[], int (* CompareFunc)(char*  a, char* b))
{
    for (int j = 0;j < size;j++)
    {
        for (int i = 0;i < size - 1 - j;i++)
        {
            if ((*CompareFunc)(index[i], index[i+1]))
            {
                Change(&index[i], &index[i+1]);
            }
        }
    }
}

void Change(char** a, char** b)
{
    char* c = *a;
    *a = *b;
    *b = c;
}

int min(int a, int b)
{
    if (a > b) return b;
    else return a;
}

int CompareStrings(char* str1, char* str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    int ind1 = 0;
    int ind2 = 0;

    while (ind1 < len1 && ind2 < len2)
    {
        // TODO: use isalpha
        if (!isalpha(str1[ind1])) ind1++;
        else if (!isalpha(str2[ind2])) ind2++;
        else
        {
            if (tolower(str1[ind1]) < tolower(str2[ind2])) return 0;
            if (tolower(str1[ind1]) > tolower(str2[ind2])) return 1;
            if (tolower(str1[ind1]) == tolower(str2[ind2]))
            {
                ind1++;
                ind2++;
            }
        }
    }
    if (len1 > len2) return 1;
    else return 0;
}

int CompareStringsReverse(char* str1, char* str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    int ind1 = 0;
    int ind2 = 0;

    while (ind1 < len1 && ind2 < len2)
    {
        if (!isalpha(str1[ind1])) ind1++;
        else if (!isalpha(str2[ind2])) ind2++;
        else
        {
            if (tolower(str1[len1 - ind1 - 1]) < tolower(str2[len2 - ind2 - 1])) return 0;
            if (tolower(str1[len1 - ind1 - 1]) > tolower(str2[len2 - ind2 - 1])) return 1;
            if (tolower(str1[len1 - ind1 - 1]) == tolower(str2[len2 - ind2 - 1]))
            {
                ind1++;
                ind2++;
            }
        }
    }
    if (len1 > len2) return 0;
    else return 1;
}

int ClearCompare(char* str1, char* str2)
{
    if (str1 > str2) return 1;
    return 0;
}
