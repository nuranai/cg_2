#include "area.h"
#include <QPainter>
#include <QTimerEvent>
#include <iostream>
#include <math.h>

using namespace std;

Area::Area(QWidget *parent):QWidget(parent)
{
//    Coords** shapeOrinigCoords = new Coords*[4];
//    shapeOrinigCoords[0] = new Coords(2, 0, 0);
//    shapeOrinigCoords[1] = new Coords(0, 2, 0);
//    shapeOrinigCoords[2] = new Coords(-2, 0, 0);
//    shapeOrinigCoords[3] = new Coords(0, 0, 4);


//    Connection** shapeOriniConnections = new Connection*[6];
//    shapeOriniConnections[0] = new Connection(0, 1);
//    shapeOriniConnections[1] = new Connection(0, 2);
//    shapeOriniConnections[2] = new Connection(0, 3);
//    shapeOriniConnections[3] = new Connection(1, 2);
//    shapeOriniConnections[4] = new Connection(1, 3);
//    shapeOriniConnections[5] = new Connection(2, 3);


//    shapeOrigin = new Shape(4, shapeOrinigCoords, 6, shapeOriniConnections);

    setFixedSize(QSize(500, 500));
}

void Area::showEvent(QShowEvent *) {
    update();
}

void Area::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(Qt::red);

    Coords** shapeCoords = new Coords*[4];
    shapeCoords[0] = new Coords(2, 0, 0);
    shapeCoords[1] = new Coords(0, 2, 0);
    shapeCoords[2] = new Coords(-2, 0, 0);
    shapeCoords[3] = new Coords(0, 0, 4);

    Connection** shapeConnections = new Connection*[6];
    shapeConnections[0] = new Connection(0, 1);
    shapeConnections[1] = new Connection(0, 2);
    shapeConnections[2] = new Connection(0, 3);
    shapeConnections[3] = new Connection(1, 2);
    shapeConnections[4] = new Connection(1, 3);
    shapeConnections[5] = new Connection(2, 3);

    Shape* shape = new Shape(4, shapeCoords, 6, shapeConnections);

    float** transformMatrix = new float*[4];

    for (int i = 0; i < 4; i++) {
        transformMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                transformMatrix[i][j] = 1;
            }
            else
                transformMatrix[i][j] = 0;
        }
    }

    float _viewX = viewX;
    float _viewY = viewY;
    float _viewZ = viewZ;

    Coords* view = new Coords(_viewX, _viewY, _viewZ);

    float cos1 = cos(M_PI * alpha/ 180);
    float sin1 = sin(M_PI * alpha / 180);

    rotateShapeY(transformMatrix, cos1, sin1);
    float** superCoords = new float*[4];
    for(int i = 0; i < 4 ; i++) {
        superCoords[i] = new float[1];
    }

    superCoords[0][0] = viewX;
    superCoords[0][1] = viewY;
    superCoords[0][2] = viewZ;
    superCoords[0][3] = 1;


    matrixMultiplication(superCoords, 1, 4, transformMatrix, 4, 4);

    view->setX(superCoords[0][0]);
    view->setY(superCoords[0][1]);
    view->setZ(superCoords[0][2]);

//    viewX = view->getX();
//    viewY = view->getY();
//    viewZ = view->getZ();

//    cout << superCoords[0][0] << ' ' <<superCoords[0][1] << ' ' <<superCoords[0][2] << ' ' << endl;
    Coords* translateCoords = new Coords(-view->getX(), -view->getY(), -view->getZ());

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                transformMatrix[i][j] = 1;
            }
            else
                transformMatrix[i][j] = 0;
        }
    }

    cout << translateCoords->getX() << ' ' << translateCoords->getY() << ' ' << translateCoords->getZ() << endl;
    translateShape(transformMatrix, translateCoords);

    scaleShape(transformMatrix, -1, 1, 1);

    rotateShapeX(transformMatrix, 0, 1);

    float d = sqrt(view->getX() * view->getX() + view->getY() * view->getY());

    float cosU = view->getY() / d;
    float sinU = view->getX() / d;

    rotateShapeY(transformMatrix, cosU, sinU);

    float s = sqrt(d*d + view->getZ() * view->getZ());

    float cosW = d / s;
    float sinW = view->getZ() / s;

    rotateShapeX(transformMatrix, cosW, sinW);

    setNewCoords(shape, transformMatrix);

    float r = sqrt(view->getX() * view->getX() + view->getY() * view->getY() + view->getZ() * view->getZ());

    for (int i = 0; i < 4; i++) {
        Coords* first = shape->getCoords(i);

//        cout << "shapeOrigin::" << shapeOrigin->getCoords(i)->getX() << ' ' << shapeOrigin->getCoords(i)->getY() << ' ' << shapeOrigin->getCoords(i)->getZ() << ' ' << endl;
        float P = 5;
//        float newXF = first->getX()*r/first->getZ();
        float newXF = first->getX();
        newXF = newXF / P * 250 + 250;
//        float newYF = first->getY()*r/first->getZ();
        float newYF = first->getY();
        newYF =  newYF / P * 250 + 250;
        shape->SetCoords(i, newXF, 500 - newYF, first->getZ());
//        cout << "shape::" << shape->getCoords(i)->getX() << ' ' << shape->getCoords(i)->getY() << ' ' << shape->getCoords(i)->getZ() << ' ' << endl;
//        delete first;
    }

    for (int i = 0; i < 4; i++) {
        delete [] transformMatrix[i];
//        delete [] superCoords[i];
    }
    delete [] transformMatrix;
//    delete [] superCoords;
    delete view;

    int conN = shape->getConN();
    for (int i = 0; i < conN; i++) {
        Connection* connection = shape->getConnection(i);
        Coords* first = shape->getCoords(connection->getBegin());
        Coords* second = shape->getCoords(connection->getEnd());
        painter.drawLine(first->getX(), first->getY(), second->getX(), second->getY());
//        delete connection;
//        delete first;
//        delete second;
    }
    delete shape;
}

void Area::timerEvent(QTimerEvent *) {
    alpha += 1;
    if (alpha > 15)
        killTimer(myTimer);
    else
        update();
}

void Area::hideEvent(QHideEvent *) {
}

void Area::start_prog() {
    myTimer = startTimer(50);
}

void Area::setNewCoords(Shape *shape, float** changeMatrix) {
    int n = shape->getN();
    Coords **coords = new Coords*[n];
    float **newMatrix = new float*[1];
    newMatrix[0] = new float[4];
    for (int i = 0; i < n; i++) {
        coords[i] = shape->getCoords(i);
    }
    for (int i = 0; i < n; i++) {
        newMatrix[0][0] = coords[i]->getX();
        newMatrix[0][1] = coords[i]->getY();
        newMatrix[0][2] = coords[i]->getZ();
        newMatrix[0][3] = 1;
        matrixMultiplication(newMatrix, 1, 4, changeMatrix, 4, 4);
        shape->SetCoords(i, newMatrix[0][0], newMatrix[0][1], newMatrix[0][2]);
    }
    delete [] newMatrix[0];
    delete [] newMatrix;
    delete [] coords;
}

void Area::translateShape(float** oldMatrix, Coords* value) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[3][0] = value->getX();
    newMatrix[3][1] = value->getY();
    newMatrix[3][2] = value->getZ();
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}

void Area::rotateShapeX(float** oldMatrix, float cosV, float sinV) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[1][1] = cosV;
    newMatrix[1][2] = -sinV;
    newMatrix[2][1] = sinV;
    newMatrix[2][2] = cosV;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}

void Area::rotateShapeY(float** oldMatrix, float cosV, float sinV) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[0][0] = cosV;
    newMatrix[0][2] = sinV;
    newMatrix[2][0] = -sinV;
    newMatrix[2][2] = cosV;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}
void Area::rotateShapeZ(float** oldMatrix, float cosV, float sinV) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[0][0] = cosV;
    newMatrix[0][1] = -sinV;
    newMatrix[1][0] = sinV;
    newMatrix[1][1] = cosV;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}
void Area::scaleShape(float** oldMatrix, float valueX, float valueY, float valueZ) {
    float** newMatrix = new float*[4];
    for (int i = 0; i < 4; i++) {
        newMatrix[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                newMatrix[i][j] = 1;
            }
            else
                newMatrix[i][j] = 0;
        }
    }
    newMatrix[0][0] = valueX;
    newMatrix[1][1] = valueY;
    newMatrix[2][2] = valueZ;
    matrixMultiplication(oldMatrix, 4, 4, newMatrix, 4, 4);
    for (int i = 0; i < 4; i++) {
        delete [] newMatrix[i];
    }
    delete [] newMatrix;
}

void Area::matrixMultiplication(float **first, int firstRow, int firstColumn, float** second, int secondRow, int secondColumn) {
    float** matrixCopy = new float*[firstRow];

    for (int i = 0; i < firstRow; i++) {
        matrixCopy[i] = new float[firstColumn];
        for (int j = 0; j < firstColumn; j++) {
            matrixCopy[i][j] = first[i][j];
            first[i][j] = 0;
        }
    }
    for (int i = 0; i < firstRow; i++) {
        for (int j = 0; j < secondColumn; j++) {
            for (int k = 0; k < firstColumn; k++) {
                first[i][j] += matrixCopy[i][k] * second[k][j];
            }
        }
    }
    for (int i = 0; i < firstRow; i++) {
        delete [] matrixCopy[i];
    }
    delete [] matrixCopy;
}
