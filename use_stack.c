/*
 * author: jordan carney
 * use_stack.c
 * deals with facilitating data entered by user to the appropriate controller methods
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stack.h"
#include "if2pf.h"

#define INFIX_INPUT_VALUE 1
#define POSTFIX_INPUT_VALUE 2
#define COMPLETE_INPUT_VALUE 3

/* removes newline from a string */
void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

/* tells the user wassup */
void printUserExplanation() {
	printf("(1) Convert Infix to Postfix Expression\n");
    printf("(2) Evaluate Postfix Expression\n");
    printf("(3) Quit\n");
    printf("Enter selection (1, 2, 3): ");
    fflush(stdout);
}

/* outputs a fun, animated error message */
void input_error() {
	printf("\nYou aren't very good at following instructions are you? ");
    char *animated_string = "Try again!";
    int i;
    for (i = 0; i < strlen(animated_string); i++) {
      printf("%c", animated_string[i]);
      fflush(stdout);
      usleep(200000);
    }
    printf("\n\n");
}

/* user declared they are done */
void complete() {
	printf("See ya!\n");
    exit(0);
}

int main(void) {
	printUserExplanation();

	char num_input[128];
	while(fgets(num_input, sizeof(num_input), stdin) != NULL) {
		char user_input[128];
		int input = num_input[0] - '0';

		switch(input) {
			case INFIX_INPUT_VALUE:
				printf("Enter Infix Expression: ");
				fgets(user_input, sizeof(user_input), stdin);
				chomp(user_input);
				char *postfix_form = infixToPostfix(user_input);
				printf("\nPostfix: %s\n", postfix_form);
        		int val = evaluatePostfix(postfix_form);
        		printf("Value: %d\n\n", val);
        		free(postfix_form);
				break;
			case POSTFIX_INPUT_VALUE:
				printf("Enter Postfix Expression: ");
				fgets(user_input, sizeof(user_input), stdin);
				chomp(user_input);
				int postfix_val = evaluatePostfix(user_input);
       			printf("\nValue: %d\n\n", postfix_val);
				break;
			case COMPLETE_INPUT_VALUE:
				complete();
				break;
			default:
				input_error();
				break;
		}

		printUserExplanation();
	}

	return 0;
}