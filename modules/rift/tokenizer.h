#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_KEYWORDS 32

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_NEWLINE,
    TOKEN_ANNOTATION,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_BRACE_OPEN,
    TOKEN_BRACE_CLOSE,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_COMMENT,
    TOKEN_EOF,
    TOKEN_COUNT // Keep track of total token types
} TokenType;

typedef struct {
    char *text;             // The original text to be tokenized
    size_t length;          // Length of the text
    size_t cursor_position; // Current position in the text
    size_t cursor_line;     // Current line number
} TokenizerText;

typedef struct {
    char **tokens;              // Array of tokens
    TokenType *types;           // Array of token types
    size_t count;               // Number of tokens
    size_t current_index;       // Current index for scanning tokens
    size_t indentation_level;   // Current indentation level
    TokenizerText *tokenizer_text; // Pointer to the TokenizerText
    char **error_messages;      // Array of error messages
    size_t error_count;         // Count of error messages
    char *keywords[MAX_KEYWORDS]; // Array of keyword strings
    TokenType keyword_types[MAX_KEYWORDS]; // Corresponding token types for keywords
    size_t keyword_count;       // Count of keywords
} Tokenizer;

// Function prototype

extern const char *token_type_strings[];

// Function prototypes
void tokenizer_make_error(Tokenizer *tokenizer, const char *message);
void tokenizer_push_error(Tokenizer *tokenizer, const char *message);
void tokenizer_push_error_token(Tokenizer *tokenizer, const Token *error_token);
void tokenizer_make_paren_error(Tokenizer *tokenizer, char32_t paren);
void tokenizer_make_token(Tokenizer *tokenizer, TokenType type);
void tokenizer_make_identifier(Tokenizer *tokenizer, const char *identifier);
void tokenizer_newline(Tokenizer *tokenizer, bool make_token);
void tokenizer_number(Tokenizer *tokenizer);
void tokenizer_potential_identifier(Tokenizer *tokenizer);
void tokenizer_string(Tokenizer *tokenizer);
void tokenizer_annotation(Tokenizer *tokenizer);
void tokenizer_scan(Tokenizer *tokenizer);
void tokenizer_skip_whitespace(Tokenizer *tokenizer);
void tokenizer_check_indent(Tokenizer *tokenizer);


Tokenizer *tokenizer_init(const char *input);
void tokenizer_free(Tokenizer *tokenizer);
void tokenizer_scan(Tokenizer *tokenizer);
void tokenizer_make_identifier(Tokenizer *tokenizer, const char *identifier);
bool tokenizer_is_keyword(const char *identifier);
void tokenizer_init_keywords(Tokenizer *tokenizer);

#endif // TOKENIZER_H
