#include "dealer.h"
#include <iostream>
#include <string>
#include <fstream>
//#include "card.h"

using namespace std;


// CLASS DEALER //
// Implementation

Dealer::Dealer(Shoe *shoe)
{
    Dealer::theShoe2 = shoe;
    Dealer::set_up_card = new Card(3,3);
};

Dealer::Dealer()
{};

void Dealer::setBet(float tempBet){
    Dealer::bet = tempBet;
}

void Dealer::getCard(){
    Dealer::dealerHand.push_back(Dealer::theShoe2->drawCard());
}


void Dealer::showHand(){
    #if DEBUG
        string cardName2;
        LOG_FLAT("dealer_card: ", __FILE__, __LINE__,NULL);        
        for(Card *aCard  : Dealer::dealerHand){
            printf("%s ", aCard->getName().c_str());
        }
        printf("\n");
    #else
        return;
    #endif
}


// ======================================================
// ============     GET_HAND_TOTAL     ==================
// ======================================================
// Description: Returns value 1 - 11, (does not account +10 from aces)

int Dealer::getHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Dealer::dealerHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}


// CLEAR DEALER HAND
//===================
void Dealer::pickUpCards(){
    Dealer::dealerHand.clear();
}


// CHECK DEALER BLACKJACK
// ======================= 
bool Dealer::hasBlackJack(){
    
    int firstTwoCards = dealerHand[0]->getValue() + dealerHand[1]->getValue() + 10;
    if(firstTwoCards == 21 && Dealer::isSoft()){
        Dealer::BlackJack = true;
        return true;
    }
    Dealer::BlackJack = false;
    return false;
}




// ======================================================
// ============     PLAY_DEALER_HAND     ==================
// ======================================================
// Description: Dealer plays entire hand until makes or busts a hand

void Dealer::playHand(){

    Dealer::dealerBust = false;

    dealerHas = getHandTotal();

    if(Dealer::hasBlackJack()){
        return;
    }


    if(isSoft()) {
        dealerHas += 10;
    }

    while(dealerHas <= 17){
        if(!isSoft() && (dealerHas) == 17){
            break;
        }
        Dealer::dealerHand.push_back(theShoe2->drawCard());
        Dealer::showHand();
        dealerHas = getHandTotal();
        if(isSoft()) {
         dealerHas += 10;
        }
    }

    if(dealerHas > 21) {
        Dealer::dealerBust = true;
    } 
}




// ======================================================
// ============     DEALER_HAND_IS_SOFT    ==============
// ======================================================
// Description: Dealer plays entire hand until makes or busts a hand

bool Dealer::isSoft(){
    for (Card* aCard : dealerHand){
        if(aCard->getValue() == 1){
            if(Dealer::getHandTotal() <= 11){
                return true;
            }
        }
    }  
    return false;
}