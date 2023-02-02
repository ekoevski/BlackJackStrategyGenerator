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

/////////////////////////////////////////////////////////////////
/////////     CONSTRUCTORS     //////////////////////////////////
/////////////////////////////////////////////////////////////////


Shoe::Shoe()   //Default, I am using this to test
{
  // Create shoe (vector of cards)

  for(int j = 0; j < 1; j++)
  {
    for(int k = 1; k <= 4; k++)
    {
      for(int i = 1; i <= 13; i++)
      {
        Card *aCard = new Card(6,k);
        theShoe.push_back(aCard);
        low.push_back(aCard);
        mid.push_back(aCard);
        high.push_back(aCard);
        aces.push_back(aCard);

      }        
    }
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  srand(seed);

  random_shuffle(aces.begin(), aces.end());
  random_shuffle(low.begin(), low.end());
  random_shuffle(mid.begin(), mid.end());
  random_shuffle(high.begin(), high.end());



    //printf("\nDeck generation successful\n");
  //printf("Generating indices;");
  Shoe::lowIndex = low.size();
  Shoe::midIndex = mid.size();
  Shoe::highIndex = high.size();
  Shoe::acesIndex = aces.size();

  //printf(" \n Indices LOW: %d MID: %d HIGH: %d ACES: %d\n", lowIndex, midIndex, highIndex, acesIndex);
};




Shoe::Shoe(int deckNum)   // Creates 4 vectors; low, mid high and aces, decks
{
  deckNumber = deckNum;
  // Create shoe (vector of cards)

  //printf("\nGenerating LOW, MID, HIGH, ACES \n\n");
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



  //printf("\nDeck generation successful\n");
  //printf("Generating indices;");
  Shoe::lowIndex = low.size();
  Shoe::midIndex = mid.size();
  Shoe::highIndex = high.size();
  Shoe::acesIndex = aces.size();

  Shoe::combineShoe(); // generate shoe from each category


  //printf(" \n Indices LOW: %d MID: %d HIGH: %d ACES: %d\n", lowIndex, midIndex, highIndex, acesIndex);
};





/////////////////////////////////////////////////////////////////
/////////     METHODS          //////////////////////////////////
/////////////////////////////////////////////////////////////////

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
    //low.shuffle();   YOU NEED TO SHUFFLE THE LOW, MID AND HIGH otherwise you'll be pulling sequentially and throw off the randomisation
    theShoe.push_back(low[i]);
  }

  for(int i = 0; i < Shoe::midIndex; i++){
    //mid.shuffle();
    theShoe.push_back(mid[i]);
  }

  for(int i = 0; i < Shoe::highIndex; i++){
    //high.shuffle();
    theShoe.push_back(high[i]);
  }

  for(int i = 0; i < Shoe::acesIndex; i++){
    //aces.shuffle();
    theShoe.push_back(aces[i]);
  }

  Shoe::shuffle();    // Shuffle the shoe after combine

}



// Used for buttons to remove and recreate deck
void Shoe::removeLow(){
  Shoe::lowIndex--;
  //printf(" .. Shoe::removeLow().. LOW: %d", lowIndex);
  combineShoe();
}

void Shoe::removeMid(){
  Shoe::midIndex--;
  //printf(" .. Shoe::removeMid().. MID: %d", midIndex);
  combineShoe();
}

void Shoe::removeHigh(){
  Shoe::highIndex--;
  //printf(" .. Shoe::removeHigh().. High: %d", highIndex);
  combineShoe();
}

void Shoe::removeAce(){
  Shoe::acesIndex--;
  //printf(" .. Shoe::removeAce().. Ace: %d", acesIndex);
  combineShoe();
}


// Creates auxiliarrly shoe shoe using current indices
void Shoe::combineShoe()
{
  Shoe::theShoe.clear();
  for(int i = 0; i < Shoe::lowIndex; i++){
    //low.shuffle();   YOU NEED TO SHUFFLE THE LOW, MID AND HIGH otherwise you'll be pulling sequentially and throw off the randomisation
    theShoe.push_back(low[i]);
  }

  for(int i = 0; i < Shoe::midIndex; i++){
    //mid.shuffle();
    theShoe.push_back(mid[i]);
  }

  for(int i = 0; i < Shoe::highIndex; i++){
    //high.shuffle();
    theShoe.push_back(high[i]);
  }

  for(int i = 0; i < Shoe::acesIndex; i++){
    //aces.shuffle();
    theShoe.push_back(aces[i]);
  }

  Shoe::shuffle();    // Shuffle the shoe after combine



  //printf(" \n .. Total Cards: %lu " , theShoe.size());
  //printf(" \n Indices LOW: %d MID: %d HIGH: %d ACES: %d\n", lowIndex, midIndex, highIndex, acesIndex);

};



//Prints all cards through USB
void Shoe::showAllCards()
{
    std::vector<Card*>::iterator it = theShoe.begin();
    
    int spacer = 0;
    string cardType;

    for (it = theShoe.begin(); it != end (theShoe); ++it) 
    {
        if(spacer % 7 == 0){
          //printf("\n");
          }
        cardType = (*it)->getName();
        //printf("%s ", cardType.c_str());
        spacer++;
    }
};


// Randomizes the cards in the shoe
void Shoe::shuffle()
{
  
  //printf(" ..RSHFL ..");

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  random_shuffle(theShoe.begin(), theShoe.end());


};


// Draws a card fromt the deck
Card* Shoe::drawCard()
{
  Shoe::buffer++;
  //printf("\n Shoe::drawCard() .. ");
  //int n = theShoe.size();

  if(Shoe::buffer > 4){
    Shoe::buffer = 0;
  Shoe::shuffle();                      // remove this if you remove bottom if you want cards removed from deck
      
  }
  Card* temp = theShoe[Shoe::buffer];
  //theShoe.pop_back();               // add this YOU WANT TO REMOVE CARDS FROM DECK
  //printf("%d " , temp->getValue());
  return temp;
};



// Draws a card fromt the deck
Card* Shoe::drawCardQuick()
{
  srand((unsigned) time(NULL));

  Shoe::random = rand() % shoeSize;
  Card* temp = theShoe[Shoe::random];
  return temp;
};
