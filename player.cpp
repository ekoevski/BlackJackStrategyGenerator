#include "player.h"
#include <iostream>
#include <string>
#include <fstream>
#include "logging.h"

using namespace std;


// PLAYER CLASS
// Implementation

// Constructors
//=======================

Player::Player(Shoe *shoe, int seat)
{
    Player::theShoe2 = shoe;
    Player::playerSeat = seat;
    Player::set_first_card = new Card(4,4);
    Player::set_second_card = new Card(5,4);    
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
    #if DEBUG
        LOG_FLAT("player_cards:", __FILE__, __LINE__, NULL);         
        for(Card *aCard  : Player::playerHand)
        {
            printf("%s ", aCard->getName().c_str());
        }
        printf("\n          Second Split Hand\n");
        for(Card *aCard  : Player::secondSplitHand)
        {
            printf("%s ", aCard->getName().c_str());
        }
        printf("\n          Third Split Hand\n");
        for(Card *aCard  : Player::thirdSplitHand)
        {
            printf("%s ", aCard->getName().c_str());
        }
        printf("\n");                
    #else
        return;
    #endif
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

void Player::firstTwo(int dealerUpIndex, int intent_mode, bool force_intent_mode)
{
    dealerUpIndex--;
    firstDouble = false;
    secondDouble = false;
    thirdDouble = false;
    firstBusted = false;
    secondBusted = false;
    thirdBusted = false;

    LOG_1("Player::firstTwo(dealerupIndex = %d)", __FILE__, __LINE__, dealerUpIndex);    

    //Check if blackjack
    if(Player::getFirstHandTotal() == 11 && 
      (playerHand[0]->getValue() == 1 || playerHand[1]->getValue() == 1))
    {
        LOG_ERROR("player had blackjack, return", __FILE__, __LINE__, NULL);
        Player::firstBJ = true;
        return;
    }

    Player:: testFirstHand = playerHand[0]->getValue();
    Player:: testSecondHand = playerHand[1]->getValue();
    Player:: dealerShowing = dealerUpIndex; 
    int basicStrategy = 100;
    int firstCard = playerHand[0]->getValue();
    int secondCard = playerHand[1]->getValue();

    LOG_ERROR("firstCard %d  secondCard %d", __FILE__, __LINE__, firstCard, secondCard);
    int first_hand_total = Player::firstHandFinal();

    LOG_0("Player::firstHandTotal = %d)", __FILE__, __LINE__, first_hand_total);    

    // GET BASIC STRATEGY
    //Player::printBasicStrategy();


    if(firstCard == secondCard && !g_lock_split_strategy)
    {
        basicStrategy = Player::playerSplitStrategy[firstCard - 1][dealerUpIndex];
        
        if (basicStrategy == PLAY) // Play as normal hand
        {
            if(Player::firstIsSoft())
            {
                basicStrategy = Player::playerSoftStrategy[first_hand_total][dealerUpIndex];
            }
            else
            {
                basicStrategy = Player::playerHardStrategy[first_hand_total][dealerUpIndex];
            }
        }
    }
    else
    {
        if(Player::firstIsSoft())
        {     
            basicStrategy = Player::playerSoftStrategy[first_hand_total][dealerUpIndex];
        }
        else
        {
            basicStrategy = Player::playerHardStrategy[first_hand_total][dealerUpIndex];
        }      
    }

    // IF 2 DOUBLE
    if(basicStrategy == DOUBLE)
    {
        LOG_1("basicStrategy == DOUBLE, drawCard() and stop hitting", __FILE__, __LINE__, NULL);   
        playerHand.push_back(theShoe2->drawCard());
        Player::doubleMainBet = Player::mainBet; 
        firstDouble = true;

        if(!Player::firstIsSoft() && Player::firstHandFinal() > 21)
        {
            firstBusted = true;  
        }
        return;  
    }

    // IF 1 THEN PLAY THE HAND
    if(basicStrategy == HIT)
    {
        LOG_1("basicStrategy == HIT, playFirstHAnd()", __FILE__, __LINE__, NULL);               
        playFirstHand();
        return;  
    }

    // IF 0 DO NOTHING
    if(basicStrategy == STAY)
    {
        LOG_1("Player:basicStrategy =  STAY, (basicStrategy: %d), (do nothing)", __FILE__, __LINE__, basicStrategy);         
        return;  
    }

    // CHECK IF SPLIT (1ST SPLIT)
    if(basicStrategy == SPLIT)
    {
        LOG_ERROR("1st Split ... ", __FILE__, __LINE__ , NULL);

        if(basicStrategy == DOUBLE)
        {
            firstDouble = true;
        } 
        if(basicStrategy == SPLIT)
        {
            Player::secondSplitBet = Player::mainBet;
            secondSplitHand.push_back(playerHand[1]);   
            playerHand[1] = theShoe2->drawCard();  

            secondSplitHand.push_back(theShoe2->drawCard()); 

            firstCard = playerHand[0]->getValue();
            secondCard = playerHand[1]->getValue();

            int firstSplitCard = secondSplitHand[0]->getValue();
            int secondSplitCard = secondSplitHand[1]->getValue();
            int second_hand_total = Player::secondHandFinal();

            if(Player::secondIsSoft())
            {   
                basicStrategy = Player::playerSoftStrategy[second_hand_total][dealerUpIndex];
            }
            else
            {
                basicStrategy = Player::playerHardStrategy[second_hand_total][dealerUpIndex];
            }

            if(basicStrategy == DOUBLE)
            {
                secondSplitHand.push_back(theShoe2->drawCard());
                Player::doubleSecondSplitBet = Player::secondSplitBet;
                secondDouble = true;
                if(!Player::secondIsSoft() && Player::secondHandFinal() > 21)
                {
                    secondBusted = true;  
                }
                playFirstHand();
                return; 
            }

            if(firstCard == secondCard)
            {
                basicStrategy = Player::playerSplitStrategy[firstCard-1][dealerUpIndex];

                if(basicStrategy == DOUBLE)
                {
                    secondDouble = true;
                } 
                if(basicStrategy == SPLIT)
                {
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

            if(firstSplitCard == secondSplitCard)
            {
                basicStrategy = Player::playerSplitStrategy[secondSplitCard-1][dealerUpIndex];

                if(basicStrategy == DOUBLE)
                {
                    thirdDouble = true;
                } 
                if(basicStrategy == SPLIT)
                {
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

void Player::playFirstHand()
{
    LOG_ERROR("Youre in function", __FILE__, __LINE__, NULL);
    if(firstDouble)
    {
    LOG_ERROR("first double", __FILE__, __LINE__, NULL);        
        playerHand.push_back(theShoe2->drawCard());
        Player::doubleMainBet = Player::mainBet;  
        return;
    }

    int basicStrategy = 100;
    Player::firstBusted = false;

    while(!Player::firstBusted)
    {
        for (Card *aCard : playerHand)
        {
            // Not sure what this was for
        }

        if(Player::firstIsSoft())
        {  
            basicStrategy = Player::playerSoftStrategy[Player::firstHandFinal()][Player::dealerShowing];
        }
        else
        {
            basicStrategy = Player::playerHardStrategy[Player::firstHandFinal()][Player::dealerShowing];
        }

        /// YOU NEED TO FIGURE OUT SOMETHING HERE FOR THE SPLITS
        LOG_0("basic_Stragegy: %d", __FILE__, __LINE__, basicStrategy);
        LOG_ERROR("First Hand basicStrategy: %d", __FILE__, __LINE__, basicStrategy);
        switch(basicStrategy)
        {
        case STAY:
            LOG_ERROR("STAY", __FILE__, __LINE__, NULL);
            return;
            break;

        case HIT:
            LOG_ERROR("HIT", __FILE__, __LINE__, NULL);
            playerHand.push_back(theShoe2->drawCard());
            break;

        case DOUBLE:
            LOG_ERROR("DOUBLE", __FILE__, __LINE__, NULL);        
            playerHand.push_back(theShoe2->drawCard());
            break;

        default:
            LOG_ERROR("Default..", __FILE__, __LINE__, NULL);          
            break;
        }
    
        if(!Player::firstIsSoft() && (Player::firstHandFinal() > 21))
        {
            firstBusted = true;
        }
    }
}





// ======================================================
// ============       PLAY SECOND  HAND    ==============
// ======================================================
// Description: PLAYS SECOND (SPLIT) HAND


void Player::playSecondHand()
{
    if(secondDouble)
    {
        secondSplitHand.push_back(theShoe2->drawCard());
        Player::doubleSecondSplitBet = Player::secondSplitBet;   
        return;        
    }

    int basicStrategy = 100;
    Player::secondBusted = false;
    
    while(!Player::secondBusted)
    {
        for (Card *aCard : secondSplitHand)
        {
            // Not sure why this is here
        }

        if(Player::secondIsSoft())
        { 
            basicStrategy = Player::playerSoftStrategy[Player::secondHandFinal()][Player::dealerShowing];
        }
        else
        {
            basicStrategy = Player::playerHardStrategy[Player::secondHandFinal()][Player::dealerShowing];
        }

        LOG_ERROR("Second hand basicStrategy: %d", __FILE__, __LINE__, basicStrategy);
        switch(basicStrategy)
        {
        case STAY:
            LOG_ERROR("STAY", __FILE__, __LINE__, NULL);        
            return;
            break;

        case HIT:
            LOG_ERROR("HIT", __FILE__, __LINE__, NULL);
            secondSplitHand.push_back(theShoe2->drawCard());
            break;

        case DOUBLE:
            LOG_ERROR("DOUBLE", __FILE__, __LINE__, NULL);
            secondSplitHand.push_back(theShoe2->drawCard());
            break;

        default:
                 LOG_ERROR("Default..", __FILE__, __LINE__, NULL);
            break;
        }

        if(!Player::secondIsSoft() && (Player::secondHandFinal() > 21))
        {
            secondBusted = true;
        }
    }
}









// ======================================================
// ============       PLAY THIRD  HAND    ==============
// ======================================================
// Description: PLAYS THIRD (SPLIT) HAND

void Player::playThirdHand()
{
    if(thirdDouble)
    {
        thirdSplitHand.push_back(theShoe2->drawCard());
        Player::doubleThirdSplitBet = Player::thirdSplitBet;
        return;        
    }

    int basicStrategy = 100;
    Player::thirdBusted = false;
    while(!Player::thirdBusted)
    {
        for (Card *aCard : thirdSplitHand)
        {
            // Absolutely worthless but I can't remember what this is for
        }

        // Determine basic Strategy move
        if(Player::thirdIsSoft())
        {   
            basicStrategy = Player::playerSoftStrategy[Player::thirdHandFinal()][Player::dealerShowing];
        } 
        else
        {
            basicStrategy = Player::playerHardStrategy[Player::thirdHandFinal()][Player::dealerShowing];
        }
        LOG_ERROR("Third hand basicStrategy: %d", __FILE__, __LINE__, basicStrategy);
        switch(basicStrategy)
        {
        case STAY:
            LOG_ERROR("STAY", __FILE__, __LINE__, NULL);
            return;
            break;

        case HIT:
            LOG_ERROR("HIT", __FILE__, __LINE__, NULL);
            thirdSplitHand.push_back(theShoe2->drawCard());
            break;

        case DOUBLE:
            LOG_ERROR("DOUBLE", __FILE__, __LINE__, NULL);
            thirdSplitHand.push_back(theShoe2->drawCard());
            break;

        default:
            LOG_ERROR("Default", __FILE__, __LINE__, NULL);
            break;
        }
        // Check if busted (not soft and over 21)
        if(!Player::thirdIsSoft() && Player::thirdHandFinal() > 21)
        {
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
// ===============Print Basic STRATEGY==================
// ======================================================
// precondition: has a basic strategy card vectors (soft, hard, split)
// postcondition: gives hold % for current basic strategy card
void Player::printBasicStrategy()
{
  #if (DEBUG == 1)
  VLOG_1("PLAYER BASIC STRATEGY CARD", NULL);
  VLOG_1("\nHARD STRATEGY\n", NULL);
  for(int j = 0; j < playerHardStrategy.size(); j++)
  {
    for(int i = 0; i < playerHardStrategy[j].size(); i++)
    {
     VLOG_1("%d ", playerHardStrategy[j][i]);                 
    }      
    VLOG_1("\n", NULL); 
  }
  VLOG_1("\nSOFT STRATEGY\n", NULL);          
  for(int j = 0; j < playerSoftStrategy.size(); j++)
  {
    for(int i = 0; i < playerSoftStrategy[j].size(); i++)
    {
      VLOG_1("%d ", playerSoftStrategy[j][i]);                 
    }      
    VLOG_1("\n", NULL); 
  }

  VLOG_1("\nSPLIT STRATEGY\n", NULL);
  for(int j = 0; j < playerSplitStrategy.size(); j++)
  {
    for(int i = 0; i < playerSplitStrategy[j].size(); i++)
    {
      VLOG_1("%d ", playerSplitStrategy[j][i], NULL);                
    } 
    VLOG_1("\n", NULL);
  }
  #endif
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