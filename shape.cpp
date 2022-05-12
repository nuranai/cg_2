#include "shape.h"

Coords::Coords(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

Connection::Connection(int _begin, int _end) {
    begin = _begin;
    end = _end;
}

Shape::Shape(int _n, Coords** _coords, int _conN, Connection** _connections)
{
// this shape is going to be super sexy
    n = _n;
    ver = _coords;
    conN = _conN;
    reb = _connections;
//    reb = new Connection*[_n];
//    for (int i = 0; i < _n; i++) {
//        Connection temp(i, (i + 1) % _n);
//        reb[i] = &temp;
//    }
//    ver = new Coords();
}

void Shape::SetCoords(int i, float newX, float newY, float newZ) {
    ver[i]->x = newX;
    ver[i]->y = newY;
    ver[i]->z = newZ;
}

Shape::~Shape() {
// we are about to destroy a super sexy shape
    for (int i = 0; i < n; i++) {
        delete ver[i];
    }
    delete []ver;
}
