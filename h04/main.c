#include <stdio.h>
#include <stdlib.h>

//clarification 1: strings are considered null-terminated c-style strings
//clarification 2: input string have a maximum length determined by MAXLEN enum

enum
{
    MAXLEN = 32
};

/*!
    \brief get the length of a string (like strlen())
    \param c pointer to the string
    \return length of string
*/

int length(char *c)
{
    int count, i;
    i = 0;
    count = 0;
    while (c[i] != '\0')
    {
        count++;
        i++;
    }
    return count;
}

/*!
    \brief compare two strings just like strcmp() in strings.h
    \param a pointer to first null-terminated string 
    \param b pointer to second null-terminated string 
    \return 1 if a > b \n 
           -1 if a < b \n 
            0 if a = b 
*/
int compare(char *a, char *b)
{
    int i = 0;
    if (length(a) >= length(b))
    {
        while (a[i] != '\0')
        {
            if (a[i] != b[i])
            {
                if ((int)a[i] > (int)b[i])
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
            i++;
        }
        return 0;
    }
    else
    {
        while (b[i] != '\0')
        {
            if (a[i] != b[i])
            {
                if ((int)a[i] > (int)b[i])
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
            i++;
        }
        return 0;
    }
}

int main(void){
    char *a, *b;
    a = calloc(MAXLEN, sizeof(char));
    b = calloc(MAXLEN, sizeof(char));
    //this is dumb, but just for demonstration
    a[0] = 'a';
    a[0] = 'b';
    a[0] = '\0';
    b[0] = 'a';
    b[0] = 'b';
    b[0] = '\0';
    switch (compare(a, b))
    {
    case 1:
        printf("a > b\n");
        break;
    case -1:
        printf("b > a\n");
        break;
    case 0:
        printf("a = b\n");
        break;

    default:
        printf("that wasn't supposed to happen");
        break;
    }
}
