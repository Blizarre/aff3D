#include "transfo.h"
#include "math.h"

void Transformation::translate(const float delta[3]) {
  this->m_matrix[3] +=
      m_matrix[0] * delta[0] + m_matrix[1] * delta[1] + m_matrix[2] * delta[2];
  m_matrix[7] +=
      m_matrix[4] * delta[0] + m_matrix[5] * delta[1] + m_matrix[6] * delta[2];
  m_matrix[11] +=
      m_matrix[8] * delta[0] + m_matrix[9] * delta[1] + m_matrix[10] * delta[2];
}

void Transformation::translate(const std::array<float, 3>& delta) {
  translate(delta.data());
}

/**
 *Attention, les matrices sont des 4x4 !!! Il y a une ligne et une colonne dont
 *les valeurs sont indispensables aux calculs
 **/

/**
 * 1    0       0
 * 0    cosT    -sinT
 * 0    sinT    cosT
 * */
void Transformation::rotationX(const float rot) {
  float a, b, cosR, sinR;
  cosR = cos(rot);
  sinR = sin(rot);

  a = m_matrix[1];
  b = m_matrix[2];
  m_matrix[1] = a * cosR - b * sinR;
  m_matrix[2] = a * sinR + b * cosR;

  a = m_matrix[5];
  b = m_matrix[6];
  m_matrix[5] = a * cosR - b * sinR;
  m_matrix[6] = a * sinR + b * cosR;

  a = m_matrix[9];
  b = m_matrix[10];
  m_matrix[9] = a * cosR - b * sinR;
  m_matrix[10] = a * sinR + b * cosR;
}

/**
 * cosT   0     sinT
 * 0      1     0
 * -sinT  0     cosT
 *  TODO: may be broken, check it
 * */
void Transformation::rotationY(const float rot) {
  float a, b, cosR, sinR;
  cosR = cos(rot);
  sinR = sin(rot);

  a = m_matrix[0];
  b = m_matrix[2];
  m_matrix[0] = a * cosR + b * sinR;
  m_matrix[2] = -a * sinR + b * cosR;

  a = m_matrix[4];
  b = m_matrix[6];
  m_matrix[4] = a * cosR + b * sinR;
  m_matrix[6] = -a * sinR + b * cosR;

  a = m_matrix[8];
  b = m_matrix[10];
  m_matrix[8] = a * cosR - b * sinR;
  m_matrix[10] = -a * sinR + b * cosR;
}

/**
 * cosT  -sinT 0
 * sinT  cosT  0
 * 0     0     1
 * */
void Transformation::rotationZ(const float rot) {
  float a, b, cosR, sinR;
  cosR = static_cast<float>(cos(rot));
  sinR = static_cast<float>(sin(rot));

  a = m_matrix[0];
  b = m_matrix[1];
  m_matrix[0] = a * cosR - b * sinR;
  m_matrix[1] = a * sinR + b * cosR;

  a = m_matrix[4];
  b = m_matrix[5];
  m_matrix[4] = a * cosR - b * sinR;
  m_matrix[5] = a * sinR + b * cosR;

  a = m_matrix[8];
  b = m_matrix[9];
  m_matrix[8] = a * cosR - b * sinR;
  m_matrix[9] = a * sinR + b * cosR;
}

Vertex Transformation::applyTo(const Vertex &v) const {
  float nx, ny, nz;

  nx = v.x * m_matrix[0] + v.y * m_matrix[1] + v.z * m_matrix[2] + m_matrix[3];
  ny = v.x * m_matrix[4] + v.y * m_matrix[5] + v.z * m_matrix[6] + m_matrix[7];
  nz =
      v.x * m_matrix[8] + v.y * m_matrix[9] + v.z * m_matrix[10] + m_matrix[11];
  return Vertex(nx, ny, nz);
}

// For normals we can ignore w
Normal Transformation::applyTo(const Normal &n) const {
    float nx, ny, nz;

    nx = n.x * m_matrix[0] + n.y * m_matrix[1] + n.z * m_matrix[2];
    ny = n.x * m_matrix[4] + n.y * m_matrix[5] + n.z * m_matrix[6];
    nz = n.x * m_matrix[8] + n.y * m_matrix[9] + n.z * m_matrix[10];
    return Normal{ nx, ny, nz };
}

void Transformation::applyTo(const Vertex &vIn, Vertex &vOut) const {
  vOut.x = vIn.x * m_matrix[0] + vIn.y * m_matrix[1] + vIn.z * m_matrix[2] +
           m_matrix[3];
  vOut.y = vIn.x * m_matrix[4] + vIn.y * m_matrix[5] + vIn.z * m_matrix[6] +
           m_matrix[7];
  vOut.z = vIn.x * m_matrix[8] + vIn.y * m_matrix[9] + vIn.z * m_matrix[10] +
           m_matrix[11];
}
