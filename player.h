#ifndef PLAYER_H
#define PLAYER_H


#include <iostream>
#include <fstream>
#include <string>
#include "shoe.h"
#include "card.h"
#include "logging.h"


using namespace std;


// CLASS DEALER//   DONE
// Declarations
class Player
{
  public:
  Card *set_first_card;
  Card *set_second_card;

  int strategySelector = 0;

  float playerDrop = 0;
  float winLoss = 0;

  int testFirstHand = 110;
  int testSecondHand = 110;

  float bet;
  Shoe *theShoe2;
  string cardName;
  int playerSeat;

  //To indicate double on first 2
  bool firstDouble = false;
  bool secondDouble = false;
  bool thirdDouble = false;


    // All Player Bets
  float mainBet = 0;
  float doubleMainBet = 0;
  
  float secondSplitBet = 0;
  float doubleSecondSplitBet = 0;

  float thirdSplitBet = 0;
  float doubleThirdSplitBet = 0;

  bool firstBusted = false;
  bool secondBusted = false;
  bool thirdBusted = false;

  bool firstBJ = false;


  int dealerShowing = 0;
  vector<Card*> playerHand;

  vector<Card*> secondSplitHand;
  vector<Card*> thirdSplitHand;
  
  public:
  vector<vector<int>> playerHardStrategy;
  vector<vector<int>> playerSoftStrategy;
  vector<vector<int>> playerSplitStrategy;

  public:
  Player();
  Player(Shoe *Shoe, int seat);
  // GETTERS
  float getMainBet();
  float getSecondSplitBet();
  float getThirdSplitBet();
  bool firstHandBusted() {return firstBusted;}
  bool secondHandBusted() {return secondBusted;}
  bool thirdHandBusted() {return thirdBusted;}
  float getPlayerTotalBets();
  int getSeat(){return playerSeat;}
  void getCard();
  int getFirstHandTotal();
  int getSecondHandTotal();
  int getThirdHandTotal(); 
  bool hasBlackJack();
  void setBets(float tempBet);
  void showHand();
  bool firstIsSoft();
  bool secondIsSoft();
  bool thirdIsSoft();
  void firstTwo(int dealerUpCard, int intent_mode, bool force_intent_mode); // Decide if double or split on 1st two cards
  void playFirstHand();
  void playSecondHand();
  void playThirdHand();
  int getFirstCardTotal();
  int getSecondCardTotal();
  int getThirdCardTotal();
  int firstHandFinal();
  int secondHandFinal();
  int thirdHandFinal();
  void pickUpCards();  // Clear table / Garbage collection
};




#endif // BINSEARCH_HPP