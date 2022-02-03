#ifndef PLANE_PROJECT_CAMERA_H
#define PLANE_PROJECT_CAMERA_H

#include "../vertex.h"

class Camera {
    private:

    public:
        vertex3 position;
        vertex3 direction;
        vertex3 left;
        vertex3 up;
        vertex3 velocity;

        float scale = 0.5;
        float yaw;
        
        Camera(vertex3 position);
        Camera();
        ~Camera();
        void activate();

        void move_forward();
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void update_yaw(float d_yaw);
        void update_direction_vertex();
        void update_direction_vertex2();
    private:
        float degreesToRadians(float angle);
};

#endif