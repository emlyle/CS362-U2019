/*
 * cardtest5.c - This tests the shuffle function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest5: cardtest5.c dominion.o rngs.o
 *      gcc -o cardtest5 -g  cardtest5.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "drawCard"


void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 
int myAssert(int arg1, int arg2, int testPass); 

/*
int drawCard(int player, struct gameState *state)

*/

int main() {
	int i; 
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0; 
	int testPass = 1; 
	int result = -2; 
	struct gameState G, testG;
	int k[10] = {minion, embargo, village, baron, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- CARD TEST 5: Testing %s Function ----------------\n", TESTFUNCTION);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: deck and discard piles are empty \n");

	//Empty current player's deck
	for (i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = -1; 
	}
	G.deckCount[currentPlayer] = 0; 

	//Make sure current player's discard pile is empty
	if (G.discardCount[currentPlayer] != 0) G.discardCount = 0; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = drawCard(currentPlayer, &testG);

	testPass = 1; 
	//Verify return value of -1
	printf("\treturn value = %d, expected = -1 --> ", result);
	testPass = myAssert(result, -1, testPass);


	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: deck is empty, discard pile is not empty \n");

	currentPlayer = 0; 
	
	//Empty current player's deck
	for (i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = -1;
	}
	G.deckCount[currentPlayer] = 0;

	//Add cards to discard pile
	int card = 0; 
	printf("Discard Pile:\n");
	G.discardCount[currentPlayer] = 5; 
	for (i = 0; i < G.discardCount[currentPlayer]; i++) {
		//printf("\t%d\n", G.discard[currentPlayer][i]);
		G.discard[currentPlayer][i] = card++; 
	}

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = drawCard(currentPlayer, &testG);

	testPass = 1;
	//Verify return value of 0
	printf("\treturn value = %d, expected = 0 --> ", result);
	testPass = myAssert(result, 0, testPass);

	//Verify discard count is 0
	printf("\tdiscard count = %d, expected = 0 --> ", testG.discardCount[currentPlayer]);
	testPass = myAssert(testG.discardCount[currentPlayer], 0, testPass);

	//Verify deck count is prev discard count - 1 (for the card added to the hand)
	printf("\tdeck count = %d, expected = 4 --> ", testG.deckCount[currentPlayer]);
	testPass = myAssert(testG.deckCount[currentPlayer], 4, testPass); 

	//Verify discard cards are in the deck
	int invalidCardsAdded = 0; 
	for (i = 0; i < 4; i++) {
		if (testG.deck[currentPlayer][i] < 0 || testG.deck[currentPlayer][i] > 4) {
			int invalidCardsAdded = 1; 
		}
	}
	if (invalidCardsAdded) printf("\tInvalid cards added to deck --> "); 
	else printf("\tDiscard cards successfully added to deck --> "); 
	testPass = myAssert(invalidCardsAdded, 0, testPass); 

	//Verify hand count increased by one
	printf("\thand count = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1);
	testPass = myAssert(testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1, testPass); 

	//Verify card in last position of hand is in [0, 4] (cards added to discard pile during test setup)
	printf("\tdrawn card = %d, expected = 0, 1, 2, 3, or 4 --> ", testG.hand[currentPlayer][testG.handCount[currentPlayer] - 1]); 
	if (testG.hand[currentPlayer][testG.handCount[currentPlayer] - 1] >= 0 && testG.hand[currentPlayer][testG.handCount[currentPlayer]] <= 4)
		testPass = myAssert(1, 1, testPass); 
	else testPass = myAssert(1, 0, testPass);

	
	concludeTestCase(testPass, 2);



	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------------------
	printf("TEST CASE 3: Deck is not empty \n");

	currentPlayer = 0;
	card = 0; 

	//For me: 
	printf("Current player's deck:\n");
	for (i = 0; i < G.deckCount[currentPlayer]; i++) {
		printf("\t%d\n", G.deck[currentPlayer][i]);
	}

	//Make all cards in current player's deck unique (to make verification easier)
	/*for (i = 0; i < G.deckCount[currentPlayer]; i++) {
		G.deck[currentPlayer][i] = card++;
	}*/

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = drawCard(currentPlayer, &testG);

	testPass = 1;
	//Verify return value of 0
	printf("\treturn value = %d, expected 0 --> ", result);
	testPass = myAssert(result, 0, testPass);

	//Verify hand count increased by one
	printf("\thand count = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1);
	testPass = myAssert(testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1, testPass);

	//Verify deck count decreased by one
	printf("\tdeck count = %d, expected = %d --> ", testG.deckCount[currentPlayer], G.deckCount[currentPlayer] - 1);
	testPass = myAssert(testG.deckCount[currentPlayer], G.deckCount[currentPlayer] - 1, testPass);

	//Verify previous last card in deck is now last card in hand
	printf("\tdrawn card = %d, expected = %d --> ", testG.hand[currentPlayer][testG.handCount[currenPlayer] - 1], G.deck[currentPlayer[G.deckCount[currentPlayer] - 1]); 
	testPass = myAssert(testG.hand[currentPlayer][testG.handCount[currenPlayer] - 1], G.deck[currentPlayer[G.deckCount[currentPlayer] - 1], testPass); 

	concludeTestCase(testPass, 3);



	printf(">>>>> CARD TEST 5 (%s function) COMPLETE <<<<<\n\n", TESTFUNCTION);

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
