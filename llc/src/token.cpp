#include <map>
#include <string>
#include <sstream>
#include "token.h"

namespace llc 
{
	std::map<std::string, TokenType> keywords = {
        {"program", TokenType::Program},
        {"is", TokenType::Is},
        {"begin", TokenType::Begin},
        {"end", TokenType::End},
        {"global", TokenType::Global},
        {"procedure", TokenType::Procedure},
        {"in", TokenType::In},
        {"out", TokenType::Out},
        {"integer", TokenType::IntegerType},
        {"float", TokenType::FloatType},
        {"bool", TokenType::BoolType},
        {"string", TokenType::StringType},
        {"if", TokenType::If},
        {"then", TokenType::Then},
        {"else", TokenType::Else},
        {"for", TokenType::For},
        {"return", TokenType::Return},
        {"not", TokenType::Not},
        {"true", TokenType::TrueKey},
        {"false", TokenType::FalseKey},
    };

	std::map<TokenType, std::string> token_names = {
		{ TokenType::Illegal, "ILLEGAL" },
		{ TokenType::Eof, "EOF" },
		{ TokenType::Comment, "COMMENT" },
		{ TokenType::Identifier, "IDENT" },
		{ TokenType::Integer, "INT" },
		{ TokenType::Float, "FLOAT" },
		{ TokenType::Bool, "BOOL" },
		{ TokenType::String, "STRING" },
		{ TokenType::SemiColon, ";" },
		{ TokenType::Comma, "," },
		{ TokenType::Lparen, "(" },
		{ TokenType::Rparen, ")" },
		{ TokenType::Lbracket, "[" },
		{ TokenType::Rbracket, "]" },
		{ TokenType::Assign, ":=" },
		{ TokenType::Or, "|" },
		{ TokenType::And, "&" },
		{ TokenType::Add, "+" },
		{ TokenType::Sub, "-" },
		{ TokenType::Mul, "*" },
		{ TokenType::Div, "/" },
		{ TokenType::Less, "<" },
		{ TokenType::Greater, ">" },
		{ TokenType::LessEql, "<=" },
		{ TokenType::GreaterEql, ">=" },
		{ TokenType::Eql, "==" },
		{ TokenType::Neq, "!=" },
		{ TokenType::Program, "program" },
		{ TokenType::Is, "is" },
		{ TokenType::Begin, "begin" },
		{ TokenType::End, "end" },
		{ TokenType::Global, "global" },
		{ TokenType::Procedure, "procedure" },
		{ TokenType::In, "in" },
		{ TokenType::Out, "out" },
		{ TokenType::IntegerType, "integer" },
		{ TokenType::FloatType, "float" },
		{ TokenType::BoolType, "bool" },
		{ TokenType::StringType, "string" },
		{ TokenType::If, "if" },
		{ TokenType::Then, "then" },
		{ TokenType::Else, "else" },
		{ TokenType::For, "for" },
		{ TokenType::Return, "return" },
		{ TokenType::Not, "not" },
		{ TokenType::TrueKey, "true" },
		{ TokenType::FalseKey, "false" },
	};

	std::string Coord::String()
	{
		std::ostringstream ss;
		ss << "(line " << line << ", column " << column << ")";
		return ss.str();
	}

    Token::Token() : type(TokenType::Illegal), value("")
    {}

    Token::Token(TokenType token_type_, std::string value_, Coord coord_)
        : type(token_type_), value(value_), coord(coord_) 
    {}

    Token::~Token() 
    {}

    TokenType Token::LookupIdentifier(const std::string& value)
    {
        if (keywords.find(value) != keywords.end()) {
            return keywords[value];
        }
        return TokenType::Identifier;
    }

	std::string Token::GetName() const
	{
		return token_names[type];
	}
}