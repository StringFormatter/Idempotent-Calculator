#ifndef GRAPH_SOLUTION_HPP
#define GRAPH_SOLUTION_HPP
#include <vector>
#include "idempotent.hpp"

/* Stores a contiguous part of the transient/cycle
 * along with its child, the position on the child 
 * we start at, and whether it is a cycle or not
 */
struct TransientNode {
	std::vector<int> segment;	
	TransientNode *child;
	int position;
	bool is_cycle;
};

/* Stores the TransientNode containing a domain element and 
 * the position on that transient/cycle where the element lies.
 */
struct TransientHeader {
	TransientNode *location;
	int position;
};

/* Class that implements the graph based speedup method for 
 * arbitrary idempotent iterations.
 */
class Graph {
		// Stores all the Transient Headers
		std::vector<TransientHeader*> headers;
		// Stores the reverse graph for the idempotent function
		std::vector<std::vector<int>> reverse;
		// Stores the node pointers so we can free them later
		std::vector<TransientNode*> nodes;
		// The idempotent function we want to solve for
		Idempotent f;	
	public:	
		// Constructor which implements a solution for idempotent function f
		Graph(Idempotent f);
		
		/* Populates all the transient headers and constructs 
		 * the graph topology
		 */	
		void solve();
		
		/* Find the cycle along the transient starting at n, 
		 * and constructs the TransientHeaders for each element of 
		 * the cycle. If we encounter an element we've already 
		 * seen, we stop (as the cycle along that path has already been found)
		 */
		void find_cycle(int n, std::vector<bool> *seen);

		/* Constructs the TransientNode starting at leaf
		 */
		void construct_transient(int leaf);
		
		// Computes f^i(x)
		int eval(int x, int i);

		// Frees the memory we allocated
		void free();
};
#endif
