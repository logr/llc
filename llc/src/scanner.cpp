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
#include <cctype>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include "scanner.h"
#include "token.h"

namespace llc
{
char EOF_CHAR = (char) -1;

std::string hGenerateUnexpected(char c)
{
    std::ostringstream ss;
    ss << "Unexpected value '" << c << "'";
    return ss.str();
}

std::string hGenerateCharNotAllowed(char c)
{
    std::ostringstream ss;
    ss << "Character '" << c << "' is not allowed in string";
    return ss.str();
}

Scanner::Scanner(const std::string& filepath)
{
    Init();

    if ( !filepath.empty() )
    {
        AttachFile(filepath);
    }
}

Scanner::~Scanner()
{}

void Scanner::Init()
{
    offset_ = 0;
    offset_ = 0;
    line_ = 1;
    column_ = 0;
}

void Scanner::AttachFile(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);

    typedef std::vector<char>::size_type size_type;

    if (file.is_open())
    {
        file.seekg(0, file.end);
        size_type size = static_cast<size_type>(file.tellg());
        file.seekg(0, file.beg);
        input_.resize(size);
        file.read(&input_[0], size);

        NextChar();
    }
    else
    {
        //error
    }
}

std::unique_ptr<Token> Scanner::Scan()
{
    SkipWhitespace();

    auto token = std::unique_ptr<Token>(new Token() );
    FreezePosition(token->coord);

    if (char_ == EOF_CHAR)
    {
        token->type = TokenType::Eof;
        token->value = "";
    }
    else if (char_ == '/')
    {
        ScanComment(token);
    }
    else if (std::isalpha(char_))
    {
        ScanIdentifier(token);
    }
    else if (std::isdigit(char_))
    {
        ScanNumber(token);
    }
    else if (char_ == '\"')
    {
        ScanString(token);
    }
    else
    {
        ScanSymbol(token);
    }

    return token;
}

void Scanner::NextChar()
{
    if (offset_ >= input_.size())
    {
        char_ = EOF_CHAR;
    }
    else
    {
        char_ = input_[offset_];
        offset_ += 1;
        column_ += 1;

        if (char_ == '\n')
        {
            line_ += 1;
            column_ = 0;
        }
    }
}

char Scanner::Peek()
{
    if (offset_ >= input_.size())
    {
        return EOF_CHAR;
    }
    return input_[offset_];
}

void Scanner::FreezePosition(Coord& coord)
{
    coord.line = line_;
    coord.column = column_;
}

void Scanner::SkipWhitespace()
{
    while (std::isspace(char_))
    {
        NextChar();
    }
}

void Scanner::ScanComment(const std::unique_ptr<Token>& token)
{
    std::ostringstream ss;
    while (char_ != '\n')
    {
        ss << char_;
        NextChar();
    }

    token->type = TokenType::Comment;
    token->value = ss.str();
}

void Scanner::ScanIdentifier(const std::unique_ptr<Token>& token)
{
    std::ostringstream ss;
    while (char_ == '_' | std::isalnum(char_))
    {
        ss << char_;
        NextChar();
    }

    token->value = ss.str();
    token->type = Token::LookupIdentifier(token->value);
}

void Scanner::ScanNumberPart(std::ostringstream& ss)
{
    while (char_ == '_' | std::isdigit(char_))
    {
        if (char_ != '_')
        {
            ss << char_;
        }
        NextChar();
    }
}

void Scanner::ScanNumber(const std::unique_ptr<Token>& token)
{
    std::ostringstream ss;

    ScanNumberPart(ss);
    token->type = TokenType::Integer;

    bool is_float = false;
    if (char_ == '.')
    {
        is_float = true;
        token->type = TokenType::Float;
        ScanNumberPart(ss);
    }

    token->value = ss.str();
}

void Scanner::ScanString(const std::unique_ptr<Token>& token)
{
    NextChar();

    bool skip_next = false;
    std::string allowed_chars = " _,;:.'";
    std::ostringstream ss;

    while (char_ != '"')
    {
        if (char_ == EOF_CHAR | char_ == '\n')
        {
            skip_next = true;
            warnings_.push_back(std::make_shared<Warning>("Unterminated string", token->coord));
            break;
        }

        if (!std::isalnum(char_) | allowed_chars.find(char_) == std::string::npos)
        {
            skip_next = true;
            std::string message = hGenerateCharNotAllowed(char_);
            warnings_.push_back(std::make_shared<Warning>(message, token->coord));
            break;
        }

        ss << char_;
        NextChar();
    }

    if (!skip_next)
    {
        //Consume trailing quote
        NextChar();
    }

    token->type = TokenType::String;
    token->value = ss.str();
}

void Scanner::ScanSymbol(const std::unique_ptr<Token>& token)
{
    char c;
    switch (char_)
    {
    case ';':
        token->type = TokenType::SemiColon;
        token->value = ";";
        break;
    case ',':
        token->type = TokenType::Comma;
        token->value = ";";
        break;
    case '(':
        token->type = TokenType::Lparen;
        token->value = "(";
        break;
    case ')':
        token->type = TokenType::Rparen;
        token->value = ")";
        break;
    case '[':
        token->type = TokenType::Lbracket;
        token->value = "[";
        break;
    case ']':
        token->type = TokenType::Rbracket;
        token->value = "]";
        break;
    case ':':
        c = Peek();
        if (c == '=')
        {
            NextChar();
            token->type = TokenType::Assign;
            token->value = ":=";
        }
        else
        {
            std::string message = hGenerateUnexpected(c);
            warnings_.push_back(std::make_shared<Warning>(message, token->coord));
        }
        break;
    case '|':
        token->type = TokenType::Or;
        token->value = "|";
        break;
    case '&':
        token->type = TokenType::And;
        token->value = "&";
        break;
    case '+':
        token->type = TokenType::Add;
        token->value = "+";
        break;
    case '-':
        token->type = TokenType::Sub;
        token->value = "-";
        break;
    case '*':
        token->type = TokenType::Mul;
        token->value = "*";
        break;
    case '/':
        token->type = TokenType::Div;
        token->value = "/";
        break;
    case '<':
        if (Peek() == '=')
        {
            NextChar();
            token->type = TokenType::LessEql;
            token->value = "<=";
        }
        else
        {
            token->type = TokenType::Less;
            token->value = "<";
        }
        break;
    case '>':
        if (Peek() == '=')
        {
            NextChar();
            token->type = TokenType::GreaterEql;
            token->value = ">=";
        }
        else
        {
            token->type = TokenType::Greater;
            token->value = ">";
        }
        break;
    case '=':
        c = Peek();
        if (c == '=')
        {
            NextChar();
            token->type = TokenType::Eql;
            token->value = "==";
        }
        else
        {
            std::string message = hGenerateUnexpected(c);
            warnings_.push_back(std::make_shared<Warning>(message, token->coord));
        }
        break;
    case '!':
        c = Peek();
        if (c == '=')
        {
            NextChar();
            token->type = TokenType::Neq;
            token->value = "!=";
        }
        else
        {
            std::string message = hGenerateUnexpected(c);
            warnings_.push_back(std::make_shared<Warning>(message, token->coord));
        }
        break;
    default:
        std::string message = hGenerateUnexpected(char_);
        warnings_.push_back(std::make_shared<Warning>(message, token->coord));
    }
    NextChar();
}
}