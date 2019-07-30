/*
* randomtestcard3.c - This tests tributeCase function
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

	int i, j, m, s, player, currentPlayer, numPlayers, randomCardPos, randomCard, kingdomCardsLen;
	int supplyCards[296]; //296 is the max possible supply count  
	int startingSupply[296]; 
	int supplyTotal = 0;
	struct gameState preState;
	int selectedKingdomCards[10];
	int totalTests = 1000;

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
		preState.whoseTurn = currentPlayer; 
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
		s = 0; 
		if (numPlayers == 2) {

			addToSupply(estate, 8, &preState, &supplyTotal, supplyCards);
			for (j = 0; j < 8; j++) {
				startingSupply[s] = estate; 
				s++; 
			} 
			//printf("supply count of estate = %d, supplyTotal = %d\n", preState.supplyCount[estate], supplyTotal); 

			addToSupply(duchy, 8, &preState, &supplyTotal, supplyCards);
			for (j = 0; j < 8; j++) {
				startingSupply[s] = duchy; 
				s++; 
			} 
			//printf("supply count of duchy = %d, supplyTotal = %d\n", preState.supplyCount[duchy], supplyTotal); 

			addToSupply(province, 8, &preState, &supplyTotal, supplyCards);
			//printf("supply count of province = %d, supplyTotal = %d\n", preState.supplyCount[province], supplyTotal); 
			for (j = 0; j < 8; j++) {
				startingSupply[s] = province; 
				s++; 
			} 

			addToSupply(copper, 46, &preState, &supplyTotal, supplyCards);
			//printf("supply count of copper = %d, supplyTotal = %d\n", preState.supplyCount[copper], supplyTotal); 
			for (j = 0; j < 46; j++) {
				startingSupply[s] = copper; 
				s++; 
			} 

			addToSupply(curse, 10, &preState, &supplyTotal, supplyCards);
			//printf("supply count of curse = %d, supplyTotal = %d\n", preState.supplyCount[curse], supplyTotal); 
			for (j = 0; j < 10; j++) {
				startingSupply[s] = curse; 
				s++; 
			} 

		}
		else {
			addToSupply(estate, 12, &preState, &supplyTotal, supplyCards);
			for (j = 0; j < 12; j++) {
				startingSupply[s] = estate; 
				s++; 
			} 
			//printf("supply count of estate = %d, supplyTotal = %d\n", preState.supplyCount[estate], supplyTotal); 

			addToSupply(duchy, 12, &preState, &supplyTotal, supplyCards);
			for (j = 0; j < 12; j++) {
				startingSupply[s] = duchy; 
				s++; 
			} 
			//printf("supply count of duchy = %d, supplyTotal = %d\n", preState.supplyCount[duchy], supplyTotal); 

			addToSupply(province, 12, &preState, &supplyTotal, supplyCards);
			for (j = 0; j < 12; j++) {
				startingSupply[s] = province; 
				s++; 
			} 
			//printf("supply count of province = %d, supplyTotal = %d\n", preState.supplyCount[province], supplyTotal); 

			if (numPlayers == 3) {
				addToSupply(copper, 39, &preState, &supplyTotal, supplyCards);
				for (j = 0; j < 39; j++) {
					startingSupply[s] = copper; 
					s++; 
				} 
				//printf("supply count of copper = %d, supplyTotal = %d\n", preState.supplyCount[copper], supplyTotal); 

				addToSupply(curse, 20, &preState, &supplyTotal, supplyCards);
				for (j = 0; j < 20; j++) {
					startingSupply[s] = curse; 
					s++; 
				} 
				//printf("supply count of curse = %d, supplyTotal = %d\n", preState.supplyCount[curse], supplyTotal); 
			}
			else { //numPlayers == 4 
				addToSupply(copper, 32, &preState, &supplyTotal, supplyCards);
				for (j = 0; j < 32; j++) {
					startingSupply[s] = copper; 
					s++; 
				} 
				//printf("supply count of copper = %d, supplyTotal = %d\n", preState.supplyCount[copper], supplyTotal); 

				addToSupply(curse, 30, &preState, &supplyTotal, supplyCards);
				for (j = 0; j < 30; j++) {
					startingSupply[s] = curse; 
					s++; 
				} 
				//printf("supply count of curse = %d, supplyTotal = %d\n", preState.supplyCount[curse], supplyTotal); 
			}

		}
		addToSupply(silver, 40, &preState, &supplyTotal, supplyCards);
		for (j = 0; j < 40; j++) {
			startingSupply[s] = silver; 
			s++; 
		} 
		//printf("supply count of silver = %d, supplyTotal = %d\n", preState.supplyCount[silver], supplyTotal); 

		addToSupply(gold, 30, &preState, &supplyTotal, supplyCards);
		for (j = 0; j < 30; j++) {
			startingSupply[s] = gold; 
			s++; 
		} 
		//printf("supply count of gold = %d, supplyTotal = %d\n", preState.supplyCount[gold], supplyTotal); 

		//Set random but valid supply counts for the selected kingdom cards
		for (j = 0; j < 10; j++) {
			if (selectedKingdomCards[j] == gardens || selectedKingdomCards[j] == great_hall) {
				addToSupply(selectedKingdomCards[j], 12, &preState, &supplyTotal, supplyCards);
				for (j = 0; j < 12; j++) {
					startingSupply[s] = selectedKingdomCards[j]; 
					s++; 
				} 
				//printf("supply count of kingdomCard %d = %d, supplyTotal = %d\n", selectedKingdomCards[j], preState.supplyCount[selectedKingdomCards[j]], supplyTotal); 
			}
			else {
				addToSupply(selectedKingdomCards[j], 10, &preState, &supplyTotal, supplyCards);
				for (j = 0; j < 10; j++) {
					startingSupply[s] = selectedKingdomCards[j]; 
					s++; 
				} 
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
			preState.deckCount[j] = floor(Random() * 20); //shrink total count of cards based on what possible values remain 
			totalGameCards += preState.deckCount[j]; 
			totalCardsRemaining -= totalGameCards;
			
			if (totalCardsRemaining < 0) totalCardsRemaining = 0;
			preState.discardCount[j] = floor(Random() * 20); //shrink total count of cards based on what possible values remain
			totalGameCards += preState.discardCount[j]; 
			totalCardsRemaining -= totalGameCards;

			//printf("Player %d: handCount = %d, deckCount = %d, discardCount = %d\n", j + 1, preState.handCount[j], preState.deckCount[j], preState.discardCount[j]); 
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
					randomCardPos = floor(Random() * s);
					//printf("Random Selection: %d\n", randomCardPos); 
					for (m = 0; m < currSupplyPos; m++) {
						//printf("\tDoes %d = selectedSupplyPos[%d] of %d? ", randomCardPos, m, selectedSupplyPos[m]); 
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
					if (infLoop >= 40) printf("Infinite loop...exiting hand loop\n");
				} while (cardAlreadySelected != 0 && infLoop < 40);
				//printf("LOOP EXITED!\n");  
				//printf("\n"); 
				randomCard = startingSupply[randomCardPos];
				preState.hand[player][j] = randomCard;
				selectedSupplyPos[currSupplyPos] = randomCardPos;
				currSupplyPos++; 

			}

			for (j = 0; j < preState.deckCount[player]; j++) {
				cardAlreadySelected = 0;
				infLoop = 0;
				//printf("RandomCardPos selections:\n"); 
				do {
					//printf("In the loop...\n"); 
					randomCardPos = floor(Random() * s);
					//printf("Random Selection: %d\n", randomCardPos); 
					for (m = 0; m < currSupplyPos; m++) {
						//printf("\tDoes %d = selectedSupplyPos[%d] of %d? ", randomCardPos, m, selectedSupplyPos[m]); 
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
					if (infLoop >= 40) printf("Infinite loop...exiting deck loop\n");
				} while (cardAlreadySelected != 0 && infLoop < 40);
				//printf("LOOP EXITED!\n");  
				//printf("\n"); 
				randomCard = startingSupply[randomCardPos];
				preState.deck[player][j] = randomCard;
				selectedSupplyPos[currSupplyPos] = randomCardPos;
				currSupplyPos++; 
			}

			for (j = 0; j < preState.discardCount[player]; j++) {
				cardAlreadySelected = 0;
				infLoop = 0;
				//printf("RandomCardPos selections:\n"); 
				do {
					//printf("In the loop...\n"); 
					randomCardPos = floor(Random() * s);
					//printf("Random Selection: %d\n", randomCardPos); 
					for (m = 0; m < currSupplyPos; m++) {
						//printf("\tDoes %d = selectedSupplyPos[%d] of %d? ", randomCardPos, m, selectedSupplyPos[m]); 
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
					if (infLoop >= 40) printf("Infinite loop...exiting discard loop\n");
				} while (cardAlreadySelected != 0 && infLoop < 40);
				//printf("LOOP EXITED!\n");  
				//printf("\n"); 
				randomCard = startingSupply[randomCardPos];
				preState.discard[player][j] = randomCard;
				selectedSupplyPos[currSupplyPos] = randomCardPos;
				currSupplyPos++; 
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

	printf("\n>>>>>>>>>Random Test 3 for Tribute Card Completed<<<<<<<<<\n\n\n");

	return 0;

}


void testTributeCard(struct gameState* preStatePtr, int currentPlayer) {
	int i, j, foundCards;
	int result = -1;
	int testPass = 1;
	int nextPlayer = currentPlayer + 1;
	if (nextPlayer == preStatePtr->numPlayers) nextPlayer = 0;
	//printf("Next player: %d\n", nextPlayer); 

	//Copy the pre-game state (preState) to a post-test game state (postState)
	struct gameState postState;
	memcpy(&postState, preStatePtr, sizeof(struct gameState));

	//Execute the function: 	
	result = tributeCase(&postState, currentPlayer, nextPlayer);

	//Verify return value = 0
	printf("\treturn value = %d, expected = 0 -->", result);
	testPass = myAssert(result, 0, testPass);


	//Step 1: find out which 1 or 2 cards should have been used for tribute cards 
	int expectedTributeCards[2] = { -1, -1 }; 
	int expectedNumTributeCards = 0; 
	int startingDeckCount = preStatePtr->deckCount[nextPlayer];
	int startingDiscardCount = preStatePtr->discardCount[nextPlayer];
	int startingDeckAndDiscardCount = startingDeckCount + startingDiscardCount;
	int endingDeckAndDiscardCount = postState.deckCount[nextPlayer] + postState.discardCount[nextPlayer]; 

	//If the next player's deck had at least 2 cards - get last two in array for tribute cards
	if (startingDeckCount >= 2) {
		expectedTributeCards[0] = preStatePtr->deck[nextPlayer][startingDeckCount - 1];
		expectedTributeCards[1] = preStatePtr->deck[nextPlayer][startingDeckCount - 2];
	}
	//If the next player's deck had only 1 card and discard had 0 cards - get this card for tribute card
	else if (startingDeckCount == 1 && startingDiscardCount == 1) {
		expectedTributeCards[0] = preStatePtr->deck[nextPlayer][0];
	}
	//If the next player's deck had 0 cards and discard had 1 card - get this card for tribute card 
	else if (startingDeckCount == 0 && startingDiscardCount == 1) {
		expectedTributeCards[0] = preStatePtr->discard[nextPlayer][0];
	}
	//If the next player's deck had 0 cards and discard had exactly 2 cards - get these two cards for tribute cards
	else if (startingDeckCount == 0 && startingDiscardCount == 2) {
		expectedTributeCards[0] = preStatePtr->discard[nextPlayer][1];
		expectedTributeCards[1] = preStatePtr->discard[nextPlayer][0];
	}
	//If the next player's deck had 1 card and discard had 1 card - get these two cards for tribute cards 
	else if (startingDeckCount == 1 && startingDiscardCount == 1) {
		expectedTributeCards[0] = preStatePtr->deck[nextPlayer][0];
		expectedTributeCards[1] = preStatePtr->discard[nextPlayer][0];
	}
	//If the next player's deck had 0 cards and discard had more than 2 cards - find out which two cards are missing and these are the tribute cards 
	else if (startingDeckCount == 0 && startingDiscardCount > 2) {
		//Compare discard cards before with deck cards after to see which 2 are missing
		//If 2 cards are not missing from postState, then the test fails 
		for (i = 0; i < postState.deckCount[nextPlayer]; i++) {
			foundCards = 0; //Each card from postState should be found in preState (if it can't be found, that means it was added during the tributeCase function, which should not happen)
			for (j = 0; j < preStatePtr->discardCount[nextPlayer]; j++) {
				if (postState.deck[nextPlayer][i] == preStatePtr->discard[nextPlayer][j]) {
					preStatePtr->discard[nextPlayer][j] = -1; //NOTE: We are done with any verification of the preState discard pile after this so that's why I'm changing values to -1
					foundCards = 1; 
					break; 
				}
			}
			if (foundCards == 0) {
				printf("\tA new card was found in next player's deck --> FAIL\n"); 
			}
		}
		//Check that the missing cards match the expected tribute cards 
		foundCards = 0; 
		for (i = 0; i < preStatePtr->discardCount[nextPlayer]; i++) {
			if (preStatePtr->discard[nextPlayer][i] != -1) {
				if (preStatePtr->discard[nextPlayer][i] == expectedTributeCards[0]
					|| preStatePtr->discard[nextPlayer][i] == expectedTributeCards[1]) {
					foundCards++; 
				}
			}
		}
		printf("\tDiscarded tribute cards = %d, expected = %d --> ", foundCards, 2);
		testPass = myAssert(foundCards, 2, testPass);
	}
	//If the next player's deck had 1 card and discard had more than one card - find out which two cards are misisng and these are the tribute cards 
	else if (startingDeckCount == 1 && startingDiscardCount >= 2) {
		//Compare deck card + discard cards with deck cards after to see which 2 are missing 
		//If 2 cards are not missing from postState, then the test fails 
		for (i = 0; i < postState.deckCount[nextPlayer]; i++) {
			foundCards = 0; //Each card from postState should be found in preState (if it can't be found, that means it was added during the tributeCase function, which should not happen)
			//Check the preState deck first 
			if (postState.deck[nextPlayer][i] == preStatePtr->deck[nextPlayer][0]) {
				preStatePtr->deck[nextPlayer][0] = -1; //NOTE: We are done with any verification of the preState discard pile after this so that's why I'm changing values to -1
				foundCards = 1;
			}
			else {
				//Then check the preState discard pile
				for (j = 0; j < preStatePtr->discardCount[nextPlayer]; j++) {
					if (postState.deck[nextPlayer][i] == preStatePtr->discard[nextPlayer][j]) {
						preStatePtr->discard[nextPlayer][j] = -1; //NOTE: We are done with any verification of the preState discard pile after this so that's why I'm changing values to -1
						foundCards = 1;
						break;
					}
				}
			}
			if (foundCards == 0) {
				printf("\tA new card was found in next player's deck --> FAIL\n");
			}
		}
		//Check that the missing cards match the expected tribute cards 
		foundCards = 0;
		for (i = 0; i < preStatePtr->discardCount[nextPlayer]; i++) {
			if (preStatePtr->discard[nextPlayer][i] != -1) {
				if (preStatePtr->discard[nextPlayer][i] == expectedTributeCards[0]
					|| preStatePtr->discard[nextPlayer][i] == expectedTributeCards[1]) {
					foundCards++;
				}
			}
		}
		printf("\tDiscarded tribute cards = %d, expected = %d --> ", foundCards, 2);
		testPass = myAssert(foundCards, 2, testPass);
	}
	//Else the next player had 0 cards total, so tribute cards stay -1 since empty 




	//Step 2: Verify that the deck and discard are the expected length after removing the tribute cards 
	if (expectedTributeCards[0] != -1 && expectedTributeCards[1] != -1) expectedNumTributeCards = 2; 
	else if (expectedTributeCards[0] != -1 && expectedTributeCards[1] == -1) expectedNumTributeCards = 1; 
	else if (expectedTributeCards[0] == -1 && expectedTributeCards[1] != -1) expectedNumTributeCards = 1;
	//else 0 tribute cards
	//Has deck + discard count decreased by number of tribute cards found?
	printf("\tDiscard and deck count = %d, expected = %d --> ", endingDeckAndDiscardCount, startingDeckAndDiscardCount - expectedNumTributeCards);
	testPass = myAssert(endingDeckAndDiscardCount, startingDeckAndDiscardCount - expectedNumTributeCards, testPass);
	



	//Step 3: Verify (based on the two cards) if the current player has the gains expected
	//If duplicate tribute cards, remove one since it should not have been played 
	if (expectedTributeCards[0] == expectedTributeCards[1] && expectedTributeCards[0] != -1) {
		expectedTributeCards[1] = -1; 
	}

	//Check for treasure card
	int expectedCoins = 0; 
	if (expectedTributeCards[0] == copper || expectedTributeCards[0] == silver || expectedTributeCards[0] == gold
		|| expectedTributeCards[1] == copper || expectedTributeCards[1] == silver || expectedTributeCards[1] == gold) {	//+2 coins for a treasure card
		//Verify two coins added - for the treasure card
		expectedCoins = preStatePtr->coins + 2; 
	}
	else {
		//Neither tribute card was a treasure card, so verify no coins added
		expectedCoins = preStatePtr->coins; 
	}
	printf("\tcoins = %d, expected = %d --> ", postState.coins, expectedCoins);
	testPass = myAssert(postState.coins, expectedCoins, testPass);

	//Check for action card
	int expectedActions = 0; 
	//Action Cards: 
	/*
	   adventurer = 7,
		council_room,
		feast = 9
		mine = 11
		remodel, 
		smithy,
		village,
		baron = 15
		minion = 17
		steward,
		tribute,
		ambassador, 
		cutpurse,
		embargo, 
		outpost,
		salvager, 
		sea_hag,
		treasure_map = 26
	*/
	if ((expectedTributeCards[0] >= 7 && expectedTributeCards[0] <= 9) || (expectedTributeCards[0] >= 11 && expectedTributeCards[0] <= 15) || (expectedTributeCards[0] >= 17 && expectedTributeCards[0] <= 26)
		|| (expectedTributeCards[1] >= 7 && expectedTributeCards[1] <= 9) || (expectedTributeCards[1] >= 11 && expectedTributeCards[1] <= 15) || (expectedTributeCards[1] >= 17 && expectedTributeCards[1] <= 26)) {
		//Verify two actions added 
		expectedActions = preStatePtr->numActions + 2;
	} 
	else {
		//Verify no actions added
		expectedActions = preStatePtr->numActions; 
	}
	printf("\tactions = %d, expected = %d --> ", postState.numActions, expectedActions);
	testPass = myAssert(postState.numActions, expectedActions, testPass);

	//Check for victory cards
	int expectedHandCount = 0; 
	if (expectedTributeCards[0] == estate || expectedTributeCards[0] == duchy || expectedTributeCards[0] == province
		|| expectedTributeCards[0] == gardens || expectedTributeCards[0] == great_hall
		|| expectedTributeCards[1] == estate || expectedTributeCards[1] == duchy || expectedTributeCards[1] == province
		|| expectedTributeCards[1] == gardens || expectedTributeCards[1] == great_hall) {
		//Verify two cards added to current player's hand
		expectedHandCount = preStatePtr->handCount[currentPlayer] + 2;
	}
	else {
		//Verify no cards added to current player's hand
		expectedHandCount = preStatePtr->handCount[currentPlayer];
	}
	printf("\tcurrent player's hand count = %d, expected = %d --> ", postState.handCount[currentPlayer], expectedHandCount);
	testPass = myAssert(postState.handCount[currentPlayer], expectedHandCount, testPass);


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

