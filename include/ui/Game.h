// include/ui/Game.h
#pragma once

#include <map>
#include <string>
#include<physics/Player.h>
#include <ui/GameScene.h>   // your base class for scenes
#include <vector>
#include <string>
#include <ui/ColorOption.h> 

using std::vector;
class Game {
public:
    // singleton accessor
    static Game& instance();

    // call once at startup
    void init();

    // switch active scene by name
    void changeScene(const std::string& name);

    // per-frame update & render
    void update();
    void render();

    // input dispatchers
    void onKeyDown       (unsigned char key, int x, int y);
    void onKeyUp         (unsigned char key, int x, int y);
    void onSpecialDown   (int key,         int x, int y);
    void onSpecialUp     (int key,         int x, int y);
    void onMouse         (int button, int state, int x, int y);
    void onPassiveMotion (int x, int y);
   // void setPlayerColors(const ColorOption &c1, const ColorOption &c2);
    // window was resized
    void onReshape(int w, int h);
    vector<double> playerColor1, playerColor2;
    // query which scene is currently active    
    const std::string& getCurrentSceneName() const { return currentSceneName; }
    const std::string& getPrevoiusSceneName() const { return prevoiusSceneName; }
    void setPlayerNames(const std::string& n1, const std::string& n2) {
        playerName1 = n1;
        playerName2 = n2;
    }

    const std::string& getPlayerName(int id) const {
        return (id == 1 ? playerName1 : playerName2);
    }

    ColorOption getPlayerColor(int id) {
     return getPlayerById(id).getColorOption();
    }
    
    Player& getPlayerById(int id);

    const std::vector<Player>& getPlayers() const;

    int   getWinCount(int playerId) const;
     void recordWin(int playerId);
      void addPlayer(const Player& p); 
      int getLastWinner() const {        //–– new getter for GameOverScene
       return lastWinnerId;
    }

private:
    Game();
    Game(const Game&)=delete;
    Game& operator=(const Game&)=delete;

    // all registered scenes
    std::map<std::string, GameScene*> scenes;

    // the active scene pointer
    GameScene*           currentScene     = nullptr;

    // the name of the active scene
    std::string          currentSceneName;
    std::string          prevoiusSceneName;

    // store last window size so new scenes can reapply projections
    int                  windowW = 800;
    int                  windowH = 600;
    std::string playerName1;
    std::string playerName2;
    std::vector<Player>        players;
    std::map<int,int>          winCounts;
    int lastWinnerId = 0; 

};
