#include "HoldCalculator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;

// Class HoldCalculator implementation
// Can be ran as a thread oCalculator_tableect

HoldCalculator::HoldCalculator()
{
  // Default Constructor
};

HoldCalculator::HoldCalculator( int tempRounds, 
                                int tempAces, 
                                int tempHigh, 
                                int tempMid, 
                                int tempLow, 
                                string gameName, 
                                int tempDecks, 
                                int tempPlayerCount, 
                                vector<vector<int>> &p_tempHardStrategy,
                                vector<vector<int>> &p_tempSoftStrategy,
                                vector<vector<int>> &p_tempSplitStrategy)
{
  HoldCalculator::aces = tempAces;
  HoldCalculator::high = tempHigh;
  HoldCalculator::mid = tempMid;
  HoldCalculator::low = tempLow;
  HoldCalculator::rounds = tempRounds;
  HoldCalculator::Calculator_table = new Table(gameName, tempDecks, tempPlayerCount);
  Calculator_table->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  HoldCalculator::player_action = gameName;
  p_calculator_HardStrategy = p_tempHardStrategy;
  p_calculator_SoftStrategy = p_tempSoftStrategy;
  p_calculator_SplitStrategy = p_tempSplitStrategy;
};

// ======================================================
// ============   SET BASIC STRATEGY   ==================
// ======================================================
// Description: Set the basic strategy using new vectors.

void HoldCalculator::setBasicStrategy(vector<vector<int>> hardStrategy1,
                                      vector<vector<int>> softStrategy1,
                                      vector<vector<int>> splitStrategy1)
{
  Calculator_table->setPlayerBasicStrategy(hardStrategy1, softStrategy1, splitStrategy1);    
}


// Call this for thread
void HoldCalculator::thread_callable()
{
  for (Player* aPlayer : Calculator_table->players)
  {
    aPlayer->printBasicStrategy();

  }
  Calculator_table->theDealer->casinoDrop   = 0;
  Calculator_table->theDealer->winLoss      = 0;
  Calculator_table->table_mode              = mode;
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
    Calculator_table->reset();
    Calculator_table->placeCards();

    if(mode == HARD_HAND_MODE)
    {
      LOG_0("setHardCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Calculator_table->setHardCards(playerCardTotal, dealerUpCard);
    }
    if(mode == SOFT_HAND_MODE)
    {
      LOG_0("setSoftCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Calculator_table->setSoftCards(playerCardTotal, dealerUpCard);
    }
    if(mode == SPLIT_HAND_MODE)
    {
      LOG_0("setSplitCards(playerCardTotal = %d, dealerUpCard = %d)",__FILE__, __LINE__, playerCardTotal, dealerUpCard);      
      Calculator_table->setSplitCards(playerCardTotal, dealerUpCard);
    }

    if(!Calculator_table->theDealer->hasBlackJack())
    {
      for (Player *aPlayer : Calculator_table->players)
      {
        if (aPlayer->hasBlackJack())
        {
          LOG_ERROR("END_ROUND player has BJ, goto ) ", __FILE__, __LINE__, NULL);   
          goto END_ROUND;        
        }
      }
      Calculator_table->playRound(1);      
    }
    else
    {
      LOG_ERROR("END_ROUND Dealer has BJ,break ) ", __FILE__, __LINE__, NULL);      
      END_ROUND:
      counter--;

      Calculator_table->displayTable();      
      continue;
    }
 
    LOG_0(" =============== END ROUND ================", __FILE__, __LINE__, NULL);
    Calculator_table->displayTable();
    if (counter % 1 == 0)
    {
      LOG_0("Calculator_table->theDealer->casinoDrop: %f \n \
            Calculator_table->theDealer->winLoss %f", 
            __FILE__, __LINE__, 
            Calculator_table->theDealer->casinoDrop, 
            Calculator_table->theDealer->winLoss);
      
      total_drop += Calculator_table->theDealer->casinoDrop;
      total_winloss += Calculator_table->theDealer->winLoss;
      Calculator_table->theDealer->casinoDrop = 0;
      Calculator_table->theDealer->winLoss = 0;

      switch (Calculator_table->table_mode)
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

    Calculator_table->clearTable();
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