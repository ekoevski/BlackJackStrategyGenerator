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




//   =====================================================
//  ========================================================
// ================     THIS IS THE START OF THE ROUND========
//  ========================================================
//   ======================================================

// Description: (VERY LONG METHOD) + SUBROUTINES

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

  bool first_hand_busted;
  bool second_hand_busted;
  bool third_hand_busted;
  bool dealer_busted;

  bool player_blackjack;
  bool dealer_blackjack;


  int main_bet;
  int second_split_bet;
  int third_split_bet;

  int dealer_final;
  int first_hand_final;
  int second_hand_final;
  int third_hand_final;






  for (Player *aPlayer : players)
  {
    // Load for debug
    first_hand_busted = aPlayer->firstHandBusted();
    second_hand_busted = aPlayer->secondHandBusted();
    third_hand_busted =aPlayer->thirdHandBusted();
    dealer_busted = theDealer->Busted();

    main_bet = aPlayer->getMainBet();
    second_split_bet = aPlayer->getSecondSplitBet();
    third_split_bet = aPlayer->getThirdSplitBet();
    
    dealer_final = theDealer->dealerFinal();
    first_hand_final = aPlayer->firstHandFinal();
    second_hand_final = aPlayer->secondHandFinal();
    third_hand_final = aPlayer->thirdHandFinal();

    player_blackjack = aPlayer->hasBlackJack();
    dealer_blackjack = theDealer->hasBlackJack();





    // Collect busted hands
    //====================
    if (first_hand_busted)
    {
      theDealer->addWinloss(main_bet);
      theDealer->addCasinoDrop(main_bet);
    }
    if (second_hand_busted)
    {
      theDealer->addWinloss(second_split_bet);
      theDealer->addCasinoDrop(second_split_bet);
    }
    if (third_hand_busted)
    {
      theDealer->addWinloss(third_split_bet);
      theDealer->addCasinoDrop(third_split_bet);
    }





    // DEALER BUST
    // =================
    if (dealer_busted && !player_blackjack &&
        !first_hand_busted)
    {
      theDealer->addWinloss((-1) * main_bet);
      theDealer->addCasinoDrop(main_bet);
    }
    if (dealer_busted && !player_blackjack &&
        !second_hand_busted)
    {
      theDealer->addWinloss((-1) * second_split_bet);
      theDealer->addCasinoDrop(second_split_bet);
    }
    if (dealer_busted && !player_blackjack &&
        !third_hand_busted)
    {
      theDealer->addWinloss((-1) * third_split_bet);
      theDealer->addCasinoDrop(third_split_bet);
    }

    if (dealer_busted)
    {
      continue;
    }






        // FIRST HAND
    // Dealer hand is HIGHER
    if (!player_blackjack && !first_hand_busted &&
        dealer_final > first_hand_final)
    {
      theDealer->addWinloss(main_bet);
      theDealer->addCasinoDrop(main_bet);

    }
    // Player hand is LOWER
    if (!player_blackjack && !first_hand_busted &&
        dealer_final < first_hand_final)
    {
      theDealer->addWinloss((-1) * main_bet);
      theDealer->addCasinoDrop(main_bet);

    }



        // SECOND SPLIT HAND
    // Dealer hand is HIGHER
    if (!player_blackjack && !second_hand_busted &&
        dealer_final > second_hand_final)
    {
      theDealer->addWinloss(second_split_bet);
      theDealer->addCasinoDrop(second_split_bet);

    }
    // Player hand is LOWER
    if (!player_blackjack && !second_hand_busted &&
        dealer_final < second_hand_final)
    {
      theDealer->addWinloss((-1) * second_split_bet);
      theDealer->addCasinoDrop(second_split_bet);

    }


      // THIRD SPLIT HAND
    // Dealer hand is HIGHER
    if (!player_blackjack && !third_hand_busted &&
        dealer_final > third_hand_final)
    {
      theDealer->addWinloss(third_split_bet);
      theDealer->addCasinoDrop(third_split_bet);

    }
    // Player hand is LOWER
    if (!player_blackjack && !first_hand_busted &&
        dealer_final < third_hand_final)
    {
      theDealer->addWinloss((-1) * third_split_bet);
      theDealer->addCasinoDrop(third_split_bet);

    }





    // PUSH DROP
    //===================
    // Add drop from push

    if (!player_blackjack && !first_hand_busted &&
        dealer_final == first_hand_final)
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(main_bet);
    }


    if (player_blackjack && !second_hand_busted &&
        dealer_final == second_hand_final)
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(second_split_bet);
    }


    if (!player_blackjack && !third_hand_busted &&
        dealer_final == third_hand_final)
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(third_split_bet);
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
  theDealer -> dealerHand[0] ->cardValue = dealerUp;
}

// ======================================================
// ============      SET SOFT CARDS        ==============

// Description: set manually first two cards and dealer up card
// Provide player hand total and dealer up card
void Table::setSoftCards(int playerTotal, int dealerUp)
{
  // Player
  for(Player * aPlayer : players)
  {
    aPlayer -> playerHand[0]->cardValue = 1;
    aPlayer -> playerHand[1]->cardValue = playerTotal - 11;
  }
  // Dealer
  theDealer -> dealerHand[0]->cardValue = dealerUp;
}

// ======================================================
// ============      SET SPLIT CARDS        ==============

// Description: set manually first two cards and dealer up card
// Provide player hand total and dealer up card
void Table::setSplitCards(int playerTotal, int dealerUp){
  // Player
  for(Player * aPlayer : players)
  {
    aPlayer -> playerHand[0]->cardValue = playerTotal / 2;
    aPlayer -> playerHand[1]->cardValue = playerTotal / 2;
  }
  // Dealer
  theDealer -> dealerHand[0]->cardValue = dealerUp;
}

// ======================================================
// ============    RESET PLAYER AND DEALER ==============
// ======================================================
// Description: Returns all default values for player
//              and dealer, all hands are cleared.

void Table::reset()
{
  for(Player * aPlayer : players)
  {
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