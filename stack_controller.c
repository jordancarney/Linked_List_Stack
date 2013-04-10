/*
 * author: jordan carney
 * stack_controller.c
 * linked-list stack implementation AND infix/postfix functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stack.h"
#include "if2pf.h"

int isOperator(char op)
{
  switch(op)
  {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '(':
    case ')':
    case '%':
      return true;
    default:
      return false;
  }
}

int stackPrecendence (char operator) {
  switch(operator) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
    case '%':
      return 2;
    case '^':
      return 3;
    case '(':
      return -1;
    default:
      return -2;
  }
}

int inputPrecedence (char operator) {
  switch(operator) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
    case '%':
      return 2;
    case '^':
      return 4;
    case '(':
      return 5;
    default:
      return -2;
  }
}

int evaluatePostfix(char *postfixStr) {
  stack *s;
  s = (stack*)malloc(sizeof(stack));
  stackInit(s);

  int i;
  char *sum_value, *val;

  for (i = 0; i < strlen(postfixStr); i++) {

    int continuous_digits[10], count = 0;

    if (postfixStr[i] == ' ') continue;

    while (isdigit(postfixStr[i])) {
      continuous_digits[count] = postfixStr[i] - '0';
      count++;
      i++;
    }

    if (count != 0) {
      int j, val = count - 1, sum = 0;
      for (j = 0; j < count; j++) {
        int y = pow(10,val);
        val--;
        sum+=(y * continuous_digits[j]);
      }

      sum_value = (char *)malloc(sizeof(char *) * count);
      snprintf(sum_value, sizeof(sum_value), "%d", sum);
      stackPush(s, sum_value);
    }

    if (isOperator(postfixStr[i])) {
      char *y_string = stackPop(s);
      char *x_string = stackPop(s);

      int y = atoi(y_string);
      int x = atoi(x_string);

      free(y_string);
      free(x_string);
      
      int total;

      switch(postfixStr[i]) {
        case '+':
          total = x + y;
          break;
        case '-':
          total = x - y;
          break;
        case '*':
          total = x * y;
          break;
        case '/':
          total = x / y;
          break;
        case '%':
          total = x % y;
          break;
        case '^':
          total = pow(x,y);
          break;
        default:
          break;
      }

      val = (char *)malloc(sizeof(char *));
      snprintf(val, sizeof(val), "%d", total);

      stackPush(s,val);
    }
  }

  char *result_string = stackPop(s);
  int result = atoi(result_string);
  free(result_string);

  stackDestroy(s);
  free(s);
  // free(val);

  return result;
}

char *infixToPostfix(char *infixStr) {

  stack *s;
  s = (stack*)malloc(sizeof(stack));
  stackInit(s);

  char postfix[256];
  postfix[0] = '\0';
  int i;


  for (i = 0; i < strlen(infixStr); i++) {
    int continuous_digits[10], count = 0;

    if (infixStr[i] == ' ') continue;

    while (isdigit(infixStr[i])) {  
      continuous_digits[count] = infixStr[i] - '0';
      i++;
      count++;
    }

    if (count != 0) {
      int j, val = count - 1, sum = 0;
      for (j = 0; j < count; j++) {
        int y = pow(10, val);
        val--;
        sum+=(y*continuous_digits[j]);
      }
      count = 0;

      char *ptr = (char *)malloc(sizeof(char *) * strlen(postfix) + 1);
      ptr = strcpy(ptr, postfix);

      snprintf(postfix, sizeof(postfix), "%s%d ", ptr, sum);

      free(ptr);
      continue;
    }

    if(isOperator(infixStr[i])) {
      if (infixStr[i] == '(') {
        stackPush(s, "("); 
      } else if (infixStr[i] == ')') {
        char *test_operator = (char *)stackPeek(s);
        while (strcmp(test_operator, "(")) {
          char *tok = stackPop(s);

          char *ptr = (char *)malloc(sizeof(char *) * strlen(postfix));
          ptr = strcpy(ptr, postfix);

          snprintf(postfix, sizeof(postfix), "%s%c ", ptr, tok[0]);
          test_operator = (char *)stackPeek(s);
          
          free(ptr);
          free(tok);
        }
        stackPop(s);
      } else {
        if (s->top == NULL) {
          char *value = (char *)malloc(sizeof(char *));
          snprintf(value, sizeof(value), "%c", infixStr[i]);
          stackPush(s, value);
          continue;
        }

        char *test_operator = (char *)stackPeek(s);

        while (stackPrecendence(test_operator[0]) >= inputPrecedence(infixStr[i])) {
          char *poppedElement = (char *)stackPop(s);

          char *ptr = (char *)malloc(sizeof(char *) * strlen(postfix));
          ptr = strcpy(ptr, postfix);

          snprintf(postfix, sizeof(postfix), "%s%c ", ptr, poppedElement[0]);

          free(poppedElement);
          free(ptr);

          if (!stackIsEmpty(s)) test_operator = (char *)stackPeek(s);
          else break;
        }

        char *element = (char *)malloc(sizeof(char *));
        snprintf(element, sizeof(element), "%c", infixStr[i]);
        stackPush(s, element);
        //free(element);
      }
    }
  }

  while (s->top != NULL) {
    char *poppedElement = (char *)stackPop(s);

    char *ptr = (char *)malloc(sizeof(char *) * strlen(postfix));
    ptr = strcpy(ptr, postfix);
    snprintf(postfix, sizeof(postfix), "%s%c ", ptr, poppedElement[0]);
    free(ptr);

    free(poppedElement);
  }

  postfix[strlen(postfix)-1] = 0;

  char *postfixStr = (char *)malloc(sizeof(char *) * strlen(postfix));
  strcpy(postfixStr, postfix);

  stackDestroy(s);
  free(s);

  return postfixStr;
}


stkElement stackPeek(stack *stkPtr) {
  int i, length = stackLength(stkPtr);
  stkNode *node = stkPtr->top;

  if (stackIsEmpty(stkPtr)) {
    return NULL;
  } else if (length == 1) {
    return node->element;
  }

  for (i = 0; i < length - 1; i++) {
    node = (stkNode *)node->next;
  }

  return node->element;
}

int stackLength(stack *stkPtr) {
  int length = 0;
  stkNode *node = stkPtr->top;

  while (node) {
    length++;
    node = (stkNode *)node->next;
  }

  return length;
}

bool stackIsEmpty(stack *stkPtr) {
  return stkPtr->top == NULL;
}

stkElement stackPop(stack *stkPtr) {
  int i, length = stackLength(stkPtr);
  stkNode *node = stkPtr->top;

  if (stackIsEmpty(stkPtr)) {
    return NULL;
  } else if (length == 1) {
    stkPtr->top = NULL;
    stkElement element = node->element;
    free(node);
    return element;
  }

  for (i = 0; i < length-2; i++) {
    node = (stkNode *)node->next;
  }

  stkNode *nextNode = (stkNode *)node->next;
  stkElement element = nextNode->element;

  free(nextNode->next);
  free(nextNode);

  node->next = NULL;

  return element;
}

void stackPush(stack *stkPtr, stkElement element) {
  int i, length = stackLength(stkPtr);
  stkNode *node = stkPtr->top;

  if (stackIsEmpty(stkPtr)) {
    stkNode *newNode = (stkNode*)malloc(sizeof(stkNode));
    newNode->element = element; 
    newNode->next = NULL;  
    stkPtr->top = newNode;
    return;
  }

  for (i = 0; i < length-1; i++) {
    node = (stkNode *)node->next;
  }

  stkNode *newNode = (stkNode*)malloc(sizeof(stkNode));
  newNode->element = (stkElement) element;
  newNode->next = NULL;
  
  node->next = (struct stkNode *)newNode;
}

void stackDestroy(stack *stkPtr) {
    stkNode *node = stkPtr->top;

    while (node) {
      stkNode *next_node = (stkNode *)node->next;
      //free(node->element);
      free(node);
      node = next_node;
    }

    //free(stkPtr);
}

void stackInit(stack *stkPtr) {
    stkPtr->top = NULL;
}