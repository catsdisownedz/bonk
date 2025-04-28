
#pragma once
#include <unordered_map>

class InputManager {
public:
    void keyDown(unsigned char key);
    void keyUp(unsigned char key);
    bool isPressed(unsigned char key) const;
    bool isCombo(unsigned char key1, unsigned char key2) const;
    bool isTripleCombo(unsigned char key1, unsigned char key2, unsigned char key3) const;
    //special keys
    void specialKeyDown(int key);
    void specialKeyUp(int key);
    bool isSpecialPressed(int key) const;
    bool isSpecialCombo(int key1, int key2) const ;
    bool isSpecialTripleCombo(int key1, int key2, int key3) const ;


private:
    std::unordered_map<unsigned char, bool> keyStates,specialKeyStates;
};
