#ifndef COMPILER_TOKENS_H_
#define COMPILER_TOKENS_H_

#include <cstdint>

namespace llc
{
    enum class TokenType : std::int8_t;

    struct Coord 
    {
        Coord() : line(-1), column(-1) {}
        Coord(int line_, int column_) : line(line_), column(column_) {}
		std::string String();

        int line;
        int column;
    };

    class Token
    {
    public:
        Token();
        Token(TokenType token_type, std::string value, Coord coord);
        ~Token();

        static TokenType LookupIdentifier(const std::string& value);
		std::string GetName() const;

        TokenType type;
        std::string value;
        Coord coord;
    };

    enum class TokenType : std::int8_t 
    {
        //Special
        Illegal,
        Eof,
        Comment,

        //Literals
        Identifier,
        Integer,
        Float,
        Bool,
        String,

        //Symbols
        SemiColon,
        Comma,
        Lparen,
        Rparen,
        Lbracket,
        Rbracket,
        Assign,
        Or,
        And,
        Add,
        Sub,
        Mul,
        Div,
        Less,
        Greater,
        LessEql,
        GreaterEql,
        Eql,
        Neq,

        //Keywords
        Program,
        Is,
        Begin,
        End,
        Global,
        Procedure,
        In,
        Out,
        IntegerType,
        FloatType,
        BoolType,
        StringType,
        If,
        Then,
        Else,
        For,
        Return,
        Not,
        TrueKey,
        FalseKey,
    };

}

#endif