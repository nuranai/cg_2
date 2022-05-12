#ifndef SHAPE_H
#define SHAPE_H

class Shape;

class Coords {
private:
    float x;
    float y;
    float z;
public:
    Coords(float _x, float _y, float _z);
    ~Coords() {};
    float getX() {return x;}
    float getY() {return y;}
    float getZ() {return z;}
    friend class Shape;
};

class Connection {
private:
    int begin;
    int end;
public:
    Connection(int _begin, int _end);
    ~Connection() {};
    int getBegin() {return begin;}
    int getEnd() {return end;}
    friend class Shape;
};

class Shape
{
private:
    Coords **ver;
    Connection **reb;
    int n;
    int conN;
public:
//    Connection **reb;
    Shape(int _n, Coords** _coords, int _conN, Connection** _connections);
    ~Shape();
    friend class Area;
    Coords* getCoords(int i) {return ver[i];}
    Connection* getConnection(int i) {return reb[i];}
    void SetCoords(int i, float, float, float);
    int getN() {return n;}
    int getConN() {return conN;}
};

#endif // SHAPE_H
