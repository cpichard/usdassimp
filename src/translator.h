#pragma once

#include <assimp/scene.h>           // Output data structure
#include "pxr/usd/sdf/layer.h"

PXR_NAMESPACE_USING_DIRECTIVE

bool TranslateAssimp(const aiScene *scene, SdfLayer *layer);
