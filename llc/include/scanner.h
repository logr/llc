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
#ifndef COMPILER_SCANNER_H_
#define COMPILER_SCANNER_H_

#include <vector>
#include <memory>
#include "token.h"

namespace llc
{
    class Warning; 
    class Error;

    typedef std::vector<std::shared_ptr<Warning>> warnings_vector;
    typedef std::vector<std::shared_ptr<Error>> errors_vector;

    class Scanner
    {
    public:
        Scanner();
        Scanner(std::string filepath);
        
        ~Scanner();

        Token Scan();
        void AttachFile(std::string filepath);

        int line() const { return line_; }
        int column() const { return column_; }
        bool has_errors() const { return has_errors_; }
        warnings_vector warnings() const { return warnings_; };

    private:
        void NextChar();
        char Peek();
        void FreezePosition(Coord& coord);
        void SkipWhitespace();
        void ScanComment(Token& token);
        void ScanIdentifier(Token& token);
        void ScanNumberPart(std::ostringstream& ss);
        void ScanNumber(Token& token);
        void ScanString(Token& token);
		void ScanSymbol(Token& token);

        void Init();

        char char_;
        bool has_errors_;
        unsigned int offset_;
        int line_;
        int column_;
        std::vector<char> input_;
        warnings_vector warnings_;
    };

    class Warning
    {
    public:
        Warning(std::string message, Coord coord);
        ~Warning();

        std::string message() const { return message_; }
        Coord coord() const { return coord_; }

    private:
        std::string message_;
        Coord coord_;
    };
}

#endif