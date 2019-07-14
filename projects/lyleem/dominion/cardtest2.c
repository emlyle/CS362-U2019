/*
 * cardtest2.c - This tests the initializeGame function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "shuffle"


/*
cardtest2 - shuffle:
TC: state->deckCount == 0 --> return -1
TC: state->deckCount == 5
return 0
verify cards are in a different order
verify deck count is the same
TC: state->deckCound == 1
return 0
verify card is the same
verify deck count == 1

*/


void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 
int myAssert(int arg1, int arg2, int testPass); 



int main() {
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0; 
	int testPass = 1; 
	int result = -2; 
	struct gameState G, testG;
	int k[10] = {minion, embargo, village, baron, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- CARD TEST 1: Testing %s Function ----------------\n", TESTFUNCTION);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: deck count = 0 \n");

	//Empty current player's deck
	for (i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = -1; 
	}
	G.deckCount[currentPlayer] = 0; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = shuffle(currentPlayer, &testG);

	testPass = 1; 
	//Verify return value of -1
	printf("\treturn value = %d, expected -1 --> ", result);
	testPass = myAssert(result, -1, testPass);

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: deck count = 1 \n");

	currentPlayer = 0; 

	//Empty current player's deck except for one card
	for (i = 1; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = -1;
	}
	G.deckCount[currentPlayer] = 1;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = shuffle(currentPlayer, &testG);

	testPass = 1;
	//Verify return value of 0
	printf("\treturn value = %d, expected = 0 --> ", result);
	testPass = myAssert(result, 0, testPass);

	//Verify deck still contains one card 
	printf("\tdeck count = %d, expected = 1 --> ", testG.deckCount[currentPlayer], 1); 
	testPass = myAssert(testG.deckCount[currentPlayer], 1, testPass); 

	//Verify deck contains the same card 
	printf("\tcard = %d, expected = %d --> ", testG.deck[currentPlayer][0], G.deck[currentPlayer][0]); 
	testPass = myAssert(testG.deck[currentPlayer][0], G.deck[currentPlayer][0], testPass); 
	
	concludeTestCase(testPass, 2);



	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------------------
	printf("TEST CASE 3: deck count = 5 \n");

	currentPlayer = 0;
	int card = 0; 

	//Set any remaining cards in deck to empty
	for (i = 5; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = -1;
	}
	G.deckCount[currentPlayer] = 5;

	//Make all cards in current player's deck unique (to make verification easier)
	for (i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = card++;
	}

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = shuffle(currentPlayer, &testG);

	testPass = 1;
	//Verify return value of 0
	printf("\treturn value = %d, expected 0 --> ", result);
	testPass = myAssert(result, 0, testPass);

	//Verify deck still contains 5 cards 
	printf("\tdeck count = %d, expected = 5 --> ", testG.deckCount[currentPlayer], 1);
	testPass = myAssert(testG.deckCount[currentPlayer], 5, testPass);

	//Verify deck contains the same cards  
	int cards[5] = { -1, -1, -1, -1, -1 }; //Set all cards to empty
	int newCardFound = 0; 
	for (i = 0; i < 5; i++) {
		if (testG.deck[currentPlayer][i] >= 0 && testG.deck[currentPlayer][i] <= 4) {
			cards[testG.deck[currentPlayer][i]] = i; //set card to position in deck where it was found
		}
		else newCardFound = 1; 
	}
	if (newCardFound) {
		printf("\tdeck contains different cards --> "); 
	} 
	else printf("\tdeck contains the same cards --> "); 
	testPass = myAssert(newCardFound, 0, testPass); 

	//Verify that cards are in a new order 
	int inOrder = 0; 
	if (cards[0] == 0) inOrder++; 
	for (i = 1; i < 5; i++) {
		if (cards[i] == cards[i - 1] + 1) {
			inOrder++; 
		}
	}
	if (inOrder == 5) {
		printf("\tcards are in the same order --> "); 
		testPass = myAssert(1, 0, testPass); 
	} 
	else {
		printf("\tcards are in a new order --> ");
		testPass = myAssert(1, 1, testPass);
	}

	concludeTestCase(testPass, 3);



	printf(">>>>> CARD TEST 2 (%s function) COMPLETE <<<<<\n\n", TESTFUNCTION);

	return 0;

}



void concludeTestCase(int testPass, int testCaseNumber) {
	if (testPass) {
		printf("TEST CASE %d PASSED!\n", testCaseNumber);
	}
	else {
		printf("TEST CASE %d FAILED. See failure criteria listed above.\n", testCaseNumber);
	}
	printf("END TEST CASE %d\n\n", testCaseNumber);
}


int printFail() {
	printf("FAIL\n"); 
	return 0;
}


void printPass() {
	printf("PASS\n"); 
}


int myAssert(int arg1, int arg2, int testPass) {
	if (arg1 == arg2) {
		//PASS
		printPass(); 
	}
	else {
		//FAIL
		testPass = printFail(); 
	}

	return testPass; 
}
