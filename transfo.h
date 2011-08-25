#ifndef __TRANSFO_H__256000111

#define __TRANSFO_H__256000111

class Transformation {
    
    public:

    float matrix[16];

    Transformation () {
        this->matrix={1,0,0,0,   0,1,0,0, 0,0,1,0, 0,0,0,1};
    }

    void translate(const float delta[3]);
    
    void rotationX(const float rot);
    void rotationY(const float rot);
    void rotationZ(const float rot);
};

#endif
