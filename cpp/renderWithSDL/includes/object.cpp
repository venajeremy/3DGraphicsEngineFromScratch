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

void Object::objectTranslate(float dX, float dY, float dZ)
{

    // Multiple translations should not be made at the same time
    // If they are the position translation will be made first
    posX -= dX;
    posY -= dY;
    posZ -= dZ;
   
}

std::vector<Surface> Object::getMesh(float cameraX,float cameraY,float cameraZ,float cameraPitch,float cameraYaw,float cameraRoll)
{
    // Returns the mesh of the object (lot of parts to this)
    // For each vertex apply object rotation ( do this later )
    // For each vertex apply object relative position translation
    std::vector<Surface> returnMesh = mesh;
    for(auto it = returnMesh.begin(); it != returnMesh.end(); ++it) {
        // Add Position Of object and subtract position of camera
        it->vertices[0] += posX-cameraX;
        it->vertices[1] += posY-cameraY;
        it->vertices[2] += posZ-cameraZ;
        it->vertices[3] += posX-cameraX;
        it->vertices[4] += posY-cameraY;
        it->vertices[5] += posZ-cameraZ;
        it->vertices[6] += posX-cameraX;
        it->vertices[7] += posY-cameraY;
        it->vertices[8] += posZ-cameraZ;

        // Rotate every vertice around the origin
        sinPitch = sin(cameraPitch);
        cosPitch = cos(cameraPitch);
        sinYaw = sin(cameraYaw);
        cosYaw = cos(cameraYaw);
        sinRoll = sin(cameraRoll);
        cosRoll = cos(cameraRoll);

        // Rotate Yaw

        x1 = (it->vertices[0]*cosYaw)-(it->vertices[2]*sinYaw);
        z1 = (it->vertices[0]*sinYaw)+(it->vertices[2]*cosYaw);
        x2 = (it->vertices[3]*cosYaw)-(it->vertices[5]*sinYaw);
        z2 = (it->vertices[3]*sinYaw)+(it->vertices[5]*cosYaw);
        x3 = (it->vertices[6]*cosYaw)-(it->vertices[8]*sinYaw);
        z3 = (it->vertices[6]*sinYaw)+(it->vertices[8]*cosYaw);
 
        it->vertices[0] = x1;
        it->vertices[2] = z1;
        it->vertices[3] = x2;
        it->vertices[5] = z2;
        it->vertices[6] = x3;
        it->vertices[8] = z3;

        // Rotate Pitch

        z1 = (it->vertices[2]*cosPitch)-(it->vertices[1]*sinPitch);
        y1 = (it->vertices[2]*sinPitch)+(it->vertices[1]*cosPitch);
        z2 = (it->vertices[5]*cosPitch)-(it->vertices[4]*sinPitch);
        y2 = (it->vertices[5]*sinPitch)+(it->vertices[4]*cosPitch);
        z3 = (it->vertices[8]*cosPitch)-(it->vertices[7]*sinPitch);
        y3 = (it->vertices[8]*sinPitch)+(it->vertices[7]*cosPitch);

        it->vertices[1] = y1;
        it->vertices[2] = z1;
        it->vertices[4] = y2;
        it->vertices[5] = z2;
        it->vertices[7] = y3;
        it->vertices[8] = z3;
       
        // Rotate Roll
        
        x1 = (it->vertices[0]*cosRoll)-(it->vertices[1]*sinRoll);
        y1 = (it->vertices[0]*sinRoll)+(it->vertices[1]*cosRoll);
        x2 = (it->vertices[3]*cosRoll)-(it->vertices[4]*sinRoll);
        y2 = (it->vertices[3]*sinRoll)+(it->vertices[4]*cosRoll);
        x3 = (it->vertices[6]*cosRoll)-(it->vertices[7]*sinRoll);
        y3 = (it->vertices[6]*sinRoll)+(it->vertices[7]*cosRoll);
            
        it->vertices[0] = x1;
        it->vertices[1] = y1;
        it->vertices[3] = x2;
        it->vertices[4] = y2;
        it->vertices[6] = x3;
        it->vertices[7] = y3;
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

