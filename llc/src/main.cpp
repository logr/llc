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