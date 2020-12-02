#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder: public Shape
{
public:
    Cylinder();
    Cylinder(int param1, int param2);
    ~Cylinder();
private:
    int m_param1;
    int m_param2;
};

#endif // CYLINDER_H
