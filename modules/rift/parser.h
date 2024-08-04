#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

typedef enum {
    AST_UNKNOWN,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_BLOCK,
    AST_RETURN,
    AST_IF,
    AST_ELSE,
    AST_COUNT,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_IDENTIFIER,
    AST_LITERAL,
    AST_ASSIGNMENT,
    AST_FUNCTION_CALL,
    AST_VARIABLE_DECLARATION
} ASTNodeType;

// Abstract Syntax Tree Node structure
typedef struct ASTNode {
    ASTNodeType type;          // Type of the AST node
    struct ASTNode *next;      // Pointer to the next node (for statements)
    union {
        struct {
            struct ASTNode *condition; // Condition for if statements
            struct ASTNode *then_block; // Then block for if statements
            struct ASTNode *else_block; // Else block for if statements
        } if_node;
        struct {
            struct ASTNode *expression; // Expression for return statements
        } return_node;
        struct {
            struct ASTNode *statements; // Statements within a block
        } block_node;
    } data;
} ASTNode;

// Parser structure
typedef struct {
    Tokenizer *tokenizer;  // Pointer to the tokenizer
    Token *current_token;   // Current token being processed
} Parser;

// Function prototypes
Parser *parser_init(Tokenizer *tokenizer);
void parser_free(Parser *parser);
ASTNode *parser_parse(Parser *parser);
void parser_advance(Parser *parser);
void parser_expect(Parser *parser, TokenType expected_type);
ASTNode *parser_parse_statement(Parser *parser);
ASTNode *parser_parse_if(Parser *parser);
ASTNode *parser_parse_return(Parser *parser);
ASTNode *parser_parse_block(Parser *parser);
ASTNode *parser_parse_expression(Parser *parser);
ASTNode *parser_parse_binary_op(Parser *parser, int precedence);
ASTNode *parser_parse_unary_op(Parser *parser);
ASTNode *parser_parse_literal(Parser *parser);
ASTNode *parser_parse_identifier(Parser *parser);
ASTNode *parser_parse_function_call(Parser *parser);
ASTNode *parser_parse_assignment(Parser *parser);
ASTNode *parser_parse_variable_declaration(Parser *parser);



#endif // PARSER_H
