#include "HoldCalculator.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;


// Class HoldCalculator implementation
// Can be ran as a thread object

HoldCalculator::HoldCalculator()
{};

HoldCalculator::HoldCalculator(int tempRounds, int tempAces, int tempHigh, int tempMid, int tempLow, string tempName, int tempDecks, int dempPlayers)
{
    HoldCalculator::aces = tempAces;
    HoldCalculator::high = tempHigh;
    HoldCalculator::mid = tempMid;
    HoldCalculator::low = tempLow;
    HoldCalculator::rounds = tempRounds;
    HoldCalculator::BJ = new Table(tempName, tempDecks, dempPlayers);
};





// ======================================================
// ============   SET BASIC STRATEGY   ==================
// ======================================================
// Description: Set the basic strategy using new vectors.

void HoldCalculator::setBasicStrategy(vector<vector<int>> hardStrategy,
   vector<vector<int>> softStrategy, vector<vector<int>> splitStrategy){
    BJ->setPlayerBasicStrategy(hardStrategy, softStrategy, splitStrategy);    
   }





// ======================================================
// ===============      RUN THREAD     ==================
// ======================================================
// Description: Use this method as thread run() function

void HoldCalculator::runThread(){
  BJ->theDealer->casinoDrop = 0;
  BJ->theDealer->winLoss = 0;
  float total_drop = 0;
  float total_winloss = 0;
  int counter = 0;


    while (counter < rounds) {
      counter++;
      BJ->reset();
      BJ->placeCards();
      if(mode == 0){BJ->setHardCards(playerCardTotal, dealerUpCard);}
      if(mode == 1){BJ->setSoftCards(playerCardTotal, dealerUpCard);}
      if(mode == 2){BJ->setSplitCards(playerCardTotal, dealerUpCard);}

      BJ->playRound(1);

      if (counter % 1 == 0) {
        total_drop += BJ->theDealer->casinoDrop;
        total_winloss += BJ->theDealer->winLoss;
        BJ->theDealer->casinoDrop = 0;
        BJ->theDealer->winLoss = 0;

      }

      BJ->clearTable();
    }
    HoldCalculator::hold = (total_winloss/total_drop)*100;
  }



// ======================================================
// ===============      SETTINGS     ==================
// ======================================================
// Description: Tweaks deck size

   void HoldCalculator::settings(int tempRounds ,int tempAces, int tempHigh, int tempMid, int tempLow){
    HoldCalculator::aces = tempAces;
    HoldCalculator::high = tempHigh;
    HoldCalculator::mid = tempMid;
    HoldCalculator::low = tempLow;
    HoldCalculator::rounds = tempRounds;
   }