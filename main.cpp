#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "simulator.h"

#define FLAG_VALUE 123

using namespace std;



// This program allows user to create a deck for a BlackJack game,
// and computes a basic strategy card based on the cards that are
// in the deck.
// The hold % is then tested to check effectiveness of card


// IMPORTANT: hold % is relative to the casino (-5.00% means the casino lost and the player won)


// TO USE:      <<<<<<<<<<<<<<<<<<<
// There is no prompt, the parameters are hardcoded, change them as needed.
int aces = 4;             // Number of aces in the deck
int highCards = 16;       // Number of tens (10, Jacks, Queens, Kings)
int midCards = 12;        // Number 7s - 9s
int lowCards = 20;        // Number of small cards, 2s - 6s

int roundsToSim = 4000;    // Number of hands to play (higher = more accurate but slower)
int roundsToTest = 500000;  // Rounds to test the basic strategy cards;



// MAIN
//=================== 
int main() {

  for (int i = 3; i > 0; i--) {
    printf("    \n\n%d ", i);
    sleep(1);
  }

  Simulator* Sim = new Simulator("Blackjack", 6, 1);  // 6 decks, 1 player
 
// Optimise perfect startegy
Sim->optimize(roundsToSim, aces, highCards, midCards, lowCards);

// Test results, play 500,000 rounds to see hold %
Sim->BJ->theShoe->createShoe(aces, highCards, midCards, lowCards);
Sim->testCurrentStrategy(roundsToTest);

  printf("\n Hold percentage - the percent that the casino will keep/lose for the amount of drop(cash) that has been being bet.");
  printf("\n Drop - The total amount of cash that has been placed as a wager.");
  printf("\n IMPORTANT: hold percentage is relative to the casino (-5.00 percemt means the casino lost and the player won)\n\n.");
}
