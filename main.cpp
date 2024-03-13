#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include "idempotent.hpp"
#include "squared.hpp"
#include "graph.hpp"

void print_idempotent(Idempotent f) {
	for (int i = 0; i < f.size(); ++i) {
		std::cout << "f(" << i << ") : " << f.eval(i) << "\n";		
	}
}

void sanity_check(Idempotent f, Squared solver) {
	for (int i = 0; i < f.size(); ++i) {
		for (int j = 0; j < f.size(); ++j) {
			if (f.eval(i,j) != solver.eval(i,j)) {
				std::cout << "Check failed: \n";
				std::cout << "f(" << i << "," << j << "):\t" << f.eval(i,j) << "\n";
				std::cout << "solver(" << i << "," << j << "):\t" << solver.eval(i,j) << "\n\n";
			}
		}
	}
	std::cout << "Sanity Check Complete!\n\n";
}

void sanity_check(Idempotent f, Graph solver) {
	for (int i = 0; i < f.size(); ++i) {
		for (int j = 0; j < f.size(); ++j) {
			if (f.eval(i,j) != solver.eval(i,j)) {
				std::cout << "Check failed: \n";
				std::cout << "f(" << i << "," << j << "):\t" << f.eval(i,j) << "\n";
				std::cout << "solver(" << i << "," << j << "):\t" << solver.eval(i,j) << "\n\n";
			}
		}
	}
	std::cout << "Sanity Check Complete!\n\n";
}

void all_points(Idempotent f, Squared solver) {
	for (int i = 0; i < f.size(); ++i) {
		for (int j = 0; j < f.size(); ++j) {
			solver.eval(i,j);
		}
	}
	std::cout << "Finished Computing all points!\n";
}

void all_points(Idempotent f, Graph solver) {
	for (int i = 0; i < f.size(); ++i) {
		for (int j = 0; j < f.size(); ++j) {
			solver.eval(i,j);
		}
	}
	std::cout << "Finished Computing all points!\n";
}

uint64_t timediff(struct timespec *end, struct timespec *start) {
	return ((*end).tv_sec - (*start).tv_sec) * 1000000 + ((*end).tv_nsec - (*start).tv_nsec);
}

int main(int argc, char const *argv[]) {
	int input, size, x, i;
	std::string path;
	Idempotent f;
	struct timespec a,b;
	while(true) {
		std::cout << "MENU\n";
		std::cout << "1) Generate random idempotent function\n";
		std::cout << "2) Read function from file\n";
		std::cout << "3) Quit\n";
		std::cout << "Enter selection: ";
		std::cin >> input;
		if (input == 1) {
			std::cout << "Input domain size of idempotent function: ";
			std::cin >> size;
			f = (size);
			f.populate();
			print_idempotent(f);
		} else if (input == 2) {
			std::cout << "Enter file path: ";
			std::cin >> path;
			f = (path);
			print_idempotent(f);
		} else {
			break;
		}
		std::cout << "Preprocessing the squared solution...\n";
		clock_gettime(CLOCK_MONOTONIC_RAW, &a);
		Squared s_solver (f);
		clock_gettime(CLOCK_MONOTONIC_RAW, &b);
		std::cout << "Squared preprocessing took " << timediff(&b,&a) << " microseconds\n";
		std::cout << "Preprocessing the graph solution...\n";
		clock_gettime(CLOCK_MONOTONIC_RAW, &a);
		Graph g_solver (f);
		clock_gettime(CLOCK_MONOTONIC_RAW, &b);
		std::cout << "Graph preprocessing took " << timediff(&b,&a) << " microseconds\n";
		while(true) {
			std::cout << "SubMENU\n";
			std::cout << "1) Pointwise evaluation of squared solution\n";
			std::cout << "2) Pointwise evaluation of graph solution\n";
			std::cout << "3) Compute all points with squared solution\n";
			std::cout << "4) Compute all points with graph solution\n";
			std::cout << "5) Perform sanity check on squared solution\n";
			std::cout << "6) Perform sanity check on graph solution\n";
			std::cout << "7) Return to main menu\n";
			std::cout << "Enter selection: ";
			std::cin >> input;
			if (input == 1) {
				std::cout << "Computing f^i(x)\n";
				std::cout << "Enter x: ";
				std::cin >> x;
				std::cout << "Enter i: ";
				std::cin >> i;
				std::cout << "Real Function : " << f.eval(x,i) << "\n";
				std::cout << "Squared Solution: " << s_solver.eval(x,i) << "\n\n";
			} else if (input == 2) {
				std::cout << "Computing f^i(x)\n";
				std::cout << "Enter x: ";
				std::cin >> x;
				std::cout << "Enter i: ";
				std::cin >> i;
				std::cout << "Real Function : " << f.eval(x,i) << "\n";
				std::cout << "Squared Solution: " << g_solver.eval(x,i) << "\n\n";
			} else if (input == 3) {
				clock_gettime(CLOCK_MONOTONIC_RAW, &a);
				all_points(f, s_solver);
				clock_gettime(CLOCK_MONOTONIC_RAW, &b);
				std::cout << "Squared solution took " << timediff(&b,&a) << " microseconds\n\n";
			} else if (input == 4) {
				clock_gettime(CLOCK_MONOTONIC_RAW, &a);
				all_points(f, g_solver);
				clock_gettime(CLOCK_MONOTONIC_RAW, &b);
				std::cout << "Graph solution took " << timediff(&b,&a) << " microseconds\n\n";
			} else if (input == 5) {
				sanity_check(f, s_solver);	
			} else if (input == 6) {
				sanity_check(f, g_solver);	
			} else {
				s_solver.free();
				g_solver.free();
				break;
			}
		}		
	}
	return 0;
}
