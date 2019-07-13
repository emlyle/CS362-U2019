/*
 * unittest3.c - This tests executeAmbassadorCard function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "ambassador"


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

	 

	printf("----------------- UNIT TEST 3: Testing %s card ----------------\n", TESTCARD);

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
	testPass = myAssert(returnValue, 0, testPass); 

	//Verify handCount has decreased by the number to discard specified in choice2 
	//***This condition will fail due to a bug I added in executeAmbassadorCard for Assignment 2
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] - choice2); 
	testPass = myAssert(testG->handCount[currentPlayer], G->handCount[currentPlayer] - choice2, testPass); 


	//Verify the number of choice1 cards in the hand has decreased by the number specified to discard in choice2 
	//***This condition will fail due to a bug I added in executeAmbassadorCard for Assignment 2
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
	testPass = myAssert(choice1CardsInTestG, choice1CardsInG - choice2, testPass); 


	//Verify supply count has increased by the number specified in choice2
	//***This condition will fail due to a bug I added in executeAmbassadorCard for Assignment 2
	printf("\tsupplyCount = %d, expected = %d --> ", testG->supplyCount[choice1], G->supplyCount[choice1] - choice2);
	testPass = myAssert(testG->supplyCount[choice1], G->supplyCount[choice1] - choice2, testPass); 


	//Verify next player's hand count has increased by one
	printf("\tnext player's handCount = %d, expected = %d --> ", testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1] + 1);
	testPass = myAssert(testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1] + 1); 


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
	testPass = myAssert(choice1CardsInTestG, choice1CardsInG + 1, testPass); 

	return testPass; 
}


int testCriteriaForInvalidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass) {
	int i; 

	//Verify return value is -1 (for invalid input)
	printf("\treturn value = %d, expected = -1 --> ", returnValue);
	testPass = myAssert(returnValue, -1, testPass); 


	//Verify hand count is unchanged 
	//***If player does not have enough choice1 cards, this condition will fail due to a bug I added for Assignment 2 in executeAmbassadorCard
	printf("\thandCount = %d, expected %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer]); 
	testPass = myAssert(testG->handCount[currentPlayer], G->handCount[currentPlayer], testPass); 


	//Verify hand is unchanged 
	//***If player does not have enough choice1 cards, this condition will fail due to a bug I added for Assignment 2 in executeAmbassadorCard
	printf("\thand: \n");
	for (i = 0; i < G->handCount[currentPlayer]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer][i], G->hand[currentPlayer][i]);
		testPass = myAssert(testG->hand[currentPlayer][i], G->hand[currentPlayer][i], testPass); 
	}


	//Verify supply count is unchanged 
	//***If player does not have enough choice1 cards, this condition will fail due to a bug I added for Assignment 2 in executeAmbassadorCard
	printf("\tsupplyCount = %d, expected = %d, --> ", testG->supplyCount[choice1], G->supplyCount[choice1]);
	testPass = myAssert(testG->supplyCount[choice1], G->supplyCount[choice1], testPass); 


	//Verify next player's hand count is unchanged 
	//***If player does not have enough choice1 cards, this condition will fail due to a bug I added for Assignment 2 in executeAmbassadorCard
	printf("\thandCount = %d, expected %d --> ", testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1]);
	testPass = myAssert(testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1], testPass); 

	//Verify next player's hand is unchanged 
	//***If player does not have enough cards, this condition will fail due to a bug I added for Assignment 2 in executeAmbassadorCard
	printf("\thand: \n");
	for (i = 0; i < G->handCount[currentPlayer + 1]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer + 1][i], G->hand[currentPlayer + 1][i]);
		testPass = myAssert(testG->hand[currentPlayer + 1][i], G->hand[currentPlayer + 1][i], testPass); 
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