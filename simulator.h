#ifndef SIMULATOR_H
#define SIMULATOR_H


#include <iostream>
#include <fstream>
#include <string>
#include "table.h"
#include <vector>
#include "logging.h"


using namespace std;


// CLASS SIMULATOR  (HEADER)
// ==========================

class Simulator
{
  public:

  Table* Table_BJ;
  float total_drop = 0;
  float total_winloss = 0;
  int counter = 0;


  string gameName = "BlackJack";
  int shoeDecks = 1;
  int numberPlayers = 1;


  vector<vector<int>> currentStrategyCard;

  //Card count  (dependent on deck count x52)
  int aces = 0;
  int high = 0;
  int mid = 0;
  int low = 0;



  Simulator();
  Simulator(string name, int decks, int players);


  void exportBasicStrategy(int tempAces, int tempHigh, int tempMid, int tempLow);
  void loadBasicStrategy(int tempAces, int tempHigh, int tempMid, int tempLow);
  void optimize(int rounds, int tempAces, int tempHigh, int tempMid, int tempLow);
  void optimize_multithreaded_X7(int rounds, int tempAces, int tempHigh, int tempMid, int tempLow);
  void printBasicStrategy();

#if (BASIC_STRATEGY_STANDARD == 1)
    vector<vector<int>> hardStrategy {
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
  vector<vector<int>> softStrategy {
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


  vector<vector<int>> splitStrategy {
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
#endif

#if (BASIC_STRATEGY_ZERO == 1)
    vector<vector<int>> hardStrategy {
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
  vector<vector<int>> softStrategy {
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


  vector<vector<int>> splitStrategy {
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
#endif

  void testCurrentStrategy(int rounds);
};




#endif // BINSEARCH_HPP