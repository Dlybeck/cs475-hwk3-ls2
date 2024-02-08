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
		//runls2(argv[1], argv[2], stack, 0);
	}

	freestack(stack);



	/*// stack stores the lines to print out
	stack_t *s = initstack();	

	push(s, "Hello1");
	push(s, "Hello2");
	push(s, "Hello3");

	// print stack
	printstack(s);

	// free up stack
	freestack(s);*/
	return 0;
}
