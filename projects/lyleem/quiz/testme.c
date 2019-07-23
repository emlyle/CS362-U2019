#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	//This function randomly selects and returns one character from the charPool array 
    char charPool[9] = {'[', '(', '{', ' ', 'a', 'x', '}', ')', ']'}; 
    int randPos = rand() % 9;
    return charPool[randPos];
}

char *inputString()
{
	//This function randomly reorders and returns the characters r, e, s, e, t in a string 
    int randPos, i, j; 
    char string[6] = "12345";  
    char charPool[5] = {'r', 'e', 's', 'e', 't'};
    int strLen = 5; 
    for (i = 0; i < 5; i++) {
       randPos = rand() % strLen--; 
       string[i] = charPool[randPos];
	   //remove selected element and shift remaining elements to the left in charPool array
       for (j = randPos; j < strLen; j++) {
		charPool[j] = charPool[j + 1]; 
       }  
    }
    return string;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;

  while (1)
  { 
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9; 
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      break; 
      //exit(200);
    }

  }

}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
