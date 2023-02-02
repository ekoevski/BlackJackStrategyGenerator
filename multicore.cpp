#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "simulator.h"

#define FLAG_VALUE 123

using namespace std;



// MAIN
//=================== 


int main() {


  for (int i = 3; i > 0; i--) {
    printf("    \n\n%d ", i);
    sleep(1);
  }

  Simulator* Sim = new Simulator("Blackjack", 2, 1);  // 1 deck, 1 player


  for(int w = 5; w <= 20; w+=5){
    for(int x = 3; x <= 12; x+=3){
      for(int y = 4; y <= 16; y+=4){
        for(int z = 1; z <= 4;  z++){
          //Sim->optimizeThreaded(5000,z,y,x,w); 
        }
      }
    }
  }

  // Optimise perfect startegy
 Sim->optimizeThreaded(3000,0,32,24,40); 
  //Sim->loadBasicStrategy(4,16,12,20);

  //Sim->optimizeThreaded(50000, 4, 0, 0, 20);
  Sim->BJ->theShoe->createShoe(0, 32,24,40);
  Sim->testCurrentStrategy(500000);
}
