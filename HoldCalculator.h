#ifndef HOLDCALCULATOR_H
#define HOLDCALCULATOR_H


#include <iostream>
#include <fstream>
#include <string>
#include "table.h"
#include <vector>


using namespace std;


// CLASS CARD//   DONE
// Declarations
class HoldCalculator
{
  public:
  int aces = 4;
  int high = 16;
  int mid = 12;
  int low = 20;
  int rounds = 3000;
  Table *BJ;

  float hold = 0;

int mode = 4;
int playerCardTotal = 0;
int dealerUpCard = 0;

    // Used to set first 2 cards manually
  void setCards(int theMode, int playerTotal, int dealerUp){
    mode = theMode;
    playerCardTotal = playerTotal;
    dealerUpCard = dealerUp;
  };  //mode: 0 for hard, 1, for soft 2 for split first hand setter


  HoldCalculator();
  HoldCalculator(int rounds, int aces, int high, int mid,
   int low, string gameName, int tempDecks, int tempPlayerCount);

  void runThread();

  void settings(int rounds ,int aces, int high, int mid, int low);
  void setBasicStrategy(vector<vector<int>> tempHardStrategy,
   vector<vector<int>> tempSoftStrategy, vector<vector<int>> tempSplitStrateg);
};




#endif // BINSEARCH_HPP