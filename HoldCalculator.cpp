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
  HoldCalculator::player_action = tempName;
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


// Call this for thread
void HoldCalculator::thread_callable()
{
  Table_BJ->theDealer->casinoDrop   = 0;
  Table_BJ->theDealer->winLoss      = 0;
  Table_BJ->table_mode              = mode;
  float total_drop                  = 0;
  float total_winloss               = 0;
  int counter                       = 0;

  while (counter < rounds) 
  {
    LOG_0(" ", __FILE__, __LINE__, NULL);
    LOG_0(" ", __FILE__, __LINE__, NULL);  
    LOG_0(" ", __FILE__, __LINE__, NULL);
    LOG_0(" ============== RUN THREAD ================ ", __FILE__, __LINE__, NULL);  
    LOG_0(" ", __FILE__, __LINE__, NULL); 
        
    LOG_0("while( counter = %d < rounds = %d ) ", __FILE__, __LINE__, counter, rounds);

    counter++;
    Table_BJ->reset();
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

    if(!Table_BJ->theDealer->hasBlackJack())
    {
      for (Player *aPlayer : Table_BJ->players)
      {
        if (aPlayer->hasBlackJack())
        {
          LOG_ERROR("END_ROUND player has BJ, goto ) ", __FILE__, __LINE__, NULL);   
          goto END_ROUND;        
        }
      }
      Table_BJ->playRound(1);      
    }
    else
    {
      LOG_ERROR("END_ROUND Dealer has BJ,break ) ", __FILE__, __LINE__, NULL);      
      END_ROUND:
      counter--;

      Table_BJ->displayTable();      
      continue;
    }
 
    LOG_0(" =============== END ROUND ================", __FILE__, __LINE__, NULL);
    Table_BJ->displayTable();
    if (counter % 1 == 0)
    {
      LOG_0("Table_BJ->theDealer->casinoDrop: %f \n \
            Table_BJ->theDealer->winLoss %f", 
            __FILE__, __LINE__, 
            Table_BJ->theDealer->casinoDrop, 
            Table_BJ->theDealer->winLoss);
      
      total_drop += Table_BJ->theDealer->casinoDrop;
      total_winloss += Table_BJ->theDealer->winLoss;
      Table_BJ->theDealer->casinoDrop = 0;
      Table_BJ->theDealer->winLoss = 0;

      switch (Table_BJ->table_mode)
      {
        case HARD_HAND_MODE:
        LOG_0("\n\n\ntotal_drop:   %.2f \ntotal_winloss %.2f \nintent:       %s \nmode:         HARD_HAND_MODE \ndealer_up:    %d\nplayer_hand:  %d\nHOUSE EDGE:   %.2f%%",
              __FILE__, __LINE__, total_drop, total_winloss, player_action.c_str(), dealerUpCard, playerCardTotal, 100*(total_winloss/total_drop));
        break;

        case SOFT_HAND_MODE:
        LOG_0("\n\n\ntotal_drop:   %.2f \ntotal_winloss %.2f \nintent:       %s \nmode:         SOFT_HAND_MODE \ndealer_up:    %d\nplayer_hand:  %d\nHOUSE EDGE:   %.2f%%",
              __FILE__, __LINE__, total_drop, total_winloss, player_action.c_str(), dealerUpCard, playerCardTotal, 100*(total_winloss/total_drop));
        break;

        case SPLIT_HAND_MODE:
        LOG_0("\n\n\ntotal_drop:   %.2f \ntotal_winloss %.2f \nintent:       %s \nmode:         SPLT_HAND_MODE \ndealer_up:    %d\nplayer_hand:  %d\nHOUSE EDGE:   %.2f%%",
              __FILE__, __LINE__, total_drop, total_winloss, player_action.c_str(), dealerUpCard, playerCardTotal, 100*(total_winloss/total_drop));
        break;                
      }

      #if DEBUG
      //usleep(10000);
      #endif
    }

    Table_BJ->clearTable();
  }
  HoldCalculator::hold = (total_winloss/total_drop)*100;
}



// ======================================================
// ===============      RUN THREAD     ==================
// ======================================================
// Description: Use this method as thread run() function
void HoldCalculator::runThread()
{
  HoldCalculator::thread_callable();
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