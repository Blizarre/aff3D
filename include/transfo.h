#ifndef __TRANSFO_H__256000111

#define __TRANSFO_H__256000111

class Transformation {
    
    public:

    float matrix[16];

    Transformation () {
        // Initialisation de la matrice unité : 0 partout sauf sur la
        // diagonale où il y a des 1
        for(int i=1; i<15; i++)
            this->matrix[i] = 0;
        this->matrix[0] = 1;
        this->matrix[5] = 1;
        this->matrix[10] = 1;
        this->matrix[15] = 1;
    }

    void translate(const float delta[3]);
    
    void rotationX(const float rot);
    void rotationY(const float rot);
    void rotationZ(const float rot);
};

#endif
