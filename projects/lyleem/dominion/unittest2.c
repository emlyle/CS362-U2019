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


/*
* Function: executeMinionCard
* Parameters : int choice1, int choice2, struct gameState *state, int handPos, int currentPlayer
* Description : This function either adds 2 coins(if choice1 passed in is not 0)
* or discards the current player's hand and draws four cards and discards the 
*		hands of players with at least five cards in their hands and draws four cards for each
*		(if choice2 was not 0).If both choice1 and choice2 are 0, the function simply returns 0.
* Returns : int(value of 0)
*/


int testForEveryEstateCard(struct gameState *testG, struct gameState *G, int testPass);
int testDiscardEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass);
int testGainEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);


int main() {
	int i;
	int handPos = 0, choice1 = 0, choice2 = 0;
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int testPass = 1; 
	int estateCardPosition = -1;
	int result = -1; 
	int diffFound = 0; 
	struct gameState G, testG;
	int k[10] = {baron, embargo, village, minion, mine, cutpurse,
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
	//Criteria for all tests: 
	//Verify return value is successful (0)
	printf("\treturn value = %d, expected = 0 --> ", result); 
	if (result != 0) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Criteria for choice1 = 1 (gain 2 coins)
	//Verify 2 coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins + 2); 
	if (testG.coins != G.coins + 2) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");
	
	//Verify hand count was unchanged
	printf("\thandCount = %d, expected = %d --> "); 
	if (testG.handCount[currentPlayer] != G.handCount[currentPlayer]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand was unchanged
	printf("\thand: \n");
	for (i = 0; i < G.handCount[currentPlayer]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG.hand[currentPlayer][i], G.hand[currentPlayer][i]); 
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i]) {
			printf("FAIL\n");
			testPass = 0;
		}
		else printf("PASS\n");
	}

	//Verify hand count of next player was unchanged
	printf("\thandCount = %d, expected = %d --> ");
	if (testG.handCount[currentPlayer + 1] != G.handCount[currentPlayer + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand of next player was unchanged
	printf("\thand: \n");
		for (i = 0; i < G.handCount[currentPlayer + 1]; i++) {
			printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG.hand[currentPlayer + 1][i], G.hand[currentPlayer + 1][i]);
			if (testG.hand[currentPlayer + 1][i] != G.hand[currentPlayer + 1][i]) {
				printf("FAIL\n");
				testPass = 0;
			}
			else printf("PASS\n");
		}


	//testPass = testForEveryEstateCard(&testG, &G, testPass);
	//testPass = testDiscardEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass); 

	concludeTestCase(testPass, 1);



	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: choice1 = 0, choice 2 = 1 (Discard hand, draw new cards, next player has 4 cards)\n");
	//This test case will trigger my bugs in executeMinionCard

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
	//Criteria for all tests: 
	//Verify return value is successful (0)
	printf("\treturn value = %d, expected = 0 --> ", result);
	if (result != 0) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Criteria for choice2 = 1 (discard hand, gain cards)
	//Verify no coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins);
	if (testG.coins != G.coins) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand count is now 4
	printf("\thandCount = %d, expected = 4 --> ");
	if (testG.handCount[currentPlayer] != 4) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand has changed
	diffFound = 0; 
	printf("\thand: \n");
	for (i = 0; i < testG.handCount[currentPlayer]; i++) {
		printf("\t\tcard %d = %d, was %d\n", i + 1, testG.hand[currentPlayer][i], G.hand[currentPlayer][i]);
		if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i]) {
			diffFound = 1; 
		}
	}
	if (!diffFound) {
		printf("\t\tNo differences found in hand --> FAIL\n");
		testPass = 0;
	}
	else printf("\t\tHand changed --> PASS\n");

	//Criteria for next player having 4 cards 
	//Verify hand count of next player has not changed
	printf("\thandCount = %d, expected = %d --> ");
	if (testG.handCount[currentPlayer + 1] != G.handCount[currentPlayer + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand of next player has not changed 
	printf("\thand: \n");
		for (i = 0; i < G.handCount[currentPlayer + 1]; i++) {
			printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG.hand[currentPlayer + 1][i], G.hand[currentPlayer + 1][i]);
			if (testG.hand[currentPlayer + 1][i] != G.hand[currentPlayer + 1][i]) {
				printf("FAIL\n");
				testPass = 0;
			}
			else printf("PASS\n");
		}

	//testPass = testForEveryEstateCard(&testG, &G, testPass);
	//testPass = testGainEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass);

	concludeTestCase(testPass, 2);




	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------
	printf("TEST CASE 3: choice1 = 0, choice 2 = 1 (Discard hand, draw new cards, next player has 5 cards)\n");
	//NOTE: This test case will trigger one of my bugs in executeMinionCard

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
	//Criteria for all tests: 
	//Verify return value is successful (0)
	printf("\treturn value = %d, expected = 0 --> ", result);
	if (result != 0) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Criteria for choice2 = 1 (discard hand, gain cards)
	//Verify no coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins);
	if (testG.coins != G.coins) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand count is now 4
	printf("\thandCount = %d, expected = 4 --> ");
	if (testG.handCount[currentPlayer] != 4) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand has changed
	diffFound = 0;
	printf("\thand: \n");
		for (i = 0; i < testG.handCount[currentPlayer]; i++) {
			printf("\t\tcard %d = %d, was %d\n", i + 1, testG.hand[currentPlayer][i], G.hand[currentPlayer][i]);
			if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i]) {
				diffFound = 1;
			}
		}
	if (!diffFound) {
		printf("\t\tNo differences found in hand --> FAIL\n");
		testPass = 0;
	}
	else printf("\t\tHand changed --> PASS\n");

	//Criteria for next player having 5 cards 
	//Verify hand count of next player was unchanged
	printf("\thandCount = %d, expected = %d --> ");
	if (testG.handCount[currentPlayer + 1] != G.handCount[currentPlayer + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand of next player was unchanged
	printf("\thand: \n"); 
		for (i = 0; i < G.handCount[currentPlayer + 1]; i++) {
			printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG.hand[currentPlayer + 1][i], G.hand[currentPlayer + 1][i]);
			if (testG.hand[currentPlayer + 1][i] != G.hand[currentPlayer + 1][i]) {
				printf("FAIL\n");
				testPass = 0;
			}
			else printf("PASS\n");
		}

	//testPass = testForEveryEstateCard(&testG, &G, testPass);
	//testPass = testGainEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass);

	concludeTestCase(testPass, 3);




	// --------------------------------------------- TEST CASE 4 ------------------------------------------------
	printf("TEST CASE 4: choice1 = 0, choice2 = 0 (Do nothing\n");

	currentPlayer = 0; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //gain 4 coins
	choice2 = 0;
	result = executeMinionCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	//Criteria for all tests: 
	//Verify return value is successful (0)
	printf("\treturn value = %d, expected = 0 --> ", result);
	if (result != 0) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Criteria for choice1 = 0 and choice2 - 0 (do nothing)
	//Verify no coins were gained
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins);
	if (testG.coins != G.coins) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand count was unchanged
	printf("\thandCount = %d, expected = %d --> ");
	if (testG.handCount[currentPlayer] != G.handCount[currentPlayer]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand was unchanged
	printf("\thand: \n");
		for (i = 0; i < G.handCount[currentPlayer]; i++) {
			printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG.hand[currentPlayer][i], G.hand[currentPlayer][i]);
			if (testG.hand[currentPlayer][i] != G.hand[currentPlayer][i]) {
				printf("FAIL\n");
				testPass = 0;
			}
			else printf("PASS\n");
		}

	//Verify hand count of next player was unchanged
	printf("\thandCount = %d, expected = %d --> ");
	if (testG.handCount[currentPlayer + 1] != G.handCount[currentPlayer + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify hand of next player was unchanged
	printf("\thand: \n");
		for (i = 0; i < G.handCount[currentPlayer + 1]; i++) {
			printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG.hand[currentPlayer + 1][i], G.hand[currentPlayer + 1][i]);
			if (testG.hand[currentPlayer + 1][i] != G.hand[currentPlayer + 1][i]) {
				printf("FAIL\n");
				testPass = 0;
			}
			else printf("PASS\n");
		}

	//testPass = testForEveryEstateCard(&testG, &G, testPass);
	//testPass = testDiscardEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass); 

	concludeTestCase(testPass, 1);



	printf(">>>>> UNIT TEST 2 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

	return 0;

}



int testForEveryEstateCard(struct gameState *testG, struct gameState *G, int testPass) {
	//Verify numBuys was incremented
	printf("\tnumBuys = %d, expected = %d --> ", testG->numBuys, G->numBuys + 1);
	if (testG->numBuys != (G->numBuys + 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	return testPass; 
}


int testDiscardEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass) {
	//For discarding an estate card: 
	//Verify 4 coins have been gained 
	printf("\tcoins = %d, expected = %d --> ", testG->coins, G->coins + 4);
	if (testG->coins != (G->coins + 4)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify estate card is in current player's discard pile 
	printf("\ttop of discard pile = %d, expected = 1 --> ", testG->discard[currentPlayer][testG->discardCount[currentPlayer]]);
	if (testG->discard[currentPlayer][testG->discardCount[currentPlayer]] == estate) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify discard count was updated: testG->discardCount[currentPlayer] should be 1 greater than G->discardCount[currentPlayer];
	printf("\tdiscardCount = %d, expected = %d --> ", testG->discardCount[currentPlayer], G->discardCount[currentPlayer] + 1);
	if (testG->discardCount[currentPlayer] != (G->discardCount[currentPlayer] + 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify current player's last card is now -1 (since they should have one card less) 
	printf("\tprevious last card in hand = %d, expected = -1 --> ", testG->hand[currentPlayer][testG->handCount[currentPlayer]]);
	if (testG->hand[currentPlayer][testG->handCount[currentPlayer]] != -1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify estate card was removed and next card in hand was moved up into that position
	printf("\thand position of estate card = %d, expected = %d --> ", testG->hand[currentPlayer][estateCardPosition], G->hand[currentPlayer][estateCardPosition + 1]);
	if (testG->hand[currentPlayer][estateCardPosition] != G->hand[currentPlayer][estateCardPosition + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify that hand count was decremented 
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] + 1);
	if (testG->handCount[currentPlayer] != (G->handCount[currentPlayer] - 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	return testPass;
}


int testGainEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass) {
	//For gaining an estate card: 
	//Verify player's hand contains an extra card
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] + 1);
	if (testG->handCount[currentPlayer] != G->handCount[currentPlayer] + 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify that player's hand contains the new estate card 
	printf("\tTop of hand = %d, expected = 1 --> ", testG->hand[currentPlayer][G->handCount[currentPlayer]]);
	if (testG->hand[currentPlayer][G->handCount[currentPlayer]] != estate) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify supply count is one less now
	printf("\tsupplyCount = %d, expected = %d --> ", testG->supplyCount[estate], G->supplyCount[estate] - 1);
	if (testG->supplyCount[estate] != G->supplyCount[estate] - 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

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
