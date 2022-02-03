#include <iostream>
#include <vector>
#include <SOIL/SOIL.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <math.h>
#include "./Camera/Camera.h"
#include "./Building/BuildingMap.h"
#include <SOIL/SOIL.h>

#include "vertex.h"

#include <GL/glut.h>

using namespace std;

// Compilar
// g++ main.cpp ./Camera/Camera.h ./Camera/Camera.cpp ./Building/BuildingMap.cpp ./Building/BuildingMap.h -o a -lglut -lglfw -lGLEW -lGL -lGLU -lm -lSOIL

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800

void resize(int width, int height) {

  glViewport(0, 0, width, height);

  float aspect = float(width)/float(height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, aspect, 0.1, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(-1.0, 0.0, -5.0);
}


const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 50.0f, 50.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float last_mouse_position = 0.0;
bool first_mouse_movement = true;

float position_x = -5.0, position_y = 0.0, position_z = 10.0, look_x = 0.0, look_y = 0.0, look_z = 2.0, up_x = 0.0, up_y = 1.0, up_z = 0.0;

float angle_plane_rotation_positive = 3;
float angle_plane_rotation_negative = -3;
float plane_inclination = 0;
float velocity = 500;

Camera camera(vertex3(-5, 45, 0));
vector<Building> buildings; 

vertex3 plane_position = vertex3(-5, 25, 0);
vertex3 plane_direction = vertex3(-5, 25, 0);
vertex3 plane_left = vertex3(-1, 0, 0);
vertex3 plane_up = vertex3(0, 1, 0);
vertex3 plane_velocity = vertex3(0, 0, 0);
vertex3 look;

float plane_angle = 0.0;

float degreesToRadians(float angle) {
  return (angle * 3.14) / 180.0;
}

void update_yaw(float d_yaw) {
  printf("Angle:  %.2f\n", d_yaw);
  plane_angle += d_yaw;
}

void update_direction_vertex() {
  float angle = degreesToRadians(plane_angle);

  plane_direction.x = sin(angle < 0 ? angle * -1 : angle);
  plane_direction.z = -cos(angle < 0 ? angle * -1 : angle);
  plane_direction.normalize();
  
  glTranslatef(plane_direction.x, 0, plane_direction.z);
  plane_left = plane_up.vector_product(plane_direction);
}

void move_plane_forward(vertex3 *plane_velocity, vertex3 *plane_left, vertex3 *plane_position, vertex3 *plane_direction, vertex3 *plane_up) {
  plane_left->y = 0;
  *plane_velocity = *plane_left * (-1);
  *plane_position = *plane_position + *plane_velocity;

  look = *plane_position + *plane_direction;

  gluLookAt(plane_position->x, plane_position->y, plane_position->z, look.x, look.y, look.z, plane_up->x, plane_up->y, plane_up->z);
  camera.move_forward();
}

void move_plane_right(vertex3 *plane_velocity, vertex3 *plane_left, vertex3 *plane_position, vertex3 *plane_direction, vertex3 *plane_up) {

  *plane_velocity = *plane_direction * (-0.5);
  *plane_position = *plane_position + *plane_velocity;

  angle_plane_rotation_positive += 0.01;
  
  plane_inclination -= 3;

  if(angle_plane_rotation_positive >= 3.05) { angle_plane_rotation_positive = 3; }

  float aux = angle_plane_rotation_positive;

  update_yaw(angle_plane_rotation_positive);
  update_direction_vertex();

  look = *plane_position + *plane_direction;

  printf("POSITION X: %.2f Y: %.2f Z: %.2f\n", plane_position->x, plane_position->y, plane_position->z);
  printf("LOOK: X: %.2f Y: %.2f Z: %.2f\n", look.x, look.y, look.z);
  printf("DIRECTION X: %.2f Y: %.2f Z: %.2f\n", plane_direction->x, plane_direction->y, plane_direction->z);
  
  gluLookAt(plane_position->x, plane_position->y, plane_position->z, look.x, look.y, look.z, plane_up->x, plane_up->y, plane_up->z);
  camera.move_right();
  camera.update_yaw(aux);
  camera.update_direction_vertex();
}

void move_plane_left(vertex3 *plane_velocity, vertex3 *plane_left, vertex3 *plane_position, vertex3 *plane_direction, vertex3 *plane_up) {

  *plane_velocity = *plane_direction * 0.5;
  *plane_position = *plane_position + *plane_velocity;
  
  angle_plane_rotation_negative -= 0.01;

  plane_inclination += 3;

  if(angle_plane_rotation_negative <= -3.05) { angle_plane_rotation_negative = -3; }

  float aux = angle_plane_rotation_negative;

  update_yaw(angle_plane_rotation_negative);
  update_direction_vertex();

  look = *plane_position + *plane_direction;

  printf("POSITION X: %.2f Y: %.2f Z: %.2f\n", plane_position->x, plane_position->y, plane_position->z);
  printf("LOOK: X: %.2f Y: %.2f Z: %.2f\n", look.x, look.y, look.z);
  printf("DIRECTION X: %.2f Y: %.2f Z: %.2f\n", plane_direction->x, plane_direction->y, plane_direction->z);
  
  gluLookAt(plane_position->x, plane_position->y, plane_position->z, look.x, look.y, look.z, plane_up->x, plane_up->y, plane_up->z);
  camera.move_left();
  camera.update_yaw(aux);
  camera.update_direction_vertex();
}

void move_plane_up(vertex3 *plane_velocity, vertex3 *plane_left, vertex3 *plane_position, vertex3 *plane_direction, vertex3 *plane_up) {
  
  plane_left->y = 1;
  plane_velocity->y = plane_left->y * (0.5);
  plane_position->y = plane_position->y + plane_velocity->y;

  look = *plane_position + *plane_direction;

  cout << plane_left->x << " " << plane_left->y << " " << plane_left->z << endl;
  
  gluLookAt(plane_position->x, plane_position->y, plane_position->z, look.x, look.y, look.z, plane_up->x, plane_up->y, plane_up->z);
  camera.move_up();
}

void move_plane_down(vertex3 *plane_velocity, vertex3 *plane_left, vertex3 *plane_position, vertex3 *plane_direction, vertex3 *plane_up) {
  
  plane_left->y = 1;
  plane_velocity->y = plane_left->y * (-0.5);
  plane_position->y = plane_position->y + plane_velocity->y;

  look = *plane_position + *plane_direction;

  cout << plane_left->x << " " << plane_left->y << " " << plane_left->z << endl;
  
  gluLookAt(plane_position->x, plane_position->y, plane_position->z, look.x, look.y, look.z, plane_up->x, plane_up->y, plane_up->z);
  camera.move_down();
}

void singleBuilding(){
  // Componente do prédio
  glColor3d(1, 1, 1);
  glPushMatrix();
    glTranslated(0, 0, 0);
    glutSolidCube(4);
  glPopMatrix();

  // Janelas
  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(1.8, 0, 1);
    glScaled(0.6, 0.6, 1.001);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(-1.8, 0, 1);
    glScaled(0.6, 0.6, 1.001);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(1.8, 0, -1);
    glScaled(0.6, 0.6, 1.001);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(-1.8, 0, -1);
    glScaled(0.6, 0.6, 1.001);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(1, 0, 1.8);
    glScaled(1.001, 0.6, 0.6);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(1, 0, -1.8);
    glScaled(1.001, 0.6, 0.6);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(-1, 0, 1.8);
    glScaled(1.001, 0.6, 0.6);
    glutSolidCube(1);
  glPopMatrix();

  glColor3d(0,0,0);
  glPushMatrix();
    glTranslated(-1, 0, -1.8);
    glScaled(1.001, 0.6, 0.6);
    glutSolidCube(1);
  glPopMatrix();
}

// Cria uma linha de prédios
void createBuildings(int height){

  for(int i = 0; i < height; ++i){
    glPushMatrix();
      glTranslated(0, 0.8 + i, -20);
      singleBuilding();
    glPopMatrix();
  }
}

void mappingBuildings() {

  for(int i = 0; i < 300; i += 8) {
    for(int j = 0; j < 200; j += 10) {
      Building building(i, j, (rand() % 25) + 1);
      buildings.push_back(building);
    }
  }
}

void plane() {

  // Corpo
  glColor3d(0.5, 0.5, 0.5);
  glPushMatrix();
    glTranslated(0,0,0);
    glScaled(3.5, 0.4, 0.7);
    glutSolidSphere(1, 10, 2); // Esfera esticada
  glPopMatrix();

  glColor3d(0.5, 0.5, 0.5);
  glPushMatrix();
    glTranslated(-0.5,0,0);
    glScaled(3.5, 0.4, 0.7);
    glutSolidSphere(1, 10, 2); // Esfera esticada
  glPopMatrix();

  // Cabine
  glColor3d(0, 0, 0.8);
  glPushMatrix();
    glTranslated(0.8,0.2,0);
    glScaled(1.5,0.7,0.8);
    glRotated(40,0,1,0);
    glutSolidSphere(0.40, 10, 10);
  glPopMatrix();

  // Asa
  glColor3d(0.5, 0.5, 0.5);
  glPushMatrix();
    glTranslated(-3,0, -0); // Movimenta o objeto pra frente, trás ou cima (-0.8)
    glScaled(4, 3, 3); // x = largura do obj y = espessura
    glRotated(90, 0, 1, 0);
    glutSolidCone(1, 1, 2, 2);
  glPopMatrix();

  // Asa de cima
  glColor3d(0.5, 0.5, 0.5);
  glPushMatrix();
    glTranslated(-2.7, 0.5, 0);
    glRotated(45, 0, 0, 1);
    glScaled(0.8, 2, 0.1);
    glRotated(-20, 0, 0, 1);
    glutSolidCube(0.5);
  glPopMatrix();
}

void createFloor() {
 
  glColor3d(0.3, 0.3, 0.3);  
  glPushMatrix();
    glTranslated(0, -1, 0);
    glScaled(2000, 0.9, 2000);
    glRotated(0, 0, 0, 0);
    glutSolidCube(1);
  glPopMatrix();
}

void draw(){
  camera.activate();

  createFloor();

  // Desenha os prédios
  for(int i = 0; i < buildings.size(); ++i) {
    glPushMatrix();
      glTranslated(buildings[i].x, 0, buildings[i].y);
      createBuildings(buildings[i].height);
    glPopMatrix();
  }

  glPushMatrix();
    glTranslated(camera.position.x, camera.position.y, camera.position.z);
    glRotatef(plane_inclination, 0, camera.position.y, 0);
    plane();
  glPopMatrix();

  move_plane_forward(&plane_velocity, &plane_left, &plane_position, &plane_direction, &plane_up);
}


void controlRotation(GLFWwindow *window, int key, int scanCode, int action, int mod){

  // cout << "Key: " << key << endl;

  // Mover avião para cima
  if (key == GLFW_KEY_W) {
    // move_plane_forward(&plane_velocity, &plane_left, &plane_position, &plane_direction, &plane_up);
    move_plane_up(&plane_velocity, &plane_left, &plane_position, &plane_direction, &plane_up);
  // Mover avião para baixo
  } else if (key == GLFW_KEY_S) {
    move_plane_down(&plane_velocity, &plane_left, &plane_position, &plane_direction, &plane_up);
  // Mover avião para esquerda
  } else if (key == GLFW_KEY_A) {
    move_plane_left(&plane_velocity, &plane_left, &plane_position, &plane_direction, &plane_up);
  // Mover avião para direita
  } else if (key == GLFW_KEY_D) {
    move_plane_right(&plane_velocity, &plane_left, &plane_position, &plane_direction, &plane_up);
  } 
}

void cursor_callback(GLFWwindow* window, double x_pos, double y_pos) {
  float dx;
  
  if (first_mouse_movement) {
    dx = 0;
    last_mouse_position = x_pos;
    first_mouse_movement = false;
  }

  dx = x_pos - last_mouse_position;
  last_mouse_position = x_pos;
  camera.update_yaw(dx);
  camera.update_direction_vertex2();
}


void display() {
    
  glPushMatrix();
    glTranslated(0.9, -6, -6);
    glRotatef(90.0f, 0.0f, 10.0f, 0.0f);
    draw();
  glPopMatrix();
}

void initial_config(GLFWwindow* window) {
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwMaximizeWindow(window);
  glfwSetKeyCallback(window, controlRotation);
  glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LESS);
  
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
  mappingBuildings();

  vertex3 look = camera.position + camera.direction;
  gluLookAt(camera.position.x, camera.position.y, camera.position.z, look_x, look_y, look_z, camera.up.x, camera.up.y, camera.up.z);
  // Cor de fundo
  // glClearColor(0.0, 0.0, 0.0, 0.1);
  glClearColor(0.64,0.76,0.84,0.1);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

int main(int argc, char *argv[]) {
  
  GLFWwindow* window;

  if(!glfwInit()) {
    return -1;
  }

  glutInit(&argc, argv);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "JATO", NULL, NULL);

  if(!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  initial_config(window);

  while(!glfwWindowShouldClose(window)) {    
    // Poll for and process events
    glfwPollEvents();

    // Render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int largura, altura;
		glfwGetFramebufferSize(window, &largura, &altura);

    resize(largura, altura);
    display();

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}