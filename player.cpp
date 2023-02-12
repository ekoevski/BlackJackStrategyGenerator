#include "player.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;


// PLAYER CLASS
// Implementation

// Constructors
//=======================

Player::Player(Shoe *shoe, int seat)
{
    Player::theShoe2 = shoe;
    Player::playerSeat = seat;
};

Player::Player()
{
    Player::playerSeat = 0;
};


// Methods
//========================

void Player::setBets(float tempBet){
    Player::mainBet = tempBet;
    Player::doubleMainBet = 0;

    Player::secondSplitBet = 0;
    Player::doubleSecondSplitBet = 0;
    
    Player::thirdSplitBet = 0;
    Player::doubleThirdSplitBet = 0;
}


float Player::getPlayerTotalBets(){
    return mainBet + doubleMainBet + secondSplitBet + doubleSecondSplitBet + thirdSplitBet + doubleThirdSplitBet;
}




// GETTER: TOTAL FIRST HAND BET
// ============================
float Player::getMainBet(){
    return mainBet + doubleMainBet;
}


// GETTER: TOTAL SECOND HAND BET
// ============================
float Player::getSecondSplitBet(){
    return secondSplitBet + doubleSecondSplitBet;
}


// GETTER: TOTAL THIRD HAND BET
// ============================
float Player::getThirdSplitBet(){
    return thirdSplitBet + doubleThirdSplitBet;
}


// DRAW A CARD FROM DECK AND GIVE TO PLAYER
// ============================
void Player::getCard(){
    Player::playerHand.push_back(Player::theShoe2->drawCard());
}


// PRINT FIRST PLAYER HAND IN OUTPUT
// ================================
void Player::showHand(){
    string cardName2;
    for(Card *aCard  : Player::playerHand){
        cardName2 = aCard->getName();
    }
}



//  FIRST HAND TOTAL (NO ACE ANALYSIS)
// ====================================
// Desctiption: return 1 - 11, does not add +10 if ace present
int Player::getFirstHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Player::playerHand){
        handTotal += aCard->getValue();
    }
    return handTotal; 
}


//  SECOND HAND TOTAL (NO ACE ANALYSIS)
// ====================================
// Desctiption: return 1 - 11, does not add +10 if ace present
int Player::getSecondHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Player::secondSplitHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}


//  THIRD HAND TOTAL (NO ACE ANALYSIS)
// ====================================
// Desctiption: return 1 - 11, does not add +10 if ace present
int Player::getThirdHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Player::thirdSplitHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}



//   =====================================================
//  ========================================================
// ================         FIRST_TWO         ==============
//  ========================================================
//   ======================================================

// Description: (VERY LONG METHOD) Includes, Player AI,
// determines doubles, splits and reads basic strategy cards
// to make decision. 

void Player::firstTwo(int dealerUp){

    firstDouble = false;
    secondDouble = false;
    thirdDouble = false;

    firstBusted = false;
    secondBusted = false;
    thirdBusted = false;

    //Check if blackjack (WORKS)
    if(Player::getFirstHandTotal() == 11 && (playerHand[0]->getValue() == 1 || playerHand[1]->getValue() == 1)){
        Player::firstBJ = true;
        return;
    }


    // For debugging and testing subroutine
    Player:: testFirstHand = playerHand[0]->getValue();
    Player:: testSecondHand = playerHand[1]->getValue();
    Player:: dealerShowing = dealerUp; 



   
    int basicStrategy = 100;
    int firstCard = playerHand[0]->getValue();
    int secondCard = playerHand[1]->getValue();

    int first_hand_total = Player::firstHandFinal();



    // GET BASIC STRATEGY
        if(Player::firstIsSoft()){     
            basicStrategy = Player::basicSoftStrategy[first_hand_total][dealerUp - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[first_hand_total][dealerUp - 1];
        }



    // IF 2 DOUBLE
        if(basicStrategy == 2){
            playerHand.push_back(theShoe2->drawCard());
            Player::doubleMainBet = Player::mainBet; 
            firstDouble = true;

            if(!Player::firstIsSoft() && Player::firstHandFinal() > 21){
                firstBusted = true;  
            }
        return;  
        }


    // IF 1 THEN PLAY THE HAND
        if(basicStrategy == 1){
            playFirstHand();
        return;  
        }



    // IF 0 DO NOTHING
        if(basicStrategy == 0){
        return;  
        }


    // CHECK IF SPLIT (1ST SPLIT)
    if(firstCard == secondCard){
        basicStrategy = Player::basicSplitStrategy[firstCard - 1][dealerUp - 1];

        if(basicStrategy == 2){firstDouble = true;} 
        if(basicStrategy == 4){

            Player::secondSplitBet = Player::mainBet;
            secondSplitHand.push_back(playerHand[1]);   
            playerHand[1] = theShoe2->drawCard();  

            secondSplitHand.push_back(theShoe2->drawCard()); 

            firstCard = playerHand[0]->getValue();
            firstCard = playerHand[1]->getValue();

            int firstSplitCard = secondSplitHand[0]->getValue();
            int secondSplitCard = secondSplitHand[1]->getValue();
            int second_hand_total = Player::secondHandFinal();

        if(Player::secondIsSoft()){   
            basicStrategy = Player::basicSoftStrategy[second_hand_total][dealerUp - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[second_hand_total][dealerUp - 1];
        }

        if(basicStrategy == 2){
            secondSplitHand.push_back(theShoe2->drawCard());
            Player::doubleSecondSplitBet = Player::secondSplitBet;
            secondDouble = true;
            if(!Player::secondIsSoft() && Player::secondHandFinal() > 21){
                secondBusted = true;  
            }

            playFirstHand();
            return; 
        }



            if(firstCard == secondCard){

                basicStrategy = Player::basicSplitStrategy[firstCard-1][Player::dealerShowing - 1];

                if(basicStrategy == 2){secondDouble = true;} 
                if(basicStrategy == 4){

                    Player::thirdSplitBet = Player::mainBet; 

                    thirdSplitHand.push_back(playerHand[1]);         
                    playerHand[1] = theShoe2->drawCard();          

                    thirdSplitHand.push_back(theShoe2->drawCard());      
                    playFirstHand();
                    playSecondHand();
                    playThirdHand();                                
                    return;
                }
            }

            if(firstSplitCard == secondSplitCard){

                basicStrategy = Player::basicSplitStrategy[secondSplitCard-1][Player::dealerShowing - 1];

                if(basicStrategy == 2){thirdDouble = true;} 
                if(basicStrategy == 4){

                    Player::thirdSplitBet = Player::mainBet;  

                    thirdSplitHand.push_back(secondSplitHand[1]);        
                    secondSplitHand[1] = theShoe2->drawCard();           

                    thirdSplitHand.push_back(theShoe2->drawCard());      
                    playFirstHand();
                    playSecondHand();
                    playThirdHand();                                        
                    return;
                }
            }


            playFirstHand();
            playSecondHand();
            return;
        } 
    }
    playFirstHand();
    return;
}















// ======================================================
// ============         PLAY FIRST HAND    ==============
// ======================================================
// Description: PLAYS FIRST / MAIN HAND


void Player::playFirstHand(){

    if(firstDouble){

        playerHand.push_back(theShoe2->drawCard());
        Player::doubleMainBet = Player::mainBet;  
        return;
    }


    int basicStrategy = 100;
    Player::firstBusted = false; 
    while(!Player::firstBusted){
   
            for (Card *aCard : playerHand){
            }
            
        if(Player::firstIsSoft()){  

            basicStrategy = Player::basicSoftStrategy[Player::firstHandFinal()][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::firstHandFinal()][Player::dealerShowing - 1];
        }

        switch(basicStrategy){
            case 0:

            return;
            break;

            case 1:
            playerHand.push_back(theShoe2->drawCard());
            break;

            case 2:
            playerHand.push_back(theShoe2->drawCard());
            break;

            default:
            break;
        }
    
    if(!Player::firstIsSoft() && Player::firstHandFinal() > 21){
        firstBusted = true;
    }

    }

    
}





// ======================================================
// ============       PLAY SECOND  HAND    ==============
// ======================================================
// Description: PLAYS SECOND (SPLIT) HAND


void Player::playSecondHand(){

    if(secondDouble){
        secondSplitHand.push_back(theShoe2->drawCard());
        Player::doubleSecondSplitBet = Player::secondSplitBet;   
        return;        
    }



    int basicStrategy = 100;
    Player::secondBusted = false;
    while(!Player::secondBusted){
  
            for (Card *aCard : secondSplitHand){
            }
            
        if(Player::secondIsSoft()){ 
            basicStrategy = Player::basicSoftStrategy[Player::secondHandFinal()][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::secondHandFinal()][Player::dealerShowing - 1];
        }
        switch(basicStrategy){
            case 0:
            return;
            break;

            case 1:
            secondSplitHand.push_back(theShoe2->drawCard());
            break;

            case 2:
            secondSplitHand.push_back(theShoe2->drawCard());
            break;

            default:
            break;
        }

    if(!Player::secondIsSoft() && Player::secondHandFinal() > 21){
        secondBusted = true;
    }

    }

}









// ======================================================
// ============       PLAY THIRD  HAND    ==============
// ======================================================
// Description: PLAYS THIRD (SPLIT) HAND

void Player::playThirdHand(){

    if(thirdDouble){
        thirdSplitHand.push_back(theShoe2->drawCard());
        Player::doubleThirdSplitBet = Player::thirdSplitBet;
        return;        
    }


    int basicStrategy = 100;
    Player::thirdBusted = false;
    while(!Player::thirdBusted){

            for (Card *aCard : thirdSplitHand){
            }
            


        // Determine basic Strategy move
        if(Player::thirdIsSoft()){   
            basicStrategy = Player::basicSoftStrategy[Player::thirdHandFinal()][Player::dealerShowing - 1];
        } 
        else{
            basicStrategy = Player::basicHardStrategy[Player::thirdHandFinal()][Player::dealerShowing - 1];
        }

        switch(basicStrategy){
            case 0:
            return;
            break;

            case 1:
            thirdSplitHand.push_back(theShoe2->drawCard());
            break;

            case 2:
            thirdSplitHand.push_back(theShoe2->drawCard());
            break;

            default:
            break;
        }
    

    // Check if busted (not soft and over 21)
    if(!Player::thirdIsSoft() && Player::thirdHandFinal() > 21){
        thirdBusted = true; 
    }

    }

      
}




// CHECKS IF FIRST / MAIN HAND IS SOFT
// ===================================
bool Player::firstIsSoft(){

    for (Card* aCard : playerHand){
        if(aCard->getValue() == 1){
            if(Player::getFirstHandTotal() <= 11){
                return true;
            }
        }
    }
    return false;
}



// CHECKS IF SECOND HAND IS SOFT
// ===================================
bool Player::secondIsSoft(){
    
    for (Card* aCard : secondSplitHand){
        if(aCard->getValue() == 1){
            if(Player::getSecondHandTotal() <= 11){
                return true;
            }
        }
    }
    return false;
}


// CHECKS IF THIRD HAND IS SOFT
// ===================================
bool Player::thirdIsSoft(){
    
    for (Card* aCard : thirdSplitHand){
        if(aCard->getValue() == 1){
            if(Player::getThirdHandTotal() <= 11){
                return true;
            }
        }
    }
    return false;
}






// GET FIRST / MAIN HAND CARD TOTAL (ACCOUNTS FOR ACES + 10)
// ==========================================================
int Player::getFirstCardTotal(){
    int total = Player::getFirstHandTotal();
    if(Player::firstIsSoft() && total <= 11){
        total += 10;
    }
    return total;
}




// GET SECOND HAND CARD TOTAL (ACCOUNTS FOR ACES + 10)
// ====================================================
int Player::getSecondCardTotal(){
    int total = Player::getSecondHandTotal();
    if(Player::secondIsSoft() && total <= 11){
        total += 10;
    }
    return total;
}



// GET THIRD HAND CARD TOTAL (ACCOUNTS FOR ACES + 10)
// ====================================================
int Player::getThirdCardTotal(){
    int total = Player::getThirdHandTotal();
    if(Player::thirdIsSoft() && total <= 11){
        total += 10;
    }
    return total;
}




// CHECK IF PLAYER HAS BLACKJACK
// ==============================
// RETURN: FALSE IF NOT
bool Player::hasBlackJack(){
    if(playerHand[0]->getValue() == 10 && playerHand[1]->getValue() == 1){
        return true;
    }
    if(playerHand[1]->getValue() == 10 && playerHand[0]->getValue() == 1){
        return true;
    }
    return false;
}



// GET FIRST / MAIN HAND CARD TOTAL (ACCOUNTS FOR ACES + 10)
// ==========================================================

// NOtE: REDUNDANT BUT WORKS, GIVES TRUE HAND VALUE

int Player::firstHandFinal(){
    bool hasAce = false;
    int handTotal = 0;
    for(Card* aCard : playerHand){
        if(aCard->getValue() == 1){
            hasAce = true;
        }
        handTotal += aCard->getValue();
    }

    if(handTotal < 12 && hasAce){
        handTotal += 10;
        return handTotal;
    }
    return handTotal;
}



// GET SECOND HAND CARD TOTAL (ACCOUNTS FOR ACES + 10)
// ==========================================================

// NOtE: REDUNDANT BUT WORKS, GIVES TRUE HAND VALUE

int Player::secondHandFinal(){
    bool hasAce = false;
    int handTotal = 0;
    for(Card* aCard : secondSplitHand){
        if(aCard->getValue() == 1){
            hasAce = true;
        }
        handTotal += aCard->getValue();
    }

    if(handTotal < 12 && hasAce){
        handTotal += 10;
        return handTotal;
    }
    return handTotal;
}



// GET THIRD HAND CARD TOTAL (ACCOUNTS FOR ACES + 10)
// ==========================================================

// NOtE: REDUNDANT BUT WORKS, GIVES TRUE HAND VALUE

int Player::thirdHandFinal(){
    bool hasAce = false;
    int handTotal = 0;
    for(Card* aCard : thirdSplitHand){
        if(aCard->getValue() == 1){
            hasAce = true;
        }
        handTotal += aCard->getValue();
    }

    if(handTotal < 12 && hasAce){
        handTotal += 10;
        return handTotal;
    }
    return handTotal;
}








// ======================================================
// ============          GARBAGE           ==============
// ======================================================
// Description: Destructor, all hand vectors are cleared
void Player::pickUpCards(){
    Player::playerHand.clear();
    Player::secondSplitHand.clear();
    Player::thirdSplitHand.clear();
}