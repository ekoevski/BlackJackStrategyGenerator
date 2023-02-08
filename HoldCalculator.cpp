#include "HoldCalculator.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;

// Class is Thread Object that computes hold % for a given strategy card

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



void HoldCalculator::setBasicStrategy(vector<vector<int>> hardStrategy,
   vector<vector<int>> softStrategy, vector<vector<int>> splitStrategy){
    BJ->setPlayerBasicStrategy(hardStrategy, softStrategy, splitStrategy);    
   }



void HoldCalculator::runThread(){
  BJ->theDealer->casinoDrop = 0;
  BJ->theDealer->winLoss = 0;
  float total_drop = 0;
  float total_winloss = 0;
  int counter = 0;


  while (counter < rounds) {
    counter++;

    BJ->placeCards();

    if(mode == 0){BJ->setHardCards(playerCardTotal, dealerUpCard);}
    if(mode == 1){BJ->setSoftCards(playerCardTotal, dealerUpCard);}
    if(mode == 2){BJ->setSplitCards(playerCardTotal, dealerUpCard);}



    BJ->displayTable();
    BJ->playRound(1);

    if (counter % 100 == 0) {
      //BJ->printDrop();
      total_drop += BJ->theDealer->casinoDrop;
      total_winloss += BJ->theDealer->winLoss;
      //printf(" total W/L (%f)  rounds = %i , low = %i \n", total_winloss/total_drop, counter, BJ->theShoe->lowIndex); 
      //printf("dealer wl %f   dealer drop %f    total wl %f    total drop %f     \n" ,BJ->theDealer->winLoss , BJ->theDealer->casinoDrop, total_winloss, total_drop);
      BJ->theDealer->casinoDrop = 0;
      BJ->theDealer->winLoss = 0;

    }

    BJ->clearTable();
  }

    //printf("\n\nSimulator::run completed: hold %f percent, rounds: %d\n\n", (total_winloss/total_drop)*100, rounds);
    HoldCalculator::hold = (total_winloss/total_drop)*100;
   }





   void HoldCalculator::settings(int tempRounds ,int tempAces, int tempHigh, int tempMid, int tempLow){
    HoldCalculator::aces = tempAces;
    HoldCalculator::high = tempHigh;
    HoldCalculator::mid = tempMid;
    HoldCalculator::low = tempLow;
    HoldCalculator::rounds = tempRounds;
   }