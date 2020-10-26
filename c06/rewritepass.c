#include <assert.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// specification1: it is guaranteed that main() function exists in the game
// specification2: it is guaranteed that strcmp() function is only 1 in main()
// specification3: in disasmed code password pointer is always in a push
// instruction in the previous line like this: push $0x8049874
// specification4: in case of memry allocation error program exits immideately

/**
 * \brief search a concrete address in an elf32 file in sections
 * \param game_bin file that needs to be searched
 * \param addr address we are searching for
 * \return address of the password
 * \warning function does not preserve function pointer
 * \warning all warnings below this one are considered specifications
 * \warning game_bin is should be opened in r+ mode when passed to the function
 */
uint32_t search_address(FILE *game_bin, uint32_t addr) {
    Elf32_Ehdr elf_header;
    fseek(game_bin, 0L, SEEK_SET);
    fread(&elf_header, sizeof(Elf32_Ehdr), 1, game_bin);
    fseek(game_bin, elf_header.e_shoff, SEEK_SET);
    Elf32_Shdr sect_header;
    do {
        fread(&sect_header, sizeof(Elf32_Shdr), 1, game_bin);
    } while (!((addr >= sect_header.sh_addr) && (addr < sect_header.sh_addr + sect_header.sh_size)));
    return addr - sect_header.sh_addr + sect_header.sh_offset;
}

/**
 * \brief rewrite password address with new address
 * \param game_bin elf32 execcutable where we replace our password
 * \param password_addr address of our password
 * \param passwd new password to rewrite this(NULL-terminated string)
 * \warning the function does not preserve file pointer
 * \warning all warnings below this one are considered specifications
 * \warning NEW PASSWORD SHOULD ALWAYS BE SHORTER OR EQUAL IN LENGTH THAN THE
 * OLD ONE 
 * \warning game_bin is should be opened in r+ mode when passed to the
 * function
 * \warning password is NOT EMPTY
 */

void rewrite_password(FILE *game_bin, uint32_t password_addr, const char *passwd) {
    // unsigned char test;
    // printf("in rewrite!");
    // char *test_str = calloc(14, sizeof(char));
    // fseek(game_bin, password_addr, SEEK_SET);
    // fread(test_str, sizeof(char), 13, game_bin);
    // printf("curpassword: %s\n", test_str);

    fseek(game_bin, password_addr, SEEK_SET);
    fwrite(passwd, sizeof(char), strlen(passwd)+1, game_bin); // +1 for \0
    fflush(game_bin);
    printf("%s", passwd);
}

/**
 * \brief input new line of code
 * \param inp_str string in which we input new line in
 * \return the same as input, NULL in case of EOF
 */

char *input_code_line(char *inp_str) {
    int c, i = 0;
    while ((c = getchar()) != '\n') {
        if (c == EOF) {
            return NULL;
        }
        inp_str = realloc(inp_str, (++i) * sizeof(*inp_str));
        assert(inp_str != NULL);
        inp_str[i - 1] = c;
    }
    i += 2;
    inp_str = realloc(inp_str, i * sizeof(*inp_str));
    assert(inp_str != NULL);
    inp_str[i - 2] = '\n';
    inp_str[i - 1] = '\0';
    return inp_str;
}


int main(int argc, char *argv[]) {
    char *cur_line = NULL, *adr_str;
    uint32_t cur_addr = 0;
    while (1) {
        cur_line = input_code_line(cur_line);
        if (strstr(cur_line, "<main>:") != NULL) {
            while ((cur_line = input_code_line(cur_line)) != NULL) {
                if ((adr_str = strchr(cur_line, '$')) != NULL) {
                    cur_addr = strtol(adr_str + 3, NULL, 16); // +3 because we skip $0x
                    continue;
                }
                if (strstr(cur_line, "<strcmp@plt>") != NULL) break;
            }
            break;
        }
    }
    free(cur_line);
    // printf("cur_addr: %d\n", cur_addr);
    FILE *game_elf = fopen(argv[1], "r+");
    if(game_elf == NULL){
        perror("File opening error: ");
        exit(1);
    }
    uint32_t pass_addr = search_address(game_elf, cur_addr);
    rewrite_password(game_elf, pass_addr, "a\0");
    fclose(game_elf);
    return 0;
}