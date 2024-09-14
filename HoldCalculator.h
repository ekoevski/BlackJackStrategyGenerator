#ifndef HOLDCALCULATOR_H
#define HOLDCALCULATOR_H


#include <iostream>
#include <fstream>
#include <string>
#include "table.h"
#include <vector>
#include "logging.h"

using namespace std;


// HoldCalculator (HEADER)
// =====================
class HoldCalculator
{
  public:
  int aces = 4;
  int high = 16;
  int mid = 12;
  int low = 20;
  int rounds = 3000;
  Table *Calculator_table;
  float hold = 0;
  int mode = 4;
  int playerCardTotal = 0;
  int dealerUpCard = 0;
  float total_win_loss = 0;
  string player_action = "none";
  vector<vector<int>> p_calculator_HardStrategy;
  vector<vector<int>> p_calculator_SoftStrategy;
  vector<vector<int>> p_calculator_SplitStrategy;

  //Constructors
  HoldCalculator();
  HoldCalculator( int rounds, 
                  int aces, 
                  int high, 
                  int mid, 
                  int low, 
                  string gameName, 
                  int tempDecks, 
                  int tempPlayerCount, 
                  vector<vector<int>> &tempHardStrategy,
                  vector<vector<int>> &tempSoftStrategy,
                  vector<vector<int>> &tempSplitStrateg);

  //Methods
  void thread_callable();
  void runThread();
  void settings(int rounds ,int aces, int high, int mid, int low);
  void setBasicStrategy(vector<vector<int>> tempHardStrategy,
  vector<vector<int>> tempSoftStrategy, vector<vector<int>> tempSplitStrateg);

  // SET PLAYER AND DEALER CARDS MANUALLY
  //=======================================
  void setCards(int theMode, int playerTotal, int dealerUp)
  {
    mode = theMode;
    playerCardTotal = playerTotal;
    dealerUpCard = dealerUp + 1;
  };  //mode: 0 for hard, 1, for soft 2 for split first hand setter
};




#endif // BINSEARCH_HPP