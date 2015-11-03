#pragma once

#include "ofMain.h"
#include <vrpn_Connection.h>
#include <vrpn_Tracker.h>
#include <vrpn_Tracker_Fastrak.h>



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

protected:
    void drawMono();
    void drawLeft();
    void drawRight();
    void drawScene();
    void setCamera(const ofVec3f &cameraPos);

     static void handle_tracker(void *userdata, const vrpn_TRACKERCB t);

private:

    ofVec2f screenResolution; // in pixels
    ofVec2f screenSize; // in mm
    ofVec3f camPos; // virtual camera position
    float halfEyeDist; // Half distance between each eyes, a separation of 16 to 18 mm is confortable for most people.

    // Camera parameters
    float nearPlane;
    float farPlane;

    // Animation parameters;
    float rx;
    float ry;
    float tz;
    int sign;

    ofVec3f camPosLeft;
    ofVec3f camPosRight;


    ofLight dirLight;
    ofMaterial material;

    vrpn_Tracker_Remote* m_TrackerRemote;

};
