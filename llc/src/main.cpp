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
#include "scanner.h"

std::string FormatCoord(const llc::Coord& coord);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Expecting input file";
        exit(-1);
    }

    std::cout << "Scanning " << argv[1] << std::endl;
    llc::Scanner ss(argv[1]);

    llc::Token t = ss.Scan();
    while (t.type != llc::TokenType::Eof)
    {
        std::cout << t.GetName() << " " << t.value << std::endl;
        t = ss.Scan();
    }

    for (auto x : ss.warnings())
    {
        std::cout << x->message() << " " << x->coord().String() << std::endl;
    }
}