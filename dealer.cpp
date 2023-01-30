#include "dealer.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;


// CLASS CARD //
// Value: 1 = A, 2 = 2 .... 10 = 10, 11 = J, 12 = Q, 13 = K
// Suit: 1 = Spades, 2 = Hearts, 3 = Clubs, 4 = Diamonds 
// Implementation

Dealer::Dealer(Shoe *shoe)
{
    //printf("\nCreating Dealer\n");
    Dealer::theShoe2 = shoe;

};

Dealer::Dealer()
{};

void Dealer::setBet(float tempBet){
    Dealer::bet = tempBet;
}


// Draws a card from shoe, Player has reference to shoe object
void Dealer::getCard(){
        //printf(" .. Dealer::getCard() .. ");
    Dealer::dealerHand.push_back(Dealer::theShoe2->drawCard());
}



void Dealer::showHand(){
    //printf(" .. Dealer::showHand() .. ");
    string cardName2;
    for(Card *aCard  : Dealer::dealerHand){
        cardName2 = aCard->getName();
        //printf("\n\n=====\n");
        //printf("| %s |", cardName2.c_str());
        //printf("\n=====\n");
    }
}


// Returns the dealer's hand total
int Dealer::getHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Dealer::dealerHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}

void Dealer::pickUpCards(){
    Dealer::dealerHand.clear();
}



bool Dealer::hasBlackJack(){
    
    int firstTwoCards = dealerHand[0]->getValue() + dealerHand[1]->getValue() + 10;
    if(firstTwoCards == 21 && Dealer::isSoft()){
        Dealer::BlackJack = true;
        return true;
    }
    Dealer::BlackJack = false;
    return false;
}


// This is where dealer plays hand by house rules
// =============================================
void Dealer::playHand(){




    Dealer::dealerBust = false;

    //printf("\n\n\n\n ... Dealer::playHand() ...\n           <<<=============| DEALER PLAYS |============>>>\n\n");
    dealerHas = getHandTotal();

    if(Dealer::hasBlackJack()){
        
        //printf("\nDealer got BlackJack.");
        return;
        }


    if(isSoft()) {
        dealerHas += 10;
        //printf(" Add 10, dealer has %d", dealerHas);
    }

    while(dealerHas <= 17){
        if(!isSoft() && (dealerHas) == 17){
            break;
        }

        //printf("\ndraw\n\n");
        Dealer::dealerHand.push_back(theShoe2->drawCard());
        Dealer::showHand();
        dealerHas = getHandTotal();
        if(isSoft()) {
         dealerHas += 10;
        }


    //printf(" \n..dealer has %d", dealerHas);
    }

    if(dealerHas > 21) {
        //printf("\nDealer has BUSTED!!\n");
        Dealer::dealerBust = true;
    }
    //printf(" \n..DEALER FINAL CARDS: %d ", dealerHas);    
}





bool Dealer::isSoft(){
    //printf(" ...   Dealer::IsSoft()");

    for (Card* aCard : dealerHand){
        if(aCard->getValue() == 1){
            if(Dealer::getHandTotal() <= 11){
                //printf(" -> SOFT .... ");
                return true;
            }
        }
    }
    //printf(" -> HARD .... ");        
    return false;
}