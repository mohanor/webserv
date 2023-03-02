#include <iostream>


int main()
{
	int i = 0;
	std::string str = "server {location /best_image_on_earth {return 307 images/ecureuil.jpg;}}";

	while (str[i] != '\0')
	{
		switch (str[i])
		{
			case ' ':
				std::cout << '\n';
				i++;
				break;
			case '!':
			case '}':
			case '{':
				std::cout << str[i] << '\n';
				i++;
				break;
			case ';':
				std::cout << ";\n";
				i++;
			default:
				std::cout << str[i];
				i++;
				break;
		}
	}
	return 0;
}
