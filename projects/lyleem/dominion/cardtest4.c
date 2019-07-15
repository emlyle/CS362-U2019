/*
 * cardtest4.c - This tests the getWinners function
 *
 */


 /*
 Victory Cards: (Counted from deck, hand, and discard pile)
	 estate +1
	 duchy +3
	 province +6
	 gardens +((deckCount + handCount + discardCount) / 10)
	 great_hall +1
	 curse cards -1
	 players who haven't gone yet this round (greater than the currentPlayer position) get +1
		(BUG: it seems like the logic for this in getWinners may be incorrect so this condition may cause a failure)
 */



#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "getWinners"


int verifyWinner(int returnValue, int* playersPtr, int* playersControlPtr, int numPlayers, int winnerPos, int winnerPos2);
void concludeTestCase(int testPass, int testCaseNumber);
int printFail(); 
void printPass(); 
int myAssert(int arg1, int arg2, int testPass); 


int main() {
	int i, j; 
	int seed = 1000;
	int numPlayers, winnerPos, currentPlayer, cardCount;
	int winnerPos = -1; 
	int currentPlayer = 0; 
	int testPass = 1; 
	int result = -2; 

	struct gameState G, testG;
	int k[10] = {minion, embargo, village, baron, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- CARD TEST 4: Testing %s Function ----------------\n", TESTFUNCTION);

	// ---------------------------------------- TEST CASE 1 -------------------------------------------
	printf("TEST CASE 1: 3 players, current player is first player --> second player should win\n");

	currentPlayer = 0; 
	winnerPos = 1; 
	numPlayers = 3; 
	cardCount = 4; 
	//Set players' cards to estates and duchies
	for (i = 0; i < numPlayers; i++) {
		G.deckCount[i] = cardCount; 
		G.discardCount[i] = cardCount;
		G.handCount[i] = cardCount;
		for (j = 0; j < cardCount; j++) {
			G.deck[i][j] = estate; 
			G.hand[i][j] = duchy; 
			G.discard[i][j] = estate; 
		}
	}
	//Set winner's cards to higher total values (winner)
	for (i = 0; i < cardCount; i++) {
		G.deck[winnerPos][i] = duchy;
		G.hand[winnerPos][i] = province; 
		G.discard[winnerPos][i] = estate; 
	}

	int players[MAX_PLAYERS] = { -1, -1, -1, -1 };
	int playersControl[numPlayers] = { 0, 1, 0, 0 }; //specify winner
	
	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = getWinners(players, &testG);

	testPass = 1;
	testPass = verifyWinner(result, players, playersControl, numPlayers, winnerPos, winnerPos2);

	concludeTestCase(testPass, 1);




	// ------------------------------------------------ TEST CASE 2 ---------------------------------------------------------
	printf("TEST CASE 2: 2 players, same cards except second player has additional curse card --> first player should win\n");

	currentPlayer = 1;
	winnerPos = 0;
	numPlayers = 2;
	cardCount = 4;
	//Set players' cards to match
	for (i = 0; i < numPlayers; i++) {
		G.deckCount[i] = cardCount;
		G.discardCount[i] = cardCount;
		G.handCount[i] = cardCount;
		for (j = 0; j < cardCount; j++) {
			G.deck[i][j] = estate;
			G.hand[i][j] = duchy;
			G.discard[i][j] = province;
		}
	}
	//Add a curse card for second player which makes first player the winner
	G.hand[1][4] = curse;

	int players[MAX_PLAYERS] = { -1, -1, -1, -1 };
	int playersControl[numPlayers] = { 1, 0, 0, 0 }; //specify winner

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = getWinners(players, &testG);

	testPass = 1;
	testPass = verifyWinner(result, players, playersControl, numPlayers, winnerPos, winnerPos2);


	concludeTestCase(testPass, 2);



	// ------------------------------------------------ TEST CASE 3 ---------------------------------------------------------
	printf("TEST CASE 3: 3 players, current player is third player, first two players have same cards --> first two players should tie\n");

	//	2 players - currentPlayer = zero(has same cards as player 1 so player 1 should win with extra point)

	currentPlayer = 2;
	winnerPos = 0;
	winnerPos2 = 1; 
	numPlayers = 2;
	cardCount = 4;
	//Set players' cards to match
	for (i = 0; i < numPlayers; i++) {
		G.deckCount[i] = cardCount;
		G.discardCount[i] = cardCount;
		G.handCount[i] = cardCount;
		for (j = 0; j < cardCount; j++) {
			G.deck[i][j] = estate;
			G.hand[i][j] = duchy;
			G.discard[i][j] = estate;
		}
	}
	//Add a province card for player 0 and 1, which makes them tie as the winner
	G.hand[0][4] = province;
	G.hand[1][4] = province;

	int players[MAX_PLAYERS] = { -1, -1, -1, -1 };
	int playersControl[numPlayers] = { 1, 1, 0, 0 }; //specify winner

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = getWinners(players, &testG);

	testPass = 1;
	testPass = verifyWinner(result, players, playersControl, numPlayers, winnerPos, winnerPos2);

	concludeTestCase(testPass, 3);




	// ------------------------------------------------ TEST CASE 4 ---------------------------------------------------------
	printf("TEST CASE 4: 2 players, same cards, current player = 0 --> second player should win by bonus point for less turns\n");

	currentPlayer = 0;
	winnerPos = 1;
	winnerPos2 = -1;
	numPlayers = 2;
	cardCount = 4;
	//Set players' cards to match
	for (i = 0; i < numPlayers; i++) {
		G.deckCount[i] = cardCount;
		G.discardCount[i] = cardCount;
		G.handCount[i] = cardCount;
		for (j = 0; j < cardCount; j++) {
			G.deck[i][j] = estate;
			G.hand[i][j] = duchy;
			G.discard[i][j] = province;
		}
	}

	int players[MAX_PLAYERS] = { -1, -1, -1, -1 };
	int playersControl[numPlayers] = { 1, 0, 0, 0 }; //specify winner

	// copy the game state (G) to a test case (testG)
	memcpy(&testG, &G, sizeof(struct gameState));
	result = getWinners(players, &testG);

	testPass = 1;
	testPass = verifyWinner(result, players, playersControl, numPlayers, winnerPos, winnerPos2);

	concludeTestCase(testPass, 4);



	printf(">>>>> CARD TEST 4 (%s function) COMPLETE <<<<<\n\n", TESTFUNCTION);

	return 0;

}


int verifyWinner(int returnValue, int* playersPtr, int* playersControlPtr, int numPlayers, int winnerPos, int winnerPos2) {
	int i;
	int* winnerPtr; 

	//Verify return value of zero
	printf("\treturn value = %d, expected 0 --> ", returnValue);
	testPass = myAssert(returnValue, 0, testPass);

	//verify players[winnerPos] is 1 
	winnerPtr = players + winnerPos;
	printf("\tplayer %d = %d, expected = 1 --> ", winnerPos + 1, *winnerPtr);
	testPass = myAssert(*winnerPtr, 1, testPass);

	//If there's a tie, verify players[winnerPos2] is also 1
	if (winnerPos2 != -1) {
		winnerPtr = players + winnerPos2;
		printf("\tplayer %d = %d, expected = 1 --> ", winnerPos2 + 1, *winnerPtr);
		testPass = myAssert(*winnerPtr, 1, testPass);
	}

	//Verify result in players array is 0 for every player except the winner(s)
	for (i = 0; i < numPlayers; i++) {
		printf("\tplayer %d result = %d, expected = %d --> ", i + 1, *playersPtr, *playersControlPtr);
		testPass = myAssert(*playersPtr, *playersControlPtr, testPass);
		playersPtr++;
		playersControlPtr++;
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
