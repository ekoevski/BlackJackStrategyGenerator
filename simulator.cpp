#include "table.h"
#include "simulator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include "HoldCalculator.h"

using namespace std;


// Constructors
//=======================

Simulator::Simulator()
{};




Simulator::Simulator(string name, int decks, int players)
{
 Simulator::BJ = new Table(name, decks, players);
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
float Simulator::run(int rounds){
  BJ->theDealer->casinoDrop = 0;
  BJ->theDealer->winLoss = 0;
  total_drop = 0;
  total_winloss = 0;
  
  Simulator::counter = 0;
  while (counter < rounds) {
    counter++;
    BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);

    BJ->placeCards();
    BJ->displayTable();

    BJ->playRound(1);




    if (counter % 1000 == 0) {
      //BJ->printDrop();
      total_drop += BJ->theDealer->casinoDrop;
      total_winloss += BJ->theDealer->winLoss;
      //printf(" total W/L (%f)  rounds = %i , low = %i \n", total_winloss/total_drop, counter, BJ->theShoe->lowIndex); 

      BJ->theDealer->casinoDrop = 0;
      BJ->theDealer->winLoss = 0;
    }

    BJ->clearTable();
  }

    //printf("\n\nSimulator::run completed: hold %f percent, rounds: %d\n\n", (total_winloss/total_drop)*100, rounds);
    return (total_winloss/total_drop)*100;
};

// run returns hold %
void Simulator::testCurrentStrategy(int rounds){
  BJ->theDealer->casinoDrop = 0;
  BJ->theDealer->winLoss = 0;
  total_drop = 0;
  total_winloss = 0;
   
  Simulator::counter = 0;
  while (counter < rounds) {
    counter++;
    BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);

    BJ->placeCards();
    BJ->displayTable();

    BJ->playRound(1);




    if (counter % 50000 == 0) {
      //BJ->printDrop();
      total_drop += BJ->theDealer->casinoDrop;
      total_winloss += BJ->theDealer->winLoss;
      printf("drop: %f w/l: %f total W/L (%f)  rounds = %i , low = %i \n",BJ->theDealer->casinoDrop, BJ->theDealer->winLoss, total_winloss/total_drop, counter, BJ->theShoe->lowIndex); 

      BJ->theDealer->casinoDrop = 0;
      BJ->theDealer->winLoss = 0;
    }

    BJ->clearTable();
  }

    printf("\n\nSimulator::run completed: hold %f percent, rounds: %d\n\n", (total_winloss/total_drop)*100, rounds);

};

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



void Simulator::loadBasicStrategy(int tempAces, int tempHigh, int tempMid, int tempLow){
    fstream file;
    string filename = "strategy cards/A" + to_string(tempAces) + "T" + to_string(tempHigh) + "M" + to_string(tempMid) + "L" + to_string(tempLow) + ".txt";

    file.open(filename,ios::in);
    if (file.is_open()){ 
        
        //getline(file, tp);

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


void Simulator::optimize(int rounds){


printf("                *** START OPTIMIZATION    (%i) ROUNDS ***  \n\n", rounds);
printf(" Aces: %d  High: %d  Mid: %d  Low: %d\n", aces, high, mid, low);
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  float stayHold = 0;
  float hitHold = 0;
  float doubleHold = 0;
  float splitHold = 0;
  float min = 1000.0;
  thread th1;
  thread th2;
  thread th3;

cout << "\n HARD STRATEGY \n\n";

printf("Dealer: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 4; j < hardStrategy.size() - 1; j++){
    cout << "hard" << j << ": ";
    for(int i = 0; i < hardStrategy[j].size(); i++){



      hardStrategy[j][i] = 0;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      stayHold = Simulator::run(rounds);
      //th1(&Simulator::run, 100);
      hardStrategy[j][i] = 1;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      hitHold = Simulator::run(rounds);

      hardStrategy[j][i] = 2;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      doubleHold = Simulator::run(rounds);



      //printf("\nstayHold %f, hitHold %f, doubleHold %f \n", stayHold, hitHold, doubleHold);


      min = 1000.0;
      if(min >= stayHold){min = stayHold;           hardStrategy[j][i] = 0;}
      if(min >= hitHold){min = hitHold;             hardStrategy[j][i] = 1;}
      if(min >= doubleHold){min = doubleHold;       hardStrategy[j][i] = 2;}
      //printf("min %f", min);
      cout << hardStrategy[j][i]<< " ";
    }
    cout << endl;
  }





cout << "\n\n SOFT STRATEGY \n\n";

printf("Dealer: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 12; j < softStrategy.size()-1; j++){
        cout << "soft" << j << ": ";
    for(int i = 0; i < softStrategy[j].size(); i++){
      softStrategy[j][i] = 0;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      stayHold = Simulator::run(rounds);

      softStrategy[j][i] = 1;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      hitHold = Simulator::run(rounds);

      softStrategy[j][i] = 2;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      doubleHold = Simulator::run(rounds);

      min = 1000.0;
      if(min >= stayHold){min = stayHold;           softStrategy[j][i] = 0;}
      if(min >= hitHold){min = hitHold;             softStrategy[j][i] = 1;}
      if(min >= doubleHold){min = doubleHold;       softStrategy[j][i] = 2;}
      cout << softStrategy[j][i]<< " ";
    }
    cout << endl;   
  }

cout << "\n\n SPLIT STRATEGY \n\n";

printf("Dealer: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 0; j < splitStrategy.size(); j++){
        cout << "splt" << j << ": ";    
    for(int i = 0; i < splitStrategy[j].size(); i++){
      splitStrategy[j][i] = 0;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      stayHold = Simulator::run(rounds);

      splitStrategy[j][i] = 1;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      hitHold = Simulator::run(rounds);

      splitStrategy[j][i] = 2;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      doubleHold = Simulator::run(rounds);

      splitStrategy[j][i] = 4;
      BJ->setPlayerBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      splitHold = Simulator::run(rounds);


      min = 1000.0;
      if(min >= stayHold){min = stayHold;           splitStrategy[j][i] = 0;}
      if(min >= hitHold){min = hitHold;             splitStrategy[j][i] = 1;}
      if(min >= doubleHold){min = doubleHold;       splitStrategy[j][i] = 2;}
      if(min >= splitHold){min = splitHold;         splitStrategy[j][i] = 4;}
      cout << splitStrategy[j][i]<< " ";
    }
    cout << endl;
  }
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
std::cout << "\n\nTime difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000 << "[s]" << std::endl;
//Simulator::exportBasicStrategy(aces, high, mid, low);

}




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

  Stay->BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Hit->BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Double->BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);
  Split->BJ->theShoe->createShoe(tempAces, tempHigh, tempMid, tempLow);



cout << "\n HARD STRATEGY \n\n";

printf("Dealer: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 4; j < hardStrategy.size(); j++){
    cout << "hard" << j << ": ";
    for(int i = 0; i < hardStrategy[j].size(); i++){

      hardStrategy[j][i] = 2;
      Double->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th3(&HoldCalculator::runThread, Double);

      hardStrategy[j][i] = 0;
      Stay->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th1(&HoldCalculator::runThread, Stay);

      hardStrategy[j][i] = 1;
      Hit->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th2(&HoldCalculator::runThread, Hit);


      th3.join();
      th1.join();
      th2.join();

      //printf("\nstayHold %f, hitHold %f, doubleHold %f \n", stayHold, hitHold, doubleHold);


      min = 1000.0;
      //printf("stay->hold %f    hit->hold %f    double->hold %f\n", Stay->hold, Hit->hold, Double->hold);
      if(min >= Stay->hold){min = Stay->hold;           hardStrategy[j][i] = 0;}
      if(min >= Hit->hold){min = Hit->hold;             hardStrategy[j][i] = 1;}
      if(min >= Double->hold){min = Double->hold;       hardStrategy[j][i] = 2;}
      //printf("min %f", min);
      cout << hardStrategy[j][i]<< " ";
    }
    cout << endl;
  }





cout << "\n\n SOFT STRATEGY \n\n";

printf("Dealer: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 12; j < softStrategy.size(); j++){
        cout << "soft" << j << ": ";
    for(int i = 0; i < softStrategy[j].size(); i++){

      softStrategy[j][i] = 2;
      Double->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th3(&HoldCalculator::runThread, Double);

      softStrategy[j][i] = 0;
      Stay->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th1(&HoldCalculator::runThread,Stay);

      softStrategy[j][i] = 1;
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

printf("Dealer: A 2 3 4 5 6 7 8 9 10\n\n");
  for(int j = 0; j < splitStrategy.size(); j++){
        cout << "splt" << j + 1 << ": ";    
    for(int i = 0; i < splitStrategy[j].size(); i++){



      splitStrategy[j][i] = 2;
      Double->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th3(&HoldCalculator::runThread, Double);

      splitStrategy[j][i] = 0;
      Stay->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th1(&HoldCalculator::runThread, Stay);

      splitStrategy[j][i] = 1;
      Hit->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th2(&HoldCalculator::runThread, Hit);

      splitStrategy[j][i] = 4;
      Split->setBasicStrategy(Simulator::hardStrategy, Simulator::softStrategy, Simulator::splitStrategy);
      std::thread th4(&HoldCalculator::runThread, Split);

     th4.join();
      th1.join();
      th2.join();
      th3.join();
 

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





