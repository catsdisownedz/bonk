#pragma once

class Map{
    public:
    void draw() ;        // Pure virtual: each map draws its layout
    void display();        // Pure virtual: each map loads its own layout
    // virtual ~Map() {}               // Virtual destructor (important for cleanup)

};

