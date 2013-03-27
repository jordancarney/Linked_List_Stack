#ifndef _IF2PF_H
#define _IF2PF_H

/* function to convert an infix to postfix.  
 *
 * (1) This function may not alter infixStr 
 *
 * (2) This function may dynamiclly allocate memory for the returned
   string.  The caller is responsible for freeing this memory.
 */
char *infixToPostfix(char *infixStr);

/* function that evaluates a postfix expression and returns the result 
 * This function may not alter posfixStr
*/
int evaluatePostfix(char *postfixStr);


#endif /* if2pf.h */