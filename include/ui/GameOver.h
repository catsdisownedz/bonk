#pragma once

#include <vector>
#include <string> 


class GameOver
{
public:
    GameOver();
    void draw(int winningPlayer);
    void renderText(float x, float y, const std::string& text, void* font);
    // int winningPlayer=-1;
   

};
