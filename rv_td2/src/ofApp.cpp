#include "ofApp.h"



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

    nbFacesInImage = 0;
    for (int i = 0; i < NB_MAX_FACES; ++i ){
        roiFaces[i].setHeight(-1.);
        roiFaces[i].setWidth(-1.);
    }

    glasses.loadImage("glasses.png");
    beard.loadImage("beard.png");



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

        // Binarize
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

        grayImg.erode();
        grayImg.dilate();
        grayImg.dilate();
        grayImg.erode();

        // Find blob
        contour.findContours(grayImg,0,img_width*img_height,1,false);

        int length = 0;
        for(int i = 0; i < contour.nBlobs; i++) {
            // we keep the largest blob
            ofxCvBlob blob = contour.blobs.at(i);
            if (blob.length > length)
            {
                theBlob=blob;
                length=blob.length;
            }

        }

        roiColor=theBlob.boundingRect;


        // Face tracking
        // If no faces was recognized
        if (nbFacesInImage == 0){

            haarFinder.findHaarObjects(image);
            nbFacesInImage = haarFinder.blobs.size();
            for (int i = 0; i < haarFinder.blobs.size(); ++i){
                roiFaces[i] = haarFinder.blobs[i].boundingRect;
                roiFaces[i].setPosition(roiFaces[i].getX() - 1/4 * roiFaces[i].width,
                                        roiFaces[i].getY() - 1/4 * roiFaces[i].height);

                roiFaces[i].height *= 1.5;
                roiFaces[i].width *= 1.5;
            }

        }
        else{
            ofRectangle tmp;
            std::cout << nbFacesInImage << std::endl;
            for (int i = 0; i < NB_MAX_FACES; ++i){
                if (roiFaces[i].getHeight() >= 0. && roiFaces[i].getHeight() >= 0.){
                    haarFinder.findHaarObjects(image, roiFaces[i]);
                    if (haarFinder.blobs.size() != 0){
                        std::cout << roiFaces[i].getX() <<  "x " << roiFaces[i].getWidth() << std::endl;
                        tmp = haarFinder.blobs[0].boundingRect;
                        std::cout << roiFaces[i].getX() <<  "xbis " << roiFaces[i].getWidth() << std::endl << std::endl;
                        roiFaces[i].setPosition(roiFaces[i].getX() - 1/4 * roiFaces[i].width,
                                                roiFaces[i].getY() - 1/4 * roiFaces[i].height);


                    }
                    else{
                        roiFaces[i].height = -1;
                        roiFaces[i].width = -1;
                        nbFacesInImage--;
                    }
                }

            }

        }



    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    img.draw(0,0);
    grayImg.draw(img_width,0);
    img.draw(0,img_height);
    img.draw(img_width, img_height);

    ofTranslate(0,img_height);
    ofRect(roiColor);



    ofTranslate(img_width,0);

    for (int i = 0; i < NB_MAX_FACES; ++i){
        if (roiFaces[i].height >= 0. || roiFaces[i].width >= 0.){
            glasses.draw(roiFaces[i].getX(), roiFaces[i].getY(), 2*roiFaces[i].getWidth()/3, roiFaces[i].getHeight()/2);
            beard.draw(roiFaces[i].getX(), roiFaces[i].getY()+roiFaces[i].getHeight()/2, 2*roiFaces[i].getWidth()/3, roiFaces[i].getHeight()/2);

        }
    }

    // Patch correspondant à la couleur cible
    ofTranslate(-img_width, 0);
    ofTranslate(8, 8);
    ofFill();
    ofSetColor(0);
    ofRect(-3, -3, 64+6, 64+6);
    ofSetColor(targetColor);
    ofRect(0, 0, 64, 64);




}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    /*if(key == OF_KEY_DOWN)
    {
          tolerance++;
    }

    if(key == OF_KEY_DOWN)
    {
          tolerance--;
    }*/
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
