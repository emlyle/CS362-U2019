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

/*
* Function: executeTributeCard
* Parameters: struct gameState *state, int currentPlayer
* Description: This function gives the holder a bonus based on the last two cards in the deck of the opponent sitting to their left.
*		The bonus is based on the type of cards revealed: Action cards result in +2 actions, Treasure cards result in +2 coins,
*		and Victory cards result in +2 cards.
* Returns: int (value of 0)

Test Cases: 
Bonus based on last two cards of currentPlayer + 1
TC3: Last two cards = 2 treasure cards  (empty the player's deck to get the code to work)
	Add two actions (will likely add 4 actions)
TC4: Other player has one card (a victory card) in their deck
	Add two cards to hand? handCount += 2? (from drawCard)
	deckCounter = state->deckCount[player];
	state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to hand
	state->deckCount[player]--;
	state->handCount[player]++;//Increment hand count

TC5: Other player has no cards???
	unless the other player's deck count = 0, the code to add to cards will be missed and will fail

Treasure: 4, 5, 6 
Victory: 1, 2, 3, 10, 16
Action: 11, 12, 13, 14

*/


int testCriteriaForValidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass);
int testCriteriaForInvalidAmbassadorInput(struct gameState *testG, struct gameState *G, int handPos, int currentPlayer, int choice1, int choice2, int returnValue, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail();
void printPass();
int myAssert(int arg1, int arg2, int testPass);


int main() {
	int i;
	int handPos = 0, choice1 = 0, choice2 = 0;
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int testPass = 1;
	int result = -1;
	struct gameState G, testG;
	int k[10] = { minion, embargo, village, baron, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);



	printf("----------------- UNIT TEST 4: Testing %s card ----------------\n", TESTCARD);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: Tribute cards = treasure card + action card\n");
	//NOTE: This test will fail due to 3 existing bugs in the code: 
		//one will fail to set the tribute cards 
		//another bug will still default empty tribute cards to action cards 
		//another bug will iterature through the tribute card array an extra time, accessing an invalid address for a final card

	//Make sure last two cards in other player's deck  are 4 and 11 (treasure and action)
	if (G.deckCount[currentPlayer + 1] < 2) G.deckCount[currentPlayer + 1] = 2; 
	G.deck[currentPlayer + 1][G.deckCount[currentPlayer + 1] - 1] = silver; //Treasure card
	G.deck[currentPlayer + 1][G.deckCount[currentPlayer + 1] - 2] = smithy; //Action card
	
	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeTributeCard(&testG, currentPlayer);

	testPass = 1;
	//Verify return value = 0
	printf("\treturn value = %d, expected = 0 -->", result); 
	testPass = myAssert(result, 0, testPass); 

	//Verify two coins added - for the treasure card
	printf("\tcoins = %d, expected = %d --> ", testG.coins,G.coins + 2); 
 	testPass = myAssert(testG.coins, G.coins + 2, testPass); 

	//Verify two actions added - for the action card 
	printf("\tactions = %d, expected = %d --> ", testG.numActions, G.numActions + 2); 
	testPass = myAssert(testG.numActions, G.numActions + 2, testPass); 

	//Verify no cards added to current player's hand 
	printf("\tcurrent player's hand count = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer]);
	testPass = myAssert(testG.handCount[currentPlayer], G.handCount[currentPlayer], testPass);

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: tribute cards = 2 treasure cards in discard pile, other player's deck is empty) \n");
	//NOTE: This test will fail due to several bugs in the executeTributeCard function

	//Empty out the other player's deck
	for (i = 0; i < G.deckCount[currentPlayer + 1]; i++) {
		G.deck[currentPlayer + 1][i] = -1; 
	}
	G.deckCount[currentPlayer + 1] = 0;

	//Set the other player's discard pile cards to treasure since all will be moved to the deck and shuffled
	for (i = 0; i < G.discardCount[currentPlayer + 1]; i++) {
		G.discard[currentPlayer + 1][i] = copper;
	}

	currentPlayer = 0;
	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeTributeCard(&testG, currentPlayer);

	testPass = 1;
	//Verify return value = 0
	printf("\treturn value = %d, expected = 0 -->", result);
	testPass = myAssert(result, 0, testPass);

	//Verify two coins added - for one treasure card (the duplicate treasure card should have been discarded)
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins + 2);
	testPass = myAssert(testG.coins, G.coins + 2, testPass);

	//Verify no actions added  
	printf("\tactions = %d, expected = %d --> ", testG.numActions, G.numActions);
	testPass = myAssert(testG.numActions, G.numActions, testPass);

	//Verify no cards added to current player's hand 
	printf("\tcurrent player's hand count = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer]);
	testPass = myAssert(testG.handCount[currentPlayer], G.handCount[currentPlayer], testPass);

	concludeTestCase(testPass, 2);




	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------
	printf("TEST CASE 3: Tribute card = 1 victory card, no other cards in other player's deck\n");

	//Empty other player's deck except for one victory card (estate)
	for (i = 1; i < G.deckCount[currentPlayer + 1]; i++) {
		G.deck[currentPlayer + 1][i] = -1;
	}
	G.deck[currentPlayer + 1][0] = estate; //Victory card
	//Set deck to one card
	G.deckCount[currentPlayer + 1] = 1;

	currentPlayer = 0;
	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeTributeCard(&testG, currentPlayer);

	testPass = 1;
	//Verify return value = 0
	printf("\treturn value = %d, expected = 0 -->", result);
	testPass = myAssert(result, 0, testPass);

	//Verify no coins added 
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins);
	testPass = myAssert(testG.coins, G.coins, testPass);

	//Verify no actions added  
	printf("\tactions = %d, expected = %d --> ", testG.numActions, G.numActions);
	testPass = myAssert(testG.numActions, G.numActions, testPass);

	//Verify two cards added to current player's hand 
	printf("\tcurrent player's hand count = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 2); 
	testPass = myAssert(testG.handCount[currentPlayer], G.handCount[currentPlayer] + 2, testPass);

	concludeTestCase(testPass, 3);



	printf(">>>>> UNIT TEST 4 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

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
