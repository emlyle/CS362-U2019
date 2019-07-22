/*
* randomtestcard3.c - This tests executeTributeCard function
*
*/

/*
* Include the following lines in your makefile:
*
* randomtestcard3: randomtestcard3.c dominion.o rngs.o
*      gcc -o randomtestcard3 -g  randomtestcard3.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

void addToSupply(int card, int maxCount, struct gameState* gs, int* supplyTotalPtr, int* supplyArray);
void testTributeCard(struct gameState* preStatePtr, int currentPlayer);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail();
void printPass();
int myAssert(int arg1, int arg2, int testPass);

int main() {

	int i, j, m, player, currentPlayer, numPlayers, randomCardPos, randomCard, kingdomCardsLen;
	int supplyCards[296]; //296 is the max possible supply count  
	int supplyTotal = 0;
	struct gameState preState;
	int selectedKingdomCards[10];
	int totalTests = 30;

	//Set up random number generator
	SelectStream(2);
	PutSeed(8);

	printf("\n\n>>>>>>>>>>>>>>Random Test 3 for Tribute Card<<<<<<<<<<<<<<\n");

	for (i = 0; i < totalTests; i++) {
		printf("\nRandom Test %d\n", i + 1);


		/*********************** Test Setup ***********************/

		//Note: randomNum = floor(Random() * numberOfValuesInTheRange) + startingValue; 

		//Initialize variables to valid random values 
		numPlayers = floor(Random() * 3) + 2; //[2,4]
		preState.numPlayers = numPlayers;
		//printf("numPlayers = %d\t", numPlayers); 

		currentPlayer = floor(Random() * numPlayers); //[0,1] or [0,2] or [0,3]
		//printf("currentPlayer = %d\n\n", currentPlayer); 

		//Randomly select 10 distinct kingdom cards 
		int kingdomCards[20] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };
		kingdomCardsLen = 20;
		for (j = 0; j < 10; j++) {
			int randPos = floor(Random() * kingdomCardsLen);
			selectedKingdomCards[j] = kingdomCards[randPos];
			kingdomCardsLen--;
			for (m = randPos; m < kingdomCardsLen; m++) {
				kingdomCards[m] = kingdomCards[m + 1];
			}
		}
		//TEST: Display randomly selected cards to verify during development 
		//printf("Selected Kingdom Cards: "); 
		//for (j = 0; j < 10; j++) {
		//	printf("%d\t", selectedKingdomCards[j]); 
		//}
		//printf("\n"); 

		//Set random but valid supply counts for each card type and add each supply card to an array of all supply cards
		supplyTotal = 0; //reset for current test
		if (numPlayers == 2) {

			addToSupply(estate, 8, &preState, &supplyTotal, supplyCards);
			//printf("supply count of estate = %d, supplyTotal = %d\n", preState.supplyCount[estate], supplyTotal); 

			addToSupply(duchy, 8, &preState, &supplyTotal, supplyCards);
			//printf("supply count of duchy = %d, supplyTotal = %d\n", preState.supplyCount[duchy], supplyTotal); 

			addToSupply(province, 8, &preState, &supplyTotal, supplyCards);
			//printf("supply count of province = %d, supplyTotal = %d\n", preState.supplyCount[province], supplyTotal); 

			addToSupply(copper, 46, &preState, &supplyTotal, supplyCards);
			//printf("supply count of copper = %d, supplyTotal = %d\n", preState.supplyCount[copper], supplyTotal); 

			addToSupply(curse, 10, &preState, &supplyTotal, supplyCards);
			//printf("supply count of curse = %d, supplyTotal = %d\n", preState.supplyCount[curse], supplyTotal); 

		}
		else {
			addToSupply(estate, 12, &preState, &supplyTotal, supplyCards);
			//printf("supply count of estate = %d, supplyTotal = %d\n", preState.supplyCount[estate], supplyTotal); 

			addToSupply(duchy, 12, &preState, &supplyTotal, supplyCards);
			//printf("supply count of duchy = %d, supplyTotal = %d\n", preState.supplyCount[duchy], supplyTotal); 

			addToSupply(province, 12, &preState, &supplyTotal, supplyCards);
			//printf("supply count of province = %d, supplyTotal = %d\n", preState.supplyCount[province], supplyTotal); 

			if (numPlayers == 3) {
				addToSupply(copper, 39, &preState, &supplyTotal, supplyCards);
				//printf("supply count of copper = %d, supplyTotal = %d\n", preState.supplyCount[copper], supplyTotal); 

				addToSupply(curse, 20, &preState, &supplyTotal, supplyCards);
				//printf("supply count of curse = %d, supplyTotal = %d\n", preState.supplyCount[curse], supplyTotal); 
			}
			else { //numPlayers == 4 
				addToSupply(copper, 32, &preState, &supplyTotal, supplyCards);
				//printf("supply count of copper = %d, supplyTotal = %d\n", preState.supplyCount[copper], supplyTotal); 

				addToSupply(curse, 30, &preState, &supplyTotal, supplyCards);
				//printf("supply count of curse = %d, supplyTotal = %d\n", preState.supplyCount[curse], supplyTotal); 
			}

		}
		addToSupply(silver, 40, &preState, &supplyTotal, supplyCards);
		//printf("supply count of silver = %d, supplyTotal = %d\n", preState.supplyCount[silver], supplyTotal); 

		addToSupply(gold, 30, &preState, &supplyTotal, supplyCards);
		//printf("supply count of gold = %d, supplyTotal = %d\n", preState.supplyCount[gold], supplyTotal); 

		//Set random but valid supply counts for the selected kingdom cards
		for (j = 0; j < 10; j++) {
			if (selectedKingdomCards[j] == gardens || selectedKingdomCards[j] == great_hall) {
				addToSupply(selectedKingdomCards[j], 12, &preState, &supplyTotal, supplyCards);
				//printf("supply count of kingdomCard %d = %d, supplyTotal = %d\n", selectedKingdomCards[j], preState.supplyCount[selectedKingdomCards[j]], supplyTotal); 
			}
			else {
				addToSupply(selectedKingdomCards[j], 10, &preState, &supplyTotal, supplyCards);
				//printf("supply count of kingdomCard %d = %d, supplyTotal = %d\n", selectedKingdomCards[j], preState.supplyCount[selectedKingdomCards[j]], supplyTotal); 
			}
		}
		//Set supply count for all other kingdom cards to -1 
		for (j = 0; j < kingdomCardsLen; j++) {
			preState.supplyCount[kingdomCards[j]] = -1;
		}

		//Testing: 
		//printf("supply card array:\n"); 
		//int t = 0; 
		//for (t = 0; t < supplyTotal; t++) {
		//	printf("Card %d: %d\n", t + 1, supplyCards[t]); 
		//}
		//printf("Total supply for current test game = %d\n", supplyTotal); 

		//Initialize all possible players hand counts, deck counts, discard pile counts, and the cards in each 
		for (j = 0; j < 4; j++) {
			preState.handCount[j] = 0;
			for (m = 0; m < 5; m++) {
				preState.hand[j][m] = -1;
			}
			preState.discardCount[j] = 0; 
			for (m = 0; m < 332; m++) { //332 is the most cards that could be in the game play between max supply and starting player decks
				preState.discard[j][m] = -1; 
			}
			preState.deckCount[j] = 0; 
			for (m = 0; m < 332; m++) { //332 is the most cards that could be in the game play between max supply and starting player decks
				preState.deck[j][m] = -1; 
			}
		}


		int totalGameCards = 0;
		int totalCardsRemaining = 332; 
		for (j = 0; j < numPlayers; j++) {
			preState.handCount[j] = floor(Random() * 6); //[0,5] cards in a player's hand
			totalGameCards += preState.handCount[j];
			//printf("Hand count for player %d: %d\n", j + 1, preState.handCount[j]); 
			totalCardsRemaining -= totalGameCards; 
			
			if (totalCardsRemaining < 0) totalCardsRemaining = 0; 
			preState.deckCount[j] = floor(Random() * totalCardsRemaining); //shrink total count of cards based on what possible values remain 
			totalGameCards += preState.deckCount[j]; 
			totalCardsRemaining -= totalGameCards;
			
			if (totalCardsRemaining < 0) totalCardsRemaining = 0;
			preState.discardCount[j] = floor(Random() * totalCardsRemaining); //shrink total count of cards based on what possible values remain
			totalGameCards += preState.discardCount[j]; 
			totalCardsRemaining -= totalGameCards;
		}


		int selectedSupplyPos[332]; //332 is max possible cards that could be in all player decks, discard piles, and hands
		for (j = 0; j < 332; j++) {
			selectedSupplyPos[j] = -1; 
		}
		int currSupplyPos = 0;
		//randomly choose hand cards from the supply array since these are the possible cards that might end up in a player's hand during the current game  
		//printf("\n\n******Start Random Test %d\n", i + 1);
		for (player = 0; player < numPlayers; player++) {
			int cardAlreadySelected; 
			int infLoop; 
			for (j = 0; j < preState.handCount[player]; j++) {
				cardAlreadySelected = 0;
				infLoop = 0;
				//printf("RandomCardPos selections:\n"); 
				do {
					//printf("In the loop...\n"); 
					randomCardPos = floor(Random() * supplyTotal);
					//printf("Random Selection: %d\n", randomCardPos); 
					for (m = 0; m < currSupplyPos; m++) {
						//printf("\tDoes %d = selectedSupplyPos[%d] of %d?", randomCardPos, m, selectedSupplyPos[m]); 
						if (randomCardPos == selectedSupplyPos[m]) {
							//printf("YES! "); 
							cardAlreadySelected = 1;
							//printf("Card already selected!\n"); 
							break;
						}
						else {
							//printf("NO! Check the next position\n");
							cardAlreadySelected = 0;
						}
					}
					infLoop++;
					if (infLoop >= 20) printf("Random Test %d: Infinite loop...exiting loop\n", i + 1);
				} while (cardAlreadySelected != 0 && infLoop < 20);
				//printf("LOOP EXITED!\n");  
				//printf("\n"); 
				randomCard = supplyCards[randomCardPos];
				preState.hand[player][j] = randomCard;
				selectedSupplyPos[currSupplyPos++] = randomCardPos;

			}

			//NEW: 
			for (j = 0; j < preState.deckCount[player]; j++) {
				cardAlreadySelected = 0;
				infLoop = 0;
				//printf("RandomCardPos selections:\n"); 
				do {
					//printf("In the loop...\n"); 
					randomCardPos = floor(Random() * supplyTotal);
					//printf("Random Selection: %d\n", randomCardPos); 
					for (m = 0; m < currSupplyPos; m++) {
						//printf("\tDoes %d = selectedSupplyPos[%d] of %d?", randomCardPos, m, selectedSupplyPos[m]); 
						if (randomCardPos == selectedSupplyPos[m]) {
							//printf("YES! "); 
							cardAlreadySelected = 1;
							//printf("Card already selected!\n"); 
							break;
						}
						else {
							//printf("NO! Check the next position\n");
							cardAlreadySelected = 0;
						}
					}
					infLoop++;
					if (infLoop >= 20) printf("Random Test %d: Infinite loop...exiting loop\n", i + 1);
				} while (cardAlreadySelected != 0 && infLoop < 20);
				//printf("LOOP EXITED!\n");  
				//printf("\n"); 
				randomCard = supplyCards[randomCardPos];
				preState.deck[player][j] = randomCard;
				selectedSupplyPos[currSupplyPos++] = randomCardPos;
			}

			for (j = 0; j < preState.discardCount[player]; j++) {
				cardAlreadySelected = 0;
				infLoop = 0;
				//printf("RandomCardPos selections:\n"); 
				do {
					//printf("In the loop...\n"); 
					randomCardPos = floor(Random() * supplyTotal);
					//printf("Random Selection: %d\n", randomCardPos); 
					for (m = 0; m < currSupplyPos; m++) {
						//printf("\tDoes %d = selectedSupplyPos[%d] of %d?", randomCardPos, m, selectedSupplyPos[m]); 
						if (randomCardPos == selectedSupplyPos[m]) {
							//printf("YES! "); 
							cardAlreadySelected = 1;
							//printf("Card already selected!\n"); 
							break;
						}
						else {
							//printf("NO! Check the next position\n");
							cardAlreadySelected = 0;
						}
					}
					infLoop++;
					if (infLoop >= 20) printf("Random Test %d: Infinite loop...exiting loop\n", i + 1);
				} while (cardAlreadySelected != 0 && infLoop < 20);
				//printf("LOOP EXITED!\n");  
				//printf("\n"); 
				randomCard = supplyCards[randomCardPos];
				preState.discard[player][j] = randomCard;
				selectedSupplyPos[currSupplyPos++] = randomCardPos;
			}
		}

		//Testing: 
		//for (j = 0; j < numPlayers; j++) {
			//printf("Player %d:\n", j + 1); 

			//Set current player's discard count and pile 
			//preState.discardCount[j] = floor(Random() * MAX_DECK);
			//Set the next position in the discard pile to -1 since this is where an estate card may be added
			//preState.discard[j][preState.discardCount[j]] = -1;

			//printf("\tDiscardCount = %d\n", preState.discardCount[j]); 
			//printf("\tEnd of discard pile = %d\n", preState.discard[j][preState.discardCount[j]]);

			//Testing: 
			//printf("\tHand: ");
			//for (m = 0; m < preState.handCount[j]; m++) {
			//	printf("%d\t", preState.hand[j][m]); 
			//}
			//printf("\n");
		//}

		preState.numBuys = 1;
		preState.numActions = 1; 

		preState.coins = 0; //reset coins 
							//Set current player's coins
		for (j = 0; j < 5; j++) {
			int currCard = preState.hand[currentPlayer][j];
			if (currCard == copper) {
				preState.coins += 1;
			}
			else if (currCard == silver) {
				preState.coins += 2;
			}
			else if (currCard == gold) {
				preState.coins += 3;
			}
		}
		//printf("Coins = %d\n", preState.coins); 


		/************************ Test Setup Complete ***********************/


		//Call function under test and verify results 
		testTributeCard(&preState, currentPlayer);

	}

	printf("\n>>>>>>>>>Random Test 3 for Tribute Card Completed<<<<<<<<<\n");

	return 0;

}

void testTributeCard(struct gameState* preStatePtr, int currentPlayer) {
	int i, diffFound;
	int result = -1;
	int testPass = 1;
	int nextPlayer = currentPlayer + 1;
	if (nextPlayer == preStatePtr->numPlayers) nextPlayer = 0;
	//printf("Next player: %d\n", nextPlayer); 

	//Copy the pre-game state (preState) to a post-test game state (postState)
	struct gameState postState;
	memcpy(&postState, preStatePtr, sizeof(struct gameState));

	//Execute the function: 	
	result = executeTributeCard(&postState, currentPlayer);

	//Testing (for me): 
	//printf("Hand count of preState = %d, hand count of postState = %d\n", preStatePtr->handCount[currentPlayer], postState.handCount[currentPlayer]); 


	//Verify the results: 
	//testPass = testCriteriaForAllMinionCards(result, &postState, preStatePtr, testPass);

	//Determine which tests to run: 
	/*
	if (choice1 == 1) {
		//printf("Choice1 == 1\n"); 
		testPass = testCriteriaForNotChoice2(&postState, preStatePtr, currentPlayer, testPass);
		//Verify 2 coins were gained
		printf("\tcoins = %d, expected = %d --> ", postState.coins, preStatePtr->coins + 2);
		testPass = myAssert(postState.coins, preStatePtr->coins + 2, testPass);
	}
	else if (choice2 == 1) {
		testPass = testCriteriaForChoice2(&postState, preStatePtr, currentPlayer, testPass);
		if (preStatePtr->handCount[nextPlayer] < 5) {
			//printf("choice2 == 1 but next player has less than 5 cards\n"); 
			testPass = verifyNoChangeToNextPlayer(&postState, preStatePtr, currentPlayer, testPass);
		}
		else { //next player had at least 5 cards in their hand
			   //printf("choice2 == 1 and next player has at least 5 cards\n");  
			   //Verify hand count of next player is now 4
			printf("\tnext player's handCount = %d, expected = %d --> ", postState.handCount[nextPlayer], 4);
			testPass = myAssert(postState.handCount[nextPlayer], 4, testPass);

			//Verify next player's hand has changed
			diffFound = 0;
			printf("\tnext player's hand: \n");
			for (i = 0; i < postState.handCount[nextPlayer]; i++) {
				printf("\t\tcard %d = %d, was %d\n", i + 1, postState.hand[nextPlayer][i], preStatePtr->hand[nextPlayer][i]);
				if (postState.hand[nextPlayer][i] != preStatePtr->hand[nextPlayer][i]) {
					diffFound = 1;
				}
			}
			if (!diffFound) {
				printf("\t\tNo differences found in hand --> FAIL\n");
				testPass = 0;
			}
			else printf("\t\tHand changed --> PASS\n");
		}
	}
	else { //both choice1 and choice2 equal zero
		   //printf("Both choice1 and choice2 == 0\n"); 
		   //Verify no coins gained
		printf("\tcoins = %d, expected = %d --> ", postState.coins, preStatePtr->coins);
		testPass = myAssert(postState.coins, preStatePtr->coins, testPass);
		testPass = testCriteriaForNotChoice2(&postState, preStatePtr, currentPlayer, testPass);
	}
	*/
	concludeTestCase(testPass, 1);

}


void addToSupply(int card, int maxCount, struct gameState* gs, int* supplyTotalPtr, int* supplyArrayPtr) {
	int i = 0;
	gs->supplyCount[card] = floor(Random() * maxCount + 1); //add 1 to include zero in the range
															//printf("supplyCount of %d = %d\n", card, gs->supplyCount[card]);  
															//printf("Max supply count of %d = %d\n", card, maxCount); 	
	supplyArrayPtr += *supplyTotalPtr; //Start at correct array position 
	for (i = 0; i < maxCount; i++) {
		*supplyArrayPtr = card;
		supplyArrayPtr++; //Increment the array address to move to the next array position  
		*supplyTotalPtr += 1; //Increment the supply total value
	}
	//printf("supplyTotalPtr = %d\n", *supplyTotalPtr); 
}




void concludeTestCase(int testPass, int testCaseNumber) {
	if (testPass) {
		printf("Random test PASSED!\n");
	}
	else {
		printf("Random test FAILED. See failure criteria listed above.\n");
	}
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


/*
int main() {
int i;
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
//Note: This test will fail due to bugs in the function

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

*/