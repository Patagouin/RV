#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxCvContourFinder.h"

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

private:
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
    ofxCvContourFinder contour;
    ofxCvBlob theBlob;
    ofRectangle bb;

    ofImage glasses;
    ofImage beard;



    ofxCvHaarFinder haarFinder;

};
