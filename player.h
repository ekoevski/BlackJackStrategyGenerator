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
    vector<vector<int>> playerHardStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 0   // Just a filler
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 1   /////////////
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 2   //////////////
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 3   ///////////////
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 4
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 5
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 6
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 7
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 8
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 10
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 11
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 12
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 13
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 14
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 15  /// 4 right ones change dto 0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 16      // four right ones changed to 0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}       // HARD 21
  };   
  

  // !! SHE BRESH QDOVE sas SOFT HANDS, YOU NEED TO COMPLETE THE 13 - 0 indexing
  vector<vector<int>> playerSoftStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 0    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 1    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 2    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 3    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 4    FILLER                    
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 5    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 6    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 7    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 8  FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 9  FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 10  FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 11   FILLER
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 12
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 13
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 14
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 15
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 16
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}     // SOFT 21
  };   


  vector<vector<int>> playerSplitStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // A,A
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 2,2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 3,3
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 4,4
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 5,5   // You addded this, iffy
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 6,6
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 7,7
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 8,8
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // 9,9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}      // 10,10
  }; 




  public:
  Player();
  Player(Shoe *Shoe, int seat);
  // GETTERS
  void printBasicStrategy();
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