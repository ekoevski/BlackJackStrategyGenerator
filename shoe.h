#ifndef SHOE_H
#define SHOE_H


#include <iostream>
#include <fstream>
#include <vector>
#include "card.h"


// This is for analog pin read
#include <stdio.h>
//#include "pico/stdlib.h"
//#include "hardware/gpio.h"
//#include "hardware/adc.h"

using namespace std;


// CLASS SHOE//
// Declarations
class Shoe
{
  public:
  int cutCard;
  int deckNumber;
  vector<Card*> theShoe;
  int buffer = 0;
  int shoeSize = 0;

  // These 5 create and combine into aggregated
  // Created at constructor
  vector<Card*> low;
  vector<Card*> mid;
  vector<Card*> high;
  vector<Card*> aces;

  // Combined mid game
  vector<Card*> agregatedShoe;


  // Indices (used to remove cards from shoe)
  int lowIndex;
  int midIndex;
  int highIndex;
  int acesIndex;

  int random;

  Shoe();
  Shoe(int cutCard, int deckNumber);

  // This is reduced shoe generator
  Shoe(int deckNumber);



  void shuffle();
  void showAllCards();
  Card* drawCard();
  
  void removeLow();
  void removeMid();
  void removeHigh();
  void removeAce(); 
  
  void combineShoe();
  void createShoe(int aces, int high, int mid, int low);
  void createShoeQuick(int aces, int high, int mid, int low);
  Card* drawCardQuick();

};



#endif // BINSEARCH_HPP