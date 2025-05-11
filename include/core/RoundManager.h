#pragma once

#include "../ui/Map.h"  
#include <vector>
#include <string>
using namespace std;

class RoundManager {
public:
    RoundManager();
    int currentRound;
    int totalRounds;
    bool gameOver;
    void resetRounds();  

    // Map* getCurrentMap(); 

  
 
    void nextRound();
    bool isGameOver() const;
    // int getCurrentRound() const;
};


