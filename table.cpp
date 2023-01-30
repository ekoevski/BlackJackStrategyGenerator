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
  // printf("\n....Table::playRound()...\n");

  //  Set the bets
  for (Player *aPlayer : players)
  {
    aPlayer->setBets(bet);
  }

  // Dealer has BJ
  if (theDealer->hasBlackJack())
  {
    // Collect money if player doesnt have BJ otherwise push
    for (Player *aPlayer : players)
    {
      if (!aPlayer->hasBlackJack())
      {
        theDealer->addWinloss(aPlayer->getPlayerTotalBets());
        theDealer->addCasinoDrop(aPlayer->getPlayerTotalBets());
        // printf(" \nPAYOUT: PLAYER(%d) (Dealer has BJ) winloss: %f, drop: %f",
        //        aPlayer->playerSeat, aPlayer->getPlayerTotalBets(),
        //        aPlayer->getPlayerTotalBets());
      }
    }
    // sleep_ms(10000);
    theDealer->pickUpCards();
    return; // Clean up and end round
  }

  // Pay all BJs
  for (Player *aPlayer : players)
  {
    if (aPlayer->hasBlackJack())
    {
      theDealer->addWinloss((-1.5) * aPlayer->getPlayerTotalBets());
      theDealer->addCasinoDrop(aPlayer->getPlayerTotalBets());
      // printf(" \nPAYOUT: PLAYER(%d) (Player has BJ) winloss: %f, drop: %f",
      //        aPlayer->playerSeat, (-1.5) * aPlayer->getPlayerTotalBets(),
      //       aPlayer->getPlayerTotalBets());
    }
  }

  // Players act on hands
  for (Player *aPlayer : players)
  {
    aPlayer->firstTwo(theDealer->showUpCard());
  }

  // Dealer completes hand
  theDealer->playHand();

  // Determine winners
  for (Player *aPlayer : players)
  {
    // printf("\n\n\n------------PAYOUT-------------\n PLAYER SEAT:(%d)\n\n",
    //       aPlayer->playerSeat);

    // Take busted hands
    if (aPlayer->firstHandBusted())
    {
      theDealer->addWinloss(aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());
      // printf(" \nPAYOUT: (firstHandBusted()) winloss: %f, drop: %f",
      //     aPlayer->getMainBet(), aPlayer->getMainBet());
    }
    if (aPlayer->secondHandBusted())
    {
      theDealer->addWinloss(aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
      // printf(" \nPAYOUT: (secondHandBusted()) winloss: %f, drop: %f",
      //       aPlayer->getSecondSplitBet(), aPlayer->getSecondSplitBet());
    }
    if (aPlayer->thirdHandBusted())
    {
      theDealer->addWinloss(aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
      // printf(" \nPAYOUT: (thirdHandBusted()) winloss: %f, drop: %f",
      //      aPlayer->getThirdSplitBet(), aPlayer->getThirdSplitBet());
    }

    // If dealer busts pay and skip
    if (theDealer->Busted() && !aPlayer->hasBlackJack() &&
        !aPlayer->firstHandBusted())
    {
      theDealer->addWinloss((-1) * aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());
      // printf(" \nPAYOUT: (dealer busted) first hand winloss: %f, drop: %f",
      //      (-1) * aPlayer->getMainBet(), aPlayer->getMainBet());
    }
    if (theDealer->Busted() && !aPlayer->hasBlackJack() &&
        !aPlayer->secondHandBusted())
    {
      theDealer->addWinloss((-1) * aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
      // printf(" \nPAYOUT: (dealer busted) second hand winloss: %f, drop: %f",
      //      (-1) * aPlayer->getSecondSplitBet(), aPlayer->getSecondSplitBet());
    }
    if (theDealer->Busted() && !aPlayer->hasBlackJack() &&
        !aPlayer->thirdHandBusted())
    {
      theDealer->addWinloss((-1) * aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
      // printf(" \nPAYOUT: (dealer busted) third hand winloss: %f, drop: %f",
      //      (-1) * aPlayer->getThirdSplitBet(), aPlayer->getThirdSplitBet());
    }

    if (theDealer->Busted())
    {
      continue;
    }

    // Dealer hand is higerh
    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() > aPlayer->firstHandFinal())
    {
      theDealer->addWinloss(aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());
      // printf(" \nPAYOUT: (dealer hand is higher) winloss: %f, drop: %f",
      //      aPlayer->getMainBet(), aPlayer->getMainBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //      "!aPlayer->firstHandBusted(): %d",
      //      !aPlayer->hasBlackJack(), !aPlayer->firstHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, > "
      //      "aPlayer->firstHandFinal(): %d",
      //      theDealer->dealerFinal(), aPlayer->firstHandFinal());
    }
    // Player hand is lower
    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() < aPlayer->firstHandFinal())
    {
      theDealer->addWinloss((-1) * aPlayer->getMainBet());
      theDealer->addCasinoDrop(aPlayer->getMainBet());
      // printf(" \nPAYOUT: (dealer hand is lower) winloss: %f, drop: %f",
      //      (-1) * aPlayer->getMainBet(), aPlayer->getMainBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //      "!aPlayer->firstHandBusted(): %d",
      //      !aPlayer->hasBlackJack(), !aPlayer->firstHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, < "
      //      "aPlayer->firstHandFinal(): %d",
      //      theDealer->dealerFinal(), aPlayer->firstHandFinal());
    }

    // Dealer hand is higerh
    if (!aPlayer->hasBlackJack() && !aPlayer->secondHandBusted() &&
        theDealer->dealerFinal() > aPlayer->secondHandFinal())
    {
      theDealer->addWinloss(aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
      // printf(" \nPAYOUT: (dealer SECOND hand is higher) winloss: %f, drop: %f",
      //        aPlayer->getSecondSplitBet(), aPlayer->getSecondSplitBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //       "!aPlayer->secondHandBusted(): %d",
      //       !aPlayer->hasBlackJack(), !aPlayer->secondHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, > "
      //       "aPlayer->secondHAndFinal(): %d",
      //       theDealer->dealerFinal(), aPlayer->secondHandFinal());
    }
    // Player hand is lower
    if (!aPlayer->hasBlackJack() && !aPlayer->secondHandBusted() &&
        theDealer->dealerFinal() < aPlayer->secondHandFinal())
    {
      theDealer->addWinloss((-1) * aPlayer->getSecondSplitBet());
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
      // printf(" \nPAYOUT: (dealer SECOND hand is lower) winloss: %f, drop: %f",
      //        (-1) * aPlayer->getSecondSplitBet(), aPlayer->getSecondSplitBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //        "!aPlayer->secondHandBusted(): %d",
      //        !aPlayer->hasBlackJack(), !aPlayer->secondHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, < "
      //        "aPlayer->secondHandFinal(): %d",
      //        theDealer->dealerFinal(), aPlayer->secondHandFinal());
    }

    // Dealer hand is higerh
    if (!aPlayer->hasBlackJack() && !aPlayer->thirdHandBusted() &&
        theDealer->dealerFinal() > aPlayer->thirdHandFinal())
    {
      theDealer->addWinloss(aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
      // printf(" \nPAYOUT: (dealer THIRD hand is higher) winloss: %f, drop: %f",
      //        aPlayer->getThirdSplitBet(), aPlayer->getThirdSplitBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //        "!aPlayer->thirdHandBusted(): %d",
      //        !aPlayer->hasBlackJack(), !aPlayer->thirdHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, > "
      //        "aPlayer->thirdHandFinal(): %d",
      //        theDealer->dealerFinal(), aPlayer->thirdHandFinal());
    }
    // Player hand is lower
    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() < aPlayer->thirdHandFinal())
    {
      theDealer->addWinloss((-1) * aPlayer->getThirdSplitBet());
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
      // printf(" \nPAYOUT: (dealer THIRD hand is lower) winloss: %f, drop: %f",
      //        (-1) * aPlayer->getThirdSplitBet(), aPlayer->getThirdSplitBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //        "!aPlayer->thirdHandBusted(): %d",
      //        !aPlayer->hasBlackJack(), !aPlayer->thirdHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, < "
      //        "aPlayer->thirdHandFinal(): %d",
      //        theDealer->dealerFinal(), aPlayer->thirdHandFinal());
    }

    // Add drop from push

    if (!aPlayer->hasBlackJack() && !aPlayer->firstHandBusted() &&
        theDealer->dealerFinal() == aPlayer->firstHandFinal())
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(aPlayer->getMainBet());
      // printf(" \nPAYOUT: (PUSH) winloss: %f, drop: %f", 0.0,
      //        aPlayer->getMainBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //        "!aPlayer->firstHandBusted(): %d",
      //        !aPlayer->hasBlackJack(), !aPlayer->firstHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, > "
      //        "aPlayer->firstHandFinal(): %d",
      //        theDealer->dealerFinal(), aPlayer->firstHandFinal());
    }

    if (!aPlayer->hasBlackJack() && !aPlayer->secondHandBusted() &&
        theDealer->dealerFinal() == aPlayer->secondHandFinal())
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(aPlayer->getSecondSplitBet());
      // printf(" \nPAYOUT: (dealer SECOND hand PUSH) winloss: %f, drop: %f", 0.0,
      //       aPlayer->getSecondSplitBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //"!aPlayer->secondHandBusted(): %d",
      //       !aPlayer->hasBlackJack(), !aPlayer->secondHandBusted());
      // printf(" \n  Conditions2: theDealer->dealerFinal() %d, > "
      //       "aPlayer->secondHAndFinal(): %d",
      //       theDealer->dealerFinal(), aPlayer->secondHandFinal());
    }

    if (!aPlayer->hasBlackJack() && !aPlayer->thirdHandBusted() &&
        theDealer->dealerFinal() == aPlayer->thirdHandFinal())
    {
      theDealer->addWinloss(0.0);
      theDealer->addCasinoDrop(aPlayer->getThirdSplitBet());
      // printf(" \nPAYOUT: (dealer THIRD hand is PUSH!!) winloss: %f, drop: %f",
      //        0.0, aPlayer->getThirdSplitBet());
      // printf(" \n  Conditions: !aPlayer->hasBlackJack(): %d, && "
      //       "!aPlayer->thirdHandBusted(): %d",
      //       !aPlayer->hasBlackJack(), !aPlayer->thirdHandBusted());
      /// printf(" \n  Conditions2: theDealer->dealerFinal() %d, > "
      //      "aPlayer->thirdHandFinal(): %d",
      //      theDealer->dealerFinal(), aPlayer->thirdHandFinal());
    }
  }

  theDealer->pickUpCards();
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
    Card *newFirstCard = new Card(2 , 1);
    Card *newSecondCard = new Card(playerTotal - 2 , 1);
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0] = newFirstCard;
      aPlayer -> playerHand[1] = newSecondCard;
    }  
  }
  
  if(playerTotal > 12){
    Card *newFirstCard = new Card(10 , 1);
    Card *newSecondCard = new Card(playerTotal - 10 , 1);
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0] = newFirstCard;
      aPlayer -> playerHand[1] = newSecondCard;
    }  
  }

  // Dealer
  Card *dealerUpCard = new Card(dealerUp + 1, 1);
  theDealer -> dealerHand[0] = dealerUpCard;
}

void Table::setSoftCards(int playerTotal, int dealerUp){
  // Player

    Card *newFirstCard = new Card(1 , 1);
    Card *newSecondCard = new Card(playerTotal - 11 , 1);
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0] = newFirstCard;
      aPlayer -> playerHand[1] = newSecondCard;
    }  

  
  // Dealer
  Card *dealerUpCard = new Card(dealerUp + 1, 1);
  theDealer -> dealerHand[0] = dealerUpCard;
}


void Table::setSplitCards(int playerTotal, int dealerUp){
  // Player

    Card *newFirstCard = new Card(playerTotal + 1 , 1);
    Card *newSecondCard = new Card(playerTotal + 1 , 1);
    for(Player * aPlayer : players){
      aPlayer -> playerHand[0] = newFirstCard;
      aPlayer -> playerHand[1] = newSecondCard;
    }  
  

  // Dealer
  Card *dealerUpCard = new Card(dealerUp + 1, 1);
  theDealer -> dealerHand[0] = dealerUpCard;
}