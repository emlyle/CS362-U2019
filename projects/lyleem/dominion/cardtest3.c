/*
 * cardtest3.c - This tests the endTurn function
 *
 */

 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "endTurn"

int verifyTestCriteriaToEndTurn(struct gameState *G, struct gameState *testG, int currentPlayer, int returnValue, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 
int myAssert(int arg1, int arg2, int testPass); 


int main() {
	int seed = 1000;
	int numPlayers = 2; 	//For this entire CARD TEST 3 unit test, assume always only two players!
	int testPass = 1; 
	int result = -2; 
	int currentPlayer; 
	struct gameState G, testG;
	int k[10] = { minion, embargo, village, baron, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- CARD TEST 3: Testing %s Function ----------------\n", TESTFUNCTION);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: current player is the first player\n");
	currentPlayer = 0; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = endTurn(&testG); 
	//For me: 
	printf("G turn = %d, testG turn = %d\n", G.whoseTurn, testG.whoseTurn); 

	testPass = 1; 
	testPass = verifyTestCriteriaToEndTurn(&G, &testG, currentPlayer, result, testPass);

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: current player is the last player in the player's array \n");

	currentPlayer = 1;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	// call test function to initialize a game state and player cards
	result = endTurn(&testG);
	//For me: 
	printf("G turn = %d, testG turn = %d\n", G.whoseTurn, testG.whoseTurn);

	testPass = 1;
	testPass = verifyTestCriteriaToEndTurn(&G, &testG, currentPlayer, result, testPass);

	concludeTestCase(testPass, 1);




	printf(">>>>> CARD TEST 1 (%s function) COMPLETE <<<<<\n\n", TESTFUNCTION);

	return 0;

}




int verifyTestCriteriaToEndTurn(struct gameState *G, struct gameState *testG, int currentPlayer, int returnValue, int testPass) {
	int i; 
	int previousPlayer; 

	//For this entire CARD TEST 3 unit test, assume always only two players!
	//Update your control variables to compare against the modified testG game state: 
	if (currentPlayer == 0) {
		previousPlayer = 1;
		currentPlayer = 0; //this is a local variable that has to be updated - it doesn't "know" about endTurn modifying current player 
	}
	else {
		previousPlayer = 0;
		currentPlayer = 1; 
	}


	//Verify return value of 0
	printf("\treturn value = %d, expected 0 --> ", returnValue);
	testPass = myAssert(returnValue, 0, testPass);

	//Verify whose turn is the current player 
	printf("\tcurrent player = %d, expected = %d --> ", testG->whoseTurn, currentPlayer);
	testPass = myAssert(testG->whoseTurn, currentPlayer, testPass); 

	//Verify previous player's hand count is zero
	printf("\tprevious player's hand count = %d, expected = 0 --> ", testG->handCount[previousPlayer]); 
	testPass = myAssert(testG->handCount[previousPlayer], 0, testPass); 

	//Verify previous player's hand is empty
	int cardFoundInHand = 0; 
	for (i = 0; i < G->handCount[previousPlayer]; i++) {
		if (G->hand[previousPlayer][i] != -1) cardFoundInHand = 1; 
	}
	if (cardFoundInHand) printf("\tpreviou player's hand is not empty --> "); 
	else printf("\tprevious player's hand is empty --> "); 
	testPass = myAssert(cardFoundInHand, 0, testPass); 


	//Verify state values have been reset
	printf("\tVerify game state values have been reset:\n"); 

	printf("\t\toutpostPlayed = %d, expected = 0 --> ", testG->outpostPlayed);
	testPass = myAssert(testG->outpostPlayed, 0, testPass); 

	printf("\t\tphase = %d, expected = 0 --> ", testG->phase);
	testPass = myAssert(testG->phase, 0, testPass);

	printf("\t\tnumActions = %d, expected = 1 --> ", testG->numActions);
	testPass = myAssert(testG->numActions, 1, testPass);

	printf("\t\tcoins = %d, expected = 0 --> ", testG->coins);
	testPass = myAssert(testG->coins, 0, testPass);

	printf("\t\tnumBuys = %d, expected = 1 --> ", testG->numBuys);
	testPass = myAssert(testG->numBuys, 1, testPass);

	printf("\t\tplayedCardCount = %d, expected = 0 --> ", testG->playedCardCount);
	testPass = myAssert(testG->playedCardCount, 0, testPass);


	//Verify current player has 5 cards in hand
	printf("\tcurrent player's hand count = %d, expected = 5 --> ", testG->handCount[currentPlayer]); 
	testPass = myAssert(testG->handCount[currentPlayer], 5, testPass);

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
