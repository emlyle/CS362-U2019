/*
 * unittest2.c - This tests executeMinionCard function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest1: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "minion"

int testCriteriaForAllMinionCards(int result, struct gameState *testG, struct gameState *G, int testPass);
int testCriteriaForNotChoice2(struct gameState* testG, struct gameState* G, int currentPlayer, int testPass);
int verifyNoChangeToNextPlayer(struct gameState* testG, struct gameState* G, int currentPlayer, int testPass); 
int testCriteriaForChoice2(struct gameState* testG, struct gameState* G, int currentPlayer, int testPass);


int main() {
	int i;
	int handPos = 0, choice1 = 0, choice2 = 0;
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int testPass = 1; 
	int result = -1; 
	int diffFound = 0; 
	struct gameState G, testG;
	int k[10] = {minion, embargo, village, baron, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);



	printf("----------------- UNIT TEST 2: Testing %s card ----------------\n", TESTCARD);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: choice1 = 1 (Gain two coins)\n");

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //gain 4 coins
	choice2 = 0;  
	result = executeMinionCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1; 
	testPass = testCriteriaForAllMinionCards(result, &testG, &G, testPass);
	testPass = testCriteriaForNotChoice2(&testG, &G, currentPlayer, testPass)

	//Criteria for choice1 = 1 (gain 2 coins)
	//Verify 2 coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins + 2); 
	if (testG.coins != G.coins + 2) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");


	concludeTestCase(testPass, 1);



	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: choice1 = 0, choice 2 = 1 (Discard hand, draw new cards, next player has 4 cards)\n");

	currentPlayer = 0; 	

	//Make sure nextPlayer has 4 cards 
	if (G.handCount[currentPlayer + 1] > 4) {
		for (i = 4; i < G.handCount[currentPlayer + 1]; i++) {
			G.hand[currentPlayer + 1][i] = -1; 
		}
	}

	// Copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0; 
	choice2 = 1; //discard cards, gain 4 new cards
	result = executeMinionCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = testCriteriaForAllMinionCards(result, &testG, &G, testPass);

	//Criteria for choice2 = 1 (discard hand, gain cards)
	testPass = testCriteriaForChoice2(&testG, &G, currentPlayer, testPass);

	//Criteria for next player having 4 cards --> should be no change to that player's hand
	testPass = verifyNoChangeToNextPlayer(&testG, &G, currentPlayer, testPass);

	concludeTestCase(testPass, 2);




	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------
	printf("TEST CASE 3: choice1 = 0, choice 2 = 1 (Discard hand, draw new cards, next player has 5 cards)\n");
	//NOTE: This test case will trigger my bugs in executeMinionCard, which will cause the changes to the hand to be incorrect

	currentPlayer = 0;

	//Make sure nextPlayer has at least 5 cards 
	if (G.handCount[currentPlayer + 1] < 5) {
		for (i = G.handCount[currentPlayer + 1]; i < 5; i++) {
			G.hand[currentPlayer + 1][i] = estate;
		}
		G.handCount[currentPlayer + 1] = 5; 
	}

	// Copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0;
	choice2 = 1; //discard cards, gain 4 new cards
	result = executeMinionCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = testCriteriaForAllMinionCards(result, &testG, &G, testPass);

	//Criteria for choice2 = 1 (discard hand, gain cards)
	testPass = testCriteriaForChoice2(&testG, &G, currentPlayer, testPass);


	//Criteria for next player having 5 cards 
	//Verify hand count of next player is now 4
	printf("\tnext player's handCount = %d, expected = 4 --> ", testG.handCount[currentPlayer + 1]);
	if (testG.handCount[currentPlayer + 1] != 4) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify next player's hand has changed
	diffFound = 0;
	printf("\tnext player's hand: \n");
	for (i = 0; i < testG.handCount[currentPlayer + 1]; i++) {
		printf("\t\tcard %d = %d, was %d\n", i + 1, testG.hand[currentPlayer + 1][i], G.hand[currentPlayer + 1][i]);
		if (testG.hand[currentPlayer + 1][i] != G.hand[currentPlayer + 1][i]) {
			diffFound = 1;
		}
	}
	if (!diffFound) {
		printf("\t\tNo differences found in hand --> FAIL\n");
		testPass = 0;
	}
	else printf("\t\tHand changed --> PASS\n");


	concludeTestCase(testPass, 3);




	// --------------------------------------------- TEST CASE 4 ------------------------------------------------
	printf("TEST CASE 4: choice1 = 0, choice2 = 0 (Do nothing)\n");

	currentPlayer = 0; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0;
	choice2 = 0;
	result = executeMinionCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = testCriteriaForAllMinionCards(result, &testG, &G, testPass);

	//Criteria for choice1 = 0 and choice2 - 0 (do nothing)
	testPass = testCriteriaForAllMinionCards(result, &testG, &G, testPass);

	//Criteria for not choice 1: Verify no coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins);
	if (testG.coins != G.coins) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	testPass = testCriteriaForNotChoice2(&testG, &G, currentPlayer, testPass)

	concludeTestCase(testPass, 4);



	printf(">>>>> UNIT TEST 2 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

	return 0;

}




int testCriteriaForAllMinionCards(int result, struct gameState* testG, struct gameState* G, int testPass) {
	//Verify return value is successful (0)
	printf("\treturn value = %d, expected = 0 --> ", result);
	if (result != 0) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify actions increased by 1
	printf("\tnumActions = %d, expected = %d --> ", testG->numActions, G->numActions + 1);
	if (testG->numActions != G->numActions + 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	return testPass; 
}


int testCriteriaForNotChoice2(struct gameState* testG, struct gameState* G, int currentPlayer, int testPass) {
	//Verify hand count decreased by 1 for discarded card
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] - 1);
	if (testG->handCount[currentPlayer] != G->handCount[currentPlayer] - 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand was unchanged except for 1 card discarded
	printf("\thand: \n");
	for (i = 0; i < G->handCount[currentPlayer] - 1; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer][i], G->hand[currentPlayer][i]);
		if (testG->hand[currentPlayer][i] != G->hand[currentPlayer][i]) {
			printf("FAIL\n");
			testPass = 0;
		}
		else printf("PASS\n");
	}
	
	testPass = verifyNoChangeToNextPlayer(testG, G, currentPlayer, testPass);

	return testPass; 
}

int verifyNoChangeToNextPlayer(struct gameState* testG, struct gameState* G, int currentPlayer, int testPass) {
	//Verify hand count of next player was unchanged
	printf("\tnext player's handCount = %d, expected = %d --> ", testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1]);
	if (testG->handCount[currentPlayer + 1] != G->handCount[currentPlayer + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand of next player was unchanged
	printf("\tnext player's hand: \n");
	for (i = 0; i < G->handCount[currentPlayer + 1]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer + 1][i], G->hand[currentPlayer + 1][i]);
		if (testG->hand[currentPlayer + 1][i] != G->hand[currentPlayer + 1][i]) {
			printf("FAIL\n");
			testPass = 0;
		}
		else printf("PASS\n");
	}

	return testPass; 

}

int testCriteriaForChoice2(struct gameState* testG, struct gameState* G, int currentPlayer, int testPass) {
	//Verify no coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG->coins, G->coins);
	if (testG->coins != G->coins) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand count is now 4
	printf("\thandCount = %d, expected = 4 --> ", testG->handCount[currentPlayer]);
	if (testG->handCount[currentPlayer] != 4) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand has changed
	diffFound = 0;
	printf("\thand: \n");
	for (i = 0; i < testG->handCount[currentPlayer]; i++) {
		printf("\t\tcard %d = %d, was %d\n", i + 1, testG->hand[currentPlayer][i], G->hand[currentPlayer][i]);
		if (testG->hand[currentPlayer][i] != G->hand[currentPlayer][i]) {
			diffFound = 1;
		}
	}
	if (!diffFound) {
		printf("\t\tNo differences found in hand --> FAIL\n");
		testPass = 0;
	}
	else printf("\t\tHand changed --> PASS\n");

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
