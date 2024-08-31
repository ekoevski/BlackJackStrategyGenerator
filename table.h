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

  Shoe *theShoe;
  Dealer *theDealer;
  vector<Player*> players;



  int playerCards;
  int dealerUp;



  Table();
  Table(string Name, int decks, int playerNumber);

  void placeCards();
  //void analyze();
  void displayTable();
  //void displayAnalysis();
  void playRound(float bet);

  // Get / Set
  void setBet(float bet) {Bet = bet;};
  float getBet(){return Bet;}
  void removeLOW(){theShoe->removeLow();}
  void removeMID(){theShoe->removeMid();}
  void removeHIGH(){theShoe->removeHigh();}
  void removeACE(){theShoe->removeAce();}
  void showRestOfCards(){theShoe->showAllCards();}
  void clearTable();

  void setHardCards(int playerCards, int dealerUp);
  void setSoftCards(int playerCards, int dealerUp);
  void setSplitCards(int playerCards, int dealerUp);


  void reset();

  void dealerUpCard(int dealerUp);

  void printDrop();
  void setPlayerBasicStrategy(vector<vector<int>> hard, vector<vector<int>> soft, vector<vector<int>> split);


  // Tester freezes game when followng card values are encountered
  void Testing(int firstCard, int secondCard, int dealerUp); // 
};




#endif // BINSEARCH_HPP