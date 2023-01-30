#include "player.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;


// CLASS CARD //
// Value: 1 = A, 2 = 2 .... 10 = 10, 11 = J, 12 = Q, 13 = K
// Suit: 1 = Spades, 2 = Hearts, 3 = Clubs, 4 = Diamonds 
// Implementation

// Constructors
//=======================

Player::Player(Shoe *shoe, int seat)
{
    //printf("\nCreating Player in spot %d\n", seat);
    Player::theShoe2 = shoe;
    Player::playerSeat = seat;
};

Player::Player()
{
    Player::playerSeat = 0;
};


// Methods
//========================

// Prepares all bets
// Sets main , 1st split and 2nd split + all double bets to default values
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




// For dealer, Get the money
float Player::getMainBet(){
    return mainBet + doubleMainBet;
}

float Player::getSecondSplitBet(){
    return secondSplitBet + doubleSecondSplitBet;
}

float Player::getThirdSplitBet(){
    return thirdSplitBet + doubleThirdSplitBet;
}


// Draws a card from shoe, Player has reference to shoe object
void Player::getCard(){
        //printf(" .. Player::getCard() .. ");
    Player::playerHand.push_back(Player::theShoe2->drawCard());
}


// Display cards
void Player::showHand(){
    //printf(" .. Player::showHand() .. ");
    string cardName2;
    for(Card *aCard  : Player::playerHand){
        cardName2 = aCard->getName();
        //printf("%s ", cardName2.c_str());
    }
}



// HAND VALUE TOTAL
int Player::getFirstHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Player::playerHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}

int Player::getSecondHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Player::secondSplitHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}

int Player::getThirdHandTotal(){
    int handTotal = 0;
    
    for(Card *aCard  : Player::thirdSplitHand){
        handTotal += aCard->getValue();
    }
    return handTotal;
}



// GATEWAY TO ALL PLAYER DECISIONS
// -Determines if BlackJack or Splits
void Player::firstTwo(int dealerUp){
//printf("\n\n\n\n\n\n\n==========================  START  ===========================\n            ....PLAYER ( %d  )::FIRST TWO())...      \n============================================================\n", playerSeat);

    firstDouble = false;
    secondDouble = false;
    thirdDouble = false;

    firstBusted = false;
    secondBusted = false;
    thirdBusted = false;

    //Check if blackjack
    if(Player::getFirstHandTotal() == 11 && (playerHand[0]->getValue() == 1 || playerHand[1]->getValue() == 1)){

        //printf( "\n\n\n<<<<<<<<<<<<<<<<    BLACKJACK    >>>>>>>>>>>>>>>>>\n\n");
        Player::firstBJ = true;
        //        <=== YOU NEED TO RETURN A WINNING VALUE
        return;
    }



    Player:: testFirstHand = playerHand[0]->getValue();
    Player:: testSecondHand = playerHand[1]->getValue();
    Player:: dealerShowing = dealerUp;   // Dealer's up card






    int basicStrategy = 100;
    // Show first and second hand
    //printf("\n Player::firstTwo()..");
    int firstCard = playerHand[0]->getValue();
    //printf(" first card = %d", firstCard);
    int secondCard = playerHand[1]->getValue();
    //printf(" second card = %d", secondCard);

    int firstSplitCard;
    int secondSplitCard;


// Doubles =====================

        // Determine basic Strategy move
        if(Player::firstIsSoft()){              // YOU NEED TO COMPLETE THIS NEEEEXT

            // Add 10 because soft
            basicStrategy = Player::basicSoftStrategy[Player::getFirstHandTotal() + 11][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::getFirstHandTotal()][Player::dealerShowing - 1];
        }

        if(basicStrategy == 2){
            playerHand.push_back(theShoe2->drawCard());
            Player::doubleMainBet = Player::mainBet;   // Double themain bet
            firstDouble = true;

            if(!Player::firstIsSoft() && Player::getFirstHandTotal() > 21){
                    //printf("\n Player::playFirstHand() -> While loop -> Player(%d) first hand had     ###### ######  ##### BUSTED!! ##### ###### ###### \n", playerSeat);
                firstBusted = true;  // Busted
            }
        return;  
        }










        // 1st split
    // Check if Split
    if(firstCard == secondCard){
        // Check if split
        basicStrategy = Player::basicSplitStrategy[firstCard-1][Player::dealerShowing - 1];

        if(basicStrategy == 2){firstDouble = true;} //bypass first double
        if(basicStrategy == 4){

            Player::secondSplitBet = Player::mainBet; // Add more money

            //printf("\n\nPLAYER HAS DECIDED TO        >>>   SPLIT (1)   <<<<\n\n");
            //printf(" .. (1)playerHand[1] is %s", playerHand[1]->getName().c_str());
            secondSplitHand.push_back(playerHand[1]);          // Create 2nd hand
            //printf(" .. (2) secondSplitHand[0] is %s\n", secondSplitHand[0]->getName().c_str());
            playerHand[1] = theShoe2->drawCard();              // Complete 1st hand
            //printf("  .. (3) playerHand[0] is now %s", playerHand[0]->getName().c_str()); 
            //printf("  .. (3.5) playerHand[1] is now %s", playerHand[1]->getName().c_str());             
            //playFirstHand();                                   // Play 1st hand

            secondSplitHand.push_back(theShoe2->drawCard()); 
             //printf(" .. (4)secondSplitHand[0] is now %s\n", secondSplitHand[0]->getName().c_str());               
             //printf(" .. (5)secondSplitHand[1] is now %s\n", secondSplitHand[1]->getName().c_str()); 
   



            // 2nd split 1st hand

            firstCard = playerHand[0]->getValue();
            firstCard = playerHand[1]->getValue();

            firstSplitCard = secondSplitHand[0]->getValue();
            secondSplitCard = secondSplitHand[1]->getValue();


// DOUBLE
        // Determine basic Strategy move
        if(Player::secondIsSoft()){              // YOU NEED TO COMPLETE THIS NEEEEXT

            // Add 10 because soft
            basicStrategy = Player::basicSoftStrategy[Player::getSecondHandTotal() + 11][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::getSecondHandTotal()][Player::dealerShowing - 1];
        }

        if(basicStrategy == 2){
            secondSplitHand.push_back(theShoe2->drawCard());
            Player::doubleSecondSplitBet = Player::secondSplitBet;   // Double themain bet
            // Check if busted (not soft and over 21)
            secondDouble = true;
            if(!Player::secondIsSoft() && Player::getSecondHandTotal() > 21){
                    //printf("\n Player::playSecondSplitHand() -> While loop -> Player(%d) second split had     ###### ######  ##### BUSTED!! ##### ###### ###### \n", playerSeat);
                secondBusted = true;  // Busted
            }

            playFirstHand();
            return; 
        }



            if(firstCard == secondCard){

                basicStrategy = Player::basicSplitStrategy[firstCard-1][Player::dealerShowing - 1];

                if(basicStrategy == 2){secondDouble = true;} //bypass first double
                if(basicStrategy == 4){

                    Player::thirdSplitBet = Player::mainBet;   // Add more money

                    //printf("\n\nPLAYER HAS DECIDED TO        >>>   SPLIT AGAIN(2)   <<<<\n\n");

                    thirdSplitHand.push_back(playerHand[1]);           // Create 3rd hand
                    playerHand[1] = theShoe2->drawCard();              // Complete 2nd hand  
                    //playSecondHand();                                       // Play 2nd hand

                    thirdSplitHand.push_back(theShoe2->drawCard());         // Copmlete third Hand
                    playFirstHand();
                    playSecondHand();
                    playThirdHand();                                        // Play 3rd hand
                    return;
                }
            } // 2nd Split wnd hand

            if(firstSplitCard == secondSplitCard){

                basicStrategy = Player::basicSplitStrategy[secondSplitCard-1][Player::dealerShowing - 1];

                if(basicStrategy == 2){thirdDouble = true;} //bypass first double
                if(basicStrategy == 4){

                    Player::thirdSplitBet = Player::mainBet;  // Add more money

                    //printf("\n\nPLAYER HAS DECIDED TO        >>>   SPLIT AGAIN(2)   <<<<\n\n");
                    thirdSplitHand.push_back(secondSplitHand[1]);           // Create 3rd hand
                    secondSplitHand[1] = theShoe2->drawCard();              // Complete 2nd hand  
                    //playSecondHand();                                       // Play 2nd hand

                    thirdSplitHand.push_back(theShoe2->drawCard());         // Copmlete third Hand
                    playFirstHand();
                    playSecondHand();
                    playThirdHand();                                        // Play 3rd hand
                    return;
                }
            } // 2nd Split


            playFirstHand();
            playSecondHand();
            return;
        } 
    } // 1st split
    playFirstHand();
    return;
}



// Hit/Stay for primary, and up to two split hands
void Player::playFirstHand(){
//printf("\n ===============================\n ....PLAYER ( %d  )::playFirstHand()... \n================================\n", playerSeat);

        // show dealer card
            //printf("\nDEALER UP CARD:   ======\n");
            //printf("                  || %d||\n", dealerShowing);
            //printf("                  ======\n");


    if(firstDouble){
        //printf("\n Player::playFirstHand() -> switch bypassed -> player had decided to      >>>>  >>>>   DOUBLE  <<<<  <<<< \n");
        //double
        playerHand.push_back(theShoe2->drawCard());
        Player::doubleMainBet = Player::mainBet;   // Double themain bet
        return;
    }




    int basicStrategy = 100;
    
    // WHILE THE PLAYER HASNT BUSTED HE GETS TO PLAY
    Player::firstBusted = false; // Reset bust
    while(!Player::firstBusted){

            // PRINTS CARDS

            //printf("\n\nPlayer(%d) has %d\n",playerSeat, Player::getFirstHandTotal());
            //printf("   Player(%d) cards:", playerSeat);    
            for (Card *aCard : playerHand){
                //printf(" |%s| ", aCard->getName().c_str());
            }
            


        // Determine basic Strategy move
        if(Player::firstIsSoft()){              // YOU NEED TO COMPLETE THIS NEEEEXT

            // Add 10 because soft
            basicStrategy = Player::basicSoftStrategy[Player::getFirstHandTotal() + 11][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::getFirstHandTotal()][Player::dealerShowing - 1];
        }

        // Act on basicStrategy

        switch(basicStrategy){
            case 0:
            //printf("\n Player::playFirstHand() -> switch -> player had decided to        >> STAY << \n");
            //stay
            return;
            break;

            case 1:
            //printf("\n Player::playFirstHand() -> switch -> player had decided to        >>  HIT  << \n");
            //hit

            playerHand.push_back(theShoe2->drawCard());
            break;

            case 2:
            //printf("\n Player::playFirstHand() -> switch -> player had decided to      >>>>  >>>>   DOUBLE  <<<<  <<<< \n");
            //double
            playerHand.push_back(theShoe2->drawCard());
            break;

            default:
                        //printf("\n Player::playFirstHand() -> switch -> INVALID INPUT, DEFAULTED\n");
            // don't do nothin
            break;
        }
    

    // Check if busted (not soft and over 21)
    if(!Player::firstIsSoft() && Player::getFirstHandTotal() > 21){
            //printf("\n Player::playFirstHand() -> While loop -> Player(%d) first hand had     ###### ######  ##### BUSTED!! ##### ###### ###### \n", playerSeat);
        firstBusted = true;  // Busted
    }

    }

    
}

void Player::playSecondHand(){
//printf("\n ===============================\n ....PLAYER ( %d  )::playSecondSplitHand()... \n================================\n", playerSeat);

        // show dealer card
            //printf("\nDEALER UP CARD:   ======\n");
            //printf("                  || %d||\n", dealerShowing);
            //printf("                  ======\n");


    if(secondDouble){
        //printf("\n Player::playSecondSplitHand() -> switch bypassed -> player had decided to      >>>>  >>>>   DOUBLE  <<<<  <<<< \n");
        //double
        secondSplitHand.push_back(theShoe2->drawCard());
        Player::doubleSecondSplitBet = Player::secondSplitBet;   // Double themain bet
        return;        
    }



    int basicStrategy = 100;
    
    // WHILE THE PLAYER HASNT BUSTED HE GETS TO PLAY
    Player::secondBusted = false; // Reset bust
    while(!Player::secondBusted){

            // PRINTS CARDS

            //printf("\n\nPlayer(%d) has %d\n",playerSeat, Player::getSecondHandTotal());
            //printf("   Player(%d) cards:", playerSeat);    
            for (Card *aCard : secondSplitHand){
                //printf(" |%s| ", aCard->getName().c_str());
            }
            


        // Determine basic Strategy move
        if(Player::secondIsSoft()){              // YOU NEED TO COMPLETE THIS NEEEEXT

            // Add 10 because soft
            basicStrategy = Player::basicSoftStrategy[Player::getSecondHandTotal() + 11][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::getSecondHandTotal()][Player::dealerShowing - 1];
        }

        // Act on basicStrategy

        switch(basicStrategy){
            case 0:
            //printf("\n Player::playSecondSplitHand() -> switch -> player had decided to        >> STAY << \n");
            //stay
            return;
            break;

            case 1:
            //printf("\n Player::playSecondSplitHand() -> switch -> player had decided to        >>  HIT  << \n");
            //hit

            secondSplitHand.push_back(theShoe2->drawCard());
            break;

            case 2:
            //printf("\n Player::playSecondSplitHand() -> switch -> player had decided to      >>>>  >>>>   DOUBLE  <<<<  <<<< \n");
            //double
            secondSplitHand.push_back(theShoe2->drawCard());
            break;

            default:
            break;
                        //printf("\n Player::playSEcondSplitHand() -> switch -> INVALID INPUT, DEFAULTED\n");
            // don't do nothin
        }
    

    // Check if busted (not soft and over 21)
    if(!Player::secondIsSoft() && Player::getSecondHandTotal() > 21){
            //printf("\n Player::playSecondSplitHand() -> While loop -> Player(%d) second split had     ###### ######  ##### BUSTED!! ##### ###### ###### \n", playerSeat);
        secondBusted = true;  // Busted
    }

    }

    
}

void Player::playThirdHand(){
//printf("\n ===============================\n ....PLAYER ( %d  )::playThirdSplitHand()... \n================================\n", playerSeat);

        // show dealer card
            //printf("\nDEALER UP CARD:   ======\n");
            //printf("                  || %d||\n", dealerShowing);
            //printf("                  ======\n");

    if(thirdDouble){
        //printf("\n Player::playThirdSplitHand() -> switch bypassed-> player had decided to      >>>>  >>>>   DOUBLE  <<<<  <<<< \n");
        //double
        thirdSplitHand.push_back(theShoe2->drawCard());
        Player::doubleThirdSplitBet = Player::thirdSplitBet;   // Double themain bet
        return;        
    }


    int basicStrategy = 100;
    
    // WHILE THE PLAYER HASNT BUSTED HE GETS TO PLAY
    Player::thirdBusted = false; // Reset bust
    while(!Player::thirdBusted){

            // PRINTS CARDS

            //printf("\n\nPlayer(%d) has %d\n",playerSeat, Player::getThirdHandTotal());
            //printf("   Player(%d) cards:", playerSeat);    
            for (Card *aCard : thirdSplitHand){
                //printf(" |%s| ", aCard->getName().c_str());
            }
            


        // Determine basic Strategy move
        if(Player::thirdIsSoft()){              // YOU NEED TO COMPLETE THIS NEEEEXT

            // Add 10 because soft
            basicStrategy = Player::basicSoftStrategy[Player::getThirdHandTotal() + 11][Player::dealerShowing - 1];
        }
        else{
            basicStrategy = Player::basicHardStrategy[Player::getThirdHandTotal()][Player::dealerShowing - 1];
        }

        // Act on basicStrategy

        switch(basicStrategy){
            case 0:
            //printf("\n Player::playThirdSplitHand() -> switch -> player had decided to        >> STAY << \n");
            //stay
            return;
            break;

            case 1:
            //printf("\n Player::playThirdSplitHand() -> switch -> player had decided to        >>  HIT  << \n");
            //hit

            thirdSplitHand.push_back(theShoe2->drawCard());
            break;

            case 2:
            //printf("\n Player::playThirdSplitHand() -> switch -> player had decided to      >>>>  >>>>   DOUBLE  <<<<  <<<< \n");
            //double
            thirdSplitHand.push_back(theShoe2->drawCard());
            break;

            default:
            break;
                        //printf("\n Player::playThirdSplitHand() -> switch -> INVALID INPUT, DEFAULTED\n");
            // don't do nothin
        }
    

    // Check if busted (not soft and over 21)
    if(!Player::thirdIsSoft() && Player::getThirdHandTotal() > 21){
            //printf("\n Player::playThirdSplitHand() -> While loop -> Player(%d) third split hand     ###### ######  ##### BUSTED!! ##### ###### ###### \n", playerSeat);
        thirdBusted = true;  // Busted
    }

    }

      
}




// Check if soft, (all hands) 1st 2nd 3rd   (COMPLETE)

bool Player::firstIsSoft(){
    //printf(" ...   Player::firstIsSoft()");

    for (Card* aCard : playerHand){
        if(aCard->getValue() == 1){
            if(Player::getFirstHandTotal() <= 11){
                //printf(" -> SOFT .... ");
                return true;
            }
        }
    }
    //printf(" -> HARD .... ");        
    return false;
}

bool Player::secondIsSoft(){
    //printf(" ...   Player::secondIsSoft()");
    
    for (Card* aCard : secondSplitHand){
        if(aCard->getValue() == 1){
            if(Player::getSecondHandTotal() <= 11){
                //printf(" -> SOFT .... ");
                return true;
            }
        }
    }
    //printf(" -> HARD .... ");        
    return false;
}

bool Player::thirdIsSoft(){
    //printf(" ...   Player::thirdIsSoft()");
    
    for (Card* aCard : thirdSplitHand){
        if(aCard->getValue() == 1){
            if(Player::getThirdHandTotal() <= 11){
                //printf(" -> SOFT .... ");
                return true;
            }
        }
    }
    //printf(" -> HARD .... ");        
    return false;
}




int Player::getFirstCardTotal(){
    int total = Player::getFirstHandTotal();
    if(Player::firstIsSoft() && total <= 11){
        total += 10;
    }
    return total;
}

int Player::getSecondCardTotal(){
    int total = Player::getSecondHandTotal();
    if(Player::secondIsSoft() && total <= 11){
        total += 10;
    }
    return total;
}

int Player::getThirdCardTotal(){
    int total = Player::getThirdHandTotal();
    if(Player::thirdIsSoft() && total <= 11){
        total += 10;
    }
    return total;
}


bool Player::hasBlackJack(){
    if(playerHand[0]->getValue() == 10 && playerHand[1]->getValue() == 1){
        return true;
    }
    if(playerHand[1]->getValue() == 10 && playerHand[0]->getValue() == 1){
        return true;
    }
    return false;
}



// Add a 10 if the ace is present
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

/*
void Player::selectBasicStrategy(int selection){
    if(selection == 1){
        Player::basicHardStrategy = Player::basicHardStrategyDEFAULT;
        Player::basicSoftStrategy = Player::basicSoftStrategyDEFAULT;
        Player::basicSplitStrategy = Player::basicSplitStrategyDEFAULT;
    }
    if(selection == 2){
        Player::basicHardStrategy = Player::lowHardStrategy;
        Player::basicSoftStrategy = Player::lowSoftStrategy;
        Player::basicSplitStrategy = Player::lowSplitStrategy;
    }
}
*/

// Garbage collerction, clear table
void Player::pickUpCards(){
    Player::playerHand.clear();
    Player::secondSplitHand.clear();
    Player::thirdSplitHand.clear();
}