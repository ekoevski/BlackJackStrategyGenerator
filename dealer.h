#ifndef DEALER_H
#define DEALER_H


#include <iostream>
#include <fstream>
#include <string>
#include "shoe.h"
#include "card.h"
#include "logging.h"


using namespace std;


// CLASS DEALER//   DONE
// Declarations
class Dealer
{
  public:
  Card *set_up_card;

  float bet;
  Shoe *theShoe2;
  string cardName;
  bool dealerBust = false;
  bool BlackJack = false;

  float casinoDrop = 0;
  float winLoss = 0;

  int dealerHas = 0; // Dealer hand total 

  vector<Card*> dealerHand;
    
  public:
  Dealer();
  Dealer(Shoe *Shoe);

  float getBet(){return bet;}
  void setBet(float tempBet);
  void getCard();
  void showHand();
  int showUpCard(){return dealerHand[0]->getValue();}
  void pickUpCards();


  bool hasBlackJack(); //
  bool Busted(){ return dealerBust;}
  void addWinloss(float amount) {winLoss += amount;}
  void addCasinoDrop(float amount) {casinoDrop += amount;}
  int dealerFinal() {return dealerHas;} // Shows 0 if hand has not been played.


  void playHand();
  bool isSoft();

  int getHandTotal();

  
};




#endif // BINSEARCH_HPP