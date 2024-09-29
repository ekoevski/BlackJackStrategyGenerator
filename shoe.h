#ifndef SHOE_H
#define SHOE_H


#include <iostream>
#include <fstream>
#include <vector>
#include "card.h"
#include "logging.h"


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
  vector<Card*> min;  
  vector<Card*> low;
  vector<Card*> mid;
  vector<Card*> high;
  vector<Card*> aces;

  // Combined mid game
  vector<Card*> agregatedShoe;


  // Indices (used to remove cards from shoe)
  int minIndex;
  int lowIndex;
  int midIndex;
  int highIndex;
  int acesIndex;

  int random;

  Shoe(int deckNumber);
  void shuffle();
  void showAllCards();
  Card* drawCard();
  void combineShoe();
  void createShoe(int aces, int high, int mid, int low, int min);

};



#endif // BINSEARCH_HPP