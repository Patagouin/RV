#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxCvContourFinder.h"

static const float HEIGHT = 240.0;
static const float CAMERA_ANGLE = 56.0;

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

    void computeDistance();

private:

    ofFile conf;

    ofColor rgbToHsv(ofColor crgb);

    ofVideoGrabber cam;

    ofColor targetColor;

    int img_width, img_height;
    ofxCvColorImage img;

    ofxCvColorImage hsvImg;
    float hue;
    float sat;
    float bright;
    int tolerance;
    ofxCvGrayscaleImage grayImg;
    ofColor targetColorHsv;

    ofxCvGrayscaleImage image;
    ofRectangle roi;

//    float width = -1.;
//    float height = -1.;
    bool roiUpdate;

    ofRectangle roiInit;
    ofRectangle face;

    ofxCvBlob blobtest;

    ofxCvContourFinder contour;
    ofxCvBlob theBlob;
    ofRectangle bb;

    ofImage glasses;
    ofImage beard;

    ofxCvHaarFinder haarFinder;

    ofFile file;
    float focal;

    float sizeObject; // mm

    int sizeObjectInProj; //pixel

    double distance;


};
