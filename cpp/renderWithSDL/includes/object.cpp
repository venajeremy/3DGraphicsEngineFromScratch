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


void Object::worldRotateGrounded(float dYaw, float dPitch, float dRoll)
{
    // vertice array is in form x1, y1, z1, x2, y2, z2, x3, y3, z3,
    // Note: these rotations are relative to the camera
    if(dYaw!=0) {
        cosYaw = std::cos(dYaw);
        sinYaw = std::sin(dYaw);
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            x1 = (surf->vertices[0]*cosYaw)-(surf->vertices[2]*sinYaw);
            z1 = (surf->vertices[0]*sinYaw)+(surf->vertices[2]*cosYaw);
            x2 = (surf->vertices[3]*cosYaw)-(surf->vertices[5]*sinYaw);
            z2 = (surf->vertices[3]*sinYaw)+(surf->vertices[5]*cosYaw);
            x3 = (surf->vertices[6]*cosYaw)-(surf->vertices[8]*sinYaw);
            z3 = (surf->vertices[6]*sinYaw)+(surf->vertices[8]*cosYaw);
     
            surf->vertices[0] = x1;
            surf->vertices[2] = z1;
            surf->vertices[3] = x2;
            surf->vertices[5] = z2;
            surf->vertices[6] = x3;
            surf->vertices[8] = z3;

        }
        nposX = (posX*cos(dYaw))-(posZ*sin(dYaw));
        nposZ = (posX*sin(dYaw))+(posZ*cos(dYaw));
        posX = nposX;
        posZ = nposZ;
    }
    if(dPitch!=0) {
        cosPitch = std::cos(dPitch);
        sinPitch = std::sin(dPitch);
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            z1 = (surf->vertices[2]*cosPitch)-(surf->vertices[1]*sinPitch);
            y1 = (surf->vertices[2]*sinPitch)+(surf->vertices[1]*cosPitch);
            z2 = (surf->vertices[5]*cosPitch)-(surf->vertices[4]*sinPitch);
            y2 = (surf->vertices[5]*sinPitch)+(surf->vertices[4]*cosPitch);
            z3 = (surf->vertices[8]*cosPitch)-(surf->vertices[7]*sinPitch);
            y3 = (surf->vertices[8]*sinPitch)+(surf->vertices[7]*cosPitch);

            surf->vertices[1] = y1;
            surf->vertices[2] = z1;
            surf->vertices[4] = y2;
            surf->vertices[5] = z2;
            surf->vertices[7] = y3;
            surf->vertices[8] = z3;

        }
        nposZ = (posZ*cos(dPitch))-(posY*sin(dPitch));
        nposY = (posZ*sin(dPitch))+(posY*cos(dPitch));
        posZ = nposZ;
        posY = nposY;
    }
    if(dRoll!=0) {
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            x1 = (surf->vertices[0]*cos(dRoll))-(surf->vertices[1]*sin(dRoll));
            y1 = (surf->vertices[0]*sin(dRoll))+(surf->vertices[1]*cos(dRoll));
            x2 = (surf->vertices[3]*cos(dRoll))-(surf->vertices[4]*sin(dRoll));
            y2 = (surf->vertices[3]*sin(dRoll))+(surf->vertices[4]*cos(dRoll));
            x3 = (surf->vertices[6]*cos(dRoll))-(surf->vertices[7]*sin(dRoll));
            y3 = (surf->vertices[6]*sin(dRoll))+(surf->vertices[7]*cos(dRoll));
                
            surf->vertices[0] = x1;
            surf->vertices[1] = y1;
            surf->vertices[3] = x2;
            surf->vertices[4] = y2;
            surf->vertices[6] = x3;
            surf->vertices[7] = y3;

        }
        nposX = (posX*cos(dRoll))-(posY*sin(dRoll));
        nposY = (posX*sin(dRoll))+(posY*cos(dRoll));
        posX = nposX;
        posY = nposY;
    }

}


void Object::worldRotateFloating(float dYaw, float dPitch, float dRoll)
{
    // vertice array is in form x1, y1, z1, x2, y2, z2, x3, y3, z3,
    // Note: these rotations are relative to the camera
    if(dYaw!=0) {
        cosYaw = std::cos(dYaw);
        sinYaw = std::sin(dYaw);
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            x1 = (surf->vertices[0]*cosYaw)-(surf->vertices[2]*sinYaw);
            z1 = (surf->vertices[0]*sinYaw)+(surf->vertices[2]*cosYaw);
            x2 = (surf->vertices[3]*cosYaw)-(surf->vertices[5]*sinYaw);
            z2 = (surf->vertices[3]*sinYaw)+(surf->vertices[5]*cosYaw);
            x3 = (surf->vertices[6]*cosYaw)-(surf->vertices[8]*sinYaw);
            z3 = (surf->vertices[6]*sinYaw)+(surf->vertices[8]*cosYaw);
     
            surf->vertices[0] = x1;
            surf->vertices[2] = z1;
            surf->vertices[3] = x2;
            surf->vertices[5] = z2;
            surf->vertices[6] = x3;
            surf->vertices[8] = z3;

        }
        nposX = (posX*cos(dYaw))-(posZ*sin(dYaw));
        nposZ = (posX*sin(dYaw))+(posZ*cos(dYaw));
        posX = nposX;
        posZ = nposZ;
    }
    if(dPitch!=0) {
        cosPitch = std::cos(dPitch);
        sinPitch = std::sin(dPitch);
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            z1 = (surf->vertices[2]*cosPitch)-(surf->vertices[1]*sinPitch);
            y1 = (surf->vertices[2]*sinPitch)+(surf->vertices[1]*cosPitch);
            z2 = (surf->vertices[5]*cosPitch)-(surf->vertices[4]*sinPitch);
            y2 = (surf->vertices[5]*sinPitch)+(surf->vertices[4]*cosPitch);
            z3 = (surf->vertices[8]*cosPitch)-(surf->vertices[7]*sinPitch);
            y3 = (surf->vertices[8]*sinPitch)+(surf->vertices[7]*cosPitch);

            surf->vertices[1] = y1;
            surf->vertices[2] = z1;
            surf->vertices[4] = y2;
            surf->vertices[5] = z2;
            surf->vertices[7] = y3;
            surf->vertices[8] = z3;

        }
        nposZ = (posZ*cos(dPitch))-(posY*sin(dPitch));
        nposY = (posZ*sin(dPitch))+(posY*cos(dPitch));
        posZ = nposZ;
        posY = nposY;
    }
    if(dRoll!=0) {
        for(auto surf = mesh.begin(); surf != mesh.end(); ++surf) {
            // Rotate every vertice around the origin
            x1 = (surf->vertices[0]*cos(dRoll))-(surf->vertices[1]*sin(dRoll));
            y1 = (surf->vertices[0]*sin(dRoll))+(surf->vertices[1]*cos(dRoll));
            x2 = (surf->vertices[3]*cos(dRoll))-(surf->vertices[4]*sin(dRoll));
            y2 = (surf->vertices[3]*sin(dRoll))+(surf->vertices[4]*cos(dRoll));
            x3 = (surf->vertices[6]*cos(dRoll))-(surf->vertices[7]*sin(dRoll));
            y3 = (surf->vertices[6]*sin(dRoll))+(surf->vertices[7]*cos(dRoll));
                
            surf->vertices[0] = x1;
            surf->vertices[1] = y1;
            surf->vertices[3] = x2;
            surf->vertices[4] = y2;
            surf->vertices[6] = x3;
            surf->vertices[7] = y3;

        }
        nposX = (posX*cos(dRoll))-(posY*sin(dRoll));
        nposY = (posX*sin(dRoll))+(posY*cos(dRoll));
        posX = nposX;
        posY = nposY;
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

