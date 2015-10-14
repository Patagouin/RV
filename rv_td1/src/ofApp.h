#pragma once

#include <vector>
#include "ofMain.h"

#include "Camera.h"
#include "Material.h"
#include "Cube.h"

#define ISKEYBOARD true
#define ISMOUSE false

#define SPEEDNESS_TRANS 6

class ofApp : public ofBaseApp{

	public:
        ofApp(int w, int h);
        int w,h;

		void setup();
		void exit();
		void update();
		void draw();
		void drawAxes();
        void setFbo(int w, int h);
        int lookForCube(); // Retourne l'index du cube touve
        void createUpperCube(int index);
        void deleteCube(int index);
        void teleportToCube(int index);

        void moveMouse(int x, int y);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);



        ofTrueTypeFont verdana;

        char actualKey;

        Camera cam;

        std::vector<Cube*> cubes; // Le dernier cube est la tnt
        std::vector<Material*> materials;
        int nbStepOfTime;
        bool init;

        ofFbo fbo;
        float pixel[3];


};
