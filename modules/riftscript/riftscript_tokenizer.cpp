#include "riftscript_tokenizer.h"



#include "core/collections/hash_map.h"
#include "core/varaint/variant.h"



#include <cctype>
#include <unordered_map>
#include <sstream>

// Static token names array
const char *RiftScriptTokenizer::token_names[] = {
    "Empty", "Annotation", "Identifier", "Literal",
    "<", "<=", ">", ">=", "==", "!=", "and", "or", "not",
    "&&", "||", "!", "&", "|", "~", "^", "<<", ">>",
    "+", "-", "*", "**", "/", "%",
    "=", "+=", "-=", "*=", "**=", "/=", "%=",
    "<<=", ">>=", "&=", "|=", "^=",
    "if", "elif", "else", "for", "while", "break",
    "continue", "pass", "return", "match", "when",
    "as", "assert", "await", "breakpoint", "class",
    "class_name", "const", "enum", "extends", "func",
    "in", "is", "namespace", "preload", "self",
    "signal", "static", "super", "trait", "var",
    "void", "yield", "[", "]", "{", "}", "(", ")",
    ",", ";", ".", "..", ":", "$", "->", "_",
    "Newline", "Indent", "Dedent", "PI", "TAU",
    "INF", "NaN", "VCS conflict marker", "`", "?",
    "Error", "End of file"
};

// Keyword map for quick lookup
std::unordered_map<std::string, RiftScriptTokenizer::TokenType> RiftScriptTokenizer::keywords = {
    {"if", TokenType::IF}, {"elif", TokenType::ELIF}, {"else", TokenType::ELSE},
    {"for", TokenType::FOR}, {"while", TokenType::WHILE}, {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE}, {"pass", TokenType::PASS}, {"return", TokenType::RETURN},
    {"match", TokenType::MATCH}, {"when", TokenType::WHEN}, {"as", TokenType::AS},
    {"assert", TokenType::ASSERT}, {"await", TokenType::AWAIT}, {"breakpoint", TokenType::BREAKPOINT},
    {"class", TokenType::CLASS}, {"class_name", TokenType::CLASS_NAME}, {"const", TokenType::CONST},
    {"enum", TokenType::ENUM}, {"extends", TokenType::EXTENDS}, {"func", TokenType::FUNC},
    {"in", TokenType::IN}, {"is", TokenType::IS}, {"namespace", TokenType::NAMESPACE},
    {"preload", TokenType::PRELOAD}, {"self", TokenType::SELF}, {"signal", TokenType::SIGNAL},
    {"static", TokenType::STATIC}, {"super", TokenType::SUPER}, {"trait", TokenType::TRAIT},
    {"var", TokenType::VAR}, {"void", TokenType::VOID}, {"yield", TokenType::YIELD}
};



RiftScriptTokenizer::Token RiftScriptTokenizer::next_token() {
    // Skip whitespace
    while (current_pos < code.length() && std::isspace(code[current_pos])) {
        if (code[current_pos] == '\n') {
            cursor_line++;
            cursor_column = 1;
        } else {
            cursor_column++;
        }
        current_pos++;
    }

    // End of code
    if (current_pos >= code.length()) {
        make_token(TokenType::EOF_TOKEN);
        return current_token;
    }

    char current_char = code[current_pos];

    // Handle identifiers and keywords
    if (std::isalpha(current_char) || current_char == '_') {
        std::string identifier;
        while (current_pos < code.length() && (std::isalnum(code[current_pos]) || code[current_pos] == '_')) {
            identifier += code[current_pos++];
            cursor_column++;
        }
        if (keywords.find(identifier) != keywords.end()) {
            current_token.type = keywords[identifier];
            current_token.value = identifier;
        } else {
            current_token.type = TokenType::IDENTIFIER;
            current_token.value = identifier;
        }
        return current_token;
    }

    // Handle numeric literals
    if (std::isdigit(current_char)) {
        std::string number;
        while (current_pos < code.length() && std::isdigit(code[current_pos])) {
            number += code[current_pos++];
            cursor_column++;
        }
        current_token.type = TokenType::LITERAL;
        current_token.value = number;
        return current_token;
    }

    // Handle operators
    switch (current_char) {
        case '<':
            if (code[current_pos + 1] == '=') {
                current_pos++;
                cursor_column++;
                make_token(TokenType::LESS_EQUAL);
            } else {
                make_token(TokenType::LESS);
            }
            break;
        case '>':
            if (code[current_pos + 1] == '=') {
                current_pos++;
                cursor_column++;
                make_token(TokenType::GREATER_EQUAL);
            } else {
                make_token(TokenType::GREATER);
            }
            break;
        case '=':
            if (code[current_pos + 1] == '=') {
                current_pos++;
                cursor_column++;
                make_token(TokenType::EQUAL_EQUAL);
            } else {
                make_token(TokenType::EQUAL);
            }
            break;
        case '!':
            if (code[current_pos + 1] == '=') {
                current_pos++;
                cursor_column++;
                make_token(TokenType::BANG_EQUAL);
            } else {
                make_token(TokenType::BANG);
            }
            break;
        case '+':
            make_token(TokenType::PLUS);
            break;
        case '-':
            make_token(TokenType::MINUS);
            break;
        case '*':
            if (code[current_pos + 1] == '*') {
                current_pos++;
                cursor_column++;
                make_token(TokenType::STAR_STAR);
            } else {
                make_token(TokenType::STAR);
            }
            break;
        case '/':
            make_token(TokenType::SLASH);
            break;
        case ';':
            make_token(TokenType::SEMICOLON);
            break;
        case '(':
            make_token(TokenType::PARENTHESIS_OPEN);
            break;
        case ')':
            make_token(TokenType::PARENTHESIS_CLOSE);
            break;
        case '{':
            make_token(TokenType::BRACE_OPEN);
            break;
        case '}':
            make_token(TokenType::BRACE_CLOSE);
            break;
        case '[':
            make_token(TokenType::BRACKET_OPEN);
            break;
        case ']':
            make_token(TokenType::BRACKET_CLOSE);
            break;
        case ',':
            make_token(TokenType::COMMA);
            break;
        case '.':
            make_token(TokenType::PERIOD);
            break;
        case ':':
            make_token(TokenType::COLON);
            break;
        case '@':
            make_token(TokenType::ANNOTATION);
            break;
        default:
            make_error("Unexpected character");
            break;
    }

    current_pos++;
    cursor_column++;
    return current_token;
}
