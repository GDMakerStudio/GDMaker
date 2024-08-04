#include "tokenizer.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "core/error/error_list.h"
#include "core/error/error_macros.h"

const char *token_type_strings[] = {
    "UNKNOWN",
    "IDENTIFIER",
    "NUMBER",
    "STRING",
    "NEWLINE",
    "ANNOTATION",
    "KEYWORD",
    "OPERATOR",
    "DELIMITER",
    "PAREN_OPEN",
    "PAREN_CLOSE",
    "BRACE_OPEN",
    "BRACE_CLOSE",
    "BRACKET_OPEN",
    "BRACKET_CLOSE",
    "COMMA",
    "SEMICOLON",
    "COLON",
    "DOT",
    "COMMENT",
    "EOF"
};


const char *default_keywords[] = {
    "if", "else", "for", "while", "do", "return", "function", "var",
    "let", "const", "class", "extends", "constructor", "static", "this",
    "super", "try", "catch", "finally", "throw", "import", "from", "export",
    "async", "await", "switch", "case", "default", "break", "continue",
    "with", "new", "delete", "instanceof", "in", "of", "public", "private",
    "protected", "interface", "enum", "implements", "namespace", "volatile",
    "synchronized", "true", "false", "null", "undefined", "NaN", "Infinity",
    "use", "struct" 
};


void tokenizer_make_error(Tokenizer *tokenizer, const char *message) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Allocate memory for the new error message
    char *error_message = strdup(message);
    if (!error_message) return; // Failure: Memory allocation failed

    // Add the error message to the array
    tokenizer->error_messages = realloc(tokenizer->error_messages, (tokenizer->error_count + 1) * sizeof(char *));
    if (!tokenizer->error_messages) {
        free(error_message); // Free allocated message on failure
        return; // Failure: Memory allocation failed
    }

    tokenizer->error_messages[tokenizer->error_count] = error_message;
    tokenizer->error_count++;
}

void tokenizer_push_error(Tokenizer *tokenizer, const char *message) {
    tokenizer_make_error(tokenizer, message);
}

void tokenizer_push_error_token(Tokenizer *tokenizer, const Token *error_token) {
    if (!tokenizer || !error_token) return; // Failure: Invalid tokenizer or error_token

    // Create a formatted error message using the token's information
    char message[256];
    snprintf(message, sizeof(message), "Error with token of type %d at index %zu", error_token->type, tokenizer->current_index);

    tokenizer_make_error(tokenizer, message);
}

void tokenizer_make_paren_error(Tokenizer *tokenizer, char32_t paren) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Create a formatted error message for unmatched parenthesis
    char message[256];
    snprintf(message, sizeof(message), "Unmatched parenthesis: %c", (char)paren);

    tokenizer_make_error(tokenizer, message);
}

void tokenizer_make_token(Tokenizer *tokenizer, TokenType type) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Add the token type to the types array
    tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
    if (!tokenizer->types) return; // Failure: Memory allocation failed

    tokenizer->types[tokenizer->count] = type;
    tokenizer->count++;
}

void tokenizer_make_identifier(Tokenizer *tokenizer, const char *identifier) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Add the identifier to the tokens array
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    if (!tokenizer->tokens) return; // Failure: Memory allocation failed

    tokenizer->tokens[tokenizer->count] = strdup(identifier);
    if (!tokenizer->tokens[tokenizer->count]) return; // Failure: Memory allocation failed

    tokenizer_make_token(tokenizer, TOKEN_IDENTIFIER);
}

void tokenizer_newline(Tokenizer *tokenizer, bool make_token) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Increment the line number in the tokenizer text
    tokenizer->tokenizer_text->cursor_line++;

    // Reset the cursor position for the new line
    tokenizer->tokenizer_text->cursor_position = 0;

    if (make_token) {
        tokenizer_make_token(tokenizer, TOKEN_NEWLINE);
    }
}

void tokenizer_number(Tokenizer *tokenizer) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Read the number from the text
    const char *text = tokenizer->tokenizer_text->text;
    size_t start = tokenizer->tokenizer_text->cursor_position;

    while (isdigit(text[start]) || text[start] == '.') {
        start++;
    }

    // Create a token for the number
    char *number = strndup(&text[tokenizer->tokenizer_text->cursor_position], start - tokenizer->tokenizer_text->cursor_position);
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    if (!tokenizer->tokens) return; // Failure: Memory allocation failed

    tokenizer->tokens[tokenizer->count] = number;
    tokenizer_make_token(tokenizer, TOKEN_NUMBER);

    // Update the cursor position
    tokenizer->tokenizer_text->cursor_position = start;
}

void tokenizer_potential_identifier(Tokenizer *tokenizer) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Read the identifier from the text
    const char *text = tokenizer->tokenizer_text->text;
    size_t start = tokenizer->tokenizer_text->cursor_position;

    while (isalnum(text[start]) || text[start] == '_') {
        start++;
    }

    // Create a token for the identifier
    char *identifier = strndup(&text[tokenizer->tokenizer_text->cursor_position], start - tokenizer->tokenizer_text->cursor_position);
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    if (!tokenizer->tokens) return; // Failure: Memory allocation failed

    tokenizer->tokens[tokenizer->count] = identifier;
    tokenizer_make_token(tokenizer, TOKEN_IDENTIFIER);

    // Update the cursor position
    tokenizer->tokenizer_text->cursor_position = start;
}

void tokenizer_string(Tokenizer *tokenizer) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Read the string from the text
    const char *text = tokenizer->tokenizer_text->text;
    size_t start = tokenizer->tokenizer_text->cursor_position + 1; // Skip the opening quote
    size_t end = start;

    while (text[end] != '"' && text[end] != '\0') {
        end++;
    }

    if (text[end] == '\0') {
        tokenizer_make_error(tokenizer, "Unterminated string literal");
        return;
    }

    // Create a token for the string
    char *string = strndup(&text[start], end - start);
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    if (!tokenizer->tokens) return; // Failure: Memory allocation failed

    tokenizer->tokens[tokenizer->count] = string;
    tokenizer_make_token(tokenizer, TOKEN_STRING);

    // Update the cursor position (skip the closing quote)
    tokenizer->tokenizer_text->cursor_position = end + 1;
}

void tokenizer_annotation(Tokenizer *tokenizer) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Read the annotation from the text
    const char *text = tokenizer->tokenizer_text->text;
    size_t start = tokenizer->tokenizer_text->cursor_position + 1; // Skip the '@' symbol
    size_t end = start;

    while (isalnum(text[end]) || text[end] == '_') {
        end++;
    }

    // Create a token for the annotation
    char *annotation = strndup(&text[start], end - start);
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    if (!tokenizer->tokens) return; // Failure: Memory allocation failed

    tokenizer->tokens[tokenizer->count] = annotation;
    tokenizer_make_token(tokenizer, TOKEN_ANNOTATION);

    // Update the cursor position
    tokenizer->tokenizer_text->cursor_position = end;
}

void tokenizer_scan(Tokenizer *tokenizer) {
    if (!tokenizer || !tokenizer->tokenizer_text) return; // Failure: Invalid tokenizer or text

    while (tokenizer->tokenizer_text->cursor_position < tokenizer->tokenizer_text->length) {
        char current_char = tokenizer->tokenizer_text->text[tokenizer->tokenizer_text->cursor_position];

        if (isspace(current_char)) {
            tokenizer->tokenizer_text->cursor_position++; // Skip whitespace
        } else if (isalpha(current_char) || current_char == '_') {
            size_t start = tokenizer->tokenizer_text->cursor_position;
            while (isalnum(tokenizer->tokenizer_text->text[tokenizer->tokenizer_text->cursor_position]) ||
                   tokenizer->tokenizer_text->text[tokenizer->tokenizer_text->cursor_position] == '_') {
                tokenizer->tokenizer_text->cursor_position++;
            }
            size_t length = tokenizer->tokenizer_text->cursor_position - start;
            char *identifier = strndup(&tokenizer->tokenizer_text->text[start], length);
            tokenizer_make_identifier(tokenizer, identifier); // Process identifier
            free(identifier);
        } else if (current_char == ';') {
            tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
            tokenizer->types[tokenizer->count] = TOKEN_SEMICOLON; // Handle semicolon
            tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
            tokenizer->tokens[tokenizer->count] = strdup(";"); // Copy semicolon token
            tokenizer->count++;
            tokenizer->tokenizer_text->cursor_position++;
        } else if (current_char == '{') {
            tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
            tokenizer->types[tokenizer->count] = TOKEN_BRACE_OPEN; // Handle opening brace
            tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
            tokenizer->tokens[tokenizer->count] = strdup("{"); // Copy opening brace token
            tokenizer->count++;
            tokenizer->tokenizer_text->cursor_position++;
        } else if (current_char == '}') {
            tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
            tokenizer->types[tokenizer->count] = TOKEN_BRACE_CLOSE; // Handle closing brace
            tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
            tokenizer->tokens[tokenizer->count] = strdup("}"); // Copy closing brace token
            tokenizer->count++;
            tokenizer->tokenizer_text->cursor_position++;
        } else {
            // Handle other characters, operators, or errors
            tokenizer->tokenizer_text->cursor_position++;
        }
    }

    // Handle end of file
    tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
    tokenizer->types[tokenizer->count] = TOKEN_EOF; // Set token type to EOF
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    tokenizer->tokens[tokenizer->count] = strdup("EOF"); // Copy EOF token
    tokenizer->count++;
}

void tokenizer_newline(Tokenizer *tokenizer, bool make_token) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    if (make_token) {
        tokenizer_make_token(tokenizer, TOKEN_NEWLINE); // Add a newline token
    }

    tokenizer->tokenizer_text->cursor_position++;
}

void tokenizer_number(Tokenizer *tokenizer) {
    if (!tokenizer || !tokenizer->tokenizer_text) return; // Failure: Invalid tokenizer or text

    size_t start = tokenizer->tokenizer_text->cursor_position;

    while (isdigit(tokenizer->tokenizer_text->text[tokenizer->tokenizer_text->cursor_position]) ||
           tokenizer->tokenizer_text->text[tokenizer->tokenizer_text->cursor_position] == '.') {
        tokenizer->tokenizer_text->cursor_position++;
    }

    size_t length = tokenizer->tokenizer_text->cursor_position - start;
    char *number = strndup(&tokenizer->tokenizer_text->text[start], length);
    tokenizer_make_identifier(tokenizer, number);
    tokenizer_make_token(tokenizer, TOKEN_NUMBER);
    free(number);
}


bool tokenizer_is_keyword(const char *identifier) {
    for (size_t i = 0; i < MAX_KEYWORDS; i++) {
        if (tokenizer->keywords[i] && strcmp(tokenizer->keywords[i], identifier) == 0) {
            return true; // Found a matching keyword
        }
    }
    return false; // Not a keyword
}

void tokenizer_make_identifier(Tokenizer *tokenizer, const char *identifier) {
    if (!tokenizer) return; // Failure: Invalid tokenizer

    // Check if the identifier is a keyword
    if (tokenizer_is_keyword(identifier)) {
        tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
        tokenizer->types[tokenizer->count] = TOKEN_KEYWORD; // Set the token type to KEYWORD
    } else {
        tokenizer->types = realloc(tokenizer->types, (tokenizer->count + 1) * sizeof(TokenType));
        tokenizer->types[tokenizer->count] = TOKEN_IDENTIFIER; // Set the token type to IDENTIFIER
    }

    // Add the identifier to the tokens array
    tokenizer->tokens = realloc(tokenizer->tokens, (tokenizer->count + 1) * sizeof(char *));
    if (!tokenizer->tokens) return; // Failure: Memory allocation failed

    tokenizer->tokens[tokenizer->count] = strdup(identifier); // Copy the identifier string
    if (!tokenizer->tokens[tokenizer->count]) return; // Failure: Memory allocation failed

    tokenizer->count++;
}

void tokenizer_skip_whitespace(Tokenizer *tokenizer) {
    if (!tokenizer || !tokenizer->tokenizer_text) return;

    while (isspace(tokenizer->tokenizer_text->text[tokenizer->tokenizer_text->cursor_position])) {
        tokenizer->tokenizer_text->cursor_position++;
    }
}

void tokenizer_check_indent(Tokenizer *tokenizer) {
    size_t indent_level = 0; // Current indentation level
    size_t line_number = 1;  // Track line numbers
    size_t index = 0;        // Current token index

    while (index < tokenizer->count) {
        Token *token = &tokenizer->tokens[index];

        if (token->type == TOKEN_NEWLINE) {
            // Move to the next line and reset the indentation level
            line_number++;
            indent_level = 0;
        } else if (token->type == TOKEN_INDENT) {
            // Increase indentation level
            indent_level++;
        } else if (token->type == TOKEN_UNINDENT) {
            // Decrease indentation level
            if (indent_level > 0) {
                indent_level--;
            } else {
                // Error: Unmatched unindentation
                fprintf(stderr, "Error: Unmatched unindentation at line %zu\n", line_number);
                exit(1); // Exit on error
            }
        }

        // Move to the next token
        index++;
    }

    // Optional: Check if the final indentation level is zero
    if (indent_level != 0) {
        fprintf(stderr, "Error: Unclosed indentation at the end of input\n");
        exit(1); // Exit on error
    }
}

