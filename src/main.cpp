#include <assimp/scene.h>       // C++ importer interface
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags

int main(int argc, const char **argv) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        "obj.obj", aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                       aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    return EXIT_SUCCESS;
}
