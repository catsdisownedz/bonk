#pragma once
#include <utility>
#include <physics/GameObject.h>
#include <core/InputManager.h>
#include <string>
#include <ui/ColorOption.h>

using namespace std;
//using GameObject::setColor;

class Player : public GameObject {
    public:
        Player(int id);
        void updateScore(){score++;};
        int getId(){return id;};
        int returnScore(){return score;};
        void move(double x);
        void update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration);
        bool isJumping();
        void setJumping(bool isJumping);
        //bool Jump();
        void tick();
        bool getIsMoving();
        void draw();
        void drawHalo();
        void display();
        void jump();
        void handleInput(const InputManager& input);
        void enableBoostJump() { canBoostJump = true; }
        void disableBoostJump() { canBoostJump = false; }
        void resetFallBoost() { storedFallBoost = 0.0; isBoostingFall = false; }
        bool isFallingBoosted() const { return isBoostingFall; }
        void changeSurface();
        bool getOnSurface();
        double getCurrentMass();

        struct PlayerBounds {
            double left, right, top, bottom;
        };
    
        PlayerBounds getBounds(double radius = 0.08) const {
            auto pos = getPosition();
            return {
                pos.first - radius, // left
                pos.first + radius, // right
                pos.second + radius, // top
                pos.second - radius  // bottom
            };
        }

        int score;
        int id;
        bool jumping;
        bool isAlive;
        bool isMoving;
        bool canBoostJump = false;
        bool isBoostingFall = false;
        double storedFallBoost = 0.0; //how much extra force to add into next bounce 
        double landedTimer = 0.0;
        bool landedRecently = false;
        bool isSpaceBarPressed = false; // Track if space bar is pressed to draw halo
        bool onSurface = false;
        double baseMass = 1.0;
        double currentMass = baseMass;
        string username;
        void   setName(const std::string& n) { username = n; }
        const  std::string& getName() const  { return username; }
        void  drawName() const;
        
        const ColorOption& getColorOption() const {
            return m_color;
        }

        // call this once after you read the RGB triple from the Menu:
        void setColorOption(const ColorOption &c) {
            // 1) store it for Player::draw() and GameOver
            m_color = c;
            // 2) keep the base‐class vector<double> in sync
            GameObject::setColor({c.r, c.g, c.b});
        }
    private: 
        ColorOption m_color;
};

