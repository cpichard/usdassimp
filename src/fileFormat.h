#pragma once

#include "pxr/base/tf/staticTokens.h"
#include "pxr/pxr.h"
#include "pxr/usd/sdf/fileFormat.h"
#include <iosfwd>
#include <string>

// https://github.com/wetadigital/USDPluginExamples/tree/main/src/usdTriFileFormat

PXR_NAMESPACE_OPEN_SCOPE

// Allowing only the ply format for now

#define USDASSIMP_FILE_FORMAT_TOKENS ((Id, "assimp"))((Version, "1.0"))((Target, "usd"))((Extension, "ply"))

TF_DECLARE_PUBLIC_TOKENS(UsdAssimpFileFormatTokens, USDASSIMP_FILE_FORMAT_TOKENS);

TF_DECLARE_WEAK_AND_REF_PTRS(UsdAssimpFileFormat);

/// \class UsdAssimpFileFormat
///
///
class UsdAssimpFileFormat : public SdfFileFormat {
  public:
    // SdfFileFormat overrides.
    virtual bool CanRead(const std::string &file) const override;
    virtual bool Read(SdfLayer *layer, const std::string &resolvedPath, bool metadataOnly) const override;
    virtual bool ReadFromString(SdfLayer *layer, const std::string &str) const override;

    // We override Write methods so SdfLayer::ExportToString() etc, work.  We
    // don't support writing general Usd data back to OBJ files.  So
    // SdfLayer::Save() doesn't work, for example.
    virtual bool WriteToString(const SdfLayer &layer, std::string *str,
                               const std::string &comment = std::string()) const override;
    virtual bool WriteToStream(const SdfSpecHandle &spec, std::ostream &out, size_t indent) const override;

  protected:
    SDF_FILE_FORMAT_FACTORY_ACCESS;

    virtual ~UsdAssimpFileFormat();
    UsdAssimpFileFormat();

  private:
    bool _ReadFromStream(SdfLayer *layer, std::istream &input, bool metadataOnly, std::string *outErr) const;
};

PXR_NAMESPACE_CLOSE_SCOPE
