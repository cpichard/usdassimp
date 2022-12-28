#include "fileFormat.h"
#include "pxr/pxr.h"

#include "pxr/usd/sdf/layer.h"
#include "pxr/usd/usd/usdaFileFormat.h"

#include "pxr/base/tf/fileUtils.h"
#include "pxr/base/tf/registryManager.h"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
//#include "stream.h"
//#include "streamIO.h"
#include "translator.h"

#include <fstream>
#include <string>

#include <iostream>

PXR_NAMESPACE_OPEN_SCOPE

using std::string;

TF_DEFINE_PUBLIC_TOKENS(UsdAssimpFileFormatTokens, USDASSIMP_FILE_FORMAT_TOKENS);

TF_REGISTRY_FUNCTION(TfType) { SDF_DEFINE_FILE_FORMAT(UsdAssimpFileFormat, SdfFileFormat); }

UsdAssimpFileFormat::UsdAssimpFileFormat()
    : SdfFileFormat(UsdAssimpFileFormatTokens->Id, UsdAssimpFileFormatTokens->Version, UsdAssimpFileFormatTokens->Target,
                    UsdAssimpFileFormatTokens->Id) {}

UsdAssimpFileFormat::~UsdAssimpFileFormat() {}

bool UsdAssimpFileFormat::CanRead(const string &filePath) const {
    // TODO: check to see if it looks like valid assimp data...
    return true;
}

bool UsdAssimpFileFormat::_ReadFromStream(SdfLayer *layer, std::istream &input, bool metadataOnly, string *outErr) const {
    return false;
}

bool UsdAssimpFileFormat::Read(SdfLayer *layer, const string &resolvedPath, bool metadataOnly) const {
    Assimp::Importer importer;

    // https://assimp-docs.readthedocs.io/en/v5.1.0/usage/use_the_lib.html
    // The documentation says the returned pointer is managed by assimp, no need to delete it
    const aiScene *scene = importer.ReadFile(resolvedPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                                               aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    if (!scene) {
        TF_RUNTIME_ERROR("Failed to read scene \"%s\"", resolvedPath.c_str());
        return false;
    }

    return TranslateAssimp(scene, layer);
}

bool UsdAssimpFileFormat::ReadFromString(SdfLayer *layer, const std::string &str) const { return false; }

bool UsdAssimpFileFormat::WriteToString(const SdfLayer &layer, std::string *str, const std::string &comment) const {
    // TODO: support for assimp writing instead of usda
    return SdfFileFormat::FindById(UsdUsdaFileFormatTokens->Id)->WriteToString(layer, str, comment);
}

bool UsdAssimpFileFormat::WriteToStream(const SdfSpecHandle &spec, std::ostream &out, size_t indent) const {
    // TODO: support for assimp writing instead of usda
    return SdfFileFormat::FindById(UsdUsdaFileFormatTokens->Id)->WriteToStream(spec, out, indent);
}

PXR_NAMESPACE_CLOSE_SCOPE
