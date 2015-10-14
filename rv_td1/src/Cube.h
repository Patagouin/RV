//#ifndef CUBE_H
//#define CUBE_H

#pragma once

#include "ofMain.h"

#include "Material.h"

class Cube : public ofNode
{
public:
    Cube(Material* m);

    virtual ~Cube();
    virtual void customDraw();

    static void setToFbo(bool toFbo);

    static bool toFbo;

    void getColorId(int color[3]);

    Material *getMaterial();
private:
    Material* _m;
    ofVboMesh _mesh;
    static int idColor[3];
    int colorId[3];

};

//#endif
