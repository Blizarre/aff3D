#include "transfo.h"
#include "math.h"


void Transformation::translate(const float delta[3]) {
    float* mat = this->matrix;
    mat[3]  += mat[0] * delta[0] + mat[1] * delta[1] + mat[2] * delta[2];
    mat[7]  += mat[4] * delta[0] + mat[5] * delta[1] + mat[6] * delta[2];
    mat[11] += mat[8] * delta[0] + mat[9] * delta[1] + mat[10] * delta[2];
}

void Transformation::rotationX(const float rot) {
    float* mat = this->matrix;
    float a, b, cosR, sinR;
    cosR = cos(rot);
    sinR = sin(rot);

    a = mat[1];
    b = mat[2];
    mat[1] = a*cos(rot) - b*sin(rot);
    mat[2] = a*sin(rot) + b*cos(rot);

    a = mat[5];
    b = mat[6];
    mat[5] = a*cos(rot) - b*sin(rot);
    mat[6] = a*sin(rot) + b*cos(rot);

    a = mat[9];
    b = mat[10];
    mat[9] = a*cos(rot) - b*sin(rot);
    mat[10] = a*sin(rot) + b*cos(rot);
}


void Transformation::rotationZ(const float rot) {
    float* mat = this->matrix;
    float a, b, cosR, sinR;
    cosR = cos(rot);
    sinR = sin(rot);

    a = mat[0];
    b = mat[1];
    mat[0] = a*cos(rot) - b*sin(rot);
    mat[1] = a*sin(rot) + b*cos(rot);

    a = mat[4];
    b = mat[5];
    mat[4] = a*cos(rot) - b*sin(rot);
    mat[5] = a*sin(rot) + b*cos(rot);

    a = mat[8];
    b = mat[9];
    mat[8] = a*cos(rot) - b*sin(rot);
    mat[9] = a*sin(rot) + b*cos(rot);

// Inutile ?
//    a = matrice[12];
//    b = matrice[13];
//    matrice[12] = a*cos(rot) - b*sin(rot);
//    matrice[13] = a*sin(rot) + b*cos(rot);
}

