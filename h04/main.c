#include <stdio.h>
#include <stdlib.h>

// clarification 1: strings are considered null-terminated c-style strings
// clarification 2: input string have a maximum length determined by MAXLEN enum
// clarification 3: both strings are input in stdin

enum { MAXLEN = 32 };

/**
    \brief get the length of a string (like strlen())
    \param c pointer to the string
    \return length of string
*/
// int length(char *c){
//     int count, i;
//     i = 0;
//     count = 0;
//     while (c[i] != '\0'){
//         count++;
//         i++;
//     }
//     return count;
// }

/**
    \brief compare two strings just like strcmp() in strings.h
    \param a pointer to first null-terminated string
    \param b pointer to second null-terminated string
    \return 1 if a > b
           \n
           -1 if a < b
           \n
            0 if a = b
*/
// int compare(char *a, char *b){
//     int len_a = length(a), len_b = length(b), i;
//     for(i = 0; i < ((len_a > len_b) ? len_b : len_a); i++){
//         printf("%c, %c\n", a[i], b[i]);
//         if(a[i] != b[i]){
//             return (a[i] > b[i]) ? 1 : -1;
//         }
//     }
//     return 0;
// }

/**
 * \brief a true srtcmp() copy, definition of lexicography taken from wikipedia
 * \param a first string
 * \param b second string
 * \return 1 if a > b \n
           -1 if a < b \n
            0 if a = b
 */
int compare(char *a, char *b) {
    int i = 0;
    while ((a[i] == b[i]) && (a[i] != '\0')) i++;
    if (a[i] == '\0') return (a[i] != b[i]) ? -1 : 0;
    return ((unsigned char)a[i] < (unsigned char)b[i]) ? -1 : 1;
}

int main(void) {
    char a[3], b[3];
    // a = calloc(MAXLEN, sizeof(char));
    // b = calloc(MAXLEN, sizeof(char));
    // scanf("%s", a);
    // scanf("%s", b);
    a[0] = 'a';
    a[1] = 130;
    a[2] = '\0';
    b[0] = 'a';
    b[1] = 127;
    b[2] = '\0';
    // // printf("%d, %d\n", length(a), length(b));
    switch (compare(a, b)) {
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
