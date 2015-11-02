#include "ofApp.h"
#include <iostream>
#include <string>


//--------------------------------------------------------------
void ofApp::setup(){
    img_width = 640/2;
    img_height = 480/2;
    cam.setDesiredFrameRate(30.0);
    cam.initGrabber(img_width,img_height);

    img.allocate(img_width,img_height);

    hsvImg.allocate(img_width, img_height);
    grayImg.allocate(img_width,img_height);

    tolerance=30;

    image.allocate(img_width, img_height);
    haarFinder.setup("haarcascade_frontalface_alt2.xml");

    roiInit = ofRectangle(0,0,img_width,img_height);
    roi = roiInit;
    roiUpdate=false;

    glasses.loadImage("glasses.png");
    beard.loadImage("beard.png");


    file.open(ofToDataPath("out_camera_data.xml"), ofFile::ReadOnly);
    ofBuffer buffer = file.readToBuffer(); // read to a buffer
    ofXml data;
    data.loadFromBuffer( buffer.getText() );

    std::string camMat  = data.getValue("Camera_Matrix/data");
    std::vector<string> mat = ofXml::tokenize(camMat, " ");

    focal = atof(mat[1].c_str());

    sizeObject = 75;

    sizeObjectInProj = 1;

//    std::vector<string>::const_iterator it = camera_matrix.begin();
//    std::vector<string>::const_iterator end = camera_matrix.end();
//    //for(; it != end; ++it){
//    for(int i = 0; i < camera_matrix.size(); ++i){
//        //FIXTHIS
//        std::cerr << "camera_mat = " << camera_matrix[i] << std::endl;
//        //m_CameraMatrix.push_back(stod(camera_matrix[i]));
//    }





}

ofColor ofApp::rgbToHsv(ofColor crgb) {
    float r = crgb.r/255.f;
    float g = crgb.g/255.f;
    float b = crgb.b/255.f;
    float cmax = fmaxf(fmaxf(r,g),b);
    float cmin = fminf(fminf(r,g),b);
    float delta = cmax-cmin;
    float h = (cmax == r) ? 60.f*(g-b)/delta : (cmax == g) ? 60.f*((b-r)/delta+2.f) : 60.f*((r-g)/delta+4.f);
    if(h<0)
        h += 360;
    float s = cmax < 1e-4 ? 0 : delta/cmax;
    return ofColor(h/2,s*255,cmax*255);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if (cam.isFrameNew()){
        img.setFromPixels(cam.getPixelsRef());
        img.mirror(false,true);

        //img.convertRgbToHsv();
        hsvImg = img;
        hsvImg.convertRgbToHsv();

        image=img;

        if(ofGetKeyPressed(OF_KEY_DOWN))
        {
            tolerance--;
        }
        if(ofGetKeyPressed(OF_KEY_UP))
        {
            tolerance++;
        }

        for (int j =0; j<img_height; ++j)
        {
            for (int i=0; i<img_width ; ++i)
            {
              if( (ofInRange(hsvImg.getPixelsRef().getColor(i,j).getHue(), hue-tolerance, hue+tolerance)) &&
                  (ofInRange(hsvImg.getPixelsRef().getColor(i,j).getSaturation(), sat-tolerance, sat+tolerance)) &&
                  (ofInRange(hsvImg.getPixelsRef().getColor(i,j).getBrightness(), bright-tolerance, bright+tolerance))   )

                  grayImg.getPixelsRef().setColor(i,j,ofColor::white);


              else
                  grayImg.getPixelsRef().setColor(i,j,ofColor::black);

            }
        }

        grayImg.flagImageChanged();

        //grayImg.erode();
        //grayImg.dilate();
        //grayImg.dilate();
        //grayImg.erode();


        contour.findContours(grayImg,0,img_width*img_height,1,false);

        int lenght = 0;
        for(int i = 0; i < contour.nBlobs; i++) {

            ofxCvBlob blob = contour.blobs.at(i);
            if (blob.length > lenght)
            {
                theBlob=blob;
                lenght=blob.length;
            }

        }

        bb=theBlob.boundingRect;
        if ( bb.getWidth() <= 0)
            sizeObjectInProj = 1;
        else
            sizeObjectInProj = bb.getWidth();


        //partie 3 - detection de visage

//        if (width < 0. && height < 0. && haarFinder.blobs.size() > 0){
//            roi = haarFinder.blobs[0].boundingRect;
//            width = 1.5 * roi.width;
//            height = 1.5 * roi.height;
//            roi.setWidth(width);
//            roi.setHeight(height);

//        }
//        else{
//            ofRectangle tmp;
//            tmp = haarFinder.blobs[0].boundingRect;
//            roi.setPosition(tmp.getPosition());

//            haarFinder.findHaarObjects(image, roi);
//        }


           haarFinder.findHaarObjects(image, roi);

           if(haarFinder.blobs.size() > 0)
           {
               ofxCvBlob blobTmp = haarFinder.blobs.at(0);

               face =blobTmp.boundingRect;
//               float x = face.getX()-(face.getX())/4;
//               float y = face.getY()-(face.getY())/4;
//               float w = face.getWidth()*1.5;
//               float h = face.getHeight()*1.5;
               face.scaleFromCenter(1.5);
               //roi=ofRectangle(x,y,w,h);
               roi=face;
               roiUpdate=true;


           }
           else
               if(roiUpdate==true)
               {
                   roi=roiInit;
                   roiUpdate=false;
               }


    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    img.draw(0,0);
    grayImg.draw(img_width,0);
    img.draw(0,img_height);
    contour.draw(0,img_height);

    for(int i = 0; i < haarFinder.blobs.size(); i++) {
         //ofRect( haarFinder.blobs[i].boundingRect);
         ofRectangle rect(haarFinder.blobs[i].boundingRect);
         //glasses.draw(rect.getX(), rect.getY(),2*rect.getWidth()/3, rect.getHeight()/2);
         //glasses.resize();
         glasses.draw(rect.getX(),rect.getY()+rect.getHeight()/4,rect.getWidth(),rect.getHeight()/3);
         //beard.draw(rect.getX(), rect.getY()+rect.getHeight()/2,2*rect.getWidth()/3, rect.getHeight()/2);
         beard.draw(rect.getX(), rect.getY()+rect.getHeight()/2,rect.getWidth(), 3*rect.getHeight()/4);
      }

    ofTranslate(0,img_height);
    ofRect(bb);


    //ofRect(roi);




    // Patch correspondant à la couleur cible
    ofTranslate(8, 8);
    ofFill();
    ofSetColor(0);
    ofRect(-3, -3, 64+6, 64+6);
    ofSetColor(targetColor);
    ofRect(0, 0, 64, 64);


    computeDistance();

    std::cout << "Distance = " << distance << std::endl;


}

void ofApp::computeDistance()
{
    float D = sizeObject * (HEIGHT/ sizeObjectInProj );
    distance = (D/2.0)/atan(((CAMERA_ANGLE/180.0)* M_PI) /2.0);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    targetColor = img.getPixelsRef().getColor(x, y);


    targetColorHsv=rgbToHsv(targetColor);

    targetColorHsv.getHsb(hue,sat,bright);


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
