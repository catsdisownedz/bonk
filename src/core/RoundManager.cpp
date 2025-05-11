#include "../../include/core/RoundManager.h"
#include "../../include/core/Renderer.h"
#include <vector>

RoundManager::RoundManager(){
    currentRound=0;
    totalRounds=5;
}



void RoundManager::nextRound(){
    if(currentRound+1<totalRounds)
        currentRound++;
}

void RoundManager::resetRounds() {
    currentRound = 0;
}

