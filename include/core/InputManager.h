
#pragma once
#include <unordered_map>

class InputManager {
public:
    void keyDown(unsigned char key);
    void keyUp(unsigned char key);
    bool isPressed(unsigned char key) const;
    bool isCombo(unsigned char key1, unsigned char key2) const;

private:
    std::unordered_map<unsigned char, bool> keyStates;
};
