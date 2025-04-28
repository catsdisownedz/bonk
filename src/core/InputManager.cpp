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

