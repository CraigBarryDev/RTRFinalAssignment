#include "main.h"

#include <vector>
#include "framework/loader.h"
std::vector<GLfloat> positions;
std::vector<GLfloat> normals;
std::vector<GLuint> indices;

//Push the components of a 3 dimensional vector to a dyanmic float array
void pushVec3(std::vector<GLfloat>* arr, glm::vec3 vec) {
    arr->push_back(vec.x);
    arr->push_back(vec.y);
    arr->push_back(vec.z);   
}

int genGrid() {
    const float amplitude = 0.25f;
    const float frequency = 2.0f * glm::pi<float>();
    const float phase = 0.25f;
    const float normalSize = 0.05f;

    int nTilesX = 8;
    int nTilesZ = 8;
    float gridSizeX = 1.0f;
    float gridSizeZ = 1.0f;
    float stepSizeX = (2.0 * gridSizeX) / nTilesX;
    float stepSizeZ = (2.0 * gridSizeZ) / nTilesZ;
    float time = 0.0f;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 normalFromPosition;

    positions.clear();
    normals.clear();
    indices.clear();

    for(int j = 0; j <= nTilesZ; j++) {
        for(int i = 0; i <= nTilesX; i++) {
            //Calculate the x/y/z positions
            position.x = -gridSizeX + i * stepSizeX;
            //Y position is calculated as a sine wave
            position.y = amplitude * (sinf(position.x * frequency + phase * time) + 
                sinf(position.z * frequency + phase * time));
            position.z = -gridSizeZ + j * stepSizeZ;
            //Calculate the normal for the given x/y/z position
            normal.x = -amplitude * frequency * cosf(position.x * frequency + phase * time);
            normal.y = 1.0f;
            normal.z = -amplitude * frequency * cosf(position.z * frequency + phase * time);
            //Normalize the normal
            normal = glm::normalize(normal);
            //Add the positions and normals to their corresponding lists
            pushVec3(&positions, position);
            pushVec3(&normals, normal);
        }
    }

    //The number of positions to increment to get to the next position on the z axis
    int width = nTilesX + 1;
    //Iterate through the tiles in the z direction
    for(int j = 0; j < nTilesZ; j++) {
        //Iterate through the tiles in the x direction
        for(int i = 0; i < nTilesX; i++) {
            //The coordinate is converted to an index in the 1D array of positions
            int index = i + j * width;
            //Winding order and starting index are important
            indices.push_back(index);
            indices.push_back(index + width);
            indices.push_back(index + width + 1);
            indices.push_back(index + 1);
        }
    }

    Loader* loader = new Loader();
    int vao = loader->loadToVAO(positions, normals, indices);
    delete loader;
    return vao;
}

//Main initialization
void init(void) {
    initShaders();

    // int vao = genGrid();

    Cylinder cylinderMesh = Cylinder(20, 20, 1.0f);
    Loader* loader = new Loader();
    int vao = loader->loadToVAO(cylinderMesh.vertices,
        cylinderMesh.normals, cylinderMesh.indices);
}

//Update function called before each draw call to update program state
void update(void) {
    handleMouse();
    handleKeyboard();
}

//Called every frame to render objects before the buffers are swapped
void display(void) {

}

//Called when the application ends to deallocate memory
void cleanUp(void) {

}
