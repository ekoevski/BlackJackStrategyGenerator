#include "table.h"
#include "simulator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "logging.h"

#include <thread>
#include <chrono>
#include "HoldCalculator.h"

using namespace std;


// Constructors
//=======================

Simulator::Simulator()
{
  // Do Nothing
};

Simulator::Simulator(string name, int decks, int players)
{
  Simulator::Table_BJ = new Table(name, decks, players);
  Simulator::aces = decks * 4;
  Simulator::high = decks * 16;
  Simulator::mid = decks * 12;
  Simulator::low = decks * 20;
  Simulator::gameName = name;
  Simulator::shoeDecks = decks;
  Simulator::numberPlayers = players;

  VLOG_0("\nInitiating deck, total decks: %i total cards: %i\n\n", decks, aces + high + mid + low);
};

// ======================================================
// ===============TEST CURRENT STRATEGY==================
// ======================================================
// precondition: has a basic strategy card vectors (soft, hard, split)
// postcondition: gives hold % for current basic strategy card
void Simulator::testCurrentStrategy(int rounds)
{
  Table_BJ->theDealer->casinoDrop = 0;
  Table_BJ->theDealer->winLoss = 0;
  total_drop = 0.0;
  total_winloss = 0.0;
  
  Simulator::counter = 0;
  while (counter < rounds)
  {
    counter++;
    Table_BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
    Table_BJ->reset();
    Table_BJ->placeCards();
    Table_BJ->playRound(1);

    if (counter % 50000 == 0)
    {
      total_drop += Table_BJ->theDealer->casinoDrop;
      total_winloss += Table_BJ->theDealer->winLoss;
      VLOG_0("drop: %f w/l: %f total W/L (%f)  rounds = %i , low = %i \n",Table_BJ->theDealer->casinoDrop, Table_BJ->theDealer->winLoss, total_winloss/total_drop, counter, Table_BJ->theShoe->lowIndex); 

      Table_BJ->theDealer->casinoDrop = 0;
      Table_BJ->theDealer->winLoss = 0;
    }

  Table_BJ->clearTable();
  }
  VLOG_0("\n\nSimulator::run completed: hold %f percent, rounds: %d\n\n", (total_winloss/total_drop)*100, rounds);
};

// ======================================================
// ===============EXPORT BASIC STRATEGY==================
// ======================================================
// precondition: Takes in deck composition: # of aces, mids, highs, lows
// postcondition: Exports to .txt file a saved version of the basic strategy card using 
// number of cards.

void Simulator::exportBasicStrategy(int tempAces, int tempHigh, int tempMid, int tempLow){
    fstream file;
    string filename = "strategy cards/A" + to_string(tempAces) + "T" + to_string(tempHigh) + "M" + to_string(tempMid) + "L" + to_string(tempLow) + ".txt";


    file.open(filename,ios_base::out);
 
    for(int j = 0; j < hardStrategy.size(); j++){
      for(int i = 0; i < hardStrategy[j].size(); i++)
      {
          file << hardStrategy[j][i];
      }      
    }

    for(int j = 0; j < softStrategy.size(); j++){
      for(int i = 0; i < softStrategy[j].size(); i++)
      {
          file << softStrategy[j][i];
      }      
    }



    for(int j = 0; j < splitStrategy.size(); j++){
      for(int i = 0; i < splitStrategy[j].size(); i++)
      {
          file << splitStrategy[j][i];
      }      
    }

    file.close();
}

// ======================================================
// =============== LOAD BASIC STRATEGY ==================
// ======================================================
// precondition: Must have an available saved .txt file in /strategy cards
// postcondition: Reads text file and imports as vector in current
// basic strategy

void Simulator::loadBasicStrategy(int tempAces, int tempHigh, int tempMid, int tempLow)
{
    fstream file;
    string filename = "strategy cards/A" + to_string(tempAces) + "T" + to_string(tempHigh) + "M" + to_string(tempMid) + "L" + to_string(tempLow) + ".txt";

    file.open(filename,ios::in);
    if (file.is_open())
    {
          VLOG_0("\nHARD STRATEGY\n", NULL);
          for(int j = 0; j < hardStrategy.size(); j++){
            for(int i = 0; i < hardStrategy[j].size(); i++)
            {
                hardStrategy[j][i] = file.get()-48;
                VLOG_0("%d ", hardStrategy[j][i]);                 
            }      
                VLOG_0("\n", NULL); 
          }
          VLOG_0("\nSOFT STRATEGY\n", NULL);          
          for(int j = 0; j < softStrategy.size(); j++){
            for(int i = 0; i < softStrategy[j].size(); i++)
            {
                softStrategy[j][i] = file.get()-48;
                VLOG_0("%d ", softStrategy[j][i]);                 
            }      
            VLOG_0("\n", NULL); 
          }

          VLOG_0("\nSPLIT STRATEGY\n", NULL);
          for(int j = 0; j < splitStrategy.size(); j++){
            for(int i = 0; i < splitStrategy[j].size(); i++)
            {
                splitStrategy[j][i] = file.get()-48;
                VLOG_0("%d ", splitStrategy[j][i]);                
            } 
            VLOG_0("\n", NULL);
          }
        
        file.close(); 
    }
}

// ======================================================
// =============== OPTIMIZE BASIC STRATEGY ==============
// ======================================================
// precondition: Changes values of current basic strategy vector and 
//               tests to determine if hit, stay, double, or split
//               is the most lucrative player move.
// postcondition: Saves basic strategy card in /strategy cards as text file
//               which can be loaded using loadBasicStrategy(yada yada)

void Simulator::optimize(int rounds, int tempAces, int tempHigh, int tempMid, int tempLow)
{
  float stayHold              = 0;
  float hitHold               = 0;
  float doubleHold            = 0;
  float splitHold             = 0;
  float min                   = 1000.0;
  int player_hand_total       = 0;
  int dealer_up_card          = 0;
  char action                 ='Q';

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  // Instantiate calculator oTable_BJbjects
  HoldCalculator* Stay     = new HoldCalculator(rounds, aces, high, mid, low, "Stay", shoeDecks, numberPlayers);
  HoldCalculator* Hit      = new HoldCalculator(rounds, aces, high, mid, low, "Hit", shoeDecks, numberPlayers);
  HoldCalculator* Double   = new HoldCalculator(rounds, aces, high, mid, low, "Double", shoeDecks, numberPlayers);
  HoldCalculator* Split    = new HoldCalculator(rounds, aces, high, mid, low, "Split", shoeDecks, numberPlayers);

  // Create calculator shoe
  Stay->Calculator_table->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Hit->Calculator_table->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Double->Calculator_table->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Split->Calculator_table->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);

  
  VLOG_0("                *** START OPTIMIZATION (SINGLE-THREAD)    (%i) ROUNDS ***  \n\n", rounds);
  VLOG_0(" Aces: %d  High: %d  Mid: %d  Low: %d\n", tempAces, tempHigh, tempMid, tempLow);
  VLOG_0("\n 0 = Stay   |   1 = Hit    |  2 = Double  |   4 = Split \n\n", NULL);

#if (HARD_STRATEGY == 1)
  VLOG_0("\n HARD STRATEGY \n\n", NULL);
  VLOG_0("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n", NULL);
#if (DEBUG==1)
  sleep(2);
#endif
  // Print player card hard values
  for(int j = 5; j < hardStrategy.size() - 1; j++)
  {
    if(j < 10) 
    {
      VLOG_0("        hard %d: ", j);
    }
    if(j >= 10) 
    {
      VLOG_0("       hard %d: ", j);    
    }

    for(int i = 0; i < hardStrategy[j].size(); i++)
    {
      player_hand_total = j;
      dealer_up_card    = i;   // 

      LOG_0("Double -> runThread()", __FILE__,__LINE__, NULL);
      hardStrategy[player_hand_total][dealer_up_card] = DOUBLE;
      Double                      -> setCards(HARD_HAND_MODE, player_hand_total, dealer_up_card);  // 0 for hard mode, player total, dealer up
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      LOG_0("Stay -> runThread()", __FILE__,__LINE__, NULL);
      hardStrategy[player_hand_total][dealer_up_card] = STAY;
      Stay                        -> setCards(HARD_HAND_MODE, player_hand_total, dealer_up_card);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      LOG_0("Hit -> runThread()", __FILE__,__LINE__, NULL);
      hardStrategy[player_hand_total][dealer_up_card] = HIT;
      Hit                         -> setCards(HARD_HAND_MODE, player_hand_total, dealer_up_card);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      LOG_0("stay->hold %f, hit->hold %f, double->hold %f\n", __FILE__,__LINE__,Stay->hold, Hit->hold, Double->hold );
   
      min = 1000.0;

      if(min >= Stay->hold)
      {
        min = Stay->hold;           
        hardStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O';       
      }
      if(min >= Hit->hold)
      {
        min = Hit->hold;             
        hardStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'X'; 
      }
      if(min >= Double->hold)
      {
        min = Double->hold;       
        hardStrategy[player_hand_total][dealer_up_card] = DOUBLE;
        action = '2'; 
      }
      VLOG_0("%c ", action);
    }
    VLOG_0("\n", NULL);
  }
#endif

#if (SOFT_STRATEGY == 1)
  VLOG_0("\n\n SOFT STRATEGY \n\n", NULL);
  VLOG_0("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n", NULL);

#if (DEBUG==1)
  sleep(2);
#endif


  for(int j = 12; j < softStrategy.size() - 1; j++)
  {
    VLOG_0("        soft %d: ", j);
    for(int i = 0; i < softStrategy[j].size(); i++)
    {
      player_hand_total = j;
      dealer_up_card    = i;   // 

      LOG_0("Double -> runThread()", __FILE__,__LINE__, NULL);
      softStrategy[player_hand_total][dealer_up_card] = DOUBLE;
      Double                      -> setCards(SOFT_HAND_MODE, player_hand_total, dealer_up_card);  // 0 for hard mode, player total, dealer up
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      LOG_0("Stay -> runThread()", __FILE__,__LINE__, NULL);
      softStrategy[player_hand_total][dealer_up_card] = STAY;
      Stay                        -> setCards(SOFT_HAND_MODE, player_hand_total, dealer_up_card);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      LOG_0("Hit -> runThread()", __FILE__,__LINE__, NULL);
      softStrategy[player_hand_total][dealer_up_card] = HIT;
      Hit                         -> setCards(SOFT_HAND_MODE, player_hand_total, dealer_up_card);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      LOG_0("stay->hold %f, hit->hold %f, double->hold %f\n", __FILE__,__LINE__,Stay->hold, Hit->hold, Double->hold );
   
      min = 1000.0;

      if(min >= Stay->hold)
      {
        min = Stay->hold;           
        softStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O'; 
      }
      if(min >= Hit->hold)
      {
        min = Hit->hold;             
        softStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'X'; 
      }
      if(min >= Double->hold)
      {
        min = Double->hold;       
        softStrategy[player_hand_total][dealer_up_card] = DOUBLE;
        action = '2'; 
      }
      VLOG_0("%c ", action);      
    }
    VLOG_0("\n", NULL);    
  }
#endif

#if (SPLIT_STRATEGY == 1)
  VLOG_0("\n\n SPLIT STRATEGY \n\n", NULL);
  VLOG_0("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n", NULL);

#if (DEBUG==1)
  sleep(2);
#endif

  for(int j = 0; j < splitStrategy.size(); j++)
  {
    VLOG_0("       split %d: ", j);
    for(int i = 0; i < hardStrategy[j].size(); i++)
    {
      player_hand_total = j;
      dealer_up_card    = i;   // 

      LOG_0("Double -> runThread()", __FILE__,__LINE__, NULL);
      splitStrategy[player_hand_total][dealer_up_card] = DOUBLE;
      Double                      -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);  // 0 for hard mode, player total, dealer up
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      LOG_0("Stay -> runThread()", __FILE__,__LINE__, NULL);
      splitStrategy[player_hand_total][dealer_up_card] = STAY;
      Stay                        -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      LOG_0("Hit -> runThread()", __FILE__,__LINE__, NULL);
      splitStrategy[player_hand_total][dealer_up_card] = HIT;
      Hit                         -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      LOG_0("Hit -> runThread()", __FILE__,__LINE__, NULL);
      splitStrategy[player_hand_total][dealer_up_card] = SPLIT;
      Split                         -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);
      Split                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Split                         -> runThread();

      LOG_0("stay->hold %f, hit->hold %f, double->hold %f, split->hold %f\n", __FILE__,__LINE__,Stay->hold, Hit->hold, Double->hold );
   
      min = 1000.0;

      if(min >= Stay->hold)
      {
        min = Stay->hold;           
        splitStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O'; 
      }
      if(min >= Hit->hold)
      {
        min = Hit->hold;             
        splitStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'X'; 
      }
      if(min >= Double->hold)
      {
        min = Double->hold;       
        splitStrategy[player_hand_total][dealer_up_card] = DOUBLE;
        action = '2'; 
      }
      if(min >= Split->hold)
      {
        min = Split->hold;       
        splitStrategy[player_hand_total][dealer_up_card] = SPLIT;
        action = 'S'; 
      }
      VLOG_0("%c ", action);           
    }
    VLOG_0("\n", NULL);      
  }
  #endif

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  VLOG_0("\n\nTime difference = %d[s] \n", std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000);  
  Simulator::exportBasicStrategy(tempAces, tempHigh, tempMid, tempLow);

}


