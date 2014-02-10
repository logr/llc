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
#ifndef LLC_PARSER_H_
#define LLC_PARSER_H_

#include <vector>
#include <memory>
#include <cstdint>
#include <set>

namespace llc
{
class Warning;
class Error;
class Token;
class Scanner;
enum class TokenType : int8_t;

class Parser
{
public:
    Parser(const std::string& filepath = "");
    ~Parser();

    void AttachFile(const std::string& filepath);
    void Parse();

private:
    void AdvanceToken();
    void Expected(TokenType type);
    void Expected(const std::string& production);
    void Expected(const std::set<TokenType>& types);
    void Match(TokenType type);
    TokenType Match(const std::set<TokenType>& types);
    bool Accept(TokenType type);
    bool Accept(const std::set<TokenType>& types, TokenType& tokenType);

    void ParseProgram();
    void ParseDeclarations();
    void ParseProcedureDeclaration();
    void ParseParameter();
    void ParseVariableDeclaration();
    void ParseStatements();
    void ParseIdentifier();
    void ParseAssignmentStatement();
    void ParseIfStatement();
    void ParseLoopStatement();
    void ParseReturnStatement();
    void ParseProcedureCallStatement();
    void ParseExpression();
    void ParseArgumentList();
    void ParseArithOp();
    void ParseRelation();
    void ParseTerm();
    void ParseFactor();
    void ParseName();
    void ParseNumber();

    std::unique_ptr<Scanner> scanner_;
    std::unique_ptr<Token> curr_token_;
    std::unique_ptr<Token> next_token_;
    std::vector<Warning> warnings_;
    std::vector<Error> errors_;
};

}

#endif