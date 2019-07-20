/*
 * randomtestcard1.c - This tests executeBaronCard function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * randomtestcard1: randomtestcard1.c dominion.o rngs.o
 *      gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "baron"


int testForEveryBaronCard(struct gameState *testG, struct gameState *G, int returnValue, int testPass);
int testDiscardEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass);
int testGainEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass);
void concludeTestCase(int testPass, int testCaseNumber);
int myAssert(int arg1, int arg2, int testPass);
int printFail();
void printPass();


/*
MY NOTES: 

USE THIS TO INITIALIZE THE BARON CARD RANDOM FUNCTION CALL: 
Define variables first 
Create a loop of the number of tests you want to run (for i in 2000): 
Initialize variables: 
	choice1 - 0 or 1 
	select random numPlayers in [2,4]
	Based on numPlayers generated: 
		currentPlayer = 0 to numPlayers-1 
	gameState: 
		create a gameState G and...
		set handCount[currentPlayer] = 5
		set hand[currentPlayer][0 to 5] = random() - what makes sense for this random pool of choices? - see supply 
			based on numPlayers - then choose from this (but up the probability based on totals??)
		currentPlayer's coins and store in state->coins
			each copper in the player's hand is worth 1, silver 2, and gold 3
			so (5 coppers = 5, 5 golds = 15) so a coins value randomly in [0,15] seems reasonable 
		
		//Discard pile does not matter other than that it should be incremented from wherever it's at 
			discardCount[currentPlayer] = [0, MAX_DECK]
			discard[currentPlayer][discardCount[currentPlayer]] - I don't think I even need to set this 
		
		Set k[10] to ten distinct, random kingdom cards in [adventurer, treasure_map] which is [7, 26]
		if (numPlayers == 2)
			supplyCount[estate] = [0,8]
			supplyCount[duchy] = [0,8]
			supplyCount[province] = [0,8]
			supplyCount of copper = [0,46] 
				silver = [0,40]
				gold = [0,30]
			supplyCount[curse] = [0,10]
			supplyCount[enum] = [0,?] 
			For each position in k: 
				if k[i] == gardens || great_hall ... then supplyCount[k[i]] = [0,8]
				else supplyCount[k[i]] = [0,10] 
		if (numPlayers == 3)
			supplyCount[estate] = [0,12]
			supplyCount[province] = [0,12]
			supplyCount[duchy] = [0,8]
			supplyCount of copper = [0,39] 
				silver = [0,40]
				gold = [0,30]
			supplyCount[curse] = [0,20]
			supplyCount[enum] = [0,?] 
			For each position in k: 
				if k[i] == gardens || great_hall ... then supplyCount[k[i]] = [0,12]
				else supplyCount[k[i]] = [0,10] 
		if (numPlayers == 4)
			supplyCount[estate] = [0,12]
			supplyCount[duchy] = [0,8]
			supplyCount[province] = [0,12]
			supplyCount of copper = [0,32] 
				silver = [0,40]
				gold = [0,30]
			supplyCount[curse] = [0,30]
			For each position in k: 
				if k[i] == gardens || great_hall ... then supplyCount[k[i]] = [0,12]
				else supplyCount[k[i]] = [0,10] 
		All other kingdom cards [7, 26] not in k: supplyCount[enum] = -1
Then, call a function that (pass in G and currentPlayer)...
	copy G to testG 
	call the executeBaronCard function 
	assess results by comparing member vars in G with testG based on expected changes 

*/

int main() {
	
	int i, j, choice1, currentPlayer, numPlayers; 
	struct gameState preState; 
	int k[10]; 
	int totalTests = 2; //TODO: increase this later

	//Set up random number generator
	SelectStream(2);
	PutSeed(3);

	printf(">>>>>>>>>>>>>>Random Test 1 for Baron Card<<<<<<<<<<<<<<\n"); 

	for (i = 0; i < totalTests; i++) {
		printf("Random Test %d\n", i + 1); 

		//Initialize variables to valid random values 
		choice1 = floor(Random() * 1); //0 or 1
		numPlayers = floor(Random() * 4); //2, 3, or 4
		currentPlayer = floor(Random() * numPlayers); //0 to numPlayers - 1


		//Call function under test and verify results 


	}

	printf(">>>>>>>>>Random Test 1 for Baron Card Completed<<<<<<<<<\n"); 





//OLD STUFF: 
	int i;
	int handpos = 0, choice1 = 0, choice2 = 0, bonus = 0;
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int testPass = 1; 
	int result; 
	int estateCardPosition = -1;
	struct gameState G, testG;
	int k[10] = {baron, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);



	printf("----------------- UNIT TEST 1: Testing %s card ----------------\n", TESTCARD);

	// ----------- TEST CASE 1: choice1 = 0 (discard an estate card) --------------
	printf("TEST CASE 1: choice1 = 1 (Discard an estate card, current player has an estate card in their hand)\n");

	//Make sure the hand of this player contains an estate card:  
	for (i = 0; i < G.handCount[currentPlayer]; i++) {
		if (G.hand[currentPlayer][i] == estate) {
			estateCardPosition = i; 
		}
	}
	//If the current player does not hold an estate card, add one to their hand 
	if (estateCardPosition < 0) {
		G.hand[currentPlayer][1] = estate;
		estateCardPosition = 1;
	}

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //discard estate card
	result = executeBaronCard(choice1, &testG, handpos, currentPlayer); 

	testPass = 1; 
	testPass = testForEveryBaronCard(&testG, &G, result, testPass);
	testPass = testDiscardEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass); 

	concludeTestCase(testPass, 1);



	// ----------------------------- TEST CASE 2: choice1 = 0 (gain an estate card) ---------------------------------
	printf("TEST CASE 2: choice1 = 0 (Gain an estate card)\n");

	currentPlayer = 0; 	

	// Copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0; //gain estate card
	result = executeBaronCard(choice1, &testG, handpos, currentPlayer);

	testPass = 1;
	testPass = testForEveryBaronCard(&testG, &G, result, testPass);
	testPass = testGainEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass);

	concludeTestCase(testPass, 2);




	// ------------------------- TEST CASE 3: choice1 = 0 (discard an estate card with two estate cards in hand) ------------------------
	printf("TEST CASE 3: choice1 = 0 (Discard an estate card with two estate cards in hand)\n");
	//NOTE: This test case will trigger one of my bugs - the estate card in position 0 should be discarded but the second one will instead, so the test case will FAIL 

	currentPlayer = 0;

	//Set player's hand: 
	G.hand[currentPlayer][0] = estate; //This is the estate card that should be discarded but won't due to a bug I added for Assignment 2
	G.hand[currentPlayer][1] = copper; //This is to help the test criteria ensure that a non-estate card was moved up into position 0 
	//Make sure player's hand contains a second estate card
	estateCardPosition = -1;
	for (i = 2; i < G.handCount[currentPlayer]; i++) {
		//printf("Card #%d: %d\n", (i + 1), G.hand[currentPlayer][i]);
		if (G.hand[currentPlayer][i] == estate) {
			estateCardPosition = i;
		}
	}
	//If the current player does not hold a second estate card, add one to their hand 
	if (estateCardPosition < 0) {
		G.hand[currentPlayer][2] = estate;
	}
	estateCardPosition = 0; //Setting this to 0 since that's where I inserted the FIRST estate card, which should be discarded 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //discard estate card
	result = executeBaronCard(choice1, &testG, handpos, currentPlayer);

	testPass = 1;
	testPass = testForEveryBaronCard(&testG, &G, result, testPass);
	testPass = testDiscardEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass);

	concludeTestCase(testPass, 3); 




	//This test case will trigger a bug I added to the code for Assignment 2   
	// ------------------------- TEST CASE 4: choice1 = 0 (discard an estate card with no estate cards in hand) ------------------------
	printf("TEST CASE 4: choice1 = 0 (Discard an estate card but current player does not have an estate card in their hand)\n");

	currentPlayer = 0;

	//Remove any estate cards from current player's hand 
	for (i = 0; i < G.handCount[currentPlayer]; i++) {
		if (G.hand[currentPlayer][i] == estate) {
			G.hand[currentPlayer][i] = duchy; //replace estate card with duchy
		}
	}

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //discard estate card
	result = executeBaronCard(choice1, &testG, handpos, currentPlayer);

	testPass = 1;
	testPass = testForEveryBaronCard(&testG, &G, result, testPass);
	testPass = testGainEstateCard(&testG, &G, currentPlayer, estateCardPosition, testPass);

	concludeTestCase(testPass, 4);

	

	printf(">>>>> UNIT TEST 1 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

	return 0;

}



int testForEveryBaronCard(struct gameState *testG, struct gameState *G, int returnValue, int testPass) {
	//Verify return value is zero
	printf("\treturn value = %d, expected = 0 --> ", returnValue); 
	testPass = myAssert(returnValue, 0, testPass);

	//Verify numBuys was incremented
	printf("\tnumBuys = %d, expected = %d --> ", testG->numBuys, G->numBuys + 1);
	testPass = myAssert(testG->numBuys, G->numBuys + 1, testPass);

	return testPass; 
}


int testDiscardEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass) {
	//For discarding an estate card: 
	//Verify 4 coins have been gained 
	printf("\tcoins = %d, expected = %d --> ", testG->coins, G->coins + 4);
	testPass = myAssert(testG->coins, G->coins + 4, testPass);

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
	testPass = myAssert(testG->hand[currentPlayer][testG->handCount[currentPlayer]], -1, testPass);

	//Verify estate card was removed and next card in hand was moved up into that position
	printf("\thand position of estate card = %d, expected = %d --> ", testG->hand[currentPlayer][estateCardPosition], G->hand[currentPlayer][estateCardPosition + 1]);
	testPass = myAssert(testG->hand[currentPlayer][estateCardPosition], G->hand[currentPlayer][estateCardPosition + 1], testPass);

	//Verify that hand count was decremented 
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] + 1);
	testPass = myAssert(testG->hand[currentPlayer][estateCardPosition], G->hand[currentPlayer][estateCardPosition + 1], testPass); 

	return testPass;
}


int testGainEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass) {
	//For gaining an estate card: 
	//Verify player's hand contains an extra card
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] + 1);
	testPass = myAssert(testG->handCount[currentPlayer], G->handCount[currentPlayer] + 1, testPass); 

	//Verify that player's hand contains the new estate card 
	printf("\tTop of hand = %d, expected = 1 --> ", testG->hand[currentPlayer][G->handCount[currentPlayer]]);
	testPass = myAssert(testG->hand[currentPlayer][G->handCount[currentPlayer]], estate, testPass); 

	//Verify supply count is one less now
	printf("\tsupplyCount = %d, expected = %d --> ", testG->supplyCount[estate], G->supplyCount[estate] - 1);
	testPass = myAssert(testG->supplyCount[estate], G->supplyCount[estate] - 1, testPass);

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
