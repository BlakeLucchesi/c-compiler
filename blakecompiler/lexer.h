// C Lexer


typedef enum _token_name {
    OPERATOR,
    SEPARATOR,
    IDENTIFIER,
    KEYWORD,
    LITERAL,
    COMMENT,
} token_name;

typedef struct _token {
    char *value;
    token_name name;
    struct _token *next;
} token;


token *lex(char *input);

void print_debug(token *token);
const char *friendly_token_name(token *token);

// walks through lexemes and deallocates managed memory.
void cleanup(token *head);
