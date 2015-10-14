#include "Camera.h"

Camera::Camera()
{
    matrix.makeIdentityMatrix();
}

Camera::~Camera()
{
}

void Camera::init(int windowWidth, int windowHeight)
{
    matrix = ofMatrix4x4();
    width = windowWidth;
    height = windowHeight;
    aim_dist = 400;
    rotX = 0;
    rotY = 0;
    lastX = width/2;
    lastY = height/2;
    deltaX = 0;
    deltaZ = -aim_dist;
}


 void Camera::applyMatrix()
 {
     ofLoadIdentityMatrix();
     ofMatrix4x4 tmp;

    tmp.rotate(rotX, 1.0, 0.0, 0.0);
    ofMultMatrix(tmp);
    matrix.rotate(rotY, 0.0, 1.0, 0.0);
    matrix.translate(deltaX, 0, deltaZ);
    ofMultMatrix(matrix);

    rotY = deltaX = deltaY = deltaZ = 0;

}

void Camera::update(int x, int y, bool keyboard)
{
    if (!keyboard){
    float rateX = 180.0/float(height);
    float rateY = 180.0/float(width);
    float diffX = x - lastX;
    float diffY = lastY - y;
    lastX = x;
    lastY = y;
    rotX += diffY * rateY;
    rotY += diffX * rateX;
    }
    else{
        deltaX += x;
        deltaZ += y;
    }

}

void Camera::mousePressed(int x, int y)
{
    lastX = x;
    lastY = y;
}

void Camera::drawAim()
{
    ofDisableDepthTest();

    ofPushMatrix();
    ofTranslate(width/2, height/2);
    ofSetLineWidth(4);
    ofSetColor(100, 100, 100);
    ofLine(-10, 0, 10, 0);
    ofLine(0, -10, 0, 10);
    ofPopMatrix();

    ofEnableDepthTest();
}

ofMatrix4x4 Camera::getMatrix()
{
    return matrix;
}

void Camera::setMatrix(ofMatrix4x4 mat)
{
    matrix = mat;
}
