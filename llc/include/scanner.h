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
#ifndef LLC_SCANNER_H_
#define LLC_SCANNER_H_

#include <vector>
#include <memory>
#include "error.h"

namespace llc
{
class Token;
struct Coord;

typedef std::vector<std::shared_ptr<Warning>> warnings_vector;

class Scanner
{
public:
    Scanner(const std::string& filepath = "");
    ~Scanner();

    std::unique_ptr<Token> Scan();
    void AttachFile(const std::string& filepath);

    int line() const
    {
        return line_;
    }
    int column() const
    {
        return column_;
    }
    warnings_vector warnings() const
    {
        return warnings_;
    };

private:
    void NextChar();
    char Peek();
    void FreezePosition(Coord& coord);
    void SkipWhitespace();
    void ScanComment(const std::unique_ptr<Token>& token);
    void ScanIdentifier(const std::unique_ptr<Token>& token);
    void ScanNumberPart(std::ostringstream& ss);
    void ScanNumber(const std::unique_ptr<Token>& token);
    void ScanString(const std::unique_ptr<Token>& token);
    void ScanSymbol(const std::unique_ptr<Token>& token);

    void Init();

    char char_;
    unsigned int offset_;
    int line_;
    int column_;
    std::vector<char> input_;
    warnings_vector warnings_;
};
}

#endif