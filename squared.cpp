#include "squared.hpp"
#include "idempotent.hpp"
#include <vector>
#include <iostream>

#define BITMAX 32

// Initiallizes an instance of a TransientInfo 
void transient_insert(TransientInfo *t, int l, int i, int pos) {
	t->length = l;
	t->index = i;
	t->position = pos;
}
/* Class that implements the squared speedup method for 
 * arbitrary idempotent iterations.
 */
Squared::Squared(Idempotent f) : f(f) {
	log_table.resize(f.size());
	for (int i = 0; i < f.size(); ++i) {
		log_table[i].resize(BITMAX-__builtin_clz(f.size()));
	}
	transient.resize(f.size());
	solve();
}
/* Populates the table and vectors with the information we need to
 * compute arbitrary iterations
 */
void Squared::solve() {
	for (int i = 0; i < f.size(); ++i) {
		if (transient[i] != nullptr) {
			/* we've already explored the transient containing i
			 */
			continue;
		}
		
		update_entry(i);	
	}
	
	for (int i = 0; i < log_table[0].size(); ++i) {
		for (int j = 0; j < f.size(); ++j) {
			if (i == 0) { 
					log_table[j][i] = j; 
					continue;
			}
			if (i == 1) {
					log_table[j][i] = f.eval(j);
					continue;
			}
			log_table[j][i] = log_table[log_table[j][i-1]][i-1];
		}
	}
}
/* Updates the transient information for element i and everything 
 * along its path
 */
void Squared::update_entry(int entry) {
	/* We do the inelegant method of cycle detection where we 
	 * store all the previous values and check if we've already 
	 * seen a value. This may seem terrible considering our 
	 * discussion on Floyd's algorithm, but we need to store 
	 * all the values on the transient anyway, so it's not a 
	 * huge deal.
	 */
	std::vector<int> element_ord, cycle_vec;
	int parent, current = entry;

	while (transient[current] == nullptr) {
		transient[current] = new TransientInfo;
		transient_insert(transient[current], -1, -1, -1);
		element_ord.push_back(current);
		current = f.eval(current);
	}
	
	if (transient[current]->position != -1) {
		// We've encountered an element on this transient already
		TransientInfo *t_parent = transient[current];
		for (int i = element_ord.size()-1; i >= 0; --i) {
			parent = element_ord[i];
			TransientInfo *t_current = t_parent;
			t_parent = transient[parent];
			transient_insert(t_parent, 
							t_current->length+1, 
							t_current->index, 
							t_current->position);
			transient[parent] = t_parent;
		}
		return;
	}

	int cycle_start;
	for (cycle_start = 0; cycle_start < element_ord.size(); ++cycle_start) {
		if (current == element_ord[cycle_start]) { break; }
	}

	for (int i = 0; i < cycle_start; ++i) {
		TransientInfo *trans = transient[element_ord[i]];
		transient_insert(trans, cycle_start-i, cycles.size(), 0);
	}

	for (int j = cycle_start; j < element_ord.size(); ++j) {
		cycle_vec.push_back(element_ord[j]);
		TransientInfo *trans = transient[element_ord[j]];
		transient_insert(trans, 0, cycles.size(), j-cycle_start);
	}

	cycles.push_back(cycle_vec);
}
// Computes f^i(x)
int Squared::eval(int x, int i) {
	TransientInfo *trans = transient[x];
	if (i > trans->length) {
		int cycle_length = cycles[trans->index].size();
		int cycle_index = ((i - trans->length) % cycle_length + trans->position) % cycle_length;
		return cycles[trans->index][cycle_index];
	}
	if (i == 0) {
		return x;
	}
	int n = i;
	int y = x;
	while (n > 0) {
		int log_factor = BITMAX-__builtin_clz(n);
		y = log_table[y][log_factor];
		n = n - (1 << (log_factor-1));
	}
	return y;
}
// Debug function that prints out log_table
void Squared::print_table() {
	for (int i = 0; i < f.size(); ++i) {
		std::cout << i << ":";
		for (int j = 0; j < log_table[i].size(); ++j) {
			std::cout << "\t" << log_table[i][j];
		}
		std::cout << "\n";
	}
}
// Function that frees the memory we allocated
void Squared::free() {
	for (int i = 0; i < transient.size(); ++i) {
		delete transient[i];
	}
}
