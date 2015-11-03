#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    glEnable(GL_DEPTH_TEST);

    screenResolution = ofVec2f(2560,1440);
    screenSize = ofVec2f(600.0, 340.0);


    camPos = ofVec3f(0, 0, 500);
    halfEyeDist = 8;

    nearPlane = 10;
    farPlane = 2000;

    ofBackground(0,0,0);
    ofEnableSmoothing();
    ofEnableDepthTest();
    ofEnableAntiAliasing();
    ofSetSmoothLighting(true);
    dirLight.setDirectional();
    dirLight.setOrientation(ofQuaternion(90.f, ofVec3f(0, -1, 0)));
    dirLight.setAmbientColor(ofFloatColor(.5,.5,.5));
    dirLight.setDiffuseColor(ofFloatColor(.5,.5,.5));

    material.setAmbientColor(ofFloatColor(1.f,0.f,0.f));
    material.setDiffuseColor(ofFloatColor(1.f,0.f,0.f));

    rx = 0.f;
    ry = 0.f;
    tz = 0.f;
    sign = 1;

    m_TrackerRemote = new vrpn_Tracker_Remote("TrackPostIt@localhost");
    float data[3] = {camPos.x, camPos.y, camPos.z};
    camPos.set(data[0], data[1], data[2]);

    m_TrackerRemote->register_change_handler((void*)&camPos, handle_tracker);

}


void ofApp::handle_tracker(void *userdata, const vrpn_TRACKERCB t)
{

    ofVec3f *tmp = (ofVec3f*)userdata;
    std::cout << t.pos[0] << "x " << t.pos[1] << "y "<< std::endl;


    tmp->set(t.pos[0], t.pos[1], t.pos[2]);
//    userdata = (void*)&ofVec3f(t.pos[0], t.pos[1], t.pos[2]);

}

//--------------------------------------------------------------
void ofApp::update(){

    ry += 1.f;
    rx += 1.f;
    if (abs(tz) == 100)
        sign = -sign;
    tz = tz + sign;

    m_TrackerRemote->mainloop();
}

//--------------------------------------------------------------
void ofApp::draw(){


    glColorMask(1,1,1,1);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    drawLeft();
    glClear(GL_DEPTH_BUFFER_BIT );
    drawRight();
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(1,1,1,1);

    drawMono();
}

void ofApp::drawMono()
{
    //camPos.x = ((camPos[0] - 320.0)/2) / nearPlane;
    //camPos.y = ((camPos[1]- 240.0)/2) / nearPlane;
    setCamera(camPos);
    drawScene();
}

void ofApp::drawLeft()
{
    camPos.x = ((camPos[0] - 320.0)/2) / nearPlane;
    camPos.y = ((camPos[1]- 240.0)/2) / nearPlane;
    camPosLeft = ofVec3f(camPos.x - 5, camPos.y, camPos.z);

    setCamera(camPosLeft);
    glColorMask(1,0,0,0);
    drawScene();
}

void ofApp::drawRight()
{
    camPos.x = ((camPos[0] - 320.0)/2) / nearPlane;
    camPos.y = ((camPos[1]- 240.0)/2) / nearPlane;

    camPosRight = ofVec3f(camPos.x + 5, camPos.y, camPos.z);

    setCamera(camPosRight);
    glColorMask(0,1,1,0);
    drawScene();

}

void ofApp::setCamera(const ofVec3f& cameraPos)
{
    float nearFactor = nearPlane / cameraPos.z;
    float left   = nearFactor * (- screenSize.x / 2.f - cameraPos.x);
    float right  = nearFactor * (  screenSize.x / 2.f - cameraPos.x);
    float top    = nearFactor * (  screenSize.y / 2.f - cameraPos.y);
    float bottom = nearFactor * (- screenSize.y / 2.f - cameraPos.y);

    ofMatrix4x4 projection;
    projection.makeFrustumMatrix(left,right,bottom,top,nearPlane,farPlane);

    ofSetMatrixMode(OF_MATRIX_PROJECTION);
    ofLoadMatrix(projection);

    ofMatrix4x4 view;
    view.makeLookAtViewMatrix(cameraPos,ofVec3f(cameraPos.x,cameraPos.y,0.f),ofVec3f(0.f,1.f,0.f));

    ofSetMatrixMode(OF_MATRIX_MODELVIEW);
    ofLoadMatrix(view);
}

void ofApp::drawScene()
{
    ofEnableLighting();
    dirLight.enable();

    //camPos.x = ((camPos[0] - 320.0)/2) / nearPlane;

    //camPos.y = ((camPos[1]- 240.0)/2) / nearPlane;

    // Draw a rotating cube
    ofPushMatrix();
    ofTranslate(0, 0, tz);
    ofRotateY(ry);
    ofRotateX(rx);

    ofSetColor(255,255,255);

    material.begin();
    ofFill();
    // cube de 5cm
    ofScale(50.f,50.f,50.f);
    ofDrawBox(1);
    material.end();

    ofPopMatrix();

    dirLight.disable();
    ofDisableLighting();

    // Draw a 3D grid
    ofNoFill();
    ofSetLineWidth(1.5f);

    int numLines = 10;
    float lineDist = 40;
    int intensStep = 255 / numLines;

    // Horizontal lines
    float stepSize = screenSize.x / numLines;

    for (int i = 0; i <  numLines; i++) {
        ofSetColor(255 - i * intensStep);

        // Bottom
        ofLine(-screenSize.x/2.f, -screenSize.y/2.f, -i * lineDist,
               screenSize.x/2.f, -screenSize.y/2.f, -i * lineDist);

        // Top
        ofLine(-screenSize.x/2.f, screenSize.y/2.f, -i * lineDist,
               screenSize.x/2.f, screenSize.y/2.f, -i * lineDist);

        // Line which goes to Z
        // Bottom
        for (float j = -screenSize.x/2.f; j <  screenSize.x/2.f ; j+= stepSize) {
            ofLine(j, -screenSize.y/2.f, -i * lineDist,
                   j, -screenSize.y/2.f, -i * lineDist - lineDist);
        }

        // Top
        for (float j = -screenSize.x/2.f; j <  screenSize.x/2.f ; j+= stepSize) {
            ofLine(j, screenSize.y/2.f, -i * lineDist,
                   j, screenSize.y/2.f, -i * lineDist - lineDist);
        }
    }

    stepSize = screenSize.y / numLines;
    // Vertical lines
    for (int i = 0; i <  numLines; i++) {
        ofSetColor(255 - i * intensStep);

        // left
        ofLine(-screenSize.x/2.f, screenSize.y/2.f, -i * lineDist,
               -screenSize.x/2.f, -screenSize.y/2.f, -i * lineDist);

        // right
        ofLine(screenSize.x/2.f, -screenSize.y/2.f, -i * lineDist,
               screenSize.x/2.f, screenSize.y/2.f, -i * lineDist);

        // Line which goes to Z
        // left
        for (float j = -screenSize.y/2.f; j <  screenSize.y/2.f ; j+= stepSize) {
            ofLine(-screenSize.x/2.f, j, -i * lineDist,
                   -screenSize.x/2.f, j, -i * lineDist - lineDist);
        }

        // right
        for (float j = -screenSize.y/2.f; j <=  screenSize.y/2.f ; j+= stepSize) {
            ofLine(screenSize.x/2.f, j, -i * lineDist,
                   screenSize.x/2.f, j, -i * lineDist - lineDist);
        }
    }




}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //camPos.x = (x - screenResolution.x/2)/nearPlane;
    //camPos.y = (y - screenResolution.y/2)/nearPlane;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
