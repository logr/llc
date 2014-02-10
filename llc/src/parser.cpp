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
#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include "parser.h"
#include "scanner.h"


namespace llc
{

#define FIRST_PROC_DECL TokenType::Procedure

#define FIRST_VAR_DECL  TokenType::IntegerType, TokenType::FloatType, \
                        TokenType::StringType, TokenType::BoolType

#define FIRST_ARG_LIST  TokenType::Float, TokenType::Lparen, \
                        TokenType::Sub, TokenType::Identifier, \
                        TokenType::Integer, TokenType::String, \
                        TokenType::TrueKey, TokenType::FalseKey

#define FIRST_EXPR  TokenType::Float, TokenType::Lparen, \
                    TokenType::Sub, TokenType::Identifier, \
                    TokenType::Integer, TokenType::String, \
                    TokenType::TrueKey, TokenType::FalseKey

std::set<TokenType> first_decl = std::set<TokenType> { TokenType::Global, FIRST_VAR_DECL, FIRST_PROC_DECL };
std::set<TokenType> first_proc_decl = std::set<TokenType> { FIRST_PROC_DECL };
std::set<TokenType> first_var_decl = std::set<TokenType> { FIRST_VAR_DECL };
std::set<TokenType> first_arg_list = std::set<TokenType> { FIRST_ARG_LIST };
std::set<TokenType> first_expr = std::set<TokenType> { FIRST_EXPR };
std::set<TokenType> first_expr_top = std::set<TokenType> { FIRST_EXPR, TokenType::Not };
std::set<TokenType> first_statement = std::set<TokenType> { TokenType::If, TokenType::For, TokenType::Identifier, TokenType::Return };

bool in_set(const std::set<TokenType>& set, TokenType type)
{
    return set.find(type) != set.end();
}

Parser::Parser(const std::string& filepath)
{
    scanner_ = std::unique_ptr<Scanner>(new Scanner());

    if ( !filepath.empty() )
    {
        AttachFile(filepath);
    }
}

Parser::~Parser()
{ }

void Parser::AttachFile(const std::string& filepath)
{
    scanner_->AttachFile(filepath);
    curr_token_ = nullptr;
    next_token_ = nullptr;
    AdvanceToken();
    AdvanceToken();
}

void Parser::Parse()
{
    ParseProgram();
}

void Parser::AdvanceToken()
{
    do
    {
        curr_token_ = std::move(next_token_);
        next_token_ = std::move(scanner_->Scan());
    }
    while ( curr_token_ != nullptr && curr_token_->type == TokenType::Comment );
}

void Parser::Expected(TokenType type)
{
    std::ostringstream ss;
    ss << "Syntax error: expected " << Token::GetName(type) << " got " << *curr_token_;
    Error err(ss.str(), curr_token_->coord);
    errors_.push_back(err);
    throw err;
}

void Parser::Expected(const std::string& production)
{
    std::ostringstream ss;
    ss << "Syntax error: expected " << production << " got " << *curr_token_;
    Error err(ss.str(), curr_token_->coord);
    errors_.push_back(err);
    throw err;
}

void Parser::Expected(const std::set<TokenType>& types)
{
    std::ostringstream ss;
    ss << "Syntax error: expected (";

    std::vector<TokenType> typesarr = std::vector<TokenType>(begin(types), end(types));
    for ( size_t i = 0; i < typesarr.size(); i++ )
    {
        if ( i > 0 )
        {
            ss << ", ";
        }
        ss << Token::GetName(typesarr[i]);
    }

    ss << ") got " << *curr_token_;
    Error err(ss.str(), curr_token_->coord);
    errors_.push_back(err);
    throw err;
}

void Parser::Match(TokenType type)
{
    if ( curr_token_->type != type )
    {
        Expected(type);
    }
    else
    {
        AdvanceToken();
    }
}

TokenType Parser::Match(const std::set<TokenType>& types)
{
    if ( in_set(types, curr_token_->type) )
    {
        TokenType found = curr_token_->type;
        AdvanceToken();
        return found;
    }
    else
    {
        Expected(types);
        return TokenType::Illegal;
    }
}

bool Parser::Accept(TokenType type)
{
    if ( curr_token_->type != type )
    {
        return false;
    }
    else
    {
        AdvanceToken();
        return true;
    }
}

bool Parser::Accept(const std::set<TokenType>& types, TokenType& tokenType)
{
    if ( in_set(types, curr_token_->type) )
    {
        tokenType = curr_token_->type;
        AdvanceToken();
        return true;
    }

    tokenType = TokenType::Illegal;
    return false;
}

void Parser::ParseProgram()
{
    Match(TokenType::Program);
    ParseIdentifier();
    Match(TokenType::Is);
    ParseDeclarations();
    Match(TokenType::Begin);
    ParseStatements();
    Match(TokenType::End);
    Match(TokenType::Program);
}

void Parser::ParseIdentifier()
{
    if ( curr_token_->type == TokenType::Identifier )
    {
        //add to symbol table
        AdvanceToken();
    }
    else
    {
        Expected(TokenType::Identifier);
    }
}

void Parser::ParseDeclarations()
{
    TokenType t = curr_token_->type;
    while ( in_set(first_decl, t) )
    {
        TokenType nt = next_token_->type;
        if ( in_set(first_var_decl, t) | in_set(first_var_decl, nt) )
        {
            ParseVariableDeclaration();
        }
        else if ( in_set(first_proc_decl, t) | in_set(first_proc_decl, nt) )
        {
            ParseProcedureDeclaration();
        }

        Match(TokenType::SemiColon);
        t = curr_token_->type;
    }
}

void Parser::ParseProcedureDeclaration()
{
    bool is_global = Accept(TokenType::Global);

    Match(TokenType::Procedure);

    ParseIdentifier();

    Match(TokenType::Lparen);

    while ( in_set(first_var_decl, curr_token_->type) )
    {
        ParseParameter();

        if ( Accept(TokenType::Rparen) )
        {
            break;
        }
        Match(TokenType::Comma);
    }

    ParseDeclarations();

    Match(TokenType::Begin);

    ParseStatements();

    Match(TokenType::End);
    Match(TokenType::Procedure);
}

void Parser::ParseParameter()
{
    ParseVariableDeclaration();
    std::set<TokenType> param_dirs = std::set<TokenType> { TokenType::In, TokenType::Out };
    Match(param_dirs);
}

void Parser::ParseVariableDeclaration()
{
    bool is_global = Accept(TokenType::Global);

    Match(first_var_decl);
    ParseIdentifier();

    if ( Accept(TokenType::Lbracket) )
    {
        Match(TokenType::Integer);
        Match(TokenType::Rbracket);
    }
}

void Parser::ParseStatements()
{
    while ( in_set(first_statement, curr_token_->type) )
    {
        switch ( curr_token_->type )
        {
        case TokenType::If:
            ParseIfStatement();
            break;
        case TokenType::For:
            ParseLoopStatement();
            break;
        case TokenType::Identifier:
            if ( next_token_->type == TokenType::Lparen )
            {
                ParseProcedureCallStatement();
            }
            else
            {
                ParseAssignmentStatement();
            }
            break;
        case TokenType::Return:
            ParseReturnStatement();
            break;
        default:
            Expected("<statement>");
        }

        Match(TokenType::SemiColon);
    }
}

void Parser::ParseAssignmentStatement()
{
    Match(TokenType::Identifier);

    if ( Accept(TokenType::Lbracket) )
    {
        ParseExpression();
        Match(TokenType::Rbracket);
    }

    Match(TokenType::Assign);

    ParseExpression();
}

void Parser::ParseIfStatement()
{
    Match(TokenType::If);
    Match(TokenType::Lparen);

    if ( in_set(first_expr, curr_token_->type) )
    {
        ParseExpression();
    }
    else
    {
        Expected("<expression>");
    }

    Match(TokenType::Rparen);
    Match(TokenType::Then);
    Match(TokenType::Lparen);

    ParseStatements();

    Match(TokenType::Rparen);

    if ( Accept(TokenType::Else) )
    {
        Match(TokenType::Lparen);
        ParseStatements();
        Match(TokenType::Rparen);
    }

    Match(TokenType::End);
    Match(TokenType::If);
}

void Parser::ParseLoopStatement()
{
    Match(TokenType::For);
    Match(TokenType::Lparen);

    ParseAssignmentStatement();

    Match(TokenType::SemiColon);

    if ( in_set(first_expr, curr_token_->type) )
    {
        ParseExpression();
    }
    else
    {
        Expected("<expression>");
    }

    Match(TokenType::Rparen);
    Match(TokenType::Lparen);

    ParseStatements();

    Match(TokenType::Rparen);
    Match(TokenType::End);
    Match(TokenType::For);
}

void Parser::ParseReturnStatement()
{
    Match(TokenType::Return);
}

void Parser::ParseProcedureCallStatement()
{
    Match(TokenType::Identifier);
    Match(TokenType::Lparen);

    if ( in_set(first_arg_list, curr_token_->type) )
    {
        ParseArgumentList();
    }

    Match(TokenType::Rparen);
}

void Parser::ParseArgumentList()
{
    if ( curr_token_->type == TokenType::Not )
    {
        Expected("<argument>");
    }
    else if ( in_set(first_expr_top, curr_token_->type) )
    {
        ParseExpression();
    }
    else
    {
        Expected("<expression>");
    }

    if ( Accept(TokenType::Comma) )
    {
        ParseArgumentList();
    }
}

void Parser::ParseExpression()
{
    bool negate = Accept(TokenType::Not);

    if ( in_set(first_expr, curr_token_->type) )
    {
        ParseArithOp();
    }

    TokenType tokenType;
    std::set<TokenType> continuations = std::set<TokenType> { TokenType::And, TokenType::Or };
    if ( Accept(continuations, tokenType) )
    {
        ParseArithOp();
        ParseExpression();
    }
}

void Parser::ParseArithOp()
{
    if ( in_set(first_expr, curr_token_->type) )
    {
        ParseRelation();
    }

    TokenType tokenType;
    std::set<TokenType> continuations = std::set<TokenType> { TokenType::Add, TokenType::Sub };
    if ( Accept(continuations, tokenType) )
    {
        ParseRelation();
        ParseArithOp();
    }
}

void Parser::ParseRelation()
{
    if ( in_set(first_expr, curr_token_->type) )
    {
        ParseTerm();
    }

    TokenType tokenType;
    std::set<TokenType> continuations = std::set<TokenType>
    {
        TokenType::Less, TokenType::LessEql,
        TokenType::Greater, TokenType::GreaterEql,
        TokenType::Neq, TokenType::Eql
    };

    if ( Accept(continuations, tokenType) )
    {
        ParseTerm();
        ParseRelation();
    }
}

void Parser::ParseTerm()
{
    if ( in_set(first_expr, curr_token_->type) )
    {
        ParseFactor();
    }

    TokenType tokenType;
    std::set<TokenType> continuations = std::set<TokenType> { TokenType::Mul, TokenType::Div };

    if ( Accept(continuations, tokenType) )
    {
        ParseFactor();
        ParseTerm();
    }

}

void Parser::ParseFactor()
{
    TokenType next_type = next_token_->type;

    switch ( curr_token_->type )
    {
    case TokenType::Lparen:
        Match(TokenType::Lparen);
        ParseExpression();
        Match(TokenType::Rparen);
        break;
    case TokenType::Sub:
        if ( next_type == TokenType::Identifier )
        {
            ParseName();
        }
        else if ( next_type == TokenType::Integer | next_type == TokenType::Float )
        {
            ParseNumber();
        }
        else
        {
            Expected("<name> or <number>");
        }
        break;
    case TokenType::Integer:
    case TokenType::Float:
        ParseNumber();
        break;
    case TokenType::Identifier:
        ParseName();
        break;
    case TokenType::String:
        //Handle string
        break;
    case TokenType::TrueKey:
        break;
    case TokenType::FalseKey:
        break;
    default:
        Expected("<factor>");
        break;
    }
}

void Parser::ParseName()
{
    Match(TokenType::Identifier);

    if ( Accept(TokenType::Lbracket) )
    {
        ParseExpression();
        Match(TokenType::Rbracket);
    }
}

void Parser::ParseNumber()
{
    if ( curr_token_->type == TokenType::Sub )
    {
        AdvanceToken();
    }
    AdvanceToken();
}
}
