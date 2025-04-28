#pragma once
#include <utility>
#include <vector>

using namespace std;

class GameObject{
public:
    GameObject(double x, double y,double r, double g, double b);
    virtual ~GameObject() {};
    void drawPlatform();
    void drawPlayers();
    vector<double> getColor() const;
    void setColor(vector<double> newColor);
    pair<double, double> getPosition() const;
    void setPosition(pair<double, double> newPosition);
    pair<double, double> getVelocity();
    void setVelocity(pair<double, double> newVelocity);
    pair<double, double> getMomentum();
    void setMomentum(pair<double, double> newMomentum);
    double getMass();
    void setMass(double newMass);
    pair<double, double> getAcceleration();
    void setAcceleration(pair<double, double> newAcceleration);
    void update(pair<double, double> position, pair<double, double> velocity, pair<double, double> momentum, pair<double, double> acceleration);
    void addAcceleration(double xAcceleration, double yAcceleration);

private:
    vector<double> color;
    pair<double, double> position;
    pair<double, double> velocity;
    pair<double, double> momentum;
    double mass;
    pair<double, double> acceleration;
};