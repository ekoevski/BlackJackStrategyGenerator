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
//int miniCards      = 48 - 0;   // 64    // Number of tens (10, Jacks, Queens, Kings)
int minCards       = 48 - 30;
int lowCards       = 72 - 10;   // 80    // Number of small cards, 4s - 6s
int midCards       = 72 - 45;   // 48    // Number 7s - 9s
int highCards      = 96 - 10;   // 64    // Number of tens (10, Jacks, Queens, Kings)
int aces           = 24 - 5;   // 24     // Number of aces in the deck

int roundsToSim    = 300;     // Number of hands to play (higher = more accurate but slower)
int roundsToTest   = 50000;   // Rounds to test the basic strategy cards;

bool g_lock_split_strategy = false;
bool g_enable_blackjack    = true;
// MAIN
//=================== 
int main() 
{

  //printf("hold percentage arragy: %f", hold_percentage_array[0][0][0][0][0]);

  printf("START, use $ watch -n0.1 cat output.txt in /OUTPUT_LOG/");
  delete_log_files();
  Simulator* Sim = new Simulator("Blackjack", 6, 1);  // 6 decks, 1 player (2+ player hasn't been tested)

  Sim->optimize(roundsToSim, aces, highCards, midCards, lowCards, minCards);

  // Test results, play 500,000 rounds to see hold %
  g_enable_blackjack = true;
  Sim->Table_BJ->theShoe->createShoe(aces, highCards, midCards, lowCards, minCards);
  //Sim->loadBasicStrategy(aces, highCards, midCards, lowCards, minCards);
  Sim->testCurrentStrategy(roundsToTest);


  Sim->exportBasicStrategy(aces,highCards,midCards,lowCards,minCards);
  //VLOG_0("\n Hold percentage - the percent that the casino will keep/lose for the amount of drop(cash) that has been being bet.", NULL);
  //VLOG_0("\n Drop - The total amount of cash that has been placed as a wager.", NULL);
  //VLOG_0("\n IMPORTANT: hold percentage is relative to the casino (-5.00 percemt means the casino lost and the player won)\n\n.", NULL);
}


