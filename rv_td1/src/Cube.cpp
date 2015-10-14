#include "Cube.h"

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
int Cube::idColor[3] = { 0, 0, 0 };
bool Cube::toFbo = false;

const float _vertices[72] = {
                        1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
                        1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
                        1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
                       -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
                       -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
                        1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)

const ofIndexType _indices[36]  = {
                       0, 1, 2,   2, 3, 0,      // front
                       4, 5, 6,   6, 7, 4,      // right
                       8, 9,10,  10,11, 8,      // top
                      12,13,14,  14,15,12,      // left
                      16,17,18,  18,19,16,      // bottom
                      20,21,22,  22,23,20 };    // back

// texture ///////////////////////////////////////////////
//  3----2----5----7
//  |    |    |    |
//  |    |    |    |
//  0----1----4----6

const int _uv_indices[36] = {
    2, 3, 0, 1, // front
    3, 0, 1, 2, // right
    4, 5, 2, 1, // top
    2, 3, 0, 1, // left
    4, 6, 7, 5, // bottom
    0, 1, 2, 3  // back
};

const float _uv[24] = {
    0, 0,         1.0/3.0, 0,
    1.0/3.0, 1,   0, 1,
    2.0/3.0, 0,   2.0/3.0, 1,
    1, 0,         1, 1,
};

Cube::Cube(Material* m)
{
    idColor[0]++;
    if (idColor[0]>=256){
        idColor[0] = 0;
        idColor[1]++;
        if (idColor[1]>=256){
            idColor[1] = 0;
            idColor[2]++;
            if (idColor[2]>=256){
                std::cerr << "Nombre de cube trop important" << std::endl;
                exit(EXIT_FAILURE);
            }
       }
    }
    colorId[0] = idColor[0];
    colorId[1] = idColor[1];
    colorId[2] = idColor[2];

    _m = m;

    _mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for(unsigned int i=0; i<24; i++)
    {
        _mesh.addVertex(ofPoint(_vertices[i*3],_vertices[i*3+1],_vertices[i*3+2]));
    }
    for(unsigned int i=0; i<36; i++)
    {
        _mesh.addIndex(_indices[i]);
    }
    for(unsigned int i=0; i<36; i++)
    {
        _mesh.addTexCoord(ofVec2f(_uv[_uv_indices[i]*2], _uv[_uv_indices[i]*2+1]));
    }

    setScale(0.5);

    ofFloatColor color;
    color.set(idColor[0]/255.0,idColor[1]/255.0, idColor[2]/255.0);

    for (unsigned int i=0; i<36; i++){
            _mesh.addColor(color);
    }


}



Cube::~Cube()
{
}

void Cube::customDraw()
{

    if (toFbo){
        _mesh.enableColors();
        _mesh.draw();
    }
    else{
        _mesh.disableColors();
        _m->bind();
        _mesh.draw();
        _m->unbind();
    }

}

void Cube::setToFbo(bool _toFbo)
{
    Cube::toFbo = _toFbo;
}

void Cube::getColorId(int _colorId[3])
{
    _colorId[0] = colorId[0];
    _colorId[1] = colorId[1];
    _colorId[2] = colorId[2];

}

Material *Cube::getMaterial()
{
    return _m;

}
