#include "ofApp.h"
#include <glm/gtx/intersect.hpp>
#include "ofxGui.h"

/*
 CS116A Final Project
 Areas covered are Interactive Features and Anti-Aliasing
 Author: Aaron Dang
 Starter files supplied by: Kevin Smith
 
 
 Instructions on how to use:
     Press 'a' to toggle between whether you want to move around the world or move the objects
     Press 'r' to render an image
        Image will be saved in \bin\data
        Image will also be drawn on the world once render is finished
     Press 's' to create a new sphere
        Every sphere is auto generated a color
        Slider provided to adjust the sphere size
     Press 'l' to create a light source
     Select a sphere and press 'd' to delete a sphere
     Select a light source and press 'f' to delete a light
 
 Slider is provided to choose the level of Anti-alias. The higher, the better quality :D
 
 */

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(ofColor::black);
    //main cam
    theCam = &mainCam;
    mainCam.setDistance(30);
    mainCam.setNearClip(.1);
    //side cam
    sideCam.setPosition(glm::vec3(100, 0, 0));
    sideCam.lookAt(glm::vec3(0, 0, 0));
    sideCam.setNearClip(.1);
    //preview cam(set same at render cam)
    previewCam.setPosition(renderCam.position);
    previewCam.lookAt(renderCam.aim);
    
    //Gui sliders
    gui.setup();
    gui.add(lightIntensity.setup("Light intensity", 50.0, 10.0, 100.0));
    gui.add(powerExp.setup("Power exponent", 50.0, 10.0, 100.0));
    gui.add(sphereSize.setup("Sphere radius", 1.5, 0.0, 3.0));
    gui.add(oversamplingNum.setup("0versampling X", 4, 4, 16));
    
    
    
    //draw spheres and plane
    //
    Sphere* redSphere = new Sphere(glm::vec3(.5, -1, -7.5), 1.7, ofColor::paleVioletRed);
    sphereAmt++;
    redSphere->name =  "sphere" + to_string(sphereAmt);
    Sphere* orangeSphere = new Sphere(glm::vec3(1, -2.7,-3 ), 2, ofColor::orange);
    sphereAmt++;
    orangeSphere->name =  "sphere" + to_string(sphereAmt);
    Sphere* yellowSphere = new Sphere(glm::vec3(-3, -2, -12), 2.5, ofColor::yellow);
    sphereAmt++;
    yellowSphere->name =  "sphere" + to_string(sphereAmt);
    
    scene.push_back(new Plane(glm::vec3(0, -5, -8), glm::vec3(0, 1, 0), ofColor::skyBlue));
    scene.push_back(redSphere);
    scene.push_back(orangeSphere);
    scene.push_back(yellowSphere);
    
    //set the lights
    //
    Light* light1 = new Light(glm::vec3(0, 10,-8));
    lightAmt++;
    light1->name = "light" + to_string(lightAmt);
    Light* light2 = new Light(glm::vec3(10,8,-5));
    lightAmt++;
    light2->name = "light" + to_string(lightAmt);
    
    lights.push_back(light1);
    lights.push_back(light2);
    
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    gui.draw();
    
    theCam->begin();
    ofFill();
    
    //draws spheres and plane
    ofEnableDepthTest();
    material.begin();
    ofFill();

    for (int i = 0; i < scene.size(); i++) {
        if (objSelected() && scene[i] == selected[0])
            ofSetColor(ofColor::white);
        else ofSetColor(scene[i]->diffuseColor);
        scene[i]->draw();
    }
    //draws light bulb
    for(int b = 0; b < lights.size(); b++){
        if (objSelected() && lights[b] == selected[0])
            ofSetColor(ofColor::red);
        else ofSetColor(lights[b]->diffuseColor);
        lights[b]->draw();
    }
    
    material.end();
    ofDisableDepthTest();
    ofNoFill();
    
    ofSetColor(ofColor::white);
    renderCam.view.draw();
    theCam->end();
    image.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case OF_KEY_F1:
        theCam = &mainCam;
        break;
    case OF_KEY_F2:
        theCam = &sideCam;
        break;
    case OF_KEY_F3:
        theCam = &previewCam;
        break;
    //Press 'a' to switch modes whether you would like to move the object or the world
    case 'a':
        if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
        else mainCam.enableMouseInput();
        break;
    //Press 's' to create a brand new sphere
    //Can move it around to desired location
    case 's':
        {
            //Create a new joint and add it to the scene
            Sphere* newSphere = new Sphere(glm::vec3(0, 0, 0), sphereSize, ofColor( ofRandom(0, 255),
                                                                                   ofRandom(0, 255),
                                                                                   ofRandom(0, 255)
                                                                                   ));
            scene.push_back(newSphere);

            newSphere->name = "sphere" + to_string(sphereAmt);

            sphereAmt++;
        }
        break;
    //Press 'l' to create a brand new light
    //Can move it around to desired location
    case 'l':
        {
            //Create a new joint and add it to the scene
            Light* newLight = new Light();
            lights.push_back(newLight);

            newLight->position = glm::vec3(0, 10, 0);
            newLight->name = "light" + to_string(sphereAmt);

            lightAmt++;
        }
        break;
    //Press 'd' to delete sphere
    case 'd':
        if(objSelected() && selected[0]->name.c_str()[0] == 's'){
            int deleteIndex;
            //get the selected object name
            string selectedName = selected[0]->name;
            //Loop through all the scenes to see at what index the selected object is at
            for(int i = 0; i < scene.size(); i++){
                //If the scene object matches the name, store the index into deleteindex
                if(scene[i]->name.compare(selectedName) == 0){
                    deleteIndex = i;
                }
            }
            
            //delete
            scene.erase(scene.begin() + deleteIndex);
        
            selected.clear();
        }
        break;
    //Press 'f' to delete lights
    case 'f':
        if(objSelected() && selected[0]->name.c_str()[0] == 'l') {
            int deleteIndex;
            //get the selected object name
            string selectedName = selected[0]->name;
            //Loop through all the scenes to see at what index the selected object is at
            for(int i = 0; i < lights.size(); i++){
                //If the scene object matches the name, store the index into deleteindex
                if(lights[i]->name.compare(selectedName) == 0){
                    deleteIndex = i;
                }
            }
            
            //delete
            lights.erase(lights.begin() + deleteIndex);

            selected.clear();
        }
        break;
    case 'r':
        cout << "RENDERING" << endl;
        rayTrace();
        cout << "FINISHED" << endl;
        break;
    case OF_KEY_ALT:
        bAltKeyDown = true;
        if (!mainCam.getMouseInputEnabled()) mainCam.enableMouseInput();
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------
//All Ray Tracing Methods Start Here
//
void ofApp::rayTrace(){
    
    image.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
    
    float pixelWidth = 1.0 / imageWidth;
    float pixelHeight = 1.0 / imageHeight;
    float halfOfPixelWidth = pixelWidth / 2.0;
    float halfOfPixelHeight = pixelHeight / 2.0;

    for (int row = 0; row < imageHeight; row++) {
        for (int col = 0; col < imageWidth; col++) {

            float centerU = col * pixelWidth + halfOfPixelWidth;
            float centerV = row * pixelHeight + halfOfPixelHeight;
            // Compute the color for a pixel
            ofColor color = antiAlias(centerU, centerV, pixelWidth, pixelHeight);
            // Set computed color to pixel | flipped over
            image.setColor(col, imageHeight - row - 1, color);
        }
    }
    //image.update();
    image.save("demo.png");
    image.load("demo.png");
}
//--------------------------------------------------------------
bool ofApp::myRayIsBlocked(Ray ray){
    for(SceneObject* obj : scene){
        if(obj->intersect(ray, intersectPt, intersectNormal)){
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------
//Phong shading calculations
//
ofColor ofApp::shade(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power){
    
    float shading = 0.0;
    float shadingtwo = 0.0;
    
    for(int i = 0; i < lights.size(); i++){
        glm::vec3 n = glm::normalize(norm);
        glm::vec3 l = glm::normalize(lights[i]->position - p);
        glm::vec3 v = glm::normalize(renderCam.position - p);
        float r = glm::length(lights[i]->position - p);

        float illumination = (lights[i]->intensity / (r * r));
        float max = glm::max(0.0f, glm::dot(n, l));
        
        glm::vec3 h = (v + l)/glm::length(v + l);
        //max(0, n.h)^p where h is the bisector
        float pow = glm::max(0.0f, glm::pow(glm::dot(norm, h), power));
        
        //Shadow Calculation
        //
        //setting up for ray position
        glm::vec3 rayP = closestIntersectPt + (startPointMove * closestIntersectNorm);
        Ray myRay = Ray(rayP, glm::normalize(lights[i]->position - rayP));
    
        //Surface should ony be illuminated if nothing blocks its view of light
        if(!myRayIsBlocked(myRay)){
            //(I/r^2)max(0, n.h)^p
            shading += (illumination * pow);
            //(I/r^2) * max(0, n.1)
            shadingtwo += (illumination * max);
        }
    }
    
    ofColor resultColor = (specular * shading) + (diffuse * shadingtwo) + (diffuse * ambientCoefficient);
    return resultColor;
}
//--------------------------------------------------------------
ofColor ofApp::antiAlias(const float uCenter, const float vCenter, const float pixelWidth, const float pixelHeight){
    
    //Divided each pixel into desired number of subpixels to increase sampling  (desiredTestNum x desiredTestNum)
    float dividedPixelWidth = pixelWidth / oversamplingNum;
    float dividedPixelHeight = pixelHeight / oversamplingNum;
    float topLeftOfU = uCenter - dividedPixelWidth;
    float topLeftOfV = vCenter - dividedPixelHeight;
    
    //store the color values to average out
    float r = 0;
    float g = 0;
    float b = 0;
    ofColor colorAverage;
    
    //Loop through every subpixel that we had made
    for(int row = 0; row < oversamplingNum; row++){
        for(int col = 0; col < oversamplingNum; col++){
            
            Ray ray = renderCam.getRay(topLeftOfU + dividedPixelWidth * col, topLeftOfV + dividedPixelHeight * row);
            
            int intersectedObj;
            //if already true, make it false so I can retest
            hit = false;
            float distance = std::numeric_limits<float>::infinity();
            SceneObject *closestObject = NULL;
            
            for(int k = 0; k < scene.size(); k++){
                if(scene[k]->intersect(ray, intersectPt, intersectNormal)){
                    //determine if object is the closet
                    float closestDist = glm::length(ray.p - intersectPt);
                    if(closestDist < distance){
                        distance = closestDist;
                        closestObject = scene[k];
                        closestIntersectPt = intersectPt;
                        closestIntersectNorm = intersectNormal;
                        intersectedObj = k;
                    }
                    if(closestObject){
                        hit = true;
                    }
                }
            }
            if(hit){
                ofColor theColor = shade(closestIntersectPt, closestIntersectNorm, closestObject->diffuseColor, ofColor::white, powerExp);
                r += theColor.r;
                g += theColor.g;
                b += theColor.b;
            }
            else{
                ofColor otherColor = ofGetBackgroundColor();
            }
        }
    }
    
    colorAverage.r = r / (oversamplingNum * oversamplingNum);
    colorAverage.g = g / (oversamplingNum * oversamplingNum);
    colorAverage.b = b / (oversamplingNum * oversamplingNum);
    
    return colorAverage;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
    case OF_KEY_ALT:
        bAltKeyDown = false;
        mainCam.disableMouseInput();
        break;
    case 'x':
        bRotateX = false;
        break;
    case 'y':
        bRotateY = false;
        break;
    case 'z':
        bRotateZ = false;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (objSelected() && bDrag) {
        glm::vec3 point;
        mouseToDragPlane(x, y, point);
        if (bRotateX) {
            selected[0]->rotation += glm::vec3((point.x - lastPoint.x) * 20.0, 0, 0);
        }
        else if (bRotateY) {
            selected[0]->rotation += glm::vec3(0, (point.x - lastPoint.x) * 20.0, 0);
        }
        else if (bRotateZ) {
            selected[0]->rotation += glm::vec3(0, 0, (point.x - lastPoint.x) * 20.0);
        }
        else {
            selected[0]->position += (point - lastPoint);
        }
        lastPoint = point;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    // if we are moving the camera around, don't allow selection
    //
    if (mainCam.getMouseInputEnabled()) return;

    // clear selection list
    //
    selected.clear();

    //
    // test if something selected
    //
    vector<SceneObject *> hits;

    glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
    glm::vec3 d = p - theCam->getPosition();
    glm::vec3 dn = glm::normalize(d);

    // check for selection of scene objects
    //
    for (int i = 0; i < scene.size(); i++) {
        
        glm::vec3 point, norm;
        
        //  We hit an object
        //
        if (scene[i]->isSelectable && scene[i]->intersect(Ray(p, dn), point, norm)) {
            hits.push_back(scene[i]);
        }
    }
    // check for selection of light objects
    //
    for (int j = 0; j < lights.size(); j++) {

        glm::vec3 pointl, norml;

        //  We hit an object
        //
        if (lights[j]->isSelectable && lights[j]->intersect(Ray(p, dn), pointl, norml)) {
            hits.push_back(lights[j]);
        }
    }

    // if we selected more than one, pick nearest
    //
    SceneObject *selectedObj = NULL;
    if (hits.size() > 0) {
        selectedObj = hits[0];
        float nearestDist = std::numeric_limits<float>::infinity();
        for (int n = 0; n < hits.size(); n++) {
            float dist = glm::length(hits[n]->position - theCam->getPosition());
            if (dist < nearestDist) {
                nearestDist = dist;
                selectedObj = hits[n];
            }
        }
    }
    if (selectedObj) {
        selected.push_back(selectedObj);
        bDrag = true;
        mouseToDragPlane(x, y, lastPoint);
    }
    else {
        selected.clear();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bDrag = false;
}
//--------------------------------------------------------------

//  This projects the mouse point in screen space (x, y) to a 3D point on a plane
//  normal to the view axis of the camera passing through the point of the selected object.
//  If no object selected, the plane passing through the world origin is used.
//
bool ofApp::mouseToDragPlane(int x, int y, glm::vec3 &point) {
    glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
    glm::vec3 d = p - theCam->getPosition();
    glm::vec3 dn = glm::normalize(d);

    float dist;
    glm::vec3 pos;
    if (objSelected()) {
        pos = selected[0]->position;
    }
    else pos = glm::vec3(0, 0, 0);
    if (glm::intersectRayPlane(p, dn, pos, glm::normalize(theCam->getZAxis()), dist)) {
        point = p + dn * dist;
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

// Intersect Ray with Plane  (wrapper on glm::intersect*
//
bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normalAtIntersect) {
    float dist;
    bool insidePlane = false;
    bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
    if(hit){
        Ray r = ray;
        point = r.evalPoint(dist);
        normalAtIntersect = this->normal;
        glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
        glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);
        if(point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1] && point.z > zrange[0]){
            insidePlane = true;
        }
    }
    return insidePlane;
}


// Convert (u, v) to (x, y, z)
// We assume u,v is in [0, 1]
//
glm::vec3 ViewPlane::toWorld(float u, float v) {
    float w = width();
    float h = height();
    return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

// Get a ray from the current camera position to the (u, v) position on
// the ViewPlane
//
Ray RenderCam::getRay(float u, float v) {
    glm::vec3 pointOnPlane = view.toWorld(u, v);
    return(Ray(position, glm::normalize(pointOnPlane - position)));
}


//Sphere methods
//
void Sphere::draw() {

    //   get the current transformation matrix for this object
   //
    glm::mat4 m = getMatrix();

    //   push the current stack matrix and multiply by this object's
    //   matrix. now all vertices dran will be transformed by this matrix
    //
    ofPushMatrix();
    ofMultMatrix(m);
    ofDrawSphere(radius);
    ofPopMatrix();
    
}

// Generate a rotation matrix that rotates v1 to v2
// v1, v2 must be normalized
//
glm::mat4 SceneObject::rotateToVector(glm::vec3 v1, glm::vec3 v2) {

    glm::vec3 axis = glm::cross(v1, v2);
    glm::quat q = glm::angleAxis(glm::angle(v1, v2), glm::normalize(axis));
    return glm::toMat4(q);
}

