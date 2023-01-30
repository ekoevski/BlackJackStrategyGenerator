#ifndef CARD_H
#define CARD_H


#include <iostream>
#include <fstream>
#include <string>


using namespace std;


// CLASS CARD//   DONE
// Declarations
class Card
{
  public:
  int cardValue;
  int cardSuit;
  string cardName;

  Card();
  Card(int value, int suit);

  int getValue(){return cardValue;}
  int getSuit(){return cardSuit;}
  string getName(){return cardName;}
};




#endif // BINSEARCH_HPP