#include "ofApp.h"
//--------------------------------------------------------------
ofApp::ofApp(int _w, int _h): w(_w), h(_h)
{

}

void ofApp::setup()
{
    cam.init(ofGetWidth(),ofGetHeight());
    init = true;

    ofBackground(0,0,0);
	ofEnableDepthTest();
	ofEnableNormalizedTexCoords();
    ofEnableAntiAliasing();
    ofDisableArbTex();

	ofTrueTypeFont::setGlobalDpi(72);
    verdana.loadFont("verdana.ttf", 18, true, true, true);
	verdana.setLineHeight(18.0f);
	verdana.setLetterSpacing(1.037);

//	// Create cubes and their materials
    Material* sand = new Material("sand.png");
    Material* stonebrick = new Material("stonebrick.png");
    Material* mousse = new Material("stonebrick_mossy.png");
    materials.push_back(sand);
    materials.push_back(stonebrick);
    materials.push_back(mousse);

    Cube* c;

    Cube::setToFbo(true); // Active couleur desactive textures

    // FBO
//    fbo.allocate(w,h);

    Cube::setToFbo(false); // on déactive couleur pour remettre les textures



    for (int i=0; i < 2; i++) {
        for (int j=-10; j < 10; j++) {
            for (int k=-10; k < 10; k++) {
                if (i==1){
                    c = new Cube(sand);
                }if(rand()%2){
                    c = new Cube(mousse);
                }else{
                    c = new Cube(stonebrick);
                }

                c->move(j, i+1, k);
                cubes.push_back(c);
            }
        }
    }

    Material* tnt = new Material("tnt.png");
    materials.push_back(tnt); 
    c = new Cube(tnt);
    c->move(2, 0, -1);
    cubes.push_back(c);
    actualKey = '0';

}

//--------------------------------------------------------------
void ofApp::exit()
{
    for (unsigned int i=0; i<cubes.size(); i++) {
        delete cubes[i];
    }
    for (unsigned int i=0; i<materials.size(); i++) {
        delete materials[i];
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Constrain mouse cursor inside window
    if (ofGetKeyPressed()){
        if(actualKey == 'z')
            cam.update(0,SPEEDNESS_TRANS, ISKEYBOARD);
        else if(actualKey == 's')
            cam.update(0, -SPEEDNESS_TRANS, ISKEYBOARD);
        else if(actualKey == 'q')
            cam.update(SPEEDNESS_TRANS,0, ISKEYBOARD);
        else if(actualKey == 'd')
            cam.update(-SPEEDNESS_TRANS,0, ISKEYBOARD);
    }

    if(init){
        moveMouse(ofGetWidth()/2, ofGetHeight()/2);
        init = false;

    }else{
        int x = ofGetMouseX();
        int y = ofGetMouseY();
        if(x >= ofGetWindowWidth()-1)
            moveMouse(1,y);
        if(x <= 0)
            moveMouse(ofGetWindowWidth()-2,y);
        if(y <= 0)
            moveMouse(x,ofGetHeight()-2);
        if(y >= ofGetHeight()-1)
            moveMouse(x,1);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    nbStepOfTime++;
    ofPushMatrix();
    cam.applyMatrix();
    drawAxes();

    ofPushMatrix();
    ofScale(100,100,100);

    // FBO
    Cube::setToFbo(true); // Active couleur desactive textures
    fbo.begin();

    ofClear(255, 255, 255, 0);
    for (unsigned int i=0; i<cubes.size()-1; i++) {

        cubes[i]->draw();
    }
    ofPoint transY(0,-1 -cos((double)(nbStepOfTime/2.0)),0);
    ofPushMatrix();
    ofTranslate(transY);
    cubes[cubes.size()-1]->draw();
    ofPopMatrix();

    fbo.end();



    Cube::setToFbo(false); // on déactive couleur pour remettre les textures
    // Affichage ecran
    for (unsigned int i=0; i<cubes.size()-1; i++) {

        cubes[i]->draw();
    }
    transY = ofPoint(0,-1 -cos((double)(nbStepOfTime/2.0)),0);
    ofTranslate(transY);
    cubes[cubes.size()-1]->draw();

    ofPopMatrix();
    ofPopMatrix();

    cam.drawAim();
}

//--------------------------------------------------------------
void ofApp::drawAxes()
{
    ofPushMatrix();
    ofSetLineWidth(2);
    ofSetColor(255, 0, 0);
    ofLine(0, 0, 0, 20, 0, 0);
    verdana.drawStringAsShapes("x", 25, 0);
    ofSetColor(0, 255, 0);
    ofLine(0, 0, 0, 0, 20, 0);
    verdana.drawStringAsShapes("y", 5, 25);
    ofSetColor(0, 0, 255);
    ofLine(0, 0, 0, 0, 0, 20);
    ofTranslate(0,0,22);
    verdana.drawStringAsShapes("z", -5, -5);
    ofPopMatrix();
}

void ofApp::setFbo(int w, int h)
{
    fbo.allocate(w,h, GL_RGBA);

}

int ofApp::lookForCube()
{
    int pixelR = pixel[0] * 255;
    int pixelG = pixel[1] * 255;
    int pixelB = pixel[2] * 255;
    int tmp[3];
    unsigned int i = 0;
    while (i < cubes.size()){
        cubes.at(i)->getColorId(tmp);
        if (tmp[0] == pixelR && tmp[1] == pixelG && tmp[2] == pixelB)
            return i;
        i++;
    }
    return -1;

}

void ofApp::createUpperCube(int index)
{
    // Create cube with the same material as the one under click
    Cube *c = new Cube(cubes[index]->getMaterial());
    // Coord of the new cube
    ofVec3f pos = cubes[index]->getPosition();
    pos.y--;
    c->move(pos);
    // pop jumper's cube
    Material *m = cubes[cubes.size()-1]->getMaterial();
    ofVec3f posJumper = cubes[cubes.size()-1]->getPosition();
    cubes.pop_back();
    // push of the new cube
    cubes.push_back(c);
    // push of the jumper cube
    c = new Cube(m);
    c->move(posJumper);
    cubes.push_back(c);
}

void ofApp::deleteCube(int index)
{

    cubes.erase(cubes.begin()+index);
}

void ofApp::teleportToCube(int index)
{
    ofVec3f pos = cubes.at(index)->getPosition();
    std::cout << pos.x << "xp " << pos.y << "y "  << pos.z << "z " << std::endl;
    ofMatrix4x4 mat = cam.getMatrix();
    ofVec3f trans = mat.getTranslation();
    std::cout << trans.x << "x " << trans.y << "y "  << trans.z << "z "<< std::endl;
    mat.translate(-trans);
    mat.translate(pos.x, pos.y, pos.z);
    cam.setMatrix(mat);
}



//--------------------------------------------------------------
void ofApp::moveMouse(int x, int y)
{
#if defined(__APPLE__)
    CGWarpMouseCursorPosition(CGPointMake(ofGetWindowPositionX()+x,ofGetWindowPositionY()+y));
#elif defined(_WIN32)
    SetCursorPos(x,y); // not tested
#else // xlib
    Display *display = XOpenDisplay(0);
    Window window;
    int state;
    XGetInputFocus(display,&window,&state);
    XWarpPointer(display, None, window, 0, 0, 0, 0, x, y);
    XCloseDisplay(display);
#endif

    cam.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f')
        ofToggleFullscreen();
    else if(key == 'z')
        actualKey = 'z';
    else if(key == 's')
        actualKey = 's';
    else if(key == 'q')
        actualKey = 'q';
    else if(key == 'd')
        actualKey = 'd';
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
        cam.update(x, y, ISMOUSE);
}

//--------------------------        Cube   ------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(button==0)
        cam.update(x, y, ISMOUSE);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//    fbo.begin();

//    fbo.draw(0,0,w,h);
    if(button==0 && x>0 && y>0){
        cam.mousePressed(x, y);
        glReadPixels(x,h-y,1,1,GL_RGB,GL_FLOAT, pixel);
        int index = lookForCube();
        if (index!=-1)
            createUpperCube(index);
    }
    else if (button==2 && x>0 && y>0){
        cam.mousePressed(x, y);
        glReadPixels(x,h-y,1,1,GL_RGB,GL_FLOAT, pixel);
        int index = lookForCube();
        deleteCube(index);
    }
    else if(button==1 && x>0 && y>0){
        cam.mousePressed(x, y);
        glReadPixels(x,h-y,1,1,GL_RGB,GL_FLOAT, pixel);
        int index = lookForCube();
        if (index!=-1)
            teleportToCube(index);
    }
//    fbo.end();

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int _w, int _h){
    cam.init(ofGetWidth(),ofGetHeight());

    setFbo(_w, _h);
    w = _w;
    h = _h;
}
