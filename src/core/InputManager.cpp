//ne send input commands to game, menumanager, and network manager 
//update el player controlled gameobject instance


 #include "../../include/core/InputManager.h"

 void InputManager::keyDown(unsigned char key){
    keyStates[key] = true;
 };

 void InputManager::keyUp(unsigned char key){
    keyStates[key] = false;
 };

 bool InputManager::isPressed(unsigned char key) const{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it -> second;
 };


bool InputManager::isCombo(unsigned char key1, unsigned char key2) const {
    return isPressed(key1) && isPressed(key2);
}

bool InputManager::isTripleCombo(unsigned char key1, unsigned char key2, unsigned char key3) const {
    return isPressed(key1) && isPressed(key2) && isPressed(key3);
}

// for special keys like arrows bec thery are special 

void InputManager::specialKeyDown(int key) {
    specialKeyStates[key] = true;
}

void InputManager::specialKeyUp(int key) {
    specialKeyStates[key] = false;
}

bool InputManager::isSpecialPressed(int key) const {
    auto it = specialKeyStates.find(key);
    return it != specialKeyStates.end() && it->second;
}

bool InputManager::isSpecialCombo(int key1, int key2) const {
    return isSpecialPressed(key1) && isSpecialPressed(key2);
}

bool InputManager::isSpecialTripleCombo(int key1, int key2, int key3) const {
    return isSpecialPressed(key1) && isSpecialPressed(key2) && isSpecialPressed(key3);
}