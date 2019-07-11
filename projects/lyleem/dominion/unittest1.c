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
//Test Case 3: choice 1 = 0 (discard estate card but no estate card in deck)

//My biggest dilemma is how to initialize all these variables that must be passed as args to executeBaronCard() so use below as model 


int main() {
    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {baron, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};				//***WHERE does the estate card go????????????????????????????????????????????

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- UNIT TEST 1: Testing %s card ----------------\n", TESTCARD);

	// ----------- TEST CASE 1: choice1 = 0 (discard an estate card) --------------
	printf("TEST CASE 1: choice1 = 1 (discard an estate card, current player has an estate card to discard)\n");

	int currentPlayer = 0; 
	//Make sure the hand of this player contains an estate: 

	//Just for my own curiosity: What is player 0's hand count and what cards are in their hand????
	//printf("Current Player (0): Hand\n"); 
	//printf("Current Player (0): %d cards in hand\n", G.handCount[thisPlayer]); 
	//int x = 0; 
	//for (x = 0; x < G.handCount[thisPlayer]; x++) {
	//	printf("Card #%d: %d\n", (x + 1), G.hand[thisPlayer][x]);
	//}
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
	//testG.coins should be 4 greater than G.coins (state->coins += 4) if an estate is discarded 
	if (testG.coins != (G.coins + 4)) {
		printf("Error - coin count is incorrect: test coins = %d, original coins = %d\n", testG.coins, G.coins);
		testPass = 0;
	}
	//Verify estate card is in current player's discard pile 
	if (testG.discard[currentPlayer][testG.discardCount[currentPlayer]] == testG.hand[currentPlayer][2]) { //TODO: SET p to whereever estate was?
		printf("Error - estate card was not discarded properly\n");
		testPass = 0;
	}
	//Verify discard count was updated: testG.discardCount[currentPlayer] should be 1 greater than G.discardCount[currentPlayer];
	if (testG.discardCount[currentPlayer] != (G.discardCount[currentPlayer] + 1)) {
		printf("Error - discard count is incorrect\n");
		testPass = 0;
	}
	//Verify current player's last card is now -1 (since they should have one card less) 
	if (testG.hand[currentPlayer][testG.handCount[currentPlayer]] != -1) {
		printf("Error - estate card was not correctly removed from current player's hand\n");

		//printf("\t\tcurrentPlayer's 'last' card: %d\n", testG.hand[currentPlayer][testG.handCount[currentPlayer]]); 
		//printf("\t\tcurrentPlayer's hand count: %d\n", testG.handCount[currentPlayer]); 
		//Just for my own curiosity: What is player 0's hand count and what cards are in their hand????
		//printf("Current Test Player (0): Hand\n"); 
		//printf("Current Test Player (0): %d cards in hand\n", testG.handCount[currentPlayer]); 
		//int x = 0; 
		//for (x = 0; x < G.handCount[currentPlayer]; x++) {
		//	printf("Test Card #%d: %d\n", (x + 1), testG.hand[currentPlayer][x]);
		//	printf("Reg Card  #%d: %d\n", (x + 1), G.hand[currentPlayer][x]); 
		//}

		testPass = 0;
	}
	//testG.handCount[currentPlayer] should be 1 less than G.handCount[currentPlayer];
	if (testG.handCount[currentPlayer] != (G.handCount[currentPlayer] - 1)) {
		printf("Error - hand count is incorrect\n");
		testPass = 0;
	}

	if (testPass) {
		printf("TEST CASE 1 PASSED!\n"); 
	}
	else {
		printf("TEST CASE 1 FAILED. See failure criteria listed above.\n");
	}

	//If an estate is discarded (choice1 == 0)
		//testG.coins should be 4 greater than G.coins (state->coins += 4) if an estate is discarded 
		//testG.discard[currentPlayer][testG.discardCount[currentPlayer]] == testG.hand[currentPlayer][p]; //SET p to whereever estate was?
		//testG.discardCount[currentPlayer] should be 1 greater than G.discardCount[currentPlayer];
		//testG.hand[currentPlayer][testG.handCount[currentPlayer]] == -1;
		//testG.handCount[currentPlayer] should be 1 less than G.handCount[currentPlayer];
	//If an estate is gained (choice1 == 1)
		//TEST CASE: Supply Count > 0
			//Check if testG.hand[currentPlayer][i] contains 1 more estate card than G.hand[currentPlayer][i] (iterate thru them) --> gainCard(estate, state, 0, currentPlayer); //Add an estate card to the current player's hand
			//Check if testG.supplyCount[estate] is 1 less than G.supplyCount[estate] --> state->supplyCount[estate]--;//Decrement estates from supply
		//TEST CASE: Supply Count == 0
			//Check if game is over??? --> isGameOver(state);

	//newCards = 2;
	//xtraCoins = 0;
	//printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	//printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	//printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards);
	//assert(testG.coins == G.coins + xtraCoins);
	
	printf("END TEST CASE 1\n"); 




	// ----------- TEST 2: choice1 = 2 = +2 coins --------------
	//printf("TEST 2: choice1 = 2 = +2 coins\n");

	// copy the game state to a test case
	//memcpy(&testG, &G, sizeof(struct gameState));
	//choice1 = 2;
	//cardEffect(steward, choice1, choice2, choice3, &testG, handpos, &bonus);

	//newCards = 0;
	//xtraCoins = 2;
	//printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	//printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	//printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards);
	//assert(testG.coins == G.coins + xtraCoins);

	// ----------- TEST 3: choice1 = 3 = trash two cards --------------

	//printf("TEST 3: choice1 = 3 = trash two cards\n");
	//choice1 = 3;

	// cycle through each eligible combination of 2 cards to trash
	//for (i=1; i<G.handCount[thisPlayer]; i++) {
	//	for (j=i+1; j<G.handCount[thisPlayer]; j++) {

	//		G.hand[thisPlayer][0] = steward;
	//		G.hand[thisPlayer][1] = copper;
	//		G.hand[thisPlayer][2] = duchy;
	//		G.hand[thisPlayer][3] = estate;
	//		G.hand[thisPlayer][4] = feast;

	//		// copy the game state to a test case
	//		memcpy(&testG, &G, sizeof(struct gameState));

	//		printf("starting cards: ");
	//		for (m=0; m<testG.handCount[thisPlayer]; m++) {
	//			printf("(%d)", testG.hand[thisPlayer][m]);
	//		}
	//		printf("; ");

	//		choice2 = j;
	//		choice3 = i;
	//		remove1 = testG.hand[thisPlayer][i];
	//		remove2 = testG.hand[thisPlayer][j];
	//		cardEffect(steward, choice1, choice2, choice3, &testG, handpos, &bonus);

	//		printf("removed: (%d)(%d); ", remove1, remove2);
	//		printf("ending cards: ");

	//		// tests that the removed cards are no longer in the player's hand
	//		for (m=0; m<testG.handCount[thisPlayer]; m++) {
	//			printf("(%d)", testG.hand[thisPlayer][m]);
	//			assert(testG.hand[thisPlayer][m] != remove1);
	//			assert(testG.hand[thisPlayer][m] != remove2);
	//		}
	//		printf(", expected: ");
	//		for (m=1; m<G.handCount[thisPlayer]; m++) {
	//			if (G.hand[thisPlayer][m] != G.hand[thisPlayer][i] && G.hand[thisPlayer][m] != G.hand[thisPlayer][j]) {
	//				printf("(%d)", G.hand[thisPlayer][m]);
	//			}
	//		}
	//		printf("\n");

	//		// tests for the appropriate number of remaining cards
	//		newCards = 0;
	//		xtraCoins = 0;
	//		discarded = 3;
	//		if (i==1 && j==2) {
	//			printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	//			printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	//		}
	//		assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
	//		assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards);
	//	}

	//}

	printf("\n >>>>> Unit Test 1 Complete: %s card <<<<<\n\n", TESTCARD);


	return 0;
}

