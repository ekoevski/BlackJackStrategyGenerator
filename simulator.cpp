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

  printf("\nInitiating deck, total decks: %i total cards: %i\n\n", decks, aces + high + mid + low);
};


// run returns hold %
float Simulator::run(int rounds)
{
  Table_BJ->theDealer->casinoDrop = 0;
  Table_BJ->theDealer->winLoss = 0;
  total_drop = 0;
  total_winloss = 0;
  
  Simulator::counter = 0;
  while (counter < rounds) 
  {
    counter++;
    Table_BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
    Table_BJ->reset();
    Table_BJ->placeCards();
    Table_BJ->displayTable(); 
    Table_BJ->playRound(1);

    if (counter % 1000 == 0) 
    {
      total_drop += Table_BJ->theDealer->casinoDrop;
      total_winloss += Table_BJ->theDealer->winLoss;
      Table_BJ->theDealer->casinoDrop = 0;
      Table_BJ->theDealer->winLoss = 0;
    }

    Table_BJ->clearTable();
  }

  return (total_winloss/total_drop)*100;
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
  
  float debug_casino_drop = 0.0;
  float debug_win_loss = 0.0; 

  int debug_player_card_1 = 690000;
  int debug_player_card_2 = 690000;
  int debug_player_card_3 = 690000;
  int debug_player_card_4 = 690000;
  int debug_player_card_5 = 690000;
  int debug_player_card_6 = 690000;
  int debug_player_card_7 = 690000;
  int debug_player_card_8 = 690000;


  int debug_dealer_card_1 = 690000;
  int debug_dealer_card_2 = 690000;
  int debug_dealer_card_3 = 690000;
  int debug_dealer_card_4 = 690000;
  int debug_dealer_card_5 = 690000;
  int debug_dealer_card_6 = 690000;
  int debug_dealer_card_7 = 690000;
  int debug_dealer_card_8 = 690000;


//1st split
  int debug_2player_card_1 = 690000;
  int debug_2player_card_2 = 690000;
  int debug_2player_card_3 = 690000;
  int debug_2player_card_4 = 690000;
  int debug_2player_card_5 = 690000;
  int debug_2player_card_6 = 690000;
  int debug_2player_card_7 = 690000;
  int debug_2player_card_8 = 690000;


//2nd split
  int debug_3player_card_1 = 690000;
  int debug_3player_card_2 = 690000;
  int debug_3player_card_3 = 690000;
  int debug_3player_card_4 = 690000;
  int debug_3player_card_5 = 690000;
  int debug_3player_card_6 = 690000;
  int debug_3player_card_7 = 690000;
  int debug_3player_card_8 = 690000;

  // Vars for debug --
  int k = 0;
  float old_WL = 0.0;               
  float delta_WL= 0.0;
  Table_BJ->theDealer->winLoss = 0.0;
  // ----           --

  Simulator::counter = 0;
  while (counter < rounds)
  {
    old_WL = Table_BJ->theDealer->winLoss;
    
    counter++;
    Table_BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
    Table_BJ->reset();
    Table_BJ->placeCards();
    Table_BJ->playRound(1);

    if (counter % 50000 == 0)
    {
      total_drop += Table_BJ->theDealer->casinoDrop;
      total_winloss += Table_BJ->theDealer->winLoss;
      printf("drop: %f w/l: %f total W/L (%f)  rounds = %i , low = %i \n",Table_BJ->theDealer->casinoDrop, Table_BJ->theDealer->winLoss, total_winloss/total_drop, counter, Table_BJ->theShoe->lowIndex); 

      Table_BJ->theDealer->casinoDrop = 0;
      Table_BJ->theDealer->winLoss = 0;
    }



  // The following is for debugging only, used to read vector items from player hands
  k = 0;  
  for (Card* aCard: Table_BJ->players[0]->playerHand)
  {
    switch(k)
    {
    case 0:
    debug_player_card_1 = aCard->getValue();
    break;

    case 1:
    debug_player_card_2 = aCard->getValue();
    break;

    case 2:
    debug_player_card_3 = aCard->getValue();
    break;

    case 3:
    debug_player_card_4 = aCard->getValue();
    break;

    case 4:
    debug_player_card_5 = aCard->getValue();
    break;

    case 5:
    debug_player_card_6 = aCard->getValue();
    break;

    case 6:
    debug_player_card_7 = aCard->getValue();
    break;

    case 7:
    debug_player_card_8 = aCard->getValue();
    break;
    }
    k++;
  }

  k = 0;  
  for (Card* aCard: Table_BJ->players[0]->secondSplitHand){
    switch(k){
    case 0:
    debug_2player_card_1 = aCard->getValue();
    break;

    case 1:
    debug_2player_card_2 = aCard->getValue();
    break;

    case 2:
    debug_2player_card_3 = aCard->getValue();
    break;

    case 3:
    debug_2player_card_4 = aCard->getValue();
    break;

    case 4:
    debug_2player_card_5 = aCard->getValue();
    break;

    case 5:
    debug_2player_card_6 = aCard->getValue();
    break;

    case 6:
    debug_2player_card_7 = aCard->getValue();
    break;

    case 7:
    debug_2player_card_8 = aCard->getValue();
    break;
    }
    k++;
  }

  k = 0;  
  for (Card* aCard: Table_BJ->players[0]->thirdSplitHand)
  {
    switch(k){
    case 0:
    debug_3player_card_1 = aCard->getValue();
    break;

    case 1:
    debug_3player_card_2 = aCard->getValue();
    break;

    case 2:
    debug_3player_card_3 = aCard->getValue();
    break;

    case 3:
    debug_3player_card_4 = aCard->getValue();
    break;

    case 4:
    debug_3player_card_5 = aCard->getValue();
    break;

    case 5:
    debug_3player_card_6 = aCard->getValue();
    break;

    case 6:
    debug_3player_card_7 = aCard->getValue();
    break;

    case 7:
    debug_3player_card_8 = aCard->getValue();
    break;
    }
    k++;
  }

  k = 0;
  for (Card* aCard: Table_BJ->theDealer->dealerHand){
    switch(k){
    case 0:
    debug_dealer_card_1 = aCard->getValue();
    break;

    case 1:
    debug_dealer_card_2 = aCard->getValue();
    break;

    case 2:
    debug_dealer_card_3 = aCard->getValue();
    break;

    case 3:
    debug_dealer_card_4 = aCard->getValue();
    break;

    case 4:
    debug_dealer_card_5 = aCard->getValue();
    break;

    case 5:
    debug_dealer_card_6 = aCard->getValue();
    break;

    case 6:
    debug_dealer_card_7 = aCard->getValue();
    break;

    case 7:
    debug_dealer_card_8 = aCard->getValue();
    break;
    }
    k++;
  }


  debug_win_loss = Table_BJ->theDealer->winLoss;
  debug_casino_drop = Table_BJ->theDealer->casinoDrop;
  delta_WL = debug_win_loss - old_WL;    // debug, change in WL

//======================================================
  k = 0; // <<<====================== !!!!!! USE THIS AS A BREAKPOINT FOR DEBUGGING !!!!!!
//======================================================


  debug_player_card_1 = 6900000;
  debug_player_card_2 = 6900000;
  debug_player_card_3 = 6900000;
  debug_player_card_4 = 6900000;
  debug_player_card_5 = 6900000;
  debug_player_card_6 = 6900000;
  debug_player_card_7 = 6900000;
  debug_player_card_8 = 6900000;

  debug_2player_card_1 = 6900000;
  debug_2player_card_2 = 6900000;
  debug_2player_card_3 = 6900000;
  debug_2player_card_4 = 6900000;
  debug_2player_card_5 = 6900000;
  debug_2player_card_6 = 6900000;
  debug_2player_card_7 = 6900000;
  debug_2player_card_8 = 6900000;

  debug_3player_card_1 = 6900000;
  debug_3player_card_2 = 6900000;
  debug_3player_card_3 = 6900000;
  debug_3player_card_4 = 6900000;
  debug_3player_card_5 = 6900000;
  debug_3player_card_6 = 6900000;
  debug_3player_card_7 = 6900000;
  debug_3player_card_8 = 6900000;

  debug_dealer_card_1 = 6900000;
  debug_dealer_card_2 = 6900000;
  debug_dealer_card_3 = 6900000;
  debug_dealer_card_4 = 6900000;
  debug_dealer_card_5 = 6900000;
  debug_dealer_card_6 = 6900000;
  debug_dealer_card_7 = 6900000;
  debug_dealer_card_8 = 6900000;

  Table_BJ->clearTable();
  }
  printf("\n\nSimulator::run completed: hold %f percent, rounds: %d\n\n", (total_winloss/total_drop)*100, rounds);
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

void Simulator::loadBasicStrategy(int tempAces, int tempHigh, int tempMid, int tempLow){
    fstream file;
    string filename = "strategy cards/A" + to_string(tempAces) + "T" + to_string(tempHigh) + "M" + to_string(tempMid) + "L" + to_string(tempLow) + ".txt";

    file.open(filename,ios::in);
    if (file.is_open()){ 

          cout << endl << "HARD STRATEGY" << endl;
          for(int j = 0; j < hardStrategy.size(); j++){
            for(int i = 0; i < hardStrategy[j].size(); i++)
            {
                hardStrategy[j][i] = file.get()-48;
                cout << hardStrategy[j][i] << " ";
            }      
                cout << endl;
          }
          cout << endl << "SOFT STRATEGY" << endl;
          for(int j = 0; j < softStrategy.size(); j++){
            for(int i = 0; i < softStrategy[j].size(); i++)
            {
                softStrategy[j][i] = file.get()-48;
                cout << softStrategy[j][i] << " ";
            }      
            cout << endl;
          }


          cout << endl << "SPLIT STRATEGY" << endl;
          for(int j = 0; j < splitStrategy.size(); j++){
            for(int i = 0; i < splitStrategy[j].size(); i++)
            {
                splitStrategy[j][i] = file.get()-48;
                cout << splitStrategy[j][i] << " ";
            } 
            cout << endl;     
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



  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  // Instantiate calculator oTable_BJbjects
  HoldCalculator* Stay = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);
  HoldCalculator* Hit = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);
  HoldCalculator* Double = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);
  HoldCalculator* Split = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);

  // Create calculator shoe
  Stay->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Hit->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Double->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Split->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);

  
  printf("                *** START OPTIMIZATION (SINGLE-THREAD)    (%i) ROUNDS ***  \n\n", rounds);
  printf(" Aces: %d  High: %d  Mid: %d  Low: %d\n", tempAces, tempHigh, tempMid, tempLow);
  printf("\n 0 = Stay   |   1 = Hit    |  2 = Double  |   4 = Split \n\n");
  printf("\n HARD STRATEGY \n\n");
  printf("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n");

  // Print player card hard values
  for(int j = 5; j < hardStrategy.size(); j++)
  {
    if(j < 10) 
    {
      cout << "        hard " << j << ": ";
    }
    if(j >= 10) 
    {
      cout << "        hard" << j << ": ";
    }

    // TODO: Casino is losing 100% in all three cases
    // Test scenarios (stay, hit, double)
    for(int i = 0; i < hardStrategy[j].size(); i++)
    {
      LOG_0("Double -> runThread()", __FILE__,__LINE__, NULL);
      hardStrategy[j][i] = 2;
      Double                      -> setCards(0, 16, 6);  // 0 for hard mode, player total, dealer up
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      LOG_0("Stay -> runThread()", __FILE__,__LINE__, NULL);
      hardStrategy[j][i] = 0;
      Stay                        -> setCards(0, 16, 6);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      LOG_0("Hit -> runThread()", __FILE__,__LINE__, NULL);
      hardStrategy[j][i] = 1;
      Hit                         -> setCards(0, 16, 6);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      LOG_0("stay->hold %f, hit->hold %f, double->hold %f\n", __FILE__,__LINE__,Stay->hold, Hit->hold, Double->hold );
   
      min = 1000.0;

      if(min >= Stay->hold)
      {
        min = Stay->hold;           
        hardStrategy[j][i] = 0;
      }
      if(min >= Hit->hold)
      {
        min = Hit->hold;             
        hardStrategy[j][i] = 1;
      }
      if(min >= Double->hold)
      {
        min = Double->hold;       
        hardStrategy[j][i] = 2;
      }
      cout << hardStrategy[j][i] << " ";
    }
    cout << endl;
  }

  printf("\n\n SOFT STRATEGY \n\n");
  printf("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n");


  for(int j = 12; j < softStrategy.size() - 2; j++)
  {
    cout << "        soft" << j << ": ";

    for(int i = 0; i < softStrategy[j].size(); i++)
    {
      softStrategy[j][i] = 2;
      Double                      -> setCards(1,20,6);
      Double                      -> setBasicStrategy(Simulator::softStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      softStrategy[j][i] = 0;
      Stay                        -> setCards(1,20,6);
      Stay                        -> setBasicStrategy(Simulator::softStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      softStrategy[j][i] = 1;
      Hit                         -> setCards(1,20,6);
      Hit                         -> setBasicStrategy(Simulator::softStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      min = 1000.0;
      if(min >= Stay->hold)
      {
        min = Stay->hold;           
        softStrategy[j][i] = 0;
      }
      if(min >= Hit->hold)
      {
        min = Hit->hold;             
        softStrategy[j][i] = 1;
      }
      if(min >= Double->hold)
      {
        min = Double->hold;       
        softStrategy[j][i] = 2;
      }
      printf("%d ", softStrategy[j][i] );
    }

    printf("\n");   
  }

  cout << "\n\n SPLIT STRATEGY \n\n";
  cout << "Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n";

  for(int j = 0; j < splitStrategy.size(); j++)
  {
    cout << "       split" << j + 1 << ": ";

    for(int i = 0; i < splitStrategy[j].size(); i++)
    {
      splitStrategy[j][i] = 2;
      Double                      -> setCards(2,20,6);
      Double                      -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Double                      -> runThread();

      splitStrategy[j][i] = 0;
      Stay                        -> setCards(2,20,6);
      Stay                        -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Stay                        -> runThread();

      splitStrategy[j][i] = 1;
      Hit                         -> setCards(2,20,6);
      Hit                         -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Hit                         -> runThread();

      splitStrategy[j][i] = 4;
      Split                       -> setCards(2,20,6);
      Split                       -> setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      Split                       -> runThread();

      min = 1000.0;
      if(min >= Stay->hold){min = Stay->hold;           splitStrategy[j][i] = 0;}
      if(min >= Hit->hold){min = Hit->hold;             splitStrategy[j][i] = 1;}
      if(min >= Double->hold){min = Double->hold;       splitStrategy[j][i] = 2;}
      if(min >= Split->hold){min = Split->hold;         splitStrategy[j][i] = 4;}

      cout << splitStrategy[j][i]<< " ";
    }
      cout << endl;
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "\n\nTime difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000 << "[s]" << std::endl;
  Simulator::exportBasicStrategy(tempAces, tempHigh, tempMid, tempLow);

}




// ======================================================
// =============== OPTIMIZE (MULTI-THREADED) ============
// ======================================================
// Description: Multithreaded approach of the optimize(yada yada)
//              Note: Runs significantly slower than single threaded

void Simulator::optimizeThreaded(int rounds, int tempAces, int tempHigh, int tempMid, int tempLow){


printf("                *** START OPTIMIZATION (MULTITHREADED)    (%i) ROUNDS ***  \n\n", rounds);
printf(" Aces: %d  High: %d  Mid: %d  Low: %d\n", tempAces, tempHigh, tempMid, tempLow);
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  float stayHold = 0;
  float hitHold = 0;
  float doubleHold = 0;
  float splitHold = 0;
  float min = 1000.0;

  HoldCalculator* Stay = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);
  HoldCalculator* Hit = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);
  HoldCalculator* Double = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);
  HoldCalculator* Split = new HoldCalculator(rounds, aces, high, mid, low, "BlackJack", shoeDecks, numberPlayers);

  Stay->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Hit->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Double->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Split->Table_BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);



cout << "\n 0 = Stay   |   1 = Hit    |  2 = Double  |   4 = Split \n\n";

cout << "\n HARD STRATEGY \n\n";

printf("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 5; j < hardStrategy.size() - 1; j++){
    
    if(j < 10) {cout << "        hard " << j << ": ";}
    if(j >= 10) {cout << "        hard" << j << ": ";}

    
    for(int i = 0; i < hardStrategy[j].size(); i++){

      hardStrategy[j][i] = 2;
      Double -> setCards(0, j, i);  // 0 for hard mode, player total, dealer up
      Double->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th3(&HoldCalculator::runThread, Double);

      hardStrategy[j][i] = 0;
      Stay -> setCards(0, j, i);
      Stay->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th1(&HoldCalculator::runThread, Stay);

      hardStrategy[j][i] = 1;
      Hit -> setCards(0, j, i);
      Hit->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th2(&HoldCalculator::runThread, Hit);


      th3.join();
      th1.join();
      th2.join();

      min = 1000.0;
      if(min >= Stay->hold){min = Stay->hold;           hardStrategy[j][i] = 0;}
      if(min >= Hit->hold){min = Hit->hold;             hardStrategy[j][i] = 1;}
      if(min >= Double->hold){min = Double->hold;       hardStrategy[j][i] = 2;}
      cout << hardStrategy[j][i]<< " ";
    }
    cout << endl;
  }



// 
cout << "\n\n SOFT STRATEGY \n\n";

printf("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 13; j < softStrategy.size() - 1; j++){
cout << "        soft" << j << ": ";
    for(int i = 0; i < softStrategy[j].size(); i++){

      softStrategy[j][i] = 2;
      Double-> setCards(1,j-1,i);
      Double->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th3(&HoldCalculator::runThread, Double);

      softStrategy[j][i] = 0;
      Stay -> setCards(1,j-1,i);
      Stay->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th1(&HoldCalculator::runThread,Stay);

      softStrategy[j][i] = 1;
      Hit -> setCards(1,j-1,i);
      Hit->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th2(&HoldCalculator::runThread, Hit);


      th3.join();
      th1.join();
      th2.join();


      min = 1000.0;
      if(min >= Stay->hold){min = Stay->hold;           softStrategy[j][i] = 0;}
      if(min >= Hit->hold){min = Hit->hold;             softStrategy[j][i] = 1;}
      if(min >= Double->hold){min = Double->hold;       softStrategy[j][i] = 2;}
      cout << softStrategy[j][i]<< " ";
    }
    cout << endl;   
  }

cout << "\n\n SPLIT STRATEGY \n\n";

printf("Dealer up card: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 0; j < splitStrategy.size(); j++){
cout << "       split" << j << ": ";   
    for(int i = 0; i < splitStrategy[j].size(); i++){



      splitStrategy[j][i] = 2;
      Double->setCards(2,j,i);
      Double->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th3(&HoldCalculator::runThread, Double);

      splitStrategy[j][i] = 0;
      Stay->setCards(2,j,i);
      Stay->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th1(&HoldCalculator::runThread, Stay);

      splitStrategy[j][i] = 1;
      Hit->setCards(2,j,i);
      Hit->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th2(&HoldCalculator::runThread, Hit);

      splitStrategy[j][i] = 4;
      Split->setCards(2,j,i);
      Split->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th4(&HoldCalculator::runThread, Split);

      th3.join();
      th1.join();     
      th4.join();
      th2.join();
 
 

      min = 1000.0;
      if(min >= Stay->hold){min = Stay->hold;           splitStrategy[j][i] = 0;}
      if(min >= Hit->hold){min = Hit->hold;             splitStrategy[j][i] = 1;}
      if(min >= Double->hold){min = Double->hold;       splitStrategy[j][i] = 2;}
      if(min >= Split->hold){min = Split->hold;         splitStrategy[j][i] = 4;}

      cout << splitStrategy[j][i]<< " ";
    }
    cout << endl;
  }
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
std::cout << "\n\nTime difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000 << "[s]" << std::endl;
Simulator::exportBasicStrategy(tempAces, tempHigh, tempMid, tempLow);

}





