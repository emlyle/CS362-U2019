/*
 * unittest4.c - This tests executeTributeCard function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "tribute"


int testCriteriaForValidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass);
int testCriteriaForInvalidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 


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

	 

	printf("----------------- UNIT TEST 4: Testing %s card ----------------\n", TESTCARD);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: choice2 = 1 (discard 1 copy of choice1 card)\n");

	//Make sure the card in position 1 does not match the card in handPos (position 0)
	if (G.hand[currentPlayer][1] == G.hand[currentPlayer][0]) {
		if (G.hand[currentPlayer][1] > 0) G.hand[currentPlayer][1]--; 
		else G.hand[currentPlayer][1]++; 
	}

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	
	choice1 = G.hand[currentPlayer][1]; //card to discard 
	choice2 = 1;  //number of choice1 card to discard
	result = executeAmbassadorCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1; 
	testPass = testCriteriaForValidAmbassadorInput(&testG, &G, handPos, currentPlayer, choice1, choice2, result, testPass); 

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: choice2 = 2 (discard 2 copies of choice1 card) \n");

	currentPlayer = 0; 	
	handPos = 0; 

	//Make sure the card in position 1 does not match the card in handPos (position 0)
	if (G.hand[currentPlayer][1] == G.hand[currentPlayer][0]) {
		if (G.hand[currentPlayer][1] > 0) G.hand[currentPlayer][1]--;
		else G.hand[currentPlayer][1]++;
	}
	//Make card in position 2 match card in position 1 to ensure two copies exist
	G.hand[currentPlayer][2] = G.hand[currentPlayer][1]; 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	choice1 = G.hand[currentPlayer][1]; //card to discard 
	choice2 = 2;  //number of choice1 card to discard
	result = executeAmbassadorCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = testCriteriaForValidAmbassadorInput(&testG, &G, handPos, currentPlayer, choice1, choice2, result, testPass);

	concludeTestCase(testPass, 2);




	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------
	printf("TEST CASE 3: choice2 = 1 (discard 1 copy of choice1 card but no choice1 cards in player's hand)\n");

	currentPlayer = 0;
	handPos = 0;
	
	//Set first card in handPos to estate
	G.hand[currentPlayer][0] = estate;
	//Set all remaining cards in hand to silver 
	for (i = 1; i < G.handCount[currentPlayer]; i++) {
		G.hand[currentPlayer][i] = silver; 
	}

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	choice1 = copper; //card to discard 
	choice2 = 1;  //number of choice1 card to discard
	result = executeAmbassadorCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = testCriteriaForInvalidAmbassadorInput(&testG, &G, handPos, currentPlayer, choice1, choice2, result, testPass);

	concludeTestCase(testPass, 3);




	// --------------------------------------------- TEST CASE 4 ------------------------------------------------
	printf("TEST CASE 4: choice1 = handpos (invalid option)\n");

	currentPlayer = 0;
	handPos = 0;

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));

	choice1 = handPos; //card to discard 
	choice2 = 1;  //number of choice1 card to discard
	result = executeAmbassadorCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = testCriteriaForInvalidAmbassadorInput(&testG, &G, handPos, currentPlayer, choice1, choice2, result, testPass);

	concludeTestCase(testPass, 4);




	printf(">>>>> UNIT TEST 3 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

	return 0;

}



int testCriteriaForValidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass) {
	int i; 

	//Verify return value equals 0
	printf("\treturn value = %d, expected 0 --> ", returnValue); 
	if (returnValue != 0) testPass = printFail();
	else printPass();


	//Verify handCount has decreased by the number to discard specified in choice2 
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] - choice2); 
	if (testG->handCount[currentPlayer] != G->handCount[currentPlayer] - choice2) testPass = printFail(); //(***this will fail due to a bug I added in executeAmbassadorCard for Assignment 2)
	else printPass(); 


	//Verify the number of choice1 cards in the hand has decreased by the number specified to discard in choice2 
	//Get count of choice1 cards in testG hand
	int choice1CardsInTestG = 0; 
	for (i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == choice1) {
			choice1CardsInTestG++;
		}
	}
	//Get count of choice1 cards in G hand
	int choice1CardsInG = 0;
	for (i = 0; i < G->handCount[currentPlayer]; i++) {
		if (G->hand[currentPlayer][i] == choice1) {
			choice1CardsInG++;
		}
	}
	printf("\tchoice1 cards in hand = %d, expected = %d --> ", choice1CardsInTestG, choice1CardsInG - choice2); 
	if (choice1CardsInTestG != choice1CardsInG - choice2) testPass = printFail(); //(***this will fail due to a bug I added in executeAmbassadorCard for Assignment 2)
	else printPass();


	//Verify supply count has increased by the number specified in choice2
	printf("\tsupplyCount = %d, expected = %d --> ", testG->supplyCount[testG->hand[currentPlayer][choice1]], G->supplyCount[G->hand[currentPlayer][choice1]] - choice2);
	if (testG->supplyCount[choice1] != G->supplyCount[choice1] - choice2) testPass = printFail(); //(***this will fail due to a bug I added in executeAmbassadorCard for Assignment 2)
	else printPass();


	//Verify next player's hand count has increased by one
	printf("\tnext player's handCount = %d, expected = %d --> ", testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1] + 1);
	if (testG->handCount[currentPlayer + 1] != G->handCount[currentPlayer + 1] + 1) testPass = printFail();
	else printPass();


	//Verify next player's hand includes an extra copy of the choice1 card 
	//Get count of choice1 cards in testG next player's hand
	choice1CardsInTestG = 0;
	for (i = 0; i < testG->handCount[currentPlayer + 1]; i++) {
		if (testG->hand[currentPlayer + 1][i] == choice1) {
			choice1CardsInTestG++;
		}
	}
	//Get count of choice1 cards in G next player's hand
	choice1CardsInG = 0;
	for (i = 0; i < G->handCount[currentPlayer + 1]; i++) {
		if (G->hand[currentPlayer + 1][i] == choice1) {
			choice1CardsInG++;
		}
	}
	printf("\tchoice1 cards in hand = %d, expected = %d --> ", choice1CardsInTestG, choice1CardsInG + 1);
	if (choice1CardsInTestG != choice1CardsInG + 1) testPass = printFail();
	else printPass();

	return testPass; 
}


int testCriteriaForInvalidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass) {
	int i; 

	//Verify return value is -1 (for invalid input)
	printf("\treturn value = %d, expected = -1 --> ", returnValue);
	if (returnValue != -1) testPass = printFail();
	else printPass();

	//Verify hand count is unchanged - may fail due to my Assignment 2 bug in executeAmbassadorCard
	printf("\thandCount = %d, expected %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer]); 
	if (testG->handCount[currentPlayer] != G->handCount[currentPlayer]) testPass = printFail(); 
	else printPass(); 

	//Verify hand is unchanged - may fail due to my Assignment 2 bug in executeAmbassadorCard
	printf("\thand: \n");
	for (i = 0; i < G->handCount[currentPlayer]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer][i], G->hand[currentPlayer][i]);
		if (testG->hand[currentPlayer][i] != G->hand[currentPlayer][i]) testPass = printFail(); 
		else printPass();
	}

	//Verify supply count is unchanged - may fail due to my Assignment 2 bug in executeAmbassadorCard
	printf("\tsupplyCount = %d, expected = %d, --> ", testG->supplyCount[testG->hand[currentPlayer][choice1]], G->supplyCount[G->hand[currentPlayer][choice1]]);
	if (testG->supplyCount[testG->hand[currentPlayer][choice1]] != G->supplyCount[G->hand[currentPlayer][choice1]]) testPass = printFail();
	else printPass();

	//Verify next player's hand count is unchanged - may fail due to my Assignment 2 bug in executeAmbassadorCard
	printf("\thandCount = %d, expected %d --> ", testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1]);
	if (testG->handCount[currentPlayer + 1] != G->handCount[currentPlayer + 1]) testPass = printFail(); //will fail due to existing bug in dominion.c
	else printPass();

	//Verify next player's hand is unchanged - may fail due to my Assignment 2 bug in executeAmbassadorCard
	printf("\thand: \n");
	for (i = 0; i < G->handCount[currentPlayer + 1]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer + 1][i], G->hand[currentPlayer + 1][i]);
		if (testG->hand[currentPlayer + 1][i] != G->hand[currentPlayer + 1][i]) testPass = printFail(); //will fail due to existing bug in dominion.c
		else printPass();
	}
	
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
