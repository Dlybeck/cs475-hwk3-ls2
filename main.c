#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	stack_t *stack = initstack();

	if(argc == 2){
		//Run the first mode
		runls(argv[1], 0);
	}
	if(argc == 3){
		//Run the second mode
		runls2(argv[1], argv[2], stack, 0);
	}

	printf("6");
	freestack(stack);
	printf("6\n");
	return 0;
}
