#ifndef PLAYER_H
#define PLAYER_H


#include <iostream>
#include <fstream>
#include <string>
#include "shoe.h"
#include "card.h"


using namespace std;


// CLASS DEALER//   DONE
// Declarations
class Player
{
  public:

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

  // IF splitting
  vector<Card*> secondSplitHand;
  vector<Card*> thirdSplitHand;
  
  // Basic Strategy Card
  // 0 = STAY
  // 1 = HIT
  // 2 = DOUBLE
  // 4 = SPLIT

  public:
  vector<vector<int>> basicHardStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 0   // Just a filler
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 1   /////////////
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 2   //////////////
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 3   ///////////////
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 4
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 5
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 6
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 7
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 8
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 9
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},      // HARD 10
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},      // HARD 11
    {1, 1, 1, 0, 0, 0, 1, 1, 1, 1},      // HARD 12
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},      // HARD 13
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},      // HARD 14
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},      // HARD 15
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},      // HARD 16         
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}       // HARD 21
  };   
  

  // !! SHE BRESH QDOVE sas SOFT HANDS, YOU NEED TO COMPLETE THE 13 - 0 indexing
  vector<vector<int>> basicSoftStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 0    FILLER
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
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 12    FILLER
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {1, 1, 1, 1, 2, 2, 1, 1, 1, 1},     // SOFT 12    index: 13
    {1, 1, 1, 1, 2, 2, 1, 1, 1, 1},     // SOFT 13    index: 14
    {1, 1, 1, 2, 2, 2, 1, 1, 1, 1},     // SOFT 14
    {1, 1, 1, 2, 2, 2, 1, 1, 1, 1},     // SOFT 15
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 16
    {1, 2, 2, 2, 2, 2, 0, 0, 1, 1},     // SOFT 17
    {0, 0, 0, 0, 0, 2, 0, 0, 0, 0},     // SOFT 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}      // SOFT 21    index: 22
  };   


  vector<vector<int>> basicSplitStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},     // A,A
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 2,2
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 3,3
    {1, 1, 1, 1, 4, 4, 1, 1, 1, 1},     // 4,4
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},     // 5,5   // You addded this, iffy
    {1, 4, 4, 4, 4, 4, 1, 1, 1, 1},     // 6,6
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 7,7
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},     // 8,8
    {0, 4, 4, 4, 4, 4, 0, 4, 4, 0},     // 9,9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}      // 10,10
  }; 

/*
  vector<vector<int>> basicHardStrategyDEFAULT {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 0   // Just a filler
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 1   /////////////
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 2   //////////////
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 3   ///////////////
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 4
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 5
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 6
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 7
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},      // HARD 8
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 9
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},      // HARD 10
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},      // HARD 11
    {1, 1, 1, 0, 0, 0, 1, 1, 1, 1},      // HARD 12
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},      // HARD 13
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1},      // HARD 14
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 15  /// 4 right ones change dto 0
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 16      // four right ones changed to 0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}       // HARD 21
  };   
  

  // !! SHE BRESH QDOVE sas SOFT HANDS, YOU NEED TO COMPLETE THE 13 - 0 indexing
  vector<vector<int>> basicSoftStrategyDEFAULT {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 0    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 1    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 2    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 3    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 4    FILLER                    
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 5    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 6    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 7    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT   FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT   FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT   FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT     FILLER
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {1, 1, 1, 1, 2, 2, 1, 1, 1, 1},     // SOFT 12
    {1, 1, 1, 1, 2, 2, 1, 1, 1, 1},     // SOFT 13
    {1, 1, 1, 2, 2, 2, 1, 1, 1, 1},     // SOFT 14
    {1, 1, 1, 2, 2, 2, 1, 1, 1, 1},     // SOFT 15
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 16
    {1, 2, 2, 2, 2, 2, 0, 0, 1, 1},     // SOFT 17
    {0, 0, 0, 0, 0, 2, 0, 0, 0, 0},     // SOFT 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // SOFT 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}     // SOFT 21
  };   


  vector<vector<int>> basicSplitStrategyDEFAULT {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},     // A,A
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 2,2
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 3,3
    {1, 1, 1, 1, 4, 4, 1, 1, 1, 1},     // 4,4
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},     // 5,5   // You addded this, iffy
    {1, 4, 4, 4, 4, 4, 1, 1, 1, 1},     // 6,6
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 7,7
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},     // 8,8
    {0, 4, 4, 4, 4, 4, 0, 4, 4, 0},     // 9,9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}      // 10,10
  }; 



//  NEW----------
  vector<vector<int>> lowHardStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 1},      // HARD 0   // Just a filler
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 1},      // HARD 1   /////////////
    {1, 1, 1, 1, 2, 2, 1, 1, 1, 1},      // HARD 2   //////////////
    {1, 1, 1, 2, 2, 2, 1, 1, 1, 1},      // HARD 3   ///////////////
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 4
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 5
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 6
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 7
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 8
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},      // HARD 9
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},      // HARD 10
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},      // HARD 11
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 12
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 13
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 14
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 15
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 16
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 17
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 18
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 19
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // HARD 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}       // HARD 21
  };   
  
  // !! SHE BRESH QDOVE sas SOFT HANDS, YOU NEED TO COMPLETE THE 13 - 0 indexing
  vector<vector<int>> lowSoftStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 0    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 1    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 2    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 3    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 4    FILLER                    
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 5    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 6    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT 7    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT   FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT   FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT   FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT    FILLER
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},     // SOFT     FILLER
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {1, 2, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 12
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 13
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 14
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 15
    {1, 1, 2, 2, 2, 2, 1, 1, 1, 1},     // SOFT 16
    {1, 1, 2, 2, 2, 2, 0, 1, 1, 1},     // SOFT 17
    {0, 1, 2, 2, 2, 2, 0, 0, 1, 0},     // SOFT 18
    {0, 0, 2, 2, 2, 2, 0, 0, 0, 0},     // SOFT 19
    {0, 0, 0, 0, 2, 2, 0, 0, 0, 0},     // SOFT 20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}     // SOFT 21
  };   


  vector<vector<int>> lowSplitStrategy {
//   A  2  3  4  5  6  7  8  9 10       // Dealer's up card
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},     // A,A
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 2,2
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 3,3
    {1, 1, 2, 2, 4, 4, 1, 1, 1, 1},     // 4,4
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},     // 5,5   // You addded this, iffy
    {1, 4, 4, 4, 4, 4, 1, 1, 1, 1},     // 6,6
    {1, 4, 4, 4, 4, 4, 4, 1, 1, 1},     // 7,7
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4},     // 8,8
    {0, 4, 4, 4, 4, 4, 0, 4, 4, 0},     // 9,9
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}      // 10,10
  }; 

*/
// NEW -------------

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


  // SETTERS
  void setBets(float tempBet);



  // Player hand and decision

  void showHand();
   
  bool firstIsSoft();
  bool secondIsSoft();
  bool thirdIsSoft();

  // AI MODULE
  void firstTwo(int dealerUpCard); // Decide if double or split on 1st two cards

  void playHand();
  void playHard();
  void playSoft();

  void playFirstHand();
  void playSecondHand();
  void playThirdHand();


  int getFirstCardTotal();
  int getSecondCardTotal();
  int getThirdCardTotal();

  int firstHandFinal();
  int secondHandFinal();
  int thirdHandFinal();

  void selectBasicStrategy(int selection);


  void pickUpCards();  // Clear table / Garbage collection

};




#endif // BINSEARCH_HPP