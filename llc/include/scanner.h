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