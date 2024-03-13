#include <vector>
#include <map>
#include "idempotent.hpp"
#include "graph.hpp"


// Constructor which implements a solution for idempotent function f
Graph::Graph(Idempotent f) : f(f) {
	headers.resize(f.size());
	reverse.resize(f.size());
	int y;
	for (int x = 0; x < f.size(); ++x) {
		y = f.eval(x);
		reverse[y].push_back(x);
	}
	solve();
}

/* Populates all the transient headers and constructs 
 * the graph topology
 */	
void Graph::solve() {
	std::vector<bool> seen;
	std::multimap<int,int> leaves;
	int length, leaf;	
	seen.resize(f.size());	
	for (int i = 0; i < f.size(); ++i) {
		if (headers[i] != nullptr) {
			continue;
		}
		find_cycle(i, &seen);
	}
	for (int j = 0; j < reverse.size(); ++j) {
		if (reverse[j].size() > 0) {
			continue;
		}
		length = 0;
		leaf = j;
		while (headers[leaf] == nullptr) {
			length += 1;
			leaf = f.eval(leaf);
		}
		leaves.insert({length, j});
	}
	std::multimap<int, int>::reverse_iterator it;
	for (it = leaves.rbegin(); it != leaves.rend(); it++) {
		construct_transient(it->second);
	}
}

/* Find the cycle along the transient starting at n, 
 * and constructs the TransientHeaders for each element of 
 * the cycle. If we encounter an element we've already 
 * seen, we stop (as the cycle along that path has already been found)
 */
void Graph::find_cycle(int n, std::vector<bool> *seen) {
	/* The animal version of Floyd's algorithm
	 * A translation of the python version found on wikipedia 
	 * (but in c++ so it should be faster :)
	 */
	TransientNode *cycle_node = new TransientNode;
	nodes.push_back(cycle_node);
	cycle_node->is_cycle = true;
	int tortoise = f.eval(n);
	int hare = f.eval(tortoise);
	if ((*seen)[tortoise]) { return; }
	while (tortoise != hare) {
		if ((*seen)[tortoise]) { return; }
		tortoise = f.eval(tortoise);
		hare = f.eval(f.eval(hare));
	}
	
	tortoise = n;	
	while (tortoise != hare) {
		(*seen)[tortoise] = true;
		tortoise = f.eval(tortoise);
		hare = f.eval(hare);
	}
	
	cycle_node->segment.push_back(tortoise);
	(*seen)[hare] = true;
	hare = f.eval(tortoise);
	while (tortoise != hare) {
		(*seen)[hare] = true;
		cycle_node->segment.push_back(hare);
		hare = f.eval(hare);
	}
	
	for (int i = 0; i < cycle_node->segment.size(); ++i) {
		TransientHeader *head = new TransientHeader;
		head->location = cycle_node;
		head->position = i;
		headers[cycle_node->segment[i]] = head;
	}
}
/* Constructs the TransientNode starting at leaf
 */
void Graph::construct_transient(int leaf) {
	TransientNode *parent = new TransientNode;
	int iter, position;
	
	nodes.push_back(parent);	
	parent->is_cycle = false;
	iter = leaf;
	position = 0;
	while (headers[iter] == nullptr) {
		parent->segment.push_back(iter);
		TransientHeader *head = new TransientHeader;
		head->location = parent;
		head->position = position;
		headers[iter] = head;
		iter = f.eval(iter);
		position += 1;
	}
	parent->child = headers[iter]->location;
	parent->position = headers[iter]->position;
}

// Computes f^i(x)
int Graph::eval(int x, int i) {
	TransientNode *node = headers[x]->location;
	int position = headers[x]->position;
	int n = i;
	while (n + position >= node->segment.size() && !node->is_cycle) {
		n = n - (node->segment.size() - position);
		position = node->position;
		node = node->child;
	}
	if (n < node->segment.size() - position) {
			return node->segment[position + n];
	}
	int cycle_size = node->segment.size();
	return node->segment[(n % cycle_size + position) % cycle_size];
}

// Frees the memory we allocated
void Graph::free() {
	for (int i = 0; i < headers.size(); ++i) {
		delete headers[i];
	}
	for (int j = 0; j < nodes.size(); ++j) {
		delete nodes[j];
	}
}
