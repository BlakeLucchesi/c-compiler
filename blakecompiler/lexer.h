
typedef struct _token {
    char value;
    struct _token *next;
} token;

token *lex(char *input);
