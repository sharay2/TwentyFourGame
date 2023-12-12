/*-------------------------------------------
Program 3: TwentyFour Part Two
Course: CS 211, Spring 2023, UIC
System: Replit
Author: Stephen Harayo
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct expression{
  char data;
  int op;
  struct expression* next;
}eStack;
  
void push(char op, eStack** stack){ //pushes operator/operand onto the stack
  eStack* node = (eStack*)malloc(sizeof(eStack));
  node->data = op;
  node->next = *stack;
  (*stack) = node;
}

char pop(eStack** stack){ //pops operator/operand out of the stack and returns the popped char
  if(*stack == NULL){
    return '\0';
  }
  eStack* temp = *stack;
  char result = (*stack)-> data;
  (*stack) = (*stack)->next;
  free(temp);
  return result;
}

int **getPuzzles(char fileName[], int numPuzzles){ //reads the files and stores values in dynamic arrays
  FILE *filePtr = fopen(fileName, "r");
  int **puzzles = malloc(numPuzzles*sizeof(int *));
  
  for(int r = 0; r < numPuzzles;r++){
    puzzles[r]=malloc(4*sizeof(int));
  }
  
  for(int r = 0; r<numPuzzles; r++){
    for(int c = 0;c<4;c++){
      fscanf(filePtr,"%d", &puzzles[r][c]);
    }
  }
  fclose(filePtr);
  return puzzles;
}

void clearPuzzles(int **puzzles, int numPuzzles){ //clears array of puzzles when switching difficulties
  for(int i = 0; i < numPuzzles; i++){
    free(puzzles[i]);
    }
  free(puzzles);
}

int pemdas(char operation){ //returns precedence of given operation
  if(operation == '+' || operation == '-'){
    return 0;
  }
  else if(operation =='*' || operation == '/'){
    return 1;
  }
  return -1;
}

bool validSymbols(char userInput[]){ //checks if user enters a valid symbol in their equation
  for(int i=0;i<userInput[i]!= '\0';i++){
    if(isspace(userInput[i])){ //ignores spaces
      continue;
    }
    else if(!(userInput[i] == '+' || userInput[i] == '-' || userInput[i] == '*' || userInput[i] == '/' || userInput[i] == '(' || userInput[i] == ')' || isdigit(userInput[i]))){
      return false;
    }
  }
  return true;
}

bool checkNums(int rPuzzle[], char *userInput){ //adds all values of the puzzle and the user's input and subtracts them. if the difference between the puzzle and input is 0, then user used correct numbers
  int pSum=0;
  int uSum=0;
  for(int i = 0; i<4; i++){
    pSum+=rPuzzle[i];
  }
  for(int i = 0; userInput[i]!='\0'; i++){
    if(isdigit(userInput[i])){
      uSum+=(userInput[i] - '0');
    }
  }
  if(pSum != uSum){
    return false;
  }
  return true;
}

bool checkParenthesis(char *userInput){ //checks number of parenthesis
  int unmatched = 0;
  for(int i=0;userInput[i]!='\0';i++){ //the variable 'unmatched' should stay 0 and return true when the parenthesis are balanced
    if(userInput[i]=='('){
      unmatched++;
    }
    else if(userInput[i]==')'){
      unmatched--;
    }
  }
  if(unmatched==0){
    return true;
  }
  return false;
}

void checkNumOperators(char *userInput, int *numOperators){ //counts the number of operators in the user's equation
  int temp = 0;
  for(int i=0;userInput[i]!='\0';i++){
    if(userInput[i] == '+' || userInput[i] == '-' || userInput[i] == '*' || userInput[i] == '/'){
      temp++;
    }
  }
  *numOperators = temp;
}

void evaluateExpression(char *expression, bool par, int operator){ //takes the postfix expression of the user's input and evaluates it
  eStack *stack = NULL;
  int operand1, operand2, result;
  for(int i = 0; expression[i] !='\0';i++){ //pushes numbers into the stack until it encounters an operator
    if(isdigit(expression[i])){ 
      push(expression[i]-'0',&stack);
    }
    else{
      operand2 = pop(&stack); //pops the top of the stack
      operand1 = pop(&stack); //pops the next number in the stack
      
      if(expression[i]=='+'){ //based on the given operator, it will evaluate the two operands and push back into the stack
        result = operand1+operand2;
        printf("%d + %d = %d.\n", operand1, operand2, result);
        push(result, &stack); //pushes result back into stack for the next operation
      }
      else if(expression[i]=='-'){
        result = operand1-operand2;
        printf("%d - %d = %d.\n", operand1, operand2, result);
        push(result, &stack);
      }
      else if(expression[i]=='*'){
        result = operand1*operand2;
        printf("%d * %d = %d.\n", operand1, operand2, result);
        push(result, &stack);
      }
      else if(expression[i]=='/'){
        result = operand1/operand2;
        printf("%d / %d = %d.\n", operand1, operand2, result);
        push(result, &stack);
      }
    }
  }
  result = pop(&stack); //the last number in the stack should be result of the infix expression
  free(stack);

  if(operator!=3){ //error messages or win message
    printf("Error! Too many values in the expression.\n\n");
  }
  else if(!par){
    printf("Error! Too many closing parentheses in the expression.\n\n");
  }
  else if(result == 24){
    printf("Well done! You are a math genius.\n\n");
  }
  else{
    printf("Sorry. Your solution did not evaluate to 24.\n\n");
  }
}

void newExpression(char *userInput){ //converts the user's infix equation to a postfix equation, userInput is the infix
  eStack *stack = NULL;
  char expression[20]; //new postfix equation
  bool par = checkParenthesis(userInput); //checks if parenthesis are unbalanced
  int index = 0;
  for(int i = 0;userInput[i] != '\0' ;i++){
    if(isspace(userInput[i])!=0){ //ignores spaces
      continue;
    }
    else if(isdigit(userInput[i])){ //any number automatically will be added into the postfix equation 
      expression[index] = userInput[i];
      index++;
    }
    else if(userInput[i] == '('){
      push(userInput[i], &stack); //pushes '(' into stack
    }
    else if(userInput[i] == ')'){
      while(stack != NULL && stack->data != '('){//adds any number/operator into the new postfix equation that's within parenthesis
        expression[index] = pop(&stack);
        index++;
      }
      if(userInput[i]==')' && stack==NULL){ //indicates the end of the postfix equation where there is an extra end parenthesis
        expression[index]='\0';
        break;
      }
    pop(&stack); //pops '(' once the while loop reaches it
    }
    else{
      while(stack != NULL && pemdas(stack->data) >= pemdas(userInput[i])){ //adds the numbers/operators from the stack into the postfix equation as long as PEMDAS applies
        expression[index] = pop(&stack);
        index++;
      }
    push(userInput[i], &stack); //pushes the next operator into the stack if it has less precedence
    }
  }
  while(stack!=NULL){
    if(stack->data == '('){ //gets rid of any remaining open parenthesis
      pop(&stack);
    }
    expression[index] = pop(&stack); //pops remaining numbers/operands from the stack into the postfix equation
    index++;
  }
  expression[index] = '\0'; //indiciates end of postfix equation
  free(stack);
  int operator = 0;
  checkNumOperators(userInput, &operator); //checks if the user entered the correct amount of operators
  evaluateExpression(expression, par, operator); // evaluate new postfix equation
}

void game(int **puzzles, int *replay, int numPuzzles){
  bool validSym, validNums;
  char userInput[100];
  *replay = 1;
  
  while(*replay == 1){ //loop to play puzzles of the same difficulty
    validSym = false;
    while(!validSym && *replay!=3){
      int rIndex = rand()%numPuzzles;
      int rPuzzle[4] = {puzzles[rIndex][0],puzzles[rIndex][1],puzzles[rIndex][2],puzzles[rIndex][3]};
      printf("The numbers to use are: %d, %d, %d, %d.\n", rPuzzle[0],rPuzzle[1],rPuzzle[2],rPuzzle[3]);
      printf("Enter your solution: ");
      scanf(" %99[^\n]", userInput);

      validSym = validSymbols(userInput);
      validNums = checkNums(rPuzzle, userInput);
      
      if(!validSym){ //checks if any inputted symbol is invalid
        printf("Error! Invalid symbol entered. Please try again.\n\n");
      }
      else if(!validNums){ //checks if user uses correct numbers
        printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
      }
      else{
        newExpression(userInput); //converts user's infix equation to postfix equation and evaluates it
      
        printf("Enter: \t1 to play again,\n");
        printf("\t2 to change the difficulty level and then play again, or\n");
        printf("\t3 to exit the program.\n");
        printf("Your choice --> ");
        scanf("%d", replay);
        }
      }
    }
  return;
}

int main() {
  srand(1);
  int replay = 2;
  int **puzzles = NULL;
  int numPuzzles;
  char userDiff;
  
	printf("Welcome to the game of TwentyFour Part Two!\n");
	printf("Use each of the four numbers shown exactly once, \n");
	printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
	printf("to yield the value twenty-four.\n");

  while(replay == 2){ //frees array of puzzles when changing difficulty
    if(puzzles!=NULL){
      clearPuzzles(puzzles, numPuzzles);
    }
    printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
    printf("Your choice --> ");
    scanf(" %c", &userDiff);
    getchar();
    
    if(userDiff == 'M') {
      numPuzzles = 24;
      puzzles=getPuzzles("medium.txt", numPuzzles);
    }
    else if(userDiff == 'H') {
      numPuzzles = 11;
      puzzles=getPuzzles("hard.txt", numPuzzles);
    }
    else{
      numPuzzles = 12;
      puzzles=getPuzzles("easy.txt", numPuzzles);
    }
    
    game(puzzles, &replay, numPuzzles); //function where the whole game is played
  }
  clearPuzzles(puzzles, numPuzzles); //frees array of puzzles to avoid any leakage
  
  printf("\nThanks for playing!\n");
  printf("Exiting...\n");
	
	return 0;
}