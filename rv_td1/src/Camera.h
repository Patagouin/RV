#include "ofMain.h"

#pragma once

class Camera
{
    public:
        Camera();
        virtual ~Camera();

        void init(int windowWidth, int windowHeight);
        void applyMatrix();
        void update(int x, int y, bool keyboard);
        void mousePressed(int x, int y);
        void drawAim();

        ofMatrix4x4 getMatrix();
        void setMatrix(ofMatrix4x4 mat);

    private:
        int width;
        int height;

        float rotX;
        float rotY;

        float deltaX,deltaY,deltaZ;

        float aim_dist;
        ofMatrix4x4 matrix;

        // Previous mouse position
        int lastX;
        int lastY;
};
