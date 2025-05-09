#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class FileToVector
{
private:
	std::vector<T> tokens;

public:
	FileToVector();
	~FileToVector();

	bool parse(std::string path);
	void printTokens();
	const std::vector<T> &getTokens() const;
};

template <typename T>
inline FileToVector<T>::FileToVector()
{
}

template <typename T>
inline FileToVector<T>::~FileToVector()
{
}
template <typename T>
bool FileToVector<T>::parse(std::string path)
{
    std::ifstream input(path);
    if (!input.is_open())
        return (std::cerr << "Error: Could not open file " << path << "\n", false);
    std::string line;
    while (std::getline(input, line)) {
		std::size_t hashPos = line.find('#');
        if (hashPos != std::string::npos)
            line.erase(hashPos);
		std::istringstream iss(line);
		T token;
		while (iss >> token) {
			this->tokens.push_back(token);
		}
    }
    input.close();
    return true;
}

template <typename T>
inline void FileToVector<T>::printTokens()
{
	for (const T& token : tokens)
		std::cout << token << " ";
	std::cout << std::endl;
}


template <typename T>
inline const std::vector<T> &FileToVector<T>::getTokens() const
{
	return (tokens);
}
