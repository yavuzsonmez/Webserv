#include <string>
#include <iterator>
#include <iostream>

bool isx(char c)
{
	return (c != '(' && c != ')');
}

std::string filter_characters(std::string characters)
{
	std::string::iterator	it = characters.begin();
	std::string::iterator	ite = characters.end();

	it = std::remove_if(it, ite, isx);
	characters.erase(it, ite);
	return (characters);
}

// int	main(void)
// {
// 	std::string	str = filter_characters("(123(456(789()987)654)321)");
// 	std::cout << "string: " << str << std::endl;
// 	std::cout << "len: " << str.size() << std::endl;
// 	return (0);
// }