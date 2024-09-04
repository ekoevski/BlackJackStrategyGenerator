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
  Table_BJ->table_mode = mode;
  float total_drop            = 0;
  float total_winloss         = 0;
  int counter                 = 0;


  while (counter < rounds) 
  {
    LOG_0(" ", __FILE__, __LINE__, NULL);
    LOG_0(" ", __FILE__, __LINE__, NULL);  
    LOG_0(" ", __FILE__, __LINE__, NULL);
    LOG_0(" ============== RUN THREAD ================ ", __FILE__, __LINE__, NULL);  
    LOG_0(" ", __FILE__, __LINE__, NULL); 
        


    LOG_0("while( counter = %d < rounds = %d ) ", __FILE__, __LINE__, counter, rounds);

    counter++;

    LOG_0("reset()",__FILE__, __LINE__, NULL);
    Table_BJ->reset();

    LOG_0("PlaceCrards()",__FILE__, __LINE__, NULL);  
    Table_BJ->placeCards();

    if(mode == HARD_HAND_MODE)
    {
      LOG_0("setHardCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Table_BJ->setHardCards(playerCardTotal, dealerUpCard);
    }
    if(mode == SOFT_HAND_MODE)
    {
      LOG_0("setSoftCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Table_BJ->setSoftCards(playerCardTotal, dealerUpCard);
    }
    if(mode == SPLIT_HAND_MODE)
    {
      LOG_0("setSplitCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Table_BJ->setSplitCards(playerCardTotal, dealerUpCard);
    }

    LOG_0("playRound(1)",__FILE__, __LINE__, NULL);  
    if(!Table_BJ->theDealer->hasBlackJack())
    {
      for (Player *aPlayer : Table_BJ->players)
      {
        if (aPlayer->hasBlackJack())
        {
          LOG_ERROR("SKIP ROUND player has BJ, goto ) ", __FILE__, __LINE__, NULL);   
          goto END_ROUND;        
        }
      }
      Table_BJ->playRound(1);      
    }
    else
    {
      LOG_ERROR("Dealer has BJ,break ) ", __FILE__, __LINE__, NULL);      
      END_ROUND:
      counter--;

      Table_BJ->displayTable();      
      continue;
    }
 

    LOG_0(" =============== END ROUND ================", __FILE__, __LINE__, NULL);
    Table_BJ->displayTable();
    if (counter % 1 == 0)
    {
      LOG_0("Table_BJ->theDealer->casinoDrop: %f \n               \
      Table_BJ->theDealer->winLoss %f", __FILE__, __LINE__, Table_BJ->theDealer->casinoDrop, Table_BJ->theDealer->winLoss);      
      total_drop += Table_BJ->theDealer->casinoDrop;
      total_winloss += Table_BJ->theDealer->winLoss;

      Table_BJ->theDealer->casinoDrop = 0;
      Table_BJ->theDealer->winLoss = 0;


      switch (Table_BJ->table_mode)
      {
        case HARD_HAND_MODE:
        LOG_0("total_drop: %f total_winloss %f \n       mode: HARD_HAND_MODE, dealer_up: %d , player_hand: %d ",
              __FILE__, __LINE__, total_drop, total_winloss, dealerUpCard, playerCardTotal);
        break;

        case SOFT_HAND_MODE:
        LOG_0("total_drop: %f total_winloss %f \n       mode: SOFT_HAND_MODE, dealer_up: %d , player_hand: %d ",
              __FILE__, __LINE__, total_drop, total_winloss, dealerUpCard, playerCardTotal);
        break;

        case SPLIT_HAND_MODE:
        LOG_0("total_drop: %f total_winloss %f \n       mode: SPLIT_HAND_MODE, dealer_up: %d , player_hand: %d ",
              __FILE__, __LINE__, total_drop, total_winloss, dealerUpCard, playerCardTotal);
        break;                
      }

      #if DEBUG
        usleep(3000);
      #endif
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