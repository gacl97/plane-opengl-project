#ifndef PLANE_PROJECT_VERTEX_H
#define PLANE_PROJECT_VERTEX_H

#include <cmath>

struct vertex3 {
  float x, y, z;

  vertex3() : x(0.0), y(0.0), z(0.0) {}
  vertex3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}
  
  // Soma de vetores
  vertex3 operator+(const vertex3& vertex) {
    return vertex3(x + vertex.x, y + vertex.y, z + vertex.z);
  }

  // Multiplicação por constante
  vertex3 operator*(float constant) {
    return vertex3(x * constant, y * constant, z * constant);
  }

  float vertex_magnitude() { return sqrt(x * x + y * y + z * z); }
  // Deixar na escala
  void normalize() {
    float magnitude = vertex_magnitude();
    x = x / magnitude;
    y = y / magnitude;
    z = z / magnitude;
  }

  // Multiplicação de vetores
  vertex3 vector_product(vertex3 vertex) {
    return vertex3(y * vertex.z - z * vertex.y, z * vertex.x - x * vertex.z, x * vertex.y - y * vertex.x);
  }
};

#endif