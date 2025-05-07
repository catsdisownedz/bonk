// hena kol el entitites el fl game worls
// el mfrood yeddy data le renderer for visualization
// we ya5od physics properties mn physics engine
#include <physics/GameObject.h>
#include<vector>
using namespace std;
GameObject::GameObject(double x, double y,double r, double g, double b)
{
    color={r,g,b};
    position = {x, y};
    velocity = {0.0, 0.0};
    momentum = {0.0, 0.0};
    acceleration = {0.0, 0.0};
    mass = 1.0;
}
vector<double> GameObject::getColor() const { return color; }
void GameObject::setColor(vector<double> newColor) { color=newColor; }
pair<double, double> GameObject::getPosition() const { return position; }
void GameObject::setPosition(pair<double, double> newPosition) { position = newPosition; }
pair<double, double> GameObject::getVelocity() { return velocity; }
void GameObject::setVelocity(pair<double, double> newVelocity) { velocity = newVelocity; }
pair<double, double> GameObject::getMomentum() { return momentum; }
void GameObject::setMomentum(pair<double, double> newMomentum) { momentum = newMomentum; }
double GameObject::getMass() { return mass; }
void GameObject::setMass(double newMass) { mass = newMass; }
pair<double, double> GameObject::getAcceleration() { return acceleration; }
void GameObject::setAcceleration(pair<double, double> newAcceleration) { acceleration = newAcceleration; }
void GameObject::update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration)
{
    this->position = position;
    this->velocity = velocity;
    this->momentum = momentum;
    this->acceleration = acceleration;
}
void GameObject::addAcceleration(double xAcceleration, double yAcceleration){
    acceleration.first += xAcceleration;
    acceleration.second += yAcceleration;
}

