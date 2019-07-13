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
TC2: Last two cards = treasure + action card, deck as is (probably not empty but double check)
	Add two coins
	Add two actions
TC3: Last two cards = 2 action cards  (empty the player's deck to get the code to work)
	Add two actions (will likely add 4 actions)
TC4: Other player has only one card (a treasure card)
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
	G.deck[currentPlayer + 1][G.deckCount[currentPlayer + 1] - 1] = 4; //Treasure card
	G.deck[currentPlayer + 1][G.deckCount[currentPlayer + 1] - 2] = 11;//Action card
	

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = executeTributeCard(&testG, currentPlayer);

	//For me: What is the state of the other player's deck - length and cards?
	printf("Deck count of other player = %d\nOther Player's Deck:\n", G.deckCount[currentPlayer + 1]);
	for (i = 0; i < G.deckCount[currentPlayer + 1]; i++) {
		printf("\tCard #%d: %d\n", (i + 1), G.deck[currentPlayer + 1][i]);
	}

	printf("\nAFTER TEST:\nDeck count of other player = %d\nOther Player's Deck:\n", testG.deckCount[currentPlayer + 1]);
	for (i = 0; i < testG.deckCount[currentPlayer + 1]; i++) {
		printf("\tCard #%d: %d\n", (i + 1), testG.deck[currentPlayer + 1][i]);
	}

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




	printf(">>>>> UNIT TEST 4 (%s card) COMPLETE <<<<<\n\n", TESTCARD);

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
	testPass = myAssert(testG->handCount[currentPlayer + 1], G->handCount[currentPlayer + 1] + 1, testPass);


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
