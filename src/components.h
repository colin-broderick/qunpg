#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

class SingleQubitGate
{
    private:
        int _x;
        int _y;
        std::string label;

    public:
        SingleQubitGate(std::string label);
        std::string draw();
        void move_x(int spaces);
        void move_y(int spaces);

};

#endif