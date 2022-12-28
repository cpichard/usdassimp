#include "translator.h"
#include <iostream>
#include <pxr/base/vt/array.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/primSpec.h>
#include <pxr/usd/sdf/types.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/mesh.h>

#include "pxr/base/gf/range3f.h"

// Recursive function to build the mesh hierarchy
void BuildHierarchy(UsdStageRefPtr stage, const aiScene *scene, aiNode *node, SdfPath nodePath) {
    if (!scene)
        return;
    if (!node)
        return;
    //    // TODO: the scene also has a name, could it be the root name ?
    //    const bool isRoot = node->mParent == nullptr;
    //    if (isRoot) {
    //        std::cout << "scene name" << scene->mName.C_Str() << std::endl;
    //    }
    //
    //    std::cout << "node name" << node->mName.C_Str() <<std::endl;

    // The nodes might not be named, and some formats don't even have hierarchies but usd
    // needs a root node, so we need to create one if it is not in the assimp scene

    // auto nodePrim = stage->DefinePrim();
    if (node->mNumMeshes > 0) {
        // TODO: if there are vertices but no faces, we should use a UsdGeomPoints instead
        UsdGeomMesh usdMesh = UsdGeomMesh::Define(stage, nodePath.AppendChild(TfToken("Scene")));
        for (int i = 0; i < node->mNumMeshes; ++i) {
            // Get meshes
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            if (!mesh) {
                continue;
            }

            // Meshes can have a name, but it's not mandatory, if there is a name we should
            // create a new node for it
            if (mesh->mName.length != 0) {
                // Create a new node ??
                std::cout << "Node name:" << mesh->mName.C_Str() << std::endl;
            }

            if (mesh->HasPositions()) {
                UsdAttribute pointsAttr = usdMesh.CreatePointsAttr();
                VtArray<GfVec3f> pointsArray(mesh->mNumVertices);
                for (int i = 0; i < mesh->mNumVertices; ++i) {
                    const aiVector3D &point = mesh->mVertices[i];
                    pointsArray[i].Set(point.x, point.y, point.z);
                }
                pointsAttr.Set(pointsArray);
            }
            if (mesh->HasFaces()) {
                UsdAttribute facesIdxAttr = usdMesh.CreateFaceVertexIndicesAttr();
                UsdAttribute facesCntAttr = usdMesh.CreateFaceVertexCountsAttr();

                VtArray<int> facesIdxArray;
                VtArray<int> facesCntArray;
                // The following can surely be optimized to avoid too many allocations if
                // it proves too slow
                for (int i = 0; i < mesh->mNumFaces; ++i) {
                    const aiFace &face = mesh->mFaces[i];
                    facesCntArray.push_back(face.mNumIndices);
                    for (int j = 0; j < face.mNumIndices; ++j) {
                        facesIdxArray.push_back(face.mIndices[j]);
                    }
                }
                facesIdxAttr.Set(facesIdxArray);
                facesCntAttr.Set(facesCntArray);

                // TODO: when the numIndices is always the same for each faces, it there a way to
                // only write the numIndices only once ?
            }
        }
    }
}

bool TranslateAssimp(const aiScene *scene, SdfLayer *layerPtr) {
    //
    if (!scene || !layerPtr) {
        if (!scene) {
            TF_RUNTIME_ERROR("Assimp scene is null");
        }
        if (!layerPtr) {
            TF_RUNTIME_ERROR("Assimp target layer is empty");
        }
        return false;
    }
    // TODO: check scene->flags

    // Using the same method as in the usdObj example
    // Create an anonymous layer to populate.
    SdfLayerRefPtr rootLayer = SdfLayer::CreateAnonymous(".usd");

    // Create a UsdStage on this anonymous layer.
    // We could directly use a SdfLayer but the stage allows to use the API schemas
    // and it is easier to correctly create meshes (UsdGeomMesh), lights, cameras, etc. later on
    UsdStageRefPtr stage = UsdStage::Open(rootLayer);
    BuildHierarchy(stage, scene, scene->mRootNode, SdfPath::AbsoluteRootPath());

    // Finally copy/move the content to the target layer
    layerPtr->TransferContent(rootLayer);

    return true;
}
