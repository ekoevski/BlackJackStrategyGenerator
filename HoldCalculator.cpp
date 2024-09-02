#include "HoldCalculator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;


// Class HoldCalculator implementation
// Can be ran as a thread oTable_bject

HoldCalculator::HoldCalculator()
{
  // Default Constructor
};

HoldCalculator::HoldCalculator(int tempRounds, 
                               int tempAces, 
                               int tempHigh, 
                               int tempMid, 
                               int tempLow, 
                               string tempName, 
                               int tempDecks, 
                               int dempPlayers)
{
  HoldCalculator::aces = tempAces;
  HoldCalculator::high = tempHigh;
  HoldCalculator::mid = tempMid;
  HoldCalculator::low = tempLow;
  HoldCalculator::rounds = tempRounds;
  HoldCalculator::Table_BJ = new Table(tempName, tempDecks, dempPlayers);
};





// ======================================================
// ============   SET BASIC STRATEGY   ==================
// ======================================================
// Description: Set the basic strategy using new vectors.

void HoldCalculator::setBasicStrategy(vector<vector<int>> hardStrategy1,
                                      vector<vector<int>> softStrategy1,
                                      vector<vector<int>> splitStrategy1)
{
  Table_BJ->setPlayerBasicStrategy(hardStrategy1, softStrategy1, splitStrategy1);    
}





// ======================================================
// ===============      RUN THREAD     ==================
// ======================================================
// Description: Use this method as thread run() function
void HoldCalculator::runThread()
{
  Table_BJ->theDealer->casinoDrop   = 0;
  Table_BJ->theDealer->winLoss      = 0;
  float total_drop            = 0;
  float total_winloss         = 0;
  int counter                 = 0;


  while (counter < rounds) 
  {
    LOG_0(" ", __FILE__, __LINE__, NULL);
    LOG_0(" ", __FILE__, __LINE__, NULL);  
    LOG_0(" ", __FILE__, __LINE__, NULL);  
    LOG_0("while( counter = %d < rounds = %d ) ", __FILE__, __LINE__, counter, rounds);

    counter++;

    LOG_0("reset()",__FILE__, __LINE__, NULL);
    Table_BJ->reset();

    LOG_0("PlaceCrards()",__FILE__, __LINE__, NULL);  
    Table_BJ->placeCards();

    if(mode == 0)
    {
      LOG_0("setHardCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Table_BJ->setHardCards(playerCardTotal, dealerUpCard);
    }
    if(mode == 1)
    {
      LOG_0("setSoftCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Table_BJ->setSoftCards(playerCardTotal, dealerUpCard);
    }
    if(mode == 2)
    {
      LOG_0("setSplitCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Table_BJ->setSplitCards(playerCardTotal, dealerUpCard);
    }
    LOG_0("playRound(1)",__FILE__, __LINE__, NULL);  
    Table_BJ->playRound(1);

    if (counter % 1 == 0)
    {
      LOG_0("Table_BJ->theDealer->casinoDrop: %f Table_BJ->theDealer->winLoss %f", __FILE__, __LINE__, Table_BJ->theDealer->casinoDrop, Table_BJ->theDealer->winLoss);      
      total_drop += Table_BJ->theDealer->casinoDrop;
      total_winloss += Table_BJ->theDealer->winLoss;

      Table_BJ->theDealer->casinoDrop = 0;
      Table_BJ->theDealer->winLoss = 0;
      LOG_0("total_drop: %f total_winloss %f", __FILE__, __LINE__, total_drop, total_winloss);
      usleep(100000);  // YOU ARE HERE, you're not racking good winloss
    }

    Table_BJ->clearTable();
  }
  HoldCalculator::hold = (total_winloss/total_drop)*100;
}

// ======================================================
// ===============      SETTINGS     ==================
// ======================================================
// Description: Tweaks deck size
void HoldCalculator::settings(int tempRounds,
                              int tempAces, 
                              int tempHigh, 
                              int tempMid, 
                              int tempLow)
{
  HoldCalculator::aces = tempAces;
  HoldCalculator::high = tempHigh;
  HoldCalculator::mid = tempMid;
  HoldCalculator::low = tempLow;
  HoldCalculator::rounds = tempRounds;
}