#include "operators.h"

SingleQubitGate operator+(SingleQubitGate gate, int spaces)
{
    gate.move_x(spaces);
    return gate;
}