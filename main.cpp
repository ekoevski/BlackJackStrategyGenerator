#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "simulator.h"
#include "logging.h"

using namespace std;

// user: ekoevski
// token: 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

// This program allows user to create a deck for a BlackJack game,
// and computes a basic strategy card based on the cards that are
// in the deck.
// The hold % is then tested to check effectiveness of card

// IMPORTANT: hold % is relative to the casino (-5.00% means the casino lost and the player won)

// TO USE:      <<<<<<<<<<<<<<<<<<<
// There is no prompt, the parameters are hardcoded, change them as needed.
int aces           = 24 - 0;   // 24     // Number of aces in the deck
int highCards      = 64 - 0;   // 64    // Number of tens (10, Jacks, Queens, Kings)
int midCards       = 48 - 0;   // 48    // Number 7s - 9s
int lowCards       = 80 - 0;   // 80    // Number of small cards, 2s - 6s

int roundsToSim    = 4000;     // Number of hands to play (higher = more accurate but slower)
int roundsToTest   = 500000;   // Rounds to test the basic strategy cards;


int ENABLE_BLACKJACK = 0;


// MAIN
//=================== 
int main() 
{
  printf("START, use watch -n0.1 cat output.txt in /OUTPUT_LOG/");
  delete_log_files();
  Simulator* Sim = new Simulator("Blackjack", 6, 1);  // 6 decks, 1 player (2+ player hasn't been tested)

  // Optimise perfect startegy
  //Sim->optimize_pthread_multithreaded_X7(roundsToSim, aces, highCards, midCards, lowCards);
  //Sim->optimize_multithreaded_X7(roundsToSim, aces, highCards, midCards, lowCards);
  Sim->optimize(roundsToSim, aces, highCards, midCards, lowCards);

  ENABLE_BLACKJACK = 1;
  // Test results, play 500,000 rounds to see hold %
  Sim->Table_BJ->theShoe->createShoe(aces, highCards, midCards, lowCards);

  Sim->testCurrentStrategy(roundsToTest);

  VLOG_0("\n Hold percentage - the percent that the casino will keep/lose for the amount of drop(cash) that has been being bet.", NULL);
  VLOG_0("\n Drop - The total amount of cash that has been placed as a wager.", NULL);
  VLOG_0("\n IMPORTANT: hold percentage is relative to the casino (-5.00 percemt means the casino lost and the player won)\n\n.", NULL);
}


