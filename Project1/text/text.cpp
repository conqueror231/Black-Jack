#include<iostream>
#include<fstream>
#include<string>

int main() {

	std::ifstream file("example.txt");

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::cout << line << std::endl;
		}
		file.close();
	}
	else {
		std::cout << "File not oppened!" << std::endl;
	}

}