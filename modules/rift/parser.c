#include "parser.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Function to create a new AST node
ASTNode *new_ast_node(ASTNodeType type) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->next = NULL;
    return node;
}

Parser *parser_init(Tokenizer *tokenizer) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->tokenizer = tokenizer;
    parser->current_token = &tokenizer->tokens[0];
    return parser;
}

void parser_free(Parser *parser) {
    free(parser);
}

void parser_advance(Parser *parser) {
    if (parser->tokenizer->current_index < parser->tokenizer->count - 1) {
        parser->tokenizer->current_index++;
        parser->current_token = &parser->tokenizer->tokens[parser->tokenizer->current_index];
    }
}

void parser_expect(Parser *parser, TokenType expected_type) {
    if (parser->current_token->type != expected_type) {
        fprintf(stderr, "Unexpected token: '%s' (Line: %zu, Column: %zu)\n",
                parser->current_token->value,
                parser->current_token->line,
                parser->current_token->column);
        exit(1);
    }
    parser_advance(parser);
}

ASTNode *parser_parse_statement(Parser *parser) {
    if (parser->current_token->type == TOKEN_KEYWORD) {
        if (strcmp(parser->current_token->value, "return") == 0) {
            return parser_parse_return(parser);
        } else if (strcmp(parser->current_token->value, "if") == 0) {
            return parser_parse_if(parser);
        }
    }
    return NULL;
}

ASTNode *parser_parse_if(Parser *parser) {
    parser_expect(parser, TOKEN_KEYWORD);
    parser_expect(parser, TOKEN_PAREN_OPEN);

    ASTNode *if_node = new_ast_node(AST_IF);
    if_node->data.if_node.condition = parser_parse_expression(parser);

    parser_expect(parser, TOKEN_PAREN_CLOSE);
    parser_expect(parser, TOKEN_BRACE_OPEN);

    if_node->data.if_node.then_block = parser_parse_block(parser);

    parser_expect(parser, TOKEN_BRACE_CLOSE);

    if (parser->current_token->type == TOKEN_KEYWORD && strcmp(parser->current_token->value, "else") == 0) {
        parser_expect(parser, TOKEN_KEYWORD);
        parser_expect(parser, TOKEN_BRACE_OPEN);
        
        if_node->data.if_node.else_block = parser_parse_block(parser);
        parser_expect(parser, TOKEN_BRACE_CLOSE);
    } else {
        if_node->data.if_node.else_block = NULL;
    }

    return if_node;
}

ASTNode *parser_parse_return(Parser *parser) {
    parser_expect(parser, TOKEN_KEYWORD);

    ASTNode *return_node = new_ast_node(AST_RETURN);
    return_node->data.return_node.expression = parser_parse_expression(parser);
    
    parser_expect(parser, TOKEN_SEMICOLON);

    return return_node;
}

ASTNode *parser_parse_block(Parser *parser) {
    ASTNode *block_node = new_ast_node(AST_BLOCK);
    block_node->data.block_node.statements = NULL;
    ASTNode *current_statement = NULL;

    while (parser->current_token->type != TOKEN_BRACE_CLOSE && parser->current_token->type != TOKEN_EOF) {
        ASTNode *statement = parser_parse_statement(parser);
        if (!block_node->data.block_node.statements) {
            block_node->data.block_node.statements = statement;
        } else {
            current_statement->next = statement;
        }
        current_statement = statement;
    }

    return block_node;
}

ASTNode *parser_parse_expression(Parser *parser) {
    return parser_parse_binary_op(parser, 0);
}

int get_precedence(TokenType type) {
    switch (type) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 1;
        case TOKEN_STAR:
        case TOKEN_SLASH:
            return 2;
        default:
            return 0;
    }
}

ASTNode *parser_parse_binary_op(Parser *parser, int precedence) {
    ASTNode *left = parser_parse_unary_op(parser);

    while (1) {
        int current_precedence = get_precedence(parser->current_token->type);
        if (current_precedence <= precedence) {
            break;
        }

        TokenType op = parser->current_token->type;
        parser_advance(parser);
        ASTNode *right = parser_parse_binary_op(parser, current_precedence);

        ASTNode *binary_node = new_ast_node(AST_BINARY_OP);
        binary_node->data.binary_op_node.left = left;
        binary_node->data.binary_op_node.right = right;
        binary_node->data.binary_op_node.op = op;

        left = binary_node;
    }

    return left;
}

ASTNode *parser_parse_unary_op(Parser *parser) {
    if (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS) {
        TokenType op = parser->current_token->type;
        parser_advance(parser);

        ASTNode *unary_node = new_ast_node(AST_UNARY_OP);
        unary_node->data.unary_op_node.op = op;
        unary_node->data.unary_op_node.operand = parser_parse_unary_op(parser);

        return unary_node;
    }

    return parser_parse_literal(parser);
}

ASTNode *parser_parse_literal(Parser *parser) {
    ASTNode *literal_node = new_ast_node(AST_LITERAL);
    literal_node->data.literal_node.value = strdup(parser->current_token->value);
    parser_advance(parser);

    return literal_node;
}

ASTNode *parser_parse_identifier(Parser *parser) {
    ASTNode *identifier_node = new_ast_node(AST_IDENTIFIER);
    identifier_node->data.identifier_node.name = strdup(parser->current_token->value);
    parser_advance(parser);

    return identifier_node;
}

ASTNode *parser_parse_function_call(Parser *parser) {
    ASTNode *function_node = new_ast_node(AST_FUNCTION_CALL);
    function_node->data.function_call_node.name = strdup(parser->current_token->value);
    parser_advance(parser);
    parser_expect(parser, TOKEN_PAREN_OPEN);

    // Parse arguments (not fully implemented)
    function_node->data.function_call_node.args = NULL; // Placeholder for arguments

    parser_expect(parser, TOKEN_PAREN_CLOSE);

    return function_node;
}

ASTNode *parser_parse_assignment(Parser *parser) {
    ASTNode *identifier = parser_parse_identifier(parser);
    parser_expect(parser, TOKEN_ASSIGN);

    ASTNode *assignment_node = new_ast_node(AST_ASSIGNMENT);
    assignment_node->data.assignment_node.name = identifier->data.identifier_node.name;
    assignment_node->data.assignment_node.value = parser_parse_expression(parser);

    return assignment_node;
}

ASTNode *parser_parse_variable_declaration(Parser *parser) {
    parser_expect(parser, TOKEN_KEYWORD);

    ASTNode *var_decl_node = new_ast_node(AST_VARIABLE_DECLARATION);
    var_decl_node->data.variable_declaration_node.name = strdup(parser->current_token->value);
    parser_expect(parser, TOKEN_IDENTIFIER);

    if (parser->current_token->type == TOKEN_ASSIGN) {
        parser_advance(parser);
        var_decl_node->data.variable_declaration_node.value = parser_parse_expression(parser);
    } else {
        var_decl_node->data.variable_declaration_node.value = NULL;
    }

    parser_expect(parser, TOKEN_SEMICOLON);

    return var_decl_node;
}

ASTNode *parser_parse(Parser *parser) {
    ASTNode *root = new_ast_node(AST_BLOCK);
    root->data.block_node.statements = NULL;
    ASTNode *current_statement = NULL;

    while (parser->current_token->type != TOKEN_EOF) {
        ASTNode *statement = parser_parse_statement(parser);
        if (!root->data.block_node.statements) {
            root->data.block_node.statements = statement;
        } else {
            current_statement->next = statement;
        }
        current_statement = statement;
    }

    return root;
}

// Sample function to print AST nodes for debugging purposes
void print_ast(ASTNode *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    switch (node->type) {
        case AST_STATEMENT:
            printf("Statement\n");
            break;
        case AST_EXPRESSION:
            printf("Expression\n");
            break;
        case AST_BLOCK:
            printf("Block\n");
            print_ast(node->data.block_node.statements, indent + 1);
            break;
        case AST_RETURN:
            printf("Return\n");
            print_ast(node->data.return_node.expression, indent + 1);
            break;
        case AST_IF:
            printf("If\n");
            print_ast(node->data.if_node.condition, indent + 1);
            print_ast(node->data.if_node.then_block, indent + 1);
            if (node->data.if_node.else_block) {
                print_ast(node->data.if_node.else_block, indent + 1);
            }
            break;
        case AST_BINARY_OP:
            printf("Binary Op: %d\n", node->data.binary_op_node.op);
            print_ast(node->data.binary_op_node.left, indent + 1);
            print_ast(node->data.binary_op_node.right, indent + 1);
            break;
        case AST_UNARY_OP:
            printf("Unary Op: %d\n", node->data.unary_op_node.op);
            print_ast(node->data.unary_op_node.operand, indent + 1);
            break;
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->data.identifier_node.name);
            break;
        case AST_LITERAL:
            printf("Literal: %s\n", node->data.literal_node.value);
            break;
        case AST_ASSIGNMENT:
            printf("Assignment: %s\n", node->data.assignment_node.name);
            print_ast(node->data.assignment_node.value, indent + 1);
            break;
        case AST_FUNCTION_CALL:
            printf("Function Call: %s\n", node->data.function_call_node.name);
            print_ast(node->data.function_call_node.args, indent + 1);
            break;
        case AST_VARIABLE_DECLARATION:
            printf("Variable Declaration: %s\n", node->data.variable_declaration_node.name);
            if (node->data.variable_declaration_node.value) {
                print_ast(node->data.variable_declaration_node.value, indent + 1);
            }
            break;
        default:
            printf("Unknown AST Node\n");
            break;
    }

    print_ast(node->next, indent);
}
