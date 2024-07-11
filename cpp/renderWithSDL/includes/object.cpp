#include "object.hpp"

Object::Object(std::string objectFile, float inPosX, float inPosY, float inPosZ, float inYaw, float inPitch, float inRoll)
{
    
    mesh = createMeshFromFile(objectFile);
    
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

std::vector<Surface> Object::createMeshFromFile(std::string filename){

    // Used for reading files:
    std::string line;
    std::istringstream iss;
    std::string type;




    // Store materials from .mtl
    
    struct diffuseColor {
        float r,g,b;
    };
    struct material {
        std::string name;
        std::string map_Kd;
        diffuseColor dColor;
    };

    std::vector<material> materials;

    struct vertex {
        float x,y,z;
    };

    struct textureCoord {
        float u,v;
    };

    struct normal {
        float x,y,z;
    };

    struct spaceVert {
        float u,v,w;
    };

    struct face {
        int v1,v2,v3,vt1,vt2,vt3,vn1,vn2,vn3,vp1,vp2,vp3;
        material mat;
    };

    std::vector<vertex> vertices;
    std::vector<textureCoord> textureCoords;
    std::vector<normal> normals;
    std::vector<spaceVert> spaceVerts;
    

    // Read .obj
    
    std::ifstream objectFile;
    std::string filePath = "objects/"+filename+"/"+filename+".obj";
    
    objectFile.open(filePath);

    std::vector<Surface> returnMesh;

    if(objectFile.is_open()){


        material currentMaterial;

        std::string materialName;

        int faceCornerCout;
        
        while ( getline (objectFile,line) ) {
            // Read each line
            std::replace(line.begin(), line.end(), '/', ' ');
            iss.clear(); 
            iss.str(line);
            iss >> type;
            
            if(type == "mtllib"){
                // Read .mtl
                std::string mtlName;
                iss >> mtlName;
                std::ifstream materialsFile;
                std::string materialPath = "objects/"+filename+"/"+mtlName;

                materialsFile.open(materialPath);

                
                if(materialsFile.is_open()){
                    

                    std::string name = "";
                    std::string textureMap = "";
                    float diffuseColorR;
                    float diffuseColorG;
                    float diffuseColorB;


                    while ( getline (materialsFile,line) ) {
                        // Read each line
                        iss.clear(); 
                        iss.str(line);
                        iss >> type;

                        if(type == "newmtl"){
                            if(name == ""){
                                // If this is the first material to be added
                                iss >> name;
                            } else {
                                // If its not the first material add the previous info into the materials list
                                material newMaterial;
                                newMaterial.name = name;
                                newMaterial.dColor.r = diffuseColorR;
                                newMaterial.dColor.g = diffuseColorG;
                                newMaterial.dColor.b = diffuseColorB;
                                materials.push_back(newMaterial);
                                iss >> name;
                            }
                        }
                        
                        // Kd -> diffuse color
                        if(type == "Kd"){
                            iss >> diffuseColorR >> diffuseColorG >> diffuseColorB;
                        }

                        // map_Kd -> texture map
                        if(type == "map_Kd"){
                            iss >> textureMap; 
                        }

                    }
                    material newMaterial;
                    newMaterial.name = name;
                    newMaterial.map_Kd = textureMap;
                    newMaterial.dColor.r = diffuseColorR;
                    newMaterial.dColor.g = diffuseColorG;
                    newMaterial.dColor.b = diffuseColorB;
                    materials.push_back(newMaterial);


                    materialsFile.close();

                } else {
                    std::cout << "Could Not Open Materials File For Object: " << filename << "\n";
                    std::cout << "Could Not Find: "+materialPath+"\n";
                }

            }

            if(type == "v"){
                
                // Add vertex
                // x,y,z of new vertex in vertex list

                vertex newVertex;
                iss >> newVertex.x >> newVertex.y >> newVertex.z;

                vertices.push_back(newVertex);
                
            }

            if(type == "vt"){

                // Add texture coorinates
                // u,v of new texture coord in texture coords list
                textureCoord newTC;
                iss >> newTC.u >> newTC.v;

                textureCoords.push_back(newTC);

            }

            if(type == "usemtl"){
                iss >> materialName;
                // Equipt mtl file
                for(material materialSearch : materials){
                    
                    if(materialSearch.name == materialName){
                        // Set color of surface
                        currentMaterial = materialSearch;
                    }
                }
                if(currentMaterial.name!=materialName){
                    std::cout << "Object file trying to use material: " << materialName << " not found in .mtl file!";
                }
            }
            
            if(type == "f"){
                 
                // Add surface using the vertices created
                face newFace;

                std::string s;

                iss >> newFace.v1 >> newFace.vt1 >> newFace.vn1 >> newFace.v2 >> newFace.vt2 >> newFace.vn2 >> newFace.v3 >> newFace.vt3 >> newFace.vn3;

                std::cout << newFace.v1<<", "<<newFace.v2<<", "<<newFace.v3<<"\n";

                // Obj file starts vertex and other lists at 1 instead of 0 as we store them
                newFace.v1-=1;
                newFace.v2-=1;
                newFace.v3-=1;
                newFace.vt1-=1;
                newFace.vt2-=1;
                newFace.vt3-=1;

                // Create new surface
                Surface newSurface(
                        {vertices[newFace.v1].x,vertices[newFace.v1].y,vertices[newFace.v1].z,
                        vertices[newFace.v2].x,vertices[newFace.v2].y,vertices[newFace.v2].z,
                        vertices[newFace.v3].x,vertices[newFace.v3].y,vertices[newFace.v3].z},
                        {textureCoords[newFace.vt1].u,textureCoords[newFace.vt1].v,
                        textureCoords[newFace.vt2].u,textureCoords[newFace.vt2].v,
                        textureCoords[newFace.vt3].u,textureCoords[newFace.vt3].v},
                        currentMaterial.map_Kd);

                returnMesh.push_back(newSurface);

            }





        }
        objectFile.close();

    } else {
        std::cout << "Could Not Open Object: " << filename << "\n";
        std::cout << "Object must be in the objects folder and with the naming objects/<name>/<name>.obj\n";
    }

    return returnMesh;
}

std::vector<Surface> Object::getMesh(float cameraX,float cameraY,float cameraZ,float cameraPitch,float cameraYaw,float cameraRoll)
{
    // Returns the mesh of the object (lot of parts to this)
    // For each vertex apply object rotation ( do this later )
    // For each vertex apply object relative position translation
    std::vector<Surface> returnMesh = mesh;
    for(auto it = returnMesh.begin(); it != returnMesh.end(); ++it) {
        // Rotate object (still need to do this), add its position and subtract position of camera
        it->vertices[0] += posX-cameraX;
        it->vertices[1] += posY-cameraY;
        it->vertices[2] += posZ-cameraZ;
        it->vertices[3] += posX-cameraX;
        it->vertices[4] += posY-cameraY;
        it->vertices[5] += posZ-cameraZ;
        it->vertices[6] += posX-cameraX;
        it->vertices[7] += posY-cameraY;
        it->vertices[8] += posZ-cameraZ;

        // Rotate every vertice around the camera
        
        // Calculate sin and cos
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

Surface::Surface(std::array<float,9> inVerticies, std::array<float,6> inTextureCoords, std::string inTextureMap)
{
    // color in form r, g, b, alpha
    // verties in form x1,y1,z1,x2,y2,z3,x3,y3,z3
    // textureCoords in form u1,v1,u2,v2,u3,v3
    // textureMap in form of string file name (name of file inside of /textures/<obj name>/)
    vertices = inVerticies;
    textureCoords = inTextureCoords;
    textureMap = inTextureMap;
}

