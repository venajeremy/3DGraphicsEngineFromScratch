#include "object.hpp"

Object::Object(std::vector<Surface> inMesh, float inPosX, float inPosY, float inPosZ, float inYaw, float inPitch, float inRoll)
{
    mesh = inMesh;
    // XYZ is relative to camera
    posX = inPosX;
    posY = inPosY;
    posZ = inPosZ;

    // Rotations is relative to object itself
    yaw = inYaw;
    pitch = inPitch;
    roll = inRoll;
}

void Object::worldTranslate(float dX, float dY, float dZ)
{

    // Multiple translations should not be made at the same time
    // If they are the position translation will be made first
    posX -= dX;
    posY -= dY;
    posZ -= dZ;
   
}

void Object::worldRotate(float dYaw, float dPitch, float dRoll)
{
    // vertice array is in form x1, y1, z1, x2, y2, z2, x3, y3, z3,
    // Note: these rotations are relative to the camera

    if(dYaw!=0) {
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            surf->vertices[0] = (surf->vertices[0]*cos(dYaw))-(surf->vertices[2]*sin(dYaw));
            surf->vertices[2] = (surf->vertices[0]*sin(dYaw))+(surf->vertices[2]*cos(dYaw));
            surf->vertices[3] = (surf->vertices[3]*cos(dYaw))-(surf->vertices[5]*sin(dYaw));
            surf->vertices[5] = (surf->vertices[3]*sin(dYaw))+(surf->vertices[5]*cos(dYaw));
            surf->vertices[6] = (surf->vertices[6]*cos(dYaw))-(surf->vertices[8]*sin(dYaw));
            surf->vertices[8] = (surf->vertices[6]*sin(dYaw))+(surf->vertices[8]*cos(dYaw));

        }
        posX = (posX*cos(dYaw))-(posZ*sin(dYaw));
        posZ = (posX*sin(dYaw))+(posZ*cos(dYaw));
    }
    if(dPitch!=0) {

        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            surf->vertices[2] = (surf->vertices[2]*cos(dPitch))-(surf->vertices[1]*sin(dPitch));
            surf->vertices[1] = (surf->vertices[2]*sin(dPitch))+(surf->vertices[1]*cos(dPitch));
            surf->vertices[5] = (surf->vertices[5]*cos(dPitch))-(surf->vertices[4]*sin(dPitch));
            surf->vertices[4] = (surf->vertices[5]*sin(dPitch))+(surf->vertices[4]*cos(dPitch));
            surf->vertices[8] = (surf->vertices[8]*cos(dPitch))-(surf->vertices[7]*sin(dPitch));
            surf->vertices[7] = (surf->vertices[8]*sin(dPitch))+(surf->vertices[7]*cos(dPitch));

        }
        posZ = (posZ*cos(dPitch))-(posY*sin(dPitch));
        posY = (posZ*sin(dPitch))+(posY*cos(dPitch));
    }
    if(dRoll!=0) {
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            surf->vertices[1] = (surf->vertices[1]*cos(dRoll))-(surf->vertices[2]*sin(dRoll));
            surf->vertices[2] = (surf->vertices[1]*sin(dRoll))+(surf->vertices[2]*cos(dRoll));
            surf->vertices[4] = (surf->vertices[4]*cos(dRoll))-(surf->vertices[5]*sin(dRoll));
            surf->vertices[5] = (surf->vertices[4]*sin(dRoll))+(surf->vertices[5]*cos(dRoll));
            surf->vertices[7] = (surf->vertices[7]*cos(dRoll))-(surf->vertices[8]*sin(dRoll));
            surf->vertices[8] = (surf->vertices[7]*sin(dRoll))+(surf->vertices[8]*cos(dRoll));

        }
        posX = (posX*cos(dRoll))-(posY*sin(dRoll));
        posY = (posX*sin(dRoll))+(posY*cos(dRoll));
    }
}

void Object::objectRotate(float dYaw, float dPitch, float dRoll)
{

    // Note: these rotations are relative to the object's origin

    if(dYaw!=0) {
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            surf->vertices[0] = (surf->vertices[0]*cos(dYaw))-(surf->vertices[2]*sin(dYaw));
            surf->vertices[2] = (surf->vertices[0]*sin(dYaw))+(surf->vertices[2]*cos(dYaw));
            surf->vertices[3] = (surf->vertices[3]*cos(dYaw))-(surf->vertices[5]*sin(dYaw));
            surf->vertices[5] = (surf->vertices[3]*sin(dYaw))+(surf->vertices[5]*cos(dYaw));
            surf->vertices[6] = (surf->vertices[6]*cos(dYaw))-(surf->vertices[8]*sin(dYaw));
            surf->vertices[8] = (surf->vertices[6]*sin(dYaw))+(surf->vertices[8]*cos(dYaw));

        }
        //posX = (posX*cos(dYaw))-(posZ*sin(dYaw));
       // posZ = (posX*sin(dYaw))+(posZ*cos(dYaw));
    }
    if(dPitch!=0) {

        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            surf->vertices[2] = (surf->vertices[2]*cos(dPitch))-(surf->vertices[1]*sin(dPitch));
            surf->vertices[1] = (surf->vertices[2]*sin(dPitch))+(surf->vertices[1]*cos(dPitch));
            surf->vertices[5] = (surf->vertices[5]*cos(dPitch))-(surf->vertices[4]*sin(dPitch));
            surf->vertices[4] = (surf->vertices[5]*sin(dPitch))+(surf->vertices[4]*cos(dPitch));
            surf->vertices[8] = (surf->vertices[8]*cos(dPitch))-(surf->vertices[7]*sin(dPitch));
            surf->vertices[7] = (surf->vertices[8]*sin(dPitch))+(surf->vertices[7]*cos(dPitch));

        }

        //posZ = (posZ*cos(dPitch))-(posY*sin(dPitch));
        //posY = (posZ*sin(dPitch))+(posY*cos(dPitch));
    }
    if(dRoll!=0) {
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            surf->vertices[1] = (surf->vertices[1]*cos(dRoll))-(surf->vertices[2]*sin(dRoll));
            surf->vertices[2] = (surf->vertices[1]*sin(dRoll))+(surf->vertices[2]*cos(dRoll));
            surf->vertices[4] = (surf->vertices[4]*cos(dRoll))-(surf->vertices[5]*sin(dRoll));
            surf->vertices[5] = (surf->vertices[4]*sin(dRoll))+(surf->vertices[5]*cos(dRoll));
            surf->vertices[7] = (surf->vertices[7]*cos(dRoll))-(surf->vertices[8]*sin(dRoll));
            surf->vertices[8] = (surf->vertices[7]*sin(dRoll))+(surf->vertices[8]*cos(dRoll));

        }
        //posX = (posX*cos(dRoll))-(posY*sin(dRoll));
        //posY = (posX*sin(dRoll))+(posY*cos(dRoll));
    }
}

std::vector<Surface> Object::getMesh()
{
    // Returns the mesh of the object (lot of parts to this)
    // For each vertex apply object rotation ( do this later )
    // For each vertex apply object relative position translation
    std::vector<Surface> returnMesh = mesh;
    for(auto it = returnMesh.begin(); it != returnMesh.end(); ++it) {
        // Add in portion to compensite for object rotation later (axis of rotation should be 0,0,0)
        it->vertices[0] += posX;
        it->vertices[1] += posY;
        it->vertices[2] += posZ;
        it->vertices[3] += posX;
        it->vertices[4] += posY;
        it->vertices[5] += posZ;
        it->vertices[6] += posX;
        it->vertices[7] += posY;
        it->vertices[8] += posZ;
    }

    return returnMesh;

    
}

Surface::Surface(std::array<float,9> inVerticies, std::array<float,4> inColor)
{
    // color in form r, g, b, alpha
    // verties in form x1,y1,z1,x2,y2,z3,x3,y3,z3
    vertices = inVerticies;
    color = inColor;
}

