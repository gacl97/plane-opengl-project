
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include "Camera.h"

Camera::Camera(vertex3 initial_position) {
  position = vertex3(initial_position);
  direction = vertex3(0, 0, -1);
  direction2 = vertex3(0, 0, -1);
  left = vertex3(-1, 0, 0);
  up = vertex3(0, 1, 0);
  velocity = vertex3(0, 0, 0);
}

Camera::~Camera() noexcept = default;

void Camera::activate() {
  vertex3 look = position + direction;
  // printf("position X: %.2f Y: %.2f Z: %.2f\n", position.x, position.y, position.z);
  // printf("look: X: %.2f Y: %.2f Z: %.2f\n", look.x, look.y, look.z);
  // printf("direction X: %.2f Y: %.2f Z: %.2f\n", direction.x, direction.y, direction.z);
  gluLookAt(position.x, position.y, position.z, look.x, look.y, look.z, up.x, up.y, up.z);
}

void Camera::move_forward() {
  velocity = direction * scale;
  position = position + velocity;
}

void Camera::move_back() {
  velocity = direction * (-scale);
  position = position + velocity;
}

void Camera::move_left() {
  velocity = left * scale;
  position = position + velocity;
}

void Camera::move_right() {
  velocity = left * (-scale);
  position = position + velocity;
}


void Camera::move_up() {
  float aux1 = position.x;
  left.y = 1;
  position.x = 0;
  
  velocity.y = left.y * scale;
  position.y = position.y + velocity.y;

  position.x = aux1;
  left.y = 0;
}

void Camera::move_down() {
  float aux1 = position.x;
  left.y = 1;
  position.x = 0;
  
  velocity.y = left.y * (-scale);
  position.y = position.y + velocity.y;

  position.x = aux1;
  left.y = 0;
}

float Camera::degreesToRadians(float angle) {
  return (angle * 3.14) / 180.0;
}

void Camera::update_yaw(float d_yaw) {
  // printf("%.2f\n", d_yaw);
  yaw += d_yaw;
}

void Camera::update_direction_vertex() {
  // printf("YAW camera: %.2f\n", yaw);
  float angle = degreesToRadians(yaw);
  // printf("Angle camera: %.2f\n", angle);

  direction.x = sin(angle);
  direction.z = -cos(angle);
  direction.normalize();

  // Comentando a câmera gira em torno do avião
  left = up.vector_product(direction);
}

void Camera::update_direction_vertex2() {
  // printf("YAW camera: %.2f\n", yaw);
  float angle = degreesToRadians(yaw);
  // printf("Angle camera: %.2f\n", angle);

  direction.x = sin(angle);
  direction.z = -cos(angle);
  direction.normalize();

  // Comentando a câmera gira em torno do avião
  // left = up.vector_product(direction);
}

