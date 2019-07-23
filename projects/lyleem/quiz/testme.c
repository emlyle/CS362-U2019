#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // TODO: rewrite this function
    char charPool[9] = {'[', '(', '{', ' ', 'a', 'x', '}', ')', ']'}; 
    int randPos = rand() % 9;
    //printf("randChar = %c, randPos = %d\n", charPool[randPos], randPos); 
    return charPool[randPos];
}

char *inputString()
{
    int randPos, i, j; 
    // TODO: rewrite this function
    char string[6] = "12345";  
    char charPool[5] = {'r', 'e', 's', 'e', 't'};
    int strLen = 5; 
    for (i = 0; i < 5; i++) {
       randPos = rand() % strLen--; 
       string[i] = charPool[randPos];
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

//TEMP:
  //int i = 0; 
  //for (i = 0; i < 20; i++) {
  //   c = inputChar(); 
  //}
  //int myTempCtr = 0;



  while (1)
  { 
    //TEMP
    //myTempCtr++; 
    //if (myTempCtr == 500) {
	//printf("my exit\n"); 
	//break; 
	//exit(200); 
    //}
    //printf("%s\n", s); 


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
    if (c == ']' && state == 8) {
	state = 9;
	//printf("***finished last char if statement\n"); 
	//break; 
    }
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
