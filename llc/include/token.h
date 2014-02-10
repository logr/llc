// llc is a compiler for a toy language
// Copyright (C) 2014  Logan Romantic

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#ifndef LLC_TOKENS_H_
#define LLC_TOKENS_H_

#include <cstdint>

namespace llc
{
enum class TokenType : std::int8_t;

struct Coord
{
    Coord() : line(-1), column(-1) {}
    Coord(int line_, int column_) : line(line_), column(column_) {}

    std::string String() const;
    friend std::ostream& operator<<(std::ostream& os, const Coord& coord);

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
    static std::string GetName(TokenType type);

    friend std::ostream& operator<<(std::ostream& os, const Token& token);

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