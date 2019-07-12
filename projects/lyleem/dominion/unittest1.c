/*
 * unittest1.c - tests executeBaronCard function
 *
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 */

//NOTE: This file is currently made up of a sample unit test consisting of test cases - I will replace it with my unittest1 for the baron card after exploring how to
//run and integrate this sample file within the dominion program and make to get the output file
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "baron"

//NOTES: 
/*
* Function: executeBaronCard
* Parameters: int choice1, struct gameState *state, int handPos, int currentPlayer
* Description: This function either discards an Estate card from the current player's hand and gains 4 coins,
*	or if the current player has no Estate card or has opted not to discard one, adds an Estate card to their hand from Supply.
* Returns: int (value of 0)
*/

//Choice1 is either 0 or not 0 (true or false)
//discard is always lowest index (first) estate card (so you could test a hand with mutliple estate cards)
//You have to pass it a gameState struct, handPos, and currentPlayer 

//Just start with these three test cases and move on for now - you can always add more before you turn it in!
//Test Case 1: choice1 = 0 (discard an estate card)
//Test Case 2: choice1 = 1 (gain an estate card)
//Test Case 3: choice 1 = 0 (discard estate card but no estate card in deck) - I currently have this test case commented out since it triggers an infinite loop 


int main() {
    	int newCards = 0;
    	int discarded = 1;
    	int xtraCoins = 0;
    	int shuffledCards = 0;

    	int i, j, m, x;
    	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    	int remove1, remove2;
    	int seed = 1000;
    	int numPlayers = 2;
    	int currentPlayer = 0;
	int testPass = 1; 
	struct gameState G, testG;
	int k[10] = {baron, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};				//***WHERE does the estate card go????????????????????????????????????????????

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	int estateCardPosition = -1;

	printf("----------------- UNIT TEST 1: Testing %s card ----------------\n", TESTCARD);

	
	// ----------- TEST CASE 1: choice1 = 0 (discard an estate card) --------------
	printf("TEST CASE 1: choice1 = 1 (Discard an estate card, current player has an estate card in their hand)\n");

	//Make sure the hand of this player contains an estate card:  
	//printf("Current Player (0): %d cards in hand\n", G.handCount[currentPlayer]); 
	for (x = 0; x < G.handCount[currentPlayer]; x++) {
		//printf("Card #%d: %d\n", (x + 1), G.hand[currentPlayer][x]);
		if (G.hand[currentPlayer][x] == estate) {
			estateCardPosition = x; 
		}
	}
	//If the current player does not hold an estate card, add one to their hand 
	if (estateCardPosition < 0) {
		G.hand[currentPlayer][1] = estate;
		estateCardPosition = 1;
	}
	//printf("Game coins: %d\n", G.coins); 

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //discard estate card
	cardEffect(baron, choice1, choice1, choice2, &testG, handpos, &bonus);
	//printf("Test Game coins: %d\n", testG.coins); 	

	//Things to verify: 
	testPass = 1; 
	//For every test: 
	//testG.numBuys should be 1 greater than G.numBuys (state->numBuys++)
	printf("\tnumBuys = %d, expected = %d --> ", testG.numBuys, G.numBuys + 1); 
	if (testG.numBuys != (G.numBuys + 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n"); 

	//For discarding an estate card: 
	//testG.coins should be 4 greater than G.coins (state->coins += 4) if an estate is discarded 
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins + 4);
	if (testG.coins != (G.coins + 4)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify estate card is in current player's discard pile 
	printf("\ttop of discard pile = %d, expected = 1 --> ", testG.discard[currentPlayer][testG.discardCount[currentPlayer]]); 
	if (testG.discard[currentPlayer][testG.discardCount[currentPlayer]] == estate) { 
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify discard count was updated: testG.discardCount[currentPlayer] should be 1 greater than G.discardCount[currentPlayer];
	printf("\tdiscardCount = %d, expected = %d --> ", testG.discardCount[currentPlayer], G.discardCount[currentPlayer] + 1); 
	if (testG.discardCount[currentPlayer] != (G.discardCount[currentPlayer] + 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify current player's last card is now -1 (since they should have one card less) 
	printf("\tprevious last card in hand = %d, expected = -1 --> ", testG.hand[currentPlayer][testG.handCount[currentPlayer]]); 
	if (testG.hand[currentPlayer][testG.handCount[currentPlayer]] != -1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify estate card was removed and next card in hand was moved up into that position
	printf("\thand position of estate card = %d, expected = %d --> ", testG.hand[currentPlayer][estateCardPosition]);
	if (testG.hand[currentPlayer][estateCardPosition] != G.hand[currentPlayer][estateCardPosition + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify that hand count was decremented 
	printf("\thandCount = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1); 
	if (testG.handCount[currentPlayer] != (G.handCount[currentPlayer] - 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n"); 


	if (testPass) {
		printf("TEST CASE 1 PASSED!\n"); 
	}
	else {
		printf("TEST CASE 1 FAILED. See failure criteria listed above.\n");
	}
	
	printf("END TEST CASE 1\n\n"); 



	// ----------------------------- TEST CASE 2: choice1 = 0 (gain an estate card) ---------------------------------
	printf("TEST CASE 2: choice1 = 0 (Gain an estate card)\n");

	currentPlayer = 0; 

	//TEMP for me: 
	//for (x = 0; x < G.handCount[currentPlayer]; x++) {
	//	printf("Card #%d: %d\n", (x + 1), G.hand[currentPlayer][x]);
		//if (G.hand[currentPlayer][x] == 1) {
		//	G.hand[currentPlayer][x] = 2; //replace estate card (1) with another type of card (2 - duchy)
		//}
	//}
	

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0; //gain estate card
	cardEffect(baron, choice1, choice1, choice2, &testG, handpos, &bonus);
	//printf("Test Game coins: %d\n", testG.coins); 	

	//State of hand after test - TEMP FOR ME
	//printf("Current Player (0): %d cards in hand\n", testG.handCount[currentPlayer]);
	//for (x = 0; x < testG.handCount[currentPlayer]; x++) {
	//	printf("Card #%d: %d\n", (x + 1), testG.hand[currentPlayer][x]);
	//}
	//printf("Game coins: %d\n", testG.coins);


	//Things to verify: 
	testPass = 1;
	//For every test: 
	//testG.numBuys should be 1 greater than G.numBuys (state->numBuys++)
	printf("\tnumBuys = %d, expected = %d --> ", testG.numBuys, G.numBuys + 1); 
	if (testG.numBuys != (G.numBuys + 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n"); 
	
	//For gaining an estate card: 
	//Verify player's hand contains an extra card
	printf("\thandCount = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1); 
	if (testG.handCount[currentPlayer] != G.handCount[currentPlayer] + 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify that player's hand contains the new estate card 
	printf("\tTop of hand = %d, expected = 1 --> ", testG.hand[currentPlayer][G.handCount[currentPlayer]]); 
	if (testG.hand[currentPlayer][G.handCount[currentPlayer]] != estate) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify supply count is one less now
	printf("\tsupplyCount = %d, expected = %d --> ", testG.supplyCount[estate], G.supplyCount[estate] - 1); 
	if (testG.supplyCount[estate] != G.supplyCount[estate] - 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	
	//NOT TESTED HERE: 
	//TEST CASE: Supply Count == 0
	//Check if game is over??? --> isGameOver(state);


	if (testPass) {
		printf("TEST CASE 2 PASSED!\n");
	}
	else {
		printf("TEST CASE 2 FAILED. See failure criteria listed above.\n");
	}
	
	printf("END TEST CASE 2\n\n");




	// ------------------------- TEST CASE 3: choice1 = 0 (discard an estate card with two estate cards in hand) ------------------------
	printf("TEST CASE 3: choice1 = 0 (Discard an estate card with two estate cards in hand)\n");
	//NOTE: This test case will trigger one of my bugs - the estate card in position 0 should be discarded but the second one will instead 

	currentPlayer = 0;

	G.hand[currentPlayer][0] = estate; //This is the estate card that should be discarded but won't due to a bug I added for Assignment 2
	G.hand[currentPlayer][1] = copper; //This is to help the test criteria ensure that a non-estate card was moved up into position 0 
	//Make sure player's hand contains a second estate card
	estateCardPosition = -1;
	for (x = 2; x < G.handCount[currentPlayer]; x++) {
		//printf("Card #%d: %d\n", (x + 1), G.hand[currentPlayer][x]);
		if (G.hand[currentPlayer][x] == estate) {
			estateCardPosition = x;
		}
	}
	//If the current player does not hold a second estate card, add one to their hand 
	if (estateCardPosition < 0) {
		G.hand[currentPlayer][2] = estate;
	}
	estateCardPosition = 0; //Setting this to 0 since that's where I inserted the FIRST estate card, which should be discarded 
	//printf("Game coins: %d\n", G.coins);


	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //discard estate card
	cardEffect(baron, choice1, choice1, choice2, &testG, handpos, &bonus);
	//printf("Test Game coins: %d\n", testG.coins);

	//Things to verify:
	testPass = 1;
	//For every test:
	//testG.numBuys should be 1 greater than G.numBuys (state->numBuys++)
	printf("\tnumBuys = %d, expected = %d --> ", testG.numBuys, G.numBuys + 1); 
	if (testG.numBuys != (G.numBuys + 1)) {
		printf("FAIL\n");
		testPass = 0;
	} 
	else printf("PASS\n"); 

	//For discarding an estate card: 
	//testG.coins should be 4 greater than G.coins (state->coins += 4) if an estate is discarded 
	printf("\tcoins = %d, expected = %d --> ", testG.coins, G.coins + 4);
	if (testG.coins != (G.coins + 4)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify estate card is in current player's discard pile 
	printf("\ttop of discard pile = %d, expected = 1 --> ", testG.discard[currentPlayer][testG.discardCount[currentPlayer]]);
	if (testG.discard[currentPlayer][testG.discardCount[currentPlayer]] == estate) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify discard count was updated: testG.discardCount[currentPlayer] should be 1 greater than G.discardCount[currentPlayer];
	printf("\tdiscardCount = %d, expected = %d --> ", testG.discardCount[currentPlayer], G.discardCount[currentPlayer] + 1);
	if (testG.discardCount[currentPlayer] != (G.discardCount[currentPlayer] + 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify current player's last card is now -1 (since they should have one card less) 
	printf("\tprevious last card in hand = %d, expected = -1 --> ", testG.hand[currentPlayer][testG.handCount[currentPlayer]]);
	if (testG.hand[currentPlayer][testG.handCount[currentPlayer]] != -1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify estate card was removed and next card in hand was moved up into that position
	printf("\thand position of estate card = %d, expected = %d --> ", testG.hand[currentPlayer][estateCardPosition]);
	if (testG.hand[currentPlayer][estateCardPosition] != G.hand[currentPlayer][estateCardPosition + 1]) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify that hand count was decremented 
	printf("\thandCount = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1); 
	if (testG.handCount[currentPlayer] != (G.handCount[currentPlayer] - 1)) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n"); 


	if (testPass) {
		printf("TEST CASE 3 PASSED!\n");
	}
	else {
		printf("TEST CASE 3 FAILED. See failure criteria listed above.\n");
	}


	printf("END TEST CASE 3\n\n");


	




	/* //This test case will trigger a bug I added to the code for Assignment 2 that will cause an infinite loop - commented out to prevent entire test suite from crashing
	// ------------------------- TEST CASE 3: choice1 = 0 (discard an estate card with no estate cards in hand) ------------------------
	printf("TEST CASE 3: choice1 = 0 (Discard an estate card but current player does not have an estate card in their hand)\n");
	//This is really like gaining an estate card, then 

	int currentPlayer = 0;
	//Make sure the hand of this player contains an estate card:  

	//Remove any estate cards from current player's hand 
	for (x = 0; x < G.handCount[currentPlayer]; x++) {
		printf("Card #%d: %d\n", (x + 1), G.hand[currentPlayer][x]);
		if (G.hand[currentPlayer][x] == 1) {
			G.hand[currentPlayer][x] = 2; //replace estate card (1) with another type of card (2 - duchy)
		}
	}
	//printf("Game coins: %d\n", G.coins); 


	//G.hand[currentPlayer][0] = estate //This would catch my bug if NO other cards are estates
	//G.hand[currentPlayer][1] = estate 
	//Iterate through G.hand[currentPlayer] and if anything equals estate, set it to something else 
	//If choice1 == 1, then set supply - (TC1) empty it (TC2) verify it contains an estate 


	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1; //discard estate card
	cardEffect(baron, choice1, choice1, choice2, &testG, handpos, &bonus);
	//printf("Test Game coins: %d\n", testG.coins); 	

	//Things to verify: 
	int testPass = 1;
	//For every test: 
	//testG.numBuys should be 1 greater than G.numBuys (state->numBuys++)
	if (testG.numBuys != (G.numBuys + 1)) {
		printf("Error - numBuys is incorrect\n");
		testPass = 0;
	}

	//For gaining an estate card (since player does not have an estate card to discard): 
	testPass = 1;
	//For every test: 
	//testG.numBuys should be 1 greater than G.numBuys (state->numBuys++)
	if (testG.numBuys != (G.numBuys + 1)) {
		printf("Error - numBuys is incorrect\n");
		testPass = 0;
	}

	//For gaining an estate card: 
	//Verify player's hand contains an extra card
	printf("handCount = %d, expected = %d --> ", testG.handCount[currentPlayer], G.handCount[currentPlayer] + 1);
	if (testG.handCount[currentPlayer] != G.handCount[currentPlayer] + 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify that player's hand contains the new estate card 
	printf("Top of hand = %d, expected = 1 --> ", testG.hand[currentPlayer][G.handCount[currentPlayer]]);
	if (testG.hand[currentPlayer][G.handCount[currentPlayer]] != 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	//Verify supply count is one less now
	printf("supplyCount = %d, expected = %d --> ", testG.supplyCount[estate], G.supplyCount[estate] - 1);
	if (testG.supplyCount[estate] != G.supplyCount[estate] - 1) {
		printf("FAIL\n");
		testPass = 0;
	}
	else printf("PASS\n");

	if (testPass) {
		printf("TEST CASE 3 PASSED!\n");
	}
	else {
		printf("TEST CASE 3 FAILED. See failure criteria listed above.\n");
	}


	printf("END TEST CASE 3\n");


	*/


	printf(">>>>> UNIT TEST 1 (%s card) Complete <<<<<\n\n", TESTCARD);

	return 0;






	// ----------- TEST 2: choice1 = 2 = +2 coins --------------
	//printf("TEST 2: choice1 = 2 = +2 coins\n");

	// copy the game state to a test case
	//memcpy(&testG, &G, sizeof(struct gameState));
	//choice1 = 2;
	//cardEffect(steward, choice1, choice2, choice3, &testG, handpos, &bonus);

	//newCards = 0;
	//xtraCoins = 2;
	//printf("hand count = %d, expected = %d\n", testG.handCount[currentPlayer], G.handCount[currentPlayer] + newCards - discarded);
	//printf("deck count = %d, expected = %d\n", testG.deckCount[currentPlayer], G.deckCount[currentPlayer] - newCards + shuffledCards);
	//printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	//assert(testG.handCount[currentPlayer] == G.handCount[currentPlayer] + newCards - discarded);
	//assert(testG.deckCount[currentPlayer] == G.deckCount[currentPlayer] - newCards + shuffledCards);
	//assert(testG.coins == G.coins + xtraCoins);

	// ----------- TEST 3: choice1 = 3 = trash two cards --------------

	//printf("TEST 3: choice1 = 3 = trash two cards\n");
	//choice1 = 3;

	// cycle through each eligible combination of 2 cards to trash
	//for (i=1; i<G.handCount[currentPlayer]; i++) {
	//	for (j=i+1; j<G.handCount[currentPlayer]; j++) {

	//		G.hand[currentPlayer][0] = steward;
	//		G.hand[currentPlayer][1] = copper;
	//		G.hand[currentPlayer][2] = duchy;
	//		G.hand[currentPlayer][3] = estate;
	//		G.hand[currentPlayer][4] = feast;

	//		// copy the game state to a test case
	//		memcpy(&testG, &G, sizeof(struct gameState));

	//		printf("starting cards: ");
	//		for (m=0; m<testG.handCount[currentPlayer]; m++) {
	//			printf("(%d)", testG.hand[currentPlayer][m]);
	//		}
	//		printf("; ");

	//		choice2 = j;
	//		choice3 = i;
	//		remove1 = testG.hand[currentPlayer][i];
	//		remove2 = testG.hand[currentPlayer][j];
	//		cardEffect(steward, choice1, choice2, choice3, &testG, handpos, &bonus);

	//		printf("removed: (%d)(%d); ", remove1, remove2);
	//		printf("ending cards: ");

	//		// tests that the removed cards are no longer in the player's hand
	//		for (m=0; m<testG.handCount[currentPlayer]; m++) {
	//			printf("(%d)", testG.hand[currentPlayer][m]);
	//			assert(testG.hand[currentPlayer][m] != remove1);
	//			assert(testG.hand[currentPlayer][m] != remove2);
	//		}
	//		printf(", expected: ");
	//		for (m=1; m<G.handCount[currentPlayer]; m++) {
	//			if (G.hand[currentPlayer][m] != G.hand[currentPlayer][i] && G.hand[currentPlayer][m] != G.hand[currentPlayer][j]) {
	//				printf("(%d)", G.hand[currentPlayer][m]);
	//			}
	//		}
	//		printf("\n");

	//		// tests for the appropriate number of remaining cards
	//		newCards = 0;
	//		xtraCoins = 0;
	//		discarded = 3;
	//		if (i==1 && j==2) {
	//			printf("hand count = %d, expected = %d\n", testG.handCount[currentPlayer], G.handCount[currentPlayer] + newCards - discarded);
	//			printf("deck count = %d, expected = %d\n", testG.deckCount[currentPlayer], G.deckCount[currentPlayer] - newCards + shuffledCards);
	//		}
	//		assert(testG.handCount[currentPlayer] == G.handCount[currentPlayer] + newCards - discarded);
	//		assert(testG.deckCount[currentPlayer] == G.deckCount[currentPlayer] - newCards + shuffledCards);
	//	}

	//}

}

