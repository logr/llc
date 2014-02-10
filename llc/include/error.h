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
#ifndef LLC_ERROR_H_
#define LLC_ERROR_H_

#include <string>
#include "token.h"

namespace llc
{
class Warning
{
public:
    Warning(std::string message, Coord coord) : message_(message), coord_(coord)
    { }

    ~Warning()
    { }

    friend std::ostream& operator<<(std::ostream& os, const Warning& warning)
    {
        os << warning.message_ << " " << warning.coord_;
        return os;
    }

    std::string message() const
    {
        return message_;
    }
    Coord coord() const
    {
        return coord_;
    }

private:
    std::string message_;
    Coord coord_;
};

class Error : public std::exception, public Warning
{
public:
    Error(const std::string& message, Coord coord) : Warning(message, coord)
    { }

    ~Error() throw()
    { }

    virtual const char* what() const throw()
    {
        return message().c_str();
    }
};
}

#endif