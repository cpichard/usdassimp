
#include <pxr/usd/usd/stage.h>
#include <string>
int main(int argc, char **argv) {

    // Only 2 arguments on the command line
    auto stage1 = pxr::UsdStage::Open(argv[1]);
    if (!stage1) return 1;
    std::string stage1Str;
    stage1->ExportToString(&stage1Str);
    auto stage2 = pxr::UsdStage::Open(argv[2]);
    if (!stage2) return 1;
    std::string stage2Str;
    stage1->ExportToString(&stage2Str);
    return stage1Str!=stage2Str;    
}
