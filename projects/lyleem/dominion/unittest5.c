/*
 * unittest5.c - This tests executeMineCard function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest5: unittest5.c dominion.o rngs.o
 *      gcc -o unittest5 -g  unittest5.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "mine"

int validInputCriteria(struct gameState *G, struct gameState *testG, int choice1, int choice2, int currentPlayer, int returnValue, int testPass);
int invalidInputCriteria(struct gameState *G, struct gameState *testG, int choice1, int choice2, int curretnPlayer, int returnValue, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 
int myAssert(int arg1, int arg2, int testPass); 

/*
* Function: executeMineCard
* Parameters: int choice1, int choice2, struct gameState *state, int handPos, int currentPlayer
* Description: This function discards a treasure card (passed in as choice1) from the current player's hand 
*		and adds a more valuable treasure card from supply (passed in as choice2) to the current player's hand.  
* Returns: int (value of 0 or -1)

Test Cases: 
choice1 is the position of the card to trash - card must be treasure card 4, 5, or 6 (nothing outside of that range)
choice2 is the card to gain - must be in [0, 26] 
return value is 0 (or -1 if out of the bounds above)
player must gain the choice2 card
player must discard the card from choice1 position  (BUG - this does not happen)
hand count should not change (discard and gain --> no change)
supply count for [choice2 card] should decrement

*/



int main() {
	int i;
	int handPos = 0, choice1 = 0, choice2 = 0;
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int testPass = 1; 
	int result = -1; 
	struct gameState G, testG;
	int k[10] = {minion, embargo, village, baron, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	 

	printf("----------------- UNIT TEST 5: Testing %s card ----------------\n", TESTCARD);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: trash a copper, gain a silver \n");

	//Make it easy to tell if copper is trashed and silver is gained by not having any duplicates of these in the player's hand
	//choice1 = hand position 0
	G.hand[currentPlayer][choice1] = copper; //card to trash
	//Set all other cards to estate 
	for (i = 1; i < G.handCount[currentPlayer]; i++) {
		G.hand[currentPlayer][i] = estate; 
	}
	choice2 = silver; //card to gain

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeMineCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1; 
	testPass = validInputCriteria(&G, &testG, choice1, choice2, currentPlayer, result, testPass); 

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: trash an adventurer (invalid option), gain a silver\n");

	currentPlayer = 0; 	
	handPos = 0; 

	//Make it easy to tell if adventurer is trashed and silver is gained by not having any duplicates of these in the player's hand
	//choice1 = hand position 0
	G.hand[currentPlayer][choice1] = adventurer; //card to trash
											 //Set all other cards to estate 
	for (i = 1; i < G.handCount[currentPlayer]; i++) {
		G.hand[currentPlayer][i] = estate;
	}
	choice2 = silver; //card to gain

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeMineCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = invalidInputCriteria(&G, &testG, choice1, choice2, currentPlayer, result, testPass);

	concludeTestCase(testPass, 2);




	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------
	printf("TEST CASE 3: trash a copper, gain a 27 (invalid card enum value)\n");

	currentPlayer = 0;
	handPos = 0;

	//Make it easy to tell if adventurer is trashed and silver is gained by not having any duplicates of these in the player's hand
	//choice1 = hand position 0
	G.hand[currentPlayer][choice1] = copper; //card to trash
												 //Set all other cards to estate 
	for (i = 1; i < G.handCount[currentPlayer]; i++) {
		G.hand[currentPlayer][i] = estate;
	}
	choice2 = 27; //card to gain

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeMineCard(choice1, choice2, &testG, handPos, currentPlayer);

	testPass = 1;
	testPass = invalidInputCriteria(&G, &testG, choice1, choice2, currentPlayer, result, testPass);

	concludeTestCase(testPass, 3);



	printf(">>>>> UNIT TEST 5 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

	return 0;

}



int validInputCriteria(struct gameState *G, struct gameState *testG, int choice1, int choice2, int currentPlayer, int returnValue, int testPass) {
	int i;

	//Verify return value of 0
	printf("\treturn value = %d, expected 0 --> ", returnValue);
	testPass = myAssert(returnValue, 0, testPass);

	//Verify no change to player's hand count
	printf("\thandCount = %d, expected %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer]);
	testPass = myAssert(testG->handCount[currentPlayer], G->handCount[currentPlayer], testPass);

	//Verify card from choice1 position has been discarded
	//Verify player has gained the choice2 card
	int discardedCard = G->hand[currentPlayer][choice1];
	int discardedCardFound = 0;
	int newCard = choice2;
	int newCardFound = 0;
	for (i = 0; i < G->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == discardedCard) {
			discardedCardFound = 1;
		}
		else if (testG->hand[currentPlayer][i] == newCard) {
			newCardFound = 1;
		}
	}
	if (discardedCardFound) printf("\tDiscarded card still in player's hand --> ");
	else printf("\tDiscarded card is no longer in player's hand --> ");
	testPass = myAssert(discardedCardFound, 0, testPass);
	if (newCardFound) printf("\tGained card found in player's hand --> ");
	else printf("\tGained card not found in player's hand --> ");
	testPass = myAssert(newCardFound, 1, testPass);

	//Supply count for choice2 card should decrement 
	printf("\tsupply count = %d, expected %d --> ", testG->supplyCount[choice2], G->supplyCount[choice2] - 1);
	testPass = myAssert(testG->supplyCount[choice2], G->supplyCount[choice2] - 1, testPass);

	return testPass;
}

int invalidInputCriteria(struct gameState *G, struct gameState *testG, int choice1, int choice2, int currentPlayer, int returnValue, int testPass) {
	int i;

	//Verify return value of -1
	printf("\treturn value = %d, expected -1 --> ", returnValue);
	testPass = myAssert(returnValue, -1, testPass);

	//Verify no change to player's hand count
	printf("\thandCount = %d, expected %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer]);
	testPass = myAssert(testG->handCount[currentPlayer], G->handCount[currentPlayer], testPass);

	//Verify hand is unchanged 
	printf("\thand: \n");
	for (i = 0; i < G->handCount[currentPlayer]; i++) {
		printf("\t\tcard %d = %d, expected = %d --> ", i + 1, testG->hand[currentPlayer][i], G->hand[currentPlayer][i]);
		testPass = myAssert(testG->hand[currentPlayer][i], G->hand[currentPlayer][i], testPass);
	}

	//Verify no change to supply count for choice2 card
	printf("\tsupply count = %d, expected %d --> ", testG->supplyCount[choice2], G->supplyCount[choice2]);
	testPass = myAssert(testG->supplyCount[choice2], G->supplyCount[choice2], testPass);

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
