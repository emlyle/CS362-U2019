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
#include <math.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "baron"


void testBaronCard(struct gameState* preStatePtr, int choice1, int currentPlayer); 
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
			supplyCount[duchy] = [0,12]
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
			supplyCount[duchy] = [0,12]
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


void addToSupply(int card, int maxCount, struct gameState* gs, int* supplyTotalPtr, int* supplyArray);

int main() {
	
	int i, j, m, choice1, currentPlayer, numPlayers, randomCardPos, randomCard, kingdomCardsLen; 
	int supplyCards[296]; //296 is the max possible supply count  
	int supplyTotal = 0; 
	struct gameState preState; 
	int selectedKingdomCards[10]; 
	int totalTests = 30; 

	//Set up random number generator
	SelectStream(2);
	PutSeed(8);

	printf(">>>>>>>>>>>>>>Random Test 1 for Baron Card<<<<<<<<<<<<<<\n"); 

	for (i = 0; i < totalTests; i++) {
		printf("\nRandom Test %d\n", i + 1); 


		/*********************** Test Setup ***********************/

		//Note: randomNum = floor(Random() * numberOfValuesInTheRange) + startingValue; 

		//Initialize variables to valid random values 
		choice1 = floor(Random() * 2); //[0,1]
		//printf("choice1 = %d\t", choice1); 
		
		numPlayers = floor(Random() * 3) + 2; //[2,4]
		//printf("numPlayers = %d\t", numPlayers); 
		
		currentPlayer = floor(Random() * numPlayers); //[0,1] or [0,2] or [0,3]
		//printf("currentPlayer = %d\n\n", currentPlayer); 

		//Randomly select 10 distinct kingdom cards 
		int kingdomCards[20] = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}; 
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
			} else {
				addToSupply(selectedKingdomCards[j], 10, &preState, &supplyTotal, supplyCards); 
				//printf("supply count of kingdomCard %d = %d, supplyTotal = %d\n", selectedKingdomCards[j], preState.supplyCount[selectedKingdomCards[j]], supplyTotal); 
			}
		}
		//Set supply count for all other kingdom cards to -1 
		for (j = 0; j < kingdomCardsLen; j++) {
			preState.supplyCount[kingdomCards[j]] = -1; 
		}

		//Testing: 
		printf("supply card array:\n"); 
		int t = 0; 
		for (t = 0; t < supplyTotal; t++) {
			printf("Card %d: %d\n", t + 1, supplyCards[t]); 
		}
		printf("Total supply for current test game = %d\n", supplyTotal); 



		preState.handCount[currentPlayer] = 5; 
		int selectedSupplyPos[5] = {-1, -1, -1, -1, -1}; 
		//randomly choose hand cards from the supply array since these are the possible cards that might end up in a player's hand during the current game  
		//printf("\n\n******Start Random Test %d\n", i + 1); 
		for (j = 0; j < 5; j++) {
			int cardAlreadySelected = 0; 
			int infLoop = 0; 
			//printf("RandomCardPos selections:\n"); 
			do {
				//printf("In the loop...\n"); 
				randomCardPos = floor(Random() * supplyTotal);
				//printf("Random Selection: %d\n", randomCardPos); 
				for (m = 0; m < j; m++) {
					//printf("\tDoes %d = selectedSupplyPos[%d] of %d?", randomCardPos, m, selectedSupplyPos[m]); 
					if (randomCardPos == selectedSupplyPos[m]) {
						//printf("YES! "); 
						cardAlreadySelected = 1; 
						//printf("Card already selected!\n"); 
						break; 
					} else {
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
			preState.hand[currentPlayer][j] = randomCard; 
			selectedSupplyPos[j] = randomCardPos;   
		}
		//Testing: 
		//printf("Current player's hand: "); 
		int estates = 0; 
		for (j = 0; j < 5; j++) {
			//printf("%d\t", preState.hand[currentPlayer][j]); 
			if (preState.hand[currentPlayer][j] == estate) {
				//printf("Random Test %d: Estate card drawn! choice1 = %d\n", i + 1, choice1); 
				estates++; 
			}
		}
		if (choice1 == 1 && (estates > 1 || estates == 0)) {
			printf("\tThis test should fail\n"); 
		} else printf("\tThis test should pass\n"); 	
		
		//Set numBuys 
		preState.numBuys = 1; 		

		//Set current player's coins
		for (j = 0; j < 5; j++) {
			int currCard = preState.hand[currentPlayer][j]; 
			if (currCard == copper) {
				preState.coins += 1; 
			} else if (currCard == silver) {
				preState.coins += 2; 
			} else if (currCard == gold) {
				preState.coins += 3; 
			}
		}	
		//printf("Coins = %d\n", preState.coins); 
	
		//Set current player's discard count and pile 
		preState.discardCount[currentPlayer] = floor(Random() * MAX_DECK); 
		//Set the next position in the discard pile to -1 since this is where an estate card may be added
		preState.discard[currentPlayer][preState.discardCount[currentPlayer]] = -1; 
		//printf("discardCount = %d\n", preState.discardCount[currentPlayer]); 
		//printf("end of discard pile = %d\n", preState.discard[currentPlayer][preState.discardCount[currentPlayer]]); 

		//No need to set current player's deck - it is never used by executeBaronCard or any methods called by this function 

		/************************ Test Setup Complete ***********************/



		
		//Call function under test and verify results 
		testBaronCard(&preState, choice1, currentPlayer); 


	}


	printf("\n>>>>>>>>>Random Test 1 for Baron Card Completed<<<<<<<<<\n"); 

	return 0; 
}



void testBaronCard(struct gameState* preStatePtr, int choice1, int currentPlayer) {
	int i; 
	int result = -1; 
	int testPass = 1; 
	int estateCardPosition = -1;

	//Copy the pre-game state (preState) to a post-test game state (postState)
	struct gameState postState; 
	memcpy(&postState, preStatePtr, sizeof(struct gameState));

	//Execute the function: 	
	//Note: the third arg (handPos) is never actually used by executeBaronCard, so just pass in 0
	result = executeBaronCard(choice1, &postState, 0, currentPlayer);

	//Testing (for me): 
	//printf("Hand count of preState = %d, hand count of postState = %d\n", preStatePtr->handCount[currentPlayer], postState.handCount[currentPlayer]); 
	


	//Verify the results: 
	testPass = testForEveryBaronCard(&postState, preStatePtr, result, testPass);

	if (choice1 == 1) {
		for (i = 0; i < 5; i++) {
			if (preStatePtr->hand[currentPlayer][i] == estate) {
				estateCardPosition = i; 
				break; 
			}
		}
	}

	if (choice1 == 1 && estateCardPosition > -1) {	
		//Call if player will discard an estate card - find first estate card pos in hand
		testPass = testDiscardEstateCard(&postState, preStatePtr, currentPlayer, estateCardPosition, testPass); 
	} else {
		//Call if player will gain an estate card
		testPass = testGainEstateCard(&postState, preStatePtr, currentPlayer, estateCardPosition, testPass);
	}

	concludeTestCase(testPass, 1);

}


void addToSupply(int card, int maxCount, struct gameState* gs, int* supplyTotalPtr, int* supplyArrayPtr) {
	int i = 0; 
	gs->supplyCount[card] = floor(Random() * maxCount + 1); //add 1 to include zero in the range
	//printf("supplyCount of %d = %d\n", card, gs->supplyCount[card]);  
	
	supplyArrayPtr += *supplyTotalPtr; //Start at correct array position 
	for (i = 0; i < maxCount; i++) {
		*supplyArrayPtr = card;
		supplyArrayPtr++; //Increment the array address to move to the next array position  
		*supplyTotalPtr += 1; //Increment the supply total value
	}
	printf("supplyTotalPtr = %d\n", *supplyTotalPtr); 
}



/*
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



	// ----------------------------- TEST CASE 2: choice1 = 0 (gain an estate card) ---------------------------------
	printf("TEST CASE 2: choice1 = 0 (Gain an estate card)\n");

	currentPlayer = 0; 	

	// Copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0; //gain estate card



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


}
*/


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
	//printf("In testDiscardEstateCard...\n"); 

	//For discarding an estate card: 
	//Verify 4 coins have been gained 
	printf("\tcoins = %d, expected = %d --> ", testG->coins, G->coins + 4);
	testPass = myAssert(testG->coins, G->coins + 4, testPass);

	//Verify estate card is in current player's discard pile 
	printf("\ttop of discard pile = %d, expected = 1 --> ", testG->discard[currentPlayer][testG->discardCount[currentPlayer] - 1]);
	if (testG->discard[currentPlayer][testG->discardCount[currentPlayer] - 1] != estate) {
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
	printf("\tprevious last card in hand = %d, expected = -1 --> ", testG->hand[currentPlayer][testG->handCount[currentPlayer] + 1]);
	testPass = myAssert(testG->hand[currentPlayer][testG->handCount[currentPlayer] + 1], -1, testPass);

	//Verify estate card was removed and next card in hand was moved up into that position
	printf("\thand position of estate card = %d, expected = %d --> ", testG->hand[currentPlayer][estateCardPosition], G->hand[currentPlayer][estateCardPosition + 1]);
	testPass = myAssert(testG->hand[currentPlayer][estateCardPosition], G->hand[currentPlayer][estateCardPosition + 1], testPass);

	//Verify that hand count was decremented 
	printf("\thandCount = %d, expected = %d --> ", testG->handCount[currentPlayer], G->handCount[currentPlayer] - 1);
	testPass = myAssert(testG->handCount[currentPlayer], G->handCount[currentPlayer] - 1, testPass); 

	return testPass;
}


int testGainEstateCard(struct gameState *testG, struct gameState *G, int currentPlayer, int estateCardPosition, int testPass) {
	//printf("In testGainEstateCard...\n"); 

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
		printf("Random Test PASSED!\n");
	}
	else {
		printf("Random Test  FAILED. See failure criteria listed above.\n");
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

