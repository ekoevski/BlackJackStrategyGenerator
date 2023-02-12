#include "table.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

// CLASS TABLE //
// Implementation

/// MAIN CONSTRUCTOR
//====================
Table::Table(string newGameName, int numberOfDecks, int numOfPlayers)
{
  // printf("\nGenerating %d deck shoe.. \n", numberOfDecks);

  Table::gameName = newGameName;
  Table::shoeCount = numberOfDecks;
  Table::numberOfPlayers = numOfPlayers;

  // >>>>>>>>>>>>>>>>>>>>DEFAULT CONSTRUCTOR IS FOR TESTING <<<<<<<<<<<<<<<<
  Table::theShoe = new Shoe(shoeCount); // <= Use this shoe for normal operation
  // Table::theShoe = new Shoe();

  // theShoe->showAllCards();
  Table::theDealer = new Dealer(theShoe);

  // Create a list of players in seats 1 - 6
  for (int i = 1; i <= Table::numberOfPlayers; i++)
  {
    Table::players.push_back(new Player(theShoe, i));
  }
}




// METHODS
//=======================
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

void Table::displayTable()
{
  // printf("\nTABLE CARDS:\n============\n");
  // printf("\nDealer hand:\n");
  theDealer->showHand();
  // printf("\n");

  for (Player *aPlayer : players)
  {
    // printf("Player %d cards: ", aPlayer->getSeat());
    aPlayer->showHand();
    // printf("\n");
  }

  for (Player *aPlayer : players)
  {
    // printf("\nPlayer %d has: (%d)", aPlayer->getSeat(),
    //        aPlayer->getFirstHandTotal());
  }
};

void Table::clearTable()
{
  // printf("\n\nTable::clearTable()...  CLEAR TABLE CARDS \n\n ");
  for (Player *aPlayer : Table::players)
  {
    aPlayer->pickUpCards();
  }
  Table::theDealer->pickUpCards();
}

// ===========================================================================
// YOU NEED TO FINISH THIS NEXT!!! ===========================================
// ===========================================================================

void Table::playRound(float bet)
{

  //  SET ALL BETS
  for (Player *aPlayer : players)
  {
    aPlayer->setBets(bet);
  }

  // DEALER BLACKJACK
  //===================================
  if (theDealer->hasBlackJack())
  {
    // IF NO PLAYER BJ COLLECT
    for (Player *aPlayer : players)
    {
      if (!aPlayer->hasBlackJack())
      {
        theDealer->addWinloss(aPlayer->getPlayerTotalBets());
        theDealer->addCasinoDrop(aPlayer->getPlayerTotalBets());
      }
    }
    return; // Clean up and end round
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

void Table::setPlayerBasicStrategy(vector<vector<int>> hard, vector<vector<int>> soft, vector<vector<int>> split)
{
  for (Player *aPlayer : players)
  {
    aPlayer->basicHardStrategy = hard;
    aPlayer->basicSoftStrategy = soft;
    aPlayer->basicSplitStrategy = split;
  }
}

void Table::printDrop()
{
  printf("\nDROP: %.2f", theDealer->casinoDrop);
  printf(" W/L: %.2f", theDealer->winLoss);
  printf(" HOLD: (%.2f p)",
         theDealer->winLoss / theDealer->casinoDrop * 100);
}



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


void Table::setSplitCards(int playerTotal, int dealerUp){
  // Player
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0]->cardValue = playerTotal + 1;
      aPlayer -> playerHand[1]->cardValue = playerTotal + 1;
    }  
  

  // Dealer
  theDealer -> dealerHand[0]->cardValue = dealerUp + 1;
}




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