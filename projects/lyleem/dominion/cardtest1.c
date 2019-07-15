/*
 * cardtest1.c - This tests the initializeGame function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "initializeGame"

int invalidInputCriteria(int returnValue, int testPass);
int validInputCriteria(struct gameState *testG, int numPlayers, int returnValue, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 
int myAssert(int arg1, int arg2, int testPass); 



int main() {
	int seed = 1000;
	int numPlayers;
	int testPass = 1; 
	int result = -2; 
	struct gameState G, testG;

	printf("----------------- CARD TEST 1: Testing %s Function ----------------\n", TESTFUNCTION);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: initialize game with a unique set of kingdom cards and 2 players \n");
	int k[10] = {minion, embargo, village, baron, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	numPlayers = 2; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = initializeGame(numPlayers, k, seed, &testG);

	testPass = 1; 
	testPass = validInputCriteria(&testG, numPlayers, result, testPass); 

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: initialize game with a unique set of kingdom cards and 3 players\n");

	//use k array from above - no reset needed
	numPlayers = 3;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = initializeGame(numPlayers, k, seed, &testG);

	testPass = 1;
	testPass = validInputCriteria(&testG, numPlayers, result, testPass);

	concludeTestCase(testPass, 2);



	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------------------
	printf("TEST CASE 3: initialize game with a unique set of kingdom cards and 4 players\n");

	//use k array from above - no reset needed
	numPlayers = 4;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = initializeGame(numPlayers, k, seed, &testG);

	testPass = 1;
	testPass = validInputCriteria(&testG, numPlayers, result, testPass);

	concludeTestCase(testPass, 3);




	// ------------------------------------------------ TEST CASE 4 ---------------------------------------------
	printf("TEST CASE 4: initialize game with a unique set of kingdom cards and 5 players\n");

	//use k array from above - no reset needed
	numPlayers = MAX_PLAYERS + 1;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = initializeGame(numPlayers, k, seed, &testG);

	testPass = 1;
	testPass = invalidInputCriteria(result, testPass);

	concludeTestCase(testPass, 4);
	
	



	// ------------------------------------------------ TEST CASE 5 ---------------------------------------------
	printf("TEST CASE 5: initialize game with a non-unique set of kingdom cards and 2 players\n");

	//Duplicate a card
	k[3] = k[0]; 
	numPlayers = 2;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = initializeGame(numPlayers, k, seed, &testG);

	testPass = 1;
	testPass = invalidInputCriteria(result, testPass);

	concludeTestCase(testPass, 5);





	printf(">>>>> CARD TEST 1 (%s function) COMPLETE <<<<<\n\n", TESTFUNCTION);

	return 0;

}




int validInputCriteria(struct gameState *testG, int numPlayers, int returnValue, int testPass) {
	int i, j, nonZeroFound; 
	int estateCount = 0; 
	int copperCount = 0; 
	int otherCardCount = 0; 
	int estatesInARow = 0; 

	//Verify return value of zero
	printf("\treturn value = %d, expected 0 --> ", returnValue);
	testPass = myAssert(returnValue, 0, testPass);

	//Verify player count was set correctly
	printf("\ttotal players = %d, expected = %d --> ", testG->numPlayers, numPlayers); 
	testPass = myAssert(testG->numPlayers, numPlayers, testPass); 

	//Verify player decks
	for (i = 0; i < numPlayers; i++) {
		if (i == 0) {
			//Verify deck count = 5 
			printf("\tPlayer %d:\n\t\tdeck count = %d, expected = 5 --> ", (i + 1), testG->deckCount[i]);
			testPass = myAssert(testG->deckCount[i], 5, testPass);
		}
		else {
			//Verify deck count = 10 
			printf("\tPlayer %d:\n\t\tdeck count = %d, expected = 10 --> ", (i + 1), testG->deckCount[i]);
			testPass = myAssert(testG->deckCount[i], 10, testPass);
		}
		
		//Check the state of each player's deck
		estateCount = 0; 
		copperCount = 0; 
		otherCardCount = 0; 
		estatesInARow = 0; 
		//printf("\t\tCards:\n"); 
		for (j = 0; j < 10; j++) {
			//printf("\t\t\tCard %d: %d\n", (j + 1), testG->deck[i][j]); 
			if (testG->deck[i][j] == estate) {
				estateCount++; 
				if (j < 3) estatesInARow++; 
			}
			else if (testG->deck[i][j] == copper) copperCount++; 
			else otherCardCount++; 
		}

		//Verify whether deck was shuffled for this player
		if (estatesInARow == 3 && copperCount == 7) {
			printf("\t\tdeck was not shuffled --> ");
			testPass = myAssert(0, 1, testPass);
		}
		else {
			printf("\t\tdeck was shuffled --> ");
			testPass = myAssert(1, 1, testPass);
		}
		//Verify deck contents for this player
		printf("\t\testate card count = %d, expected = 3 --> ", estateCount); 
		testPass = myAssert(estateCount, 3, testPass); 
		printf("\t\tcopper card cound = %d, expected 7 --> ", copperCount); 
		testPass = myAssert(copperCount, 7, testPass); 
	}

	//Verify embargo tokens are set to zero for each card type
	nonZeroFound = 0; 
	for (i = 0; i < treasure_map; i++) {
		if (testG->embargoTokens[i] != 0) nonZeroFound = 1; 
	}
	if (nonZeroFound) printf("\tembargo tokens not set to zero for all cards --> "); 
	else printf("\tembargo tokens set to zero for all cards --> ");
	testPass = myAssert(nonZeroFound, 0, testPass); 

	
	//Verify that only the first player has 5 cards in their hand 
	printf("\tfirst player hand count = %d, expected = 5 --> ", testG->handCount[0]); 
	testPass = myAssert(testG->handCount[0], 5, testPass); 
	//Verify all other players have zero cards in their hands
	nonZeroFound = 0; 
	for (i = 1; i < numPlayers; i++) {
		if (testG->handCount[i] != 0) nonZeroFound = 1; 
	}
	if (nonZeroFound) printf("\tNot all other players' hand counts set to zero --> "); 
	else printf("\tAll other players' hand counts set to zero --> "); 
	testPass = myAssert(nonZeroFound, 0, testPass); 

	//TODO: Currently not verifying supply counts - if time allows, come back and add this check 

	return testPass;
}


int invalidInputCriteria(int returnValue, int testPass) {
	//Verify return value of -1
	printf("\treturn value = %d, expected -1 --> ", returnValue);
	testPass = myAssert(returnValue, -1, testPass);

	return testPass;
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
