#include"vertex.h"


/**
 * renvoie un nouveau Vertex résultat de la multipication membre à membre
 * des éléments du vecteur courant avec ceux de v
 **/
Vertex Vertex::operator*(const Vertex v) {
    float x, y, z;
    x = v.x * this->x;
    y = v.y * this->y;
    z = v.z * this->z;
    return Vertex(x, y, z);
}

/**
 * Renvoie un nouveau Vertex résultat de la soustraction membre à membre
 * des éléments du vecteur courant avec ceux de v
 **/
Vertex Vertex::operator-(const Vertex v) {
    float x, y, z;
    x = this->x - v.x;
    y = this->y - v.y;
    z = this->z - v.z;
    return Vertex(x, y, z);
}
    
/**
 * renvoie un nouveau Vertex avec les éléments du vecteur courant divisés par une constante
 **/
Vertex Vertex::operator/(const float f) {
    float x, y, z;
    x = this->x / f;
    y = this->y / f;
    z = this->z / f;
    return Vertex(x, y, z);
}

/**
 * Normalise le vecteur courant
 **/
void Vertex::normer() {
    float n  = this->norme();
    this->x /= n;
    this->y /= n;
    this->z /= n;
}

/**
 * Calcule la norme du vecteur courant
 * TODO: optimiser sqrt au besoin, voir http://www.codemaestro.com/reviews/9  ?
 **/
float Vertex::norme() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}



