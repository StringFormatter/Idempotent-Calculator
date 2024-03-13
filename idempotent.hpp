#ifndef IDEMPOTENT_HPP
#define IDEMPOTENT_HPP
#include <vector>
#include <string>

/* Class containing all the necessary methods for our implementation of 
 * an idempotent function. We implement it with a vector that stores 
 * all the function input output pairs.
 */
class Idempotent {
		// The vector which stores the function values
		std::vector<int> mapping;	
	public:
		// Default constructor, does nothing
		Idempotent();
		// Constructor, establishes the size of the function domain/codomain
		Idempotent(int domain_size);
		// Constructor, takes in a file path and pulls in the function data
		Idempotent(std::string path);
		// Pointwise evaluation of the function
		int eval(int x);
		// Iterative evaluation i times
		int eval(int x, int i);
		// Creates a random function 
		void populate();
		// Gets the size of the domain/codomain
		int size();
};

#endif
