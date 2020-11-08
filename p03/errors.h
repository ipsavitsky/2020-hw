enum {
    E_MEM_ALLOC = 1,
    E_OVERFLOW = 2,
    E_UNDERFLOW = 3,
    E_UNEXPECTED_SYMBOL = 4,
    E_ZERO_DIVISION = 5,
    E_UNKNOWN_VAR = 6,
    E_UNBALANCED_BRACKET = 7
};

void err_print(int err);