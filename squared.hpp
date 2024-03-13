#ifndef SQUARED_SOLUTION_HPP
#define SQUARED_SOLUTION_HPP
#include <vector>
#include "idempotent.hpp"

/* Stores the length of the transient,
 * the index of the cycle it leads to in the cycle vector, and 
 * the position in the cycle the transient enters
 */
struct TransientInfo {
	int length;
	int index;
	int position;
};

// Initiallizes an instance of a TransientInfo 
void transient_insert(TransientInfo *t, int l, int i, int pos);

/* Class that implements the squared speedup method for 
 * arbitrary idempotent iterations.
 */
class Squared {
		// Stores the values for f^(log_2(i))(x) for every i,x < n for domain size n
		std::vector<std::vector<int>> log_table;
		// Stores every cycle as a vector containing the cycle elements in order
		std::vector<std::vector<int>> cycles;
		// Stores the transient info for each element in the domain
		std::vector<TransientInfo*> transient;
		// The idempotent function we are finding the solution for
		Idempotent f;	
	public:
		// Constructor which implements a solution for idempotent function f
		Squared(Idempotent f);
		/* Populates the table and vectors with the information we need to
		 * compute arbitrary iterations
		 */
		void solve();
		/* Updates the transient information for element i and everything 
		 * along its path
		 */
		void update_entry(int entry);
		// Computes f^i(x)
		int eval(int x, int i);
		// Returns the amount of space used by the solution
		int get_storage();
		// Debug function that prints out log_table
		void print_table();
		// Function that frees the memory we allocated
		void free();
};

#endif

