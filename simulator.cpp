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
// ===============Print Basic STRATEGY==================
// ======================================================
// precondition: has a basic strategy card vectors (soft, hard, split)
// postcondition: gives hold % for current basic strategy card
void Simulator::printBasicStrategy()
{
  #if (DEBUG == 1)
  VLOG_1("\nHARD STRATEGY\n", NULL);
  for(int j = 0; j < hardStrategy.size(); j++)
  {
    for(int i = 0; i < hardStrategy[j].size(); i++)
    {
     VLOG_1("%d ", hardStrategy[j][i]);                 
    }      
    VLOG_1("\n", NULL); 
  }
  VLOG_1("\nSOFT STRATEGY\n", NULL);          
  for(int j = 0; j < softStrategy.size(); j++)
  {
    for(int i = 0; i < softStrategy[j].size(); i++)
    {
      VLOG_1("%d ", softStrategy[j][i]);                 
    }      
    VLOG_1("\n", NULL); 
  }

  VLOG_1("\nSPLIT STRATEGY\n", NULL);
  for(int j = 0; j < splitStrategy.size(); j++)
  {
    for(int i = 0; i < splitStrategy[j].size(); i++)
    {
      VLOG_1("%d ", splitStrategy[j][i], NULL);                
    } 
    VLOG_1("\n", NULL);
  }
  #endif
}


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
// =============== OPTIMIZE Multithreaded x7 ==============
// ======================================================
// precondition: Changes values of current basic strategy vector and 
//               tests to determine if hit, stay, double, or split
//               is the most lucrative player move.
// postcondition: Saves basic strategy card in /strategy cards as text file
//               which can be loaded using loadBasicStrategy(yada yada)

void Simulator::optimize_multithreaded_X7(int rounds, int tempAces, int tempHigh, int tempMid, int tempLow)
{
  float stayHold              = 0;
  float hitHold               = 0;
  float doubleHold            = 0;
  float splitHold             = 0;
  float min                   = 10000000.0;
  int player_hand_total       = 0;
  int dealer_up_card          = 0;
  char action                 ='Q';

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  // Instantiate calculator oTable_BJbjects
  HoldCalculator* Stay     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Stay", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  
  HoldCalculator* Hit     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Hit", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  
  HoldCalculator* Double     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Double", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  
  HoldCalculator* Split     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Split", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  


  
  VLOG_0("                *** START OPTIMIZATION (MULTIx7-THREAD)    (%i) ROUNDS ***  \n\n", rounds);
  VLOG_0(" Aces: %d  High: %d  Mid: %d  Low: %d\n", tempAces, tempHigh, tempMid, tempLow);
  VLOG_0("\n O = Stay   |   X = Hit    |  2 = Double  |   S = Split \n\n", NULL);

#if (HARD_STRATEGY == 1)
  VLOG_0("\n HARD STRATEGY \n\n", NULL);
  VLOG_0("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n", NULL);
#if (DEBUG==1)
  VLOG_2("\n\n               ---- START DEBUG LOG ----", NULL);
    VLOG_2("\n\n                            House edge:       Double,          Hit,          Stay\n", NULL);
  sleep(1);
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
      player_hand_total = j;  // Hard code this to debug player hand total otherwise j
      dealer_up_card    = i;   //  Hard code these to debug up card   otherwise i

      hardStrategy[player_hand_total][dealer_up_card] = DOUBLE;
      #if (DEBUG == 1)
      VLOG_1("\n\n              (DOUBLE) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      VLOG_2("Hands > player hand: %d    dealer up: %d     ==   ", player_hand_total, dealer_up_card + 1);
      printBasicStrategy();
      sleep(1);
      #endif     

      Double                      -> setCards(HARD_HAND_MODE, player_hand_total, dealer_up_card);  // 0 for hard mode, player total, dealer up
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      hardStrategy[player_hand_total][dealer_up_card] = STAY;
      #if (DEBUG == 1)
      VLOG_1("\n\n              (STAY) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      VLOG_2("%.2f         ", Double->total_win_loss);
      printBasicStrategy();
      sleep(1);
      #endif

      Stay                        -> setCards(HARD_HAND_MODE, player_hand_total, dealer_up_card);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      hardStrategy[player_hand_total][dealer_up_card] = HIT;
      #if (DEBUG == 1)
      VLOG_2("%.2f          ", Stay->total_win_loss);      
      VLOG_1("\n\n              (HIT) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      printBasicStrategy();
      sleep(1);
      #endif  
      
      Hit                         -> setCards(HARD_HAND_MODE, player_hand_total, dealer_up_card);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      LOG_0("stay->hold %f, hit->hold %f, double->hold %f\n", __FILE__,__LINE__,Stay->hold, Hit->hold, Double->hold );
      #if(DEBUG == 1)
      VLOG_2("%.2f           ", Hit->total_win_loss);
      #endif
      min = 10000000.0;

      if(min >= Stay->total_win_loss)
      {
        min = Stay->total_win_loss;           
        hardStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O';       
      }
      if(min >= Hit->total_win_loss)
      {
        min = Hit->total_win_loss;             
        hardStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'X'; 
      }
      if(min >= Double->total_win_loss)
      {
        min = Double->total_win_loss;       
        hardStrategy[player_hand_total][dealer_up_card] = DOUBLE;
        action = 'D'; 
      }
      #if(DEBUG == 1)
      VLOG_2(" selected: %c \n", action);
      #endif
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
   
      min = 100000000.0;

      if(min >= Stay->total_win_loss)
      {
        min = Stay->total_win_loss;           
        softStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O'; 
      }
      if(min >= Hit->total_win_loss)
      {
        min = Hit->total_win_loss;             
        softStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'X'; 
      }
      if(min >= Double->total_win_loss)
      {
        min = Double->total_win_loss;       
        softStrategy[player_hand_total][dealer_up_card] = DOUBLE;
        action = 'D'; 
      }
      VLOG_0("%c ", action);      
    }
    VLOG_0("\n", NULL);    
  }
#endif

#if (SPLIT_STRATEGY == 1)
  VLOG_0("\n\n SPLIT STRATEGY \n\n", NULL);
  VLOG_0("(Note: P indicates play as a normal hand)\n", NULL);
  VLOG_0("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n", NULL);

#if (DEBUG==1)
  VLOG_2("\n\n               ---- START DEBUG LOG ----", NULL);
  VLOG_2("\n\n                            House edge:     Double,        Stay,           Hit           Split\n", NULL);
  sleep(1);
#endif
  for(int j = 0; j < splitStrategy.size(); j++)
  {
    VLOG_0("       split %d,%d: ", j + 1, j + 1);
    for(int i = 0; i < hardStrategy[j].size(); i++)
    {
      player_hand_total = j;   //j  
      dealer_up_card    = i;   //i 

      splitStrategy[player_hand_total][dealer_up_card] = DOUBLE;

      #if (DEBUG == 1)
      VLOG_1("\n\n              (DOUBLE) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      VLOG_2("Hands > player hand: %d,%d    dealer up: %d  == ", player_hand_total + 1, player_hand_total + 1, dealer_up_card + 1);
      printBasicStrategy();
      sleep(1);
      #endif  

      Double                      -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);  // 0 for hard mode, player total, dealer up
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double->Calculator_table->force_mode = true;
      Double->Calculator_table->force_mode_value = DOUBLE;
      Double                      -> runThread();

      splitStrategy[player_hand_total][dealer_up_card] = STAY;

      #if (DEBUG == 1)
      VLOG_1("\n\n              (STAY) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      VLOG_2("%.2f        ", Double->total_win_loss);
      printBasicStrategy();
      sleep(1);
      #endif

      Stay                        -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay->Calculator_table->force_mode = true;
      Stay->Calculator_table->force_mode_value = STAY;
      Stay                        -> runThread();

      splitStrategy[player_hand_total][dealer_up_card] = HIT;

      #if (DEBUG == 1)
      VLOG_2("%.2f        ", Stay->total_win_loss);      
      VLOG_1("\n\n              (HIT) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      printBasicStrategy();
      sleep(1);
      #endif 

      Hit                         -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit->Calculator_table->force_mode = true;
      Hit->Calculator_table->force_mode_value = HIT;
      Hit                         -> runThread();

      splitStrategy[player_hand_total][dealer_up_card] = SPLIT;

      #if (DEBUG == 1)
      VLOG_2("%.2f       ", Hit->total_win_loss);      
      VLOG_1("\n\n              (SPLIT) CALCULATOR STRATEGY)\n\n",__FILE__, __LINE__, NULL);
      printBasicStrategy();
      sleep(1);
      #endif 

      Split                         -> setCards(SPLIT_HAND_MODE, player_hand_total, dealer_up_card);
      Split                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Split->Calculator_table->force_mode = true;
      Split->Calculator_table->force_mode_value = SPLIT;
      Split                         -> runThread();

      #if(DEBUG == 1)
      VLOG_2("%.2f      ", Split->total_win_loss);
      #endif

      LOG_0("stay->hold %f, hit->hold %f, double->hold %f, split->hold %f\n", __FILE__,__LINE__,Stay->hold, Hit->hold, Double->hold );

      min = 100000000.0;

      if(min >= Stay->total_win_loss)
      {
        min = Stay->total_win_loss;           
        splitStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O'; 
      }
      if(min >= Hit->total_win_loss)
      {
        min = Hit->total_win_loss;             
        splitStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'P'; 
      }
      if(min >= Double->total_win_loss)
      {
        min = Double->total_win_loss;       
        splitStrategy[player_hand_total][dealer_up_card] = DOUBLE;
        action = 'D'; 
      }
      if(min >= Split->total_win_loss)
      {
        min = Split->total_win_loss;       
        splitStrategy[player_hand_total][dealer_up_card] = SPLIT;
        action = 'S'; 
      }
      VLOG_0("%c ", action);   
      #if(DEBUG == 1)
      VLOG_2("selected: %c \n", action);
      #endif        
    }
    VLOG_0("\n", NULL);      
  }
  #endif

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  VLOG_0("\n\nTime difference = %d[s] \n", std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000);  
  Simulator::exportBasicStrategy(tempAces, tempHigh, tempMid, tempLow);
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
  HoldCalculator* Stay     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Stay", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  
  HoldCalculator* Hit     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Hit", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  
  HoldCalculator* Double     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Double", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  
  HoldCalculator* Split     = new HoldCalculator(rounds, tempAces, tempHigh, tempMid, tempLow, "Split", shoeDecks, numberPlayers, Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);  


  
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

      if(min >= Stay->total_win_loss)
      {
        min = Stay->total_win_loss;           
        hardStrategy[player_hand_total][dealer_up_card] = STAY;
        action = 'O';       
      }
      if(min >= Hit->total_win_loss)
      {
        min = Hit->total_win_loss;             
        hardStrategy[player_hand_total][dealer_up_card] = HIT;
        action = 'X'; 
      }
      if(min >= Double->total_win_loss)
      {
        min = Double->total_win_loss;       
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


