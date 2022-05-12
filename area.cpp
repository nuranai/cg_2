#include "area.h"
#include <QPainter>
#include <QTimerEvent>
#include <iostream>
#include <math.h>

using namespace std;

Area::Area(QWidget *parent):QWidget(parent)
{
    Coords** shapeCoords = new Coords*[8];
    shapeCoords[0] = new Coords(0, 0, 0);
    shapeCoords[1] = new Coords(4, 0, 0);
    shapeCoords[2] = new Coords(0, 4, 0);
    shapeCoords[3] = new Coords(0, 0, 4);
    shapeCoords[4] = new Coords(2, 0, 1);
    shapeCoords[5] = new Coords(-2, 0, 1);
    shapeCoords[6] = new Coords(2, 1, 0);
    shapeCoords[7] = new Coords(-2, 1, 0);
    Connection** shapeConnections = new Connection*[5];
    shapeConnections[0] = new Connection(0, 1);
    shapeConnections[1] = new Connection(0, 2);
    shapeConnections[2] = new Connection(0, 3);
    shapeConnections[3] = new Connection(4, 5);
    shapeConnections[4] = new Connection(6, 7);

    shape = new Shape(8, shapeCoords, 5, shapeConnections);

    setFixedSize(QSize(500, 500));

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

    int viewX = 3;
    int viewY = 4;
    int viewZ = 6;
    Coords* translateCoords = new Coords(-viewX, -viewY, -viewZ);
    translateShape(transformMatrix, translateCoords);
    scaleShape(transformMatrix, -1, 1, 1);
    rotateShapeX(transformMatrix, 0, 1);
    float d = sqrt(viewX * viewX + viewY * viewY);
    float cosU = viewY / d;
    float sinU = viewX / d;
    rotateShapeY(transformMatrix, cosU, sinU);
    float s = sqrt(d*d + viewZ * viewZ);
    float cosW = d / s;
    float sinW = viewZ / s;
    rotateShapeX(transformMatrix, cosW, sinW);
//    rotateShapeZ(transformMatrix, cos(90*M_PI/180), sin(90*M_PI/180));
    setNewCoords(shape, transformMatrix);
    float r = sqrt(viewX * viewX + viewY * viewY + viewZ * viewZ);
    for (int i = 0; i < 8; i++) {
        Coords* first = shape->getCoords(i);
        float P = 5;
        float newXF = first->getX()*r/first->getZ();
        newXF = newXF / P * 250 + 250;
        float newYF = first->getY()*r/first->getZ();
        newYF = newYF / P * 250 + 250;
        shape->SetCoords(i, newXF, newYF, first->getZ());
        Coords* bamp = shape->getCoords(i);
    }
    float** mm = new float*[3];
    float** m = new float*[3];
    for (int i = 0; i < 3; i++) {
        mm[i] = new float [3];
        m[i] = new float [1];
        m[i][0] = 1;
        for (int j = 0; j < 3; j++) {
            mm[i][j] = 0;
        }
    }
    float centerX = 0;
    for (int i = 0; i < 8; i++) {
        centerX += shape->getCoords(i)->getX();
//        centerY += shape->getCoords(i)->getY();
    }
    centerX /= 8;
//    centerY /= 8;
    cout << centerX << endl;

    mm[0][1] = 1;
    mm[1][0] = -1;
    mm[2][0] = centerX * 2;
    mm[2][2] = 1;
    for (int i = 0; i < 8; i++) {
        Coords* cor = shape->getCoords(i);
        m[0][0] = cor->getX();
        m[0][1] = cor->getY();
//        matrixMultiplication(m, 1, 3, mm, 3, 3);
//        shape->SetCoords(i, m[0][0], m[0][1], cor->getZ());
//        cout << cor->getX() << ' ' << cor->getY() << ' ' << cor->getZ() << endl;
    }
//    mm[2][0] = -centerX;
//    mm[2][1] = -centerY;

    for (int i = 0; i < 4; i++) {
        delete [] transformMatrix[i];
//        delete [] m[i];
    }
    delete [] transformMatrix;
//    delete [] m;
}

void Area::showEvent(QShowEvent *) {
    update();
}

void Area::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(Qt::red);
    int conN = shape->getConN();
    for (int i = 0; i < conN; i++) {
        Connection* connection = shape->getConnection(i);
        Coords* first = shape->getCoords(connection->getBegin());
        Coords* second = shape->getCoords(connection->getEnd());
        painter.drawLine(first->getX(), first->getY(), second->getX(), second->getY());
    }
}

void Area::timerEvent(QTimerEvent *) {
}

void Area::hideEvent(QHideEvent *) {
}

void Area::start_prog() {
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
