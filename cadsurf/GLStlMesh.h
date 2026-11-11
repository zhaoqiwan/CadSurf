#pragma once

#include <memory>
#include "GLObject.h"
#include "StlReader.h"
#include "StlTransform.h"

class CGLStlMesh : public CGLObject
{
public:
    explicit CGLStlMesh(std::shared_ptr<const StlMesh> mesh);

    CGLObject* Copy() const override;
    void DefineDisplay() override;
    void Display(const GLDisplayMode& mode) override;
    void Hilight(const GLDisplayMode& mode) override;
    bool PrepareTransform(const StlTransform& change, StlTransform& result,
                          CBoundingBox& bounds) const;
    void SetTransform(const StlTransform& result, const CBoundingBox& bounds);
    StlTransform GetTransform() const { return *placement; }

private:
    CGLStlMesh(std::shared_ptr<const StlMesh> mesh,
               std::shared_ptr<StlTransform> placement, unsigned long objectId);
    void DrawTriangles() const;

    std::shared_ptr<const StlMesh> mesh;
    // The display object and its selection copy represent the same placement.
    std::shared_ptr<StlTransform> placement;
};
