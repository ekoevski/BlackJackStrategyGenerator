#include "table.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

// CLASS TABLE //
// Implementation

Table::Table(string newGameName, int numberOfDecks, int numOfPlayers)
{

  Table::gameName = newGameName;
  Table::shoeCount = numberOfDecks;
  Table::numberOfPlayers = numOfPlayers;
  Table::theShoe = new Shoe(shoeCount);
  Table::theDealer = new Dealer(theShoe);
  for (int i = 1; i <= Table::numberOfPlayers; i++)
  {
    Table::players.push_back(new Player(theShoe, i));
  }
}


// ======================================================
// ============     PLACE_CARDS    ==============
// ======================================================
// Description: Creates cards for player and dealer
void Table::placeCards()
{
  // YOU NEED TO DEAL DEALER CARDS
  Table::theShoe->shuffle();
  for (int j = 0; j < 2; j++)
  {

    for (int i = 0; i < (Table::numberOfPlayers); i++)
    {
      Table::players[i]->getCard();
    }
    Table::theDealer->getCard();
  }
};




// DISPLAY TABLE
//========================
void Table::displayTable()
{
  theDealer->showHand();
  for (Player *aPlayer : players)
  {
    aPlayer->showHand();
  }
};





// ======================================================
// ============     CLEAR TABLE    ==============
// ======================================================
// Description: Destructor, all hand vectors are cleared
void Table::clearTable()
{
  for (Player *aPlayer : Table::players)
  {
    aPlayer->pickUpCards();
  }
  Table::theDealer->pickUpCards();
}






// PLAY A ROUND     (NOT IN USE)
// NOT SURE ABOUT THIS, IT MAY HAVE BEEN ONE OF THE EARLIER TRIES
// DELETE IF NOT IN USE
// ===============================================================
void Table::playRound(float bet)
{
  for (Player *aPlayer : players)
  {
    aPlayer->setBets(bet);
  }

  if (theDealer->hasBlackJack())
  {
    for (Player *aPlayer : players)
    {
      if (!aPlayer->hasBlackJack())
      {
        theDealer->addWinloss(aPlayer->getPlayerTotalBets());
        theDealer->addCasinoDrop(aPlayer->getPlayerTotalBets());
      }
    }
    return;
  }





  // PAY PLAYER BLACKJACK HANDS
  //================================
  for (Player *aPlayer : players)
  {
    if (aPlayer->hasBlackJack())
    {
      theDealer->addWinloss((-1.5) * aPlayer->getPlayerTotalBets());
      theDealer->addCasinoDrop(aPlayer->getPlayerTotalBets());
    }
  }


  // PLAYER CAN NOW PLAY HAND
  //==========================================
  for (Player *aPlayer : players)
  {
    aPlayer->firstTwo(theDealer->showUpCard());
  }






  // DEALER PLAYS IN RESPONSE TO PLAYER
  //=============================================
  theDealer->playHand();












  //============================================
  //============================================
  //      ROUND COMPLETE  DETERMINE WINNERS
  //============================================
  //============================================
  for (Player *aPlayer : players)
  {

    // Collect busted hands
    //====================
    if (aPlayer->firstHandBusted())
    {
      theDealer->addWinloss(aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());
    }
    if (aPlayer->secondHandBusted())
    {
      theDealer->addWinloss(aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
    }
    if (aPlayer->thirdHandBusted())
    {
      theDealer->addWinloss(aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
    }





    // DEALER BUST
    // =================
    if (theDealer->Busted() && !aPlayer->hasBlackJack() &&
        !aPlayer->firstHandBusted())
    {
      theDealer->addWinloss((-1) * aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());
    }
    if (theDealer->Busted() && !aPlayer->hasBlackJack() &&
        !aPlayer->secondHandBusted())
    {
      theDealer->addWinloss((-1) * aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
    }
    if (theDealer->Busted() && !aPlayer->hasBlackJack() &&
        !aPlayer->thirdHandBusted())
    {
      theDealer->addWinloss((-1) * aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
    }

    if (theDealer->Busted())
    {
      continue;
    }






        // FIRST HAND
    // Dealer hand is HIGHER
    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() > aPlayer->firstHandFinal())
    {
      theDealer->addWinloss(aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());

    }
    // Player hand is LOWER
    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() < aPlayer->firstHandFinal())
    {
      theDealer->addWinloss((-1) * aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());

    }



        // SECOND SPLIT HAND
    // Dealer hand is HIGHER
    if (!aPlayer->hasBlackJack() && !aPlayer->secondHandBusted() &&
        theDealer->dealerFinal() > aPlayer->secondHandFinal())
    {
      theDealer->addWinloss(aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());

    }
    // Player hand is LOWER
    if (!aPlayer->hasBlackJack() && !aPlayer->secondHandBusted() &&
        theDealer->dealerFinal() < aPlayer->secondHandFinal())
    {
      theDealer->addWinloss((-1) * aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());

    }


      // THIRD SPLIT HAND
    // Dealer hand is HIGHER
    if (!aPlayer->hasBlackJack() && !aPlayer->thirdHandBusted() &&
        theDealer->dealerFinal() > aPlayer->thirdHandFinal())
    {
      theDealer->addWinloss(aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());

    }
    // Player hand is LOWER
    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() < aPlayer->thirdHandFinal())
    {
      theDealer->addWinloss((-1) * aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());

    }





    // PUSH DROP
    //===================
    // Add drop from push

    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() == aPlayer->firstHandFinal())
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(aPlayer->getMainBet());
    }


    if (!aPlayer->hasBlackJack() && !aPlayer->secondHandBusted() &&
        theDealer->dealerFinal() == aPlayer->secondHandFinal())
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
    }


    if (!aPlayer->hasBlackJack() && !aPlayer->thirdHandBusted() &&
        theDealer->dealerFinal() == aPlayer->thirdHandFinal())
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
    }
  }

};






// ======================================================
// ============          TESTING           ==============
// ======================================================
// Description: Give the first card value, second card value (for layer)
// give the dealer up card value, and the function will pause the round
// This funciton is useful if real-time gameplay is implemented.
// (Not for monte-carlo simulation)

void Table::Testing(int first, int second, int dealerUp)
{
  if (theDealer->showUpCard() == dealerUp)
  {
    for (Player *aPlayer : players)
    {

      if (aPlayer->testFirstHand == first &&
          aPlayer->testSecondHand == second)
      {
        // printf("\n===========\nTesting : dealer |%d|    player |%d| |%d|",
        //      dealerUp, first, second);
        sleep(100);
        ;
      }
      if (aPlayer->testFirstHand == second &&
          aPlayer->testSecondHand == first)
      {
        // printf("\n===========\nTesting : dealer |%d|    player |%d| |%d|",
        //      dealerUp, first, second);
        sleep(100);
        ;
      }
    }
  }
};




// SET PLAYER BASIC STRATEGY
// ================================
void Table::setPlayerBasicStrategy(vector<vector<int>> hard, vector<vector<int>> soft, vector<vector<int>> split)
{
  for (Player *aPlayer : players)
  {
    aPlayer->basicHardStrategy = hard;
    aPlayer->basicSoftStrategy = soft;
    aPlayer->basicSplitStrategy = split;
  }
}



// PRINT W/L AND DROP REPORT
// ================================
void Table::printDrop()
{
  printf("\nDROP: %.2f", theDealer->casinoDrop);
  printf(" W/L: %.2f", theDealer->winLoss);
  printf(" HOLD: (%.2f p)",
         theDealer->winLoss / theDealer->casinoDrop * 100);
}


// ======================================================
// ============      SET HARD CARDS        ==============
// ======================================================
// Description: set manually first two cards and dealer up card
// Provide player hand total and dealer up card

void Table::setHardCards(int playerTotal, int dealerUp){
  // Player
  if(playerTotal <= 12){
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0]->cardValue = 2;
      aPlayer -> playerHand[1]->cardValue = playerTotal - 2;
    }  
  }
  
  if(playerTotal > 12){

    for(Player * aPlayer : players){
      aPlayer -> playerHand[0]->cardValue = 10;
      aPlayer -> playerHand[1]->cardValue = playerTotal - 10;
    }  
  }

  // Dealer
  theDealer -> dealerHand[0] ->cardValue = dealerUp + 1;
}



// ======================================================
// ============      SET SOFT CARDS        ==============

// Description: set manually first two cards and dealer up card
// Provide player hand total and dealer up card

void Table::setSoftCards(int playerTotal, int dealerUp){
  // Player
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0]->cardValue = 1;
      aPlayer -> playerHand[1]->cardValue = playerTotal - 11;
    }  

  
  // Dealer
  Card *dealerUpCard = new Card(dealerUp + 1, 1);
  theDealer -> dealerHand[0]->cardValue = dealerUp + 1;
}




// ======================================================
// ============      SET SPLIT CARDS        ==============

// Description: set manually first two cards and dealer up card
// Provide player hand total and dealer up card

void Table::setSplitCards(int playerTotal, int dealerUp){
  // Player
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0]->cardValue = playerTotal + 1;
      aPlayer -> playerHand[1]->cardValue = playerTotal + 1;
    }  
  

  // Dealer
  theDealer -> dealerHand[0]->cardValue = dealerUp + 1;
}




// ======================================================
// ============    RESET PLAYER AND DEALER ==============
// ======================================================
// Description: Returns all default values for player
//              and dealer, all hands are cleared.

void Table::reset(){
    for(Player * aPlayer : players){
      aPlayer -> strategySelector = 0;
      aPlayer -> playerDrop = 0;
      aPlayer -> winLoss = 0;
      aPlayer -> testFirstHand = 110;
      aPlayer -> testSecondHand = 110;
      aPlayer -> bet = 0;
      aPlayer -> firstDouble = false;
      aPlayer -> secondDouble = false;
      aPlayer -> thirdDouble = false;
      aPlayer -> mainBet = 0;
      aPlayer -> doubleMainBet = 0;
      aPlayer -> secondSplitBet = 0;
      aPlayer -> doubleSecondSplitBet = 0;
      aPlayer -> thirdSplitBet = 0;
      aPlayer -> doubleThirdSplitBet = 0;
      aPlayer -> firstBusted = false;
      aPlayer -> secondBusted = false;
      aPlayer -> thirdBusted = false;
      aPlayer -> firstBJ = false;
      aPlayer -> dealerShowing = 0;
      aPlayer -> playerHand.clear();
      aPlayer -> secondSplitHand.clear();
      aPlayer -> thirdSplitHand.clear();
    }
    theDealer -> bet = 0;
    theDealer -> dealerBust = false;
    theDealer -> BlackJack = false;
    theDealer -> dealerHas = 0;
    theDealer -> dealerHand.clear();
}