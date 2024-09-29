#include "card.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include <fstream>

// For vector shuffling
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <chrono>

using namespace std;

            /////////////////////////////////////////
            /////////     CONSTRUCTORS     //////////
            /////////////////////////////////////////
Shoe::Shoe(int deckNum)  
{
  deckNumber = deckNum;

  for(int j = 0; j < deckNumber; j++)
  {
    for(int k = 1; k <= 4; k++)
    {
      for(int i = 1; i <= 13; i++)
      {
        Card *aCard = new Card(i,k);
        theShoe.push_back(aCard);

        if(i == 1         ){aces.push_back(aCard);}
        if(i > 1 && i < 7 ){low.push_back(aCard);}
        if(i > 6 && i < 10){mid.push_back(aCard);}
        if(i > 9          ){high.push_back(aCard);}
      }        
    }
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  
  random_shuffle(aces.begin(), aces.end());
  random_shuffle(low.begin(), low.end());
  random_shuffle(mid.begin(), mid.end());
  random_shuffle(high.begin(), high.end());


  Shoe::lowIndex = low.size();
  Shoe::midIndex = mid.size();
  Shoe::highIndex = high.size();
  Shoe::acesIndex = aces.size();

  Shoe::combineShoe(); 

};
                ///////////////////////////////////////
                /////////     METHODS          ////////
                ///////////////////////////////////////

// ======================================================
// ============     CREATES CUSTOM SHOE    ==============
// ======================================================
// Description: Creates custom shoe dependent on the number of
// aces, high, mid and low cards which are to be entered as parameters
// output: Shoe::theShoe

void Shoe::createShoe(int tempAces, int tempHigh, int tempMid, int tempLow){
    
    Shoe::theShoe.clear();
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    
    random_shuffle(aces.begin(), aces.end());
    random_shuffle(low.begin(), low.end());
    random_shuffle(mid.begin(), mid.end());
    random_shuffle(high.begin(), high.end());

    Shoe::lowIndex = tempLow;
    Shoe::midIndex = tempMid;
    Shoe::highIndex = tempHigh;
    Shoe::acesIndex = tempAces;

    Shoe::shoeSize = tempLow + tempMid + tempHigh + tempAces;


  for(int i = 0; i < Shoe::lowIndex; i++){
    theShoe.push_back(low[i]);
  }

  for(int i = 0; i < Shoe::midIndex; i++){
    theShoe.push_back(mid[i]);
  }

  for(int i = 0; i < Shoe::highIndex; i++){
    theShoe.push_back(high[i]);
  }

  for(int i = 0; i < Shoe::acesIndex; i++){
    theShoe.push_back(aces[i]);
  }

  Shoe::shuffle();    // Shuffle the shoe after combine
}

// ======================================================
// ==   (SUBROUTINE)   COMBINE SHOE        ==============
// ======================================================
// Description: Creates custom shoe dependent on the number of
// aces, high, mid and low cards
// Subroutine of createShoe(yada yada)


void Shoe::combineShoe()
{
  Shoe::theShoe.clear();

  for(int i = 0; i < Shoe::lowIndex; i++){
    theShoe.push_back(low[i]);
  }

  for(int i = 0; i < Shoe::midIndex; i++){
    theShoe.push_back(mid[i]);
  }

  for(int i = 0; i < Shoe::highIndex; i++){
    theShoe.push_back(high[i]);
  }

  for(int i = 0; i < Shoe::acesIndex; i++){
    theShoe.push_back(aces[i]);
  }

  Shoe::shuffle();    // Shuffle the shoe after combine


};

// PRINT ALL CARDS LEFT IN THE CURRENT DECK
// ========================================
// Description: Prints in a square matrix in output for debugging

void Shoe::showAllCards()
{
    #if DEBUG
      std::vector<Card*>::iterator it = theShoe.begin();
      
      int spacer = 0;
      string cardType;

      for (it = theShoe.begin(); it != end (theShoe); ++it) 
      {
          if(spacer % 7 == 0){
            if(spacer != 0)
            {
             printf("\n");              
            }
            LOG_FLAT("", __FILE__, __LINE__, NULL);
            }
          cardType = (*it)->getName();
          printf("%s ", cardType.c_str());
          spacer++;
      }
      printf("\n");

    #else
      return;
    #endif
};

// RESHUFFLE THE DECK
// ==================
void Shoe::shuffle()
{
  LOG_1("Reshuffle", __FILE__, __LINE__, NULL);
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  random_shuffle(theShoe.begin(), theShoe.end());
  #if(REVEAL_SHOE_CARDS == 1)
  showAllCards();
  #endif
};

// DRAW A RANDOM CARD FROM THE DECK
// ================================
// Return: A random card object

Card* Shoe::drawCard()
{
  Shoe::buffer++;

  if(Shoe::buffer > 4){
    Shoe::buffer = 0;
    Shoe::shuffle();                      // remove this if you remove bottom if you want cards removed from deck
  }
  Card* temp = theShoe[Shoe::buffer];
  //theShoe.pop_back();               // add this YOU WANT TO REMOVE CARDS FROM DECK
  LOG_1("drawCard() = card: %s value: %d",__FILE__, __LINE__, temp->getName().c_str(), temp->getValue());

  return temp;
};
