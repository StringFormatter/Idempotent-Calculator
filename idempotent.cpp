#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "idempotent.hpp"

// Default constructor, does nothing
Idempotent::Idempotent() {}

// Constructor, establishes the size of the function domain/codomain
Idempotent::Idempotent(int domain_size) {
	mapping.resize(domain_size);
}

// Constructor, takes in a file path and pulls in the function data
Idempotent::Idempotent(std::string path) {
	std::ifstream file (path);
	std::string line;
	int tmp;
	while (file >> tmp) {
		mapping.push_back(tmp);
	}	
	file.close();
}

// Pointwise evaluation of the function
int Idempotent::eval(int x) {
	return mapping[x];		
}

// Iterative evaluation i times
int Idempotent::eval(int x, int i) {
	int y = x;	
	for (int j = 0; j < i; ++j) {
		y = mapping[y];	
	}
	return y;
}

// Creates a random function 
void Idempotent::populate() {
	std::srand(std::time(nullptr));
	for (int i = 0; i < mapping.size(); ++i) {
		mapping[i] = std::rand() / ((RAND_MAX + 1u) / mapping.size());
	}
}

// Gets the size of the domain/codomain
int Idempotent::size() {
	return mapping.size();
}
