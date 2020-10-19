#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entry {
    long int balance;
    char *name;
    char *number;
};

struct entry *read_struct(void) {
    int c, i = 0;
    char *inp_str = NULL;
    while ((c = getchar()) != '\n') {
        if (c == EOF) {
            return NULL;
        }
        inp_str = realloc(inp_str, (++i) * sizeof(*inp_str));
        assert(inp_str != NULL);
        inp_str[i - 1] = c;
    }
    struct entry *ret = malloc(sizeof(*ret));
    assert(ret != NULL);
    //pointers for commas
    char *lexm_ptr1, *lexm_ptr2;
    lexm_ptr1 = strchr(inp_str, ',');
    lexm_ptr2 = strchr(lexm_ptr1+1, ',');
    //get the sizes of tokens
    long int size_name = lexm_ptr1 - inp_str;
    long int size_number = lexm_ptr2 - lexm_ptr1 - 1;
    long int size_buff = i - (lexm_ptr2 - inp_str) - 1;
    // allocate memory for fields os structures (+1 for '\0')
    ret->name = calloc(size_name + 1, sizeof(char));
    assert(ret->name != NULL);
    ret->number = calloc(size_number + 1, sizeof(char));
    assert(ret->number != NULL);
    char *num_buff = calloc(size_buff + 1, sizeof(char));
    assert(num_buff != NULL);
    // copy tokens in fields
    strncpy(ret->name, inp_str, size_name);
    strncpy(ret->number, lexm_ptr1+1, size_number);
    strncpy(num_buff, lexm_ptr2+1, size_buff);
    sscanf(num_buff, "%li", &(ret->balance));
    // free memory
    free(num_buff);
    free(inp_str);
    return ret;

    // strtok() solution DOES NOT WORK FOR inputs like ,454545,15
    
    // lexm_ptr = strtok(inp_str, ",");
    // ret->name = strdup(lexm_ptr);
    // lexm_ptr = strtok(NULL, ",");
    // ret->number = strdup(lexm_ptr);
    // lexm_ptr = strtok(NULL, ",");
    // ret->balance = strtol(lexm_ptr, NULL, 10);
    // return ret;
}

int main(void) {
    struct entry **database = NULL;
    struct entry *new;
    unsigned long dat_size = 0, ctr = 0;
    long sum = 0;
    while ((new = read_struct()) != NULL) {
        database = realloc(database, (++dat_size) * sizeof(struct entry *));
        assert(database != NULL);
        database[dat_size - 1] = new;
        sum += database[dat_size - 1]->balance;
        ctr++;
    }
    if(ctr == 0) printf("Empty database!\n");
    else{
            long threshold = (double)sum / dat_size;

            for (int i = 0; i < dat_size; i++) {
                if (database[i]->balance > threshold) {
                    printf("%s,%s,%ld\n", database[i]->name, database[i]->number, database[i]->balance);
                }
                free(database[i]->name);
                free(database[i]->number);
                free(database[i]);
            }
            free(database);
    }
    
    
}
