#include "main.h"

#include <vector>
#include "framework/loader.h"

//Main initialization
void init(void) {
    initShaders();

    // int vao = genGrid();

    Cylinder cylinderMesh = Cylinder(20, 20, 1.0f);
    Loader* loader = new Loader();
    RawModel* model = loader->loadToVAO(cylinderMesh.vertices,
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
