#ifndef MAP_H
#define MAP_H

class Map{
    public:
    virtual void draw() = 0;        // Pure virtual: each map draws its layout
    virtual void display() = 0;        // Pure virtual: each map loads its own layout
    // virtual ~Map() {}               // Virtual destructor (important for cleanup)

};

#endif