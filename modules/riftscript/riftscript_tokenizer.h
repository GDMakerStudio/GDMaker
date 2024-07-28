#infdef RIFTSCRIPT_TOKENIZER_H
#define RIFTSCRIPT_TOKENIZER_H

#include <string>
#include <vector>
#include <memory>

class RiftScriptTokenizer {
public:
    enum class TokenType {
        EMPTY,
        ANNOTATION,
        IDENTIFIER,
        LITERAL,
        LESS,
        LESS_EQUAL,
        GREATER,
        GREATER_EQUAL,
        EQUAL_EQUAL,
        BANG_EQUAL,
        AND,
        OR,
        NOT,
        AMPERSAND_AMPERSAND,
        PIPE_PIPE,
        BANG,
        AMPERSAND,
        PIPE,
        TILDE,
        CARET,
        LESS_LESS,
        GREATER_GREATER,
        PLUS,
        MINUS,
        STAR,
        STAR_STAR,
        SLASH,
        PERCENT,
        EQUAL,
        PLUS_EQUAL,
        MINUS_EQUAL,
        STAR_EQUAL,
        STAR_STAR_EQUAL,
        SLASH_EQUAL,
        PERCENT_EQUAL,
        LESS_LESS_EQUAL,
        GREATER_GREATER_EQUAL,
        AMPERSAND_EQUAL,
        PIPE_EQUAL,
        CARET_EQUAL,
        IF,
        ELIF,
        ELSE,
        FOR,
        WHILE,
        BREAK,
        CONTINUE,
        PASS,
        RETURN,
        MATCH,
        WHEN,
        AS,
        ASSERT,
        AWAIT,
        BREAKPOINT,
        CLASS,
        CLASS_NAME,
        CONST,
        ENUM,
        EXTENDS,
        FUNC,
        IN,
        IS,
        NAMESPACE,
        PRELOAD,
        SELF,
        SIGNAL,
        STATIC,
        SUPER,
        TRAIT,
        VAR,
        VOID,
        YIELD,
        BRACKET_OPEN,
        BRACKET_CLOSE,
        BRACE_OPEN,
        BRACE_CLOSE,
        PARENTHESIS_OPEN,
        PARENTHESIS_CLOSE,
        COMMA,
        SEMICOLON,
        PERIOD,
        PERIOD_PERIOD,
        COLON,
        DOLLAR,
        FORWARD_ARROW,
        UNDERSCORE,
        NEWLINE,
        INDENT,
        DEDENT,
        CONST_PI,
        CONST_TAU,
        CONST_INF,
        CONST_NAN,
        VCS_CONFLICT_MARKER,
        BACKTICK,
        QUESTION_MARK,
        ERROR,
        EOF
    };

    struct Token {
        TokenType type;
        std::string value;
        int line;
        int column;
    };

    struct CommentData {
        int line;
        int column;
        std::string comment;
    };

    RiftScriptTokenizer();
    
    void set_code(const std::string &p_code);
    Token next_token();
    Token peek_token() const;
    int get_token_line() const;
    int get_token_column() const;
    void set_cursor_position(int p_line, int p_column);
    void set_multiline_mode(bool p_state);
    bool is_past_cursor() const;

private:
    std::string code;
    size_t current_pos;
    Token current_token;
    int cursor_line;
    int cursor_column;
    bool multiline_mode;
    std::vector<CommentData> comments;

    void make_token(TokenType p_type);
    void make_error(const std::string &p_message);
    void push_error(const std::string &error);
    void make_paren_error(char32_t p_paren);
    bool match_keyword(const std::string &keyword);
    void annotation();
    void make_keyword_list();
    void new_line(bool token);

    static const char *token_names[];
};



#endif //
