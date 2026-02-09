%{
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

struct Token {
    std::string type;
    std::string lexeme;
    int line;
    int column;
};

extern std::vector<Token> tokens;
int line_num = 1;
int column_num = 1;

#define YY_USER_ACTION \
    column_num += yyleng;
%}

%option noyywrap
%option yylineno

KEYWORD			if|else|while|for|return|int|float|string|bool|true|false|void|struct|class|public|private|protected
NUMBER			[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?
IDENTIFIER		[a-zA-Z_][a-zA-Z0-9_]*
STRING			\"([^"\\]|\\.)*\"
OPERATOR		(\+\+|--|\+=|-=|\*=|\/=|==|!=|<=|>=|&&|\|\||[+\-*/%=<>&|!])
PUNCTUATION		[(){}\[\],;.:]
WHITESPACE		[ \t]+
COMMENT_LINE	\/\/.*
COMMENT_BLOCK	\/\*[\s\S]*?\*\/

%%

{KEYWORD}		{
					Token t;
					t.type = "KEYWORD";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

{NUMBER}		{
					Token t;
					t.type = "NUMBER";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

{STRING}		{
					Token t;
					t.type = "STRING";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

{IDENTIFIER}	{
					Token t;
					t.type = "IDENTIFIER";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

{OPERATOR}		{
					Token t;
					t.type = "OPERATOR";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

{PUNCTUATION}	{
					Token t;
					t.type = "PUNCTUATION";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

{COMMENT_LINE}	{ /* Ignore line comments */ }

{COMMENT_BLOCK}	{ /* Ignore block comments */ }

{WHITESPACE}	{ /* Ignore whitespace */ }

\n				{
					line_num++;
					column_num = 1;
				}

.				{
					Token t;
					t.type = "UNKNOWN";
					t.lexeme = yytext;
					t.line = yylineno;
					t.column = column_num - yyleng;
					tokens.push_back(t);
				}

%%
