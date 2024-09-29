#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "shoe.h"
#include "dealer.h"
#include "player.h"
#include "card.h"
#include "logging.h"


using namespace std;



// CLASS TABLE//
// Declarations
class Table
{
  public:
  string gameName;
  int shoeCount;
  float Bet;
  int numberOfPlayers;
  int table_mode;
  int force_mode_value = 5;
  bool force_mode = false;

  Shoe *theShoe;
  Dealer *theDealer;
  vector<Player*> players;

  int playerCards;
  int dealerUp;

  Table(string Name, int decks, int playerNumber);
  void placeCards();
  void displayTable();
  void playRound(float bet);

  // Get / Set
  void setBet(float bet) {Bet = bet;};
  float getBet(){return Bet;}
  void showRestOfCards(){theShoe->showAllCards();}
  void clearTable();

  void setHardCards(int playerCards, int dealerUp);
  void setSoftCards(int playerCards, int dealerUp);
  void setSplitCards(int playerCards, int dealerUp);

  void reset();

  void printDrop();
  void setPlayerBasicStrategy(vector<vector<int>> hard, vector<vector<int>> soft, vector<vector<int>> split);
};




#endif // BINSEARCH_HPP