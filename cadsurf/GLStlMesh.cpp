#include "stdafx.h"
#include "GLStlMesh.h"

#include <algorithm>
#include <limits>
#include <utility>

namespace
{
unsigned long nextStlMeshId = GLSTLMESH;
}

CGLStlMesh::CGLStlMesh(std::shared_ptr<const StlMesh> mesh)
    : CGLStlMesh(std::move(mesh), std::make_shared<StlTransform>(), nextStlMeshId++)
{
}

CGLStlMesh::CGLStlMesh(std::shared_ptr<const StlMesh> mesh,
                       std::shared_ptr<StlTransform> placement, unsigned long objectId)
    : mesh(std::move(mesh)), placement(std::move(placement))
{
    glObjType = GLSTLMESH;
    glObjID = objectId;
    itsRed = 192;
    itsGreen = 218;
    itsBlue = 245;
    itsBox.SetLimits(this->mesh->minimum[0], this->mesh->maximum[0],
                     this->mesh->minimum[1], this->mesh->maximum[1],
                     this->mesh->minimum[2], this->mesh->maximum[2]);
}

CGLObject* CGLStlMesh::Copy() const
{
    auto* copy = new CGLStlMesh(mesh, placement, glObjID);
    copy->itsBox = itsBox;
    copy->displayMode = displayMode;
    copy->itsRed = itsRed;
    copy->itsGreen = itsGreen;
    copy->itsBlue = itsBlue;
    return copy;
}

bool CGLStlMesh::PrepareTransform(const StlTransform& change, StlTransform& result,
                                  CBoundingBox& bounds) const
{
    result = change * *placement;
    const double limit = (std::numeric_limits<float>::max)();
    for (double value : result.matrix)
        if (!std::isfinite(value) || std::abs(value) > limit)
            return false;

    StlPosition low = {{limit, limit, limit}};
    StlPosition high = {{-limit, -limit, -limit}};
    for (int corner = 0; corner < 8; ++corner)
    {
        StlPosition point;
        for (int axis = 0; axis < 3; ++axis)
            point[axis] = (corner & (1 << axis)) ? mesh->maximum[axis] : mesh->minimum[axis];
        point = result.Point(point);
        for (int axis = 0; axis < 3; ++axis)
        {
            if (!std::isfinite(point[axis]) || std::abs(point[axis]) > limit)
                return false;
            low[axis] = (std::min)(low[axis], point[axis]);
            high[axis] = (std::max)(high[axis], point[axis]);
        }
    }

    bounds.SetLimits(low[0], high[0], low[1], high[1], low[2], high[2]);
    return true;
}

void CGLStlMesh::SetTransform(const StlTransform& result, const CBoundingBox& bounds)
{
    *placement = result;
    itsBox = bounds;
}

void CGLStlMesh::DrawTriangles() const
{
    glPushMatrix();
    glMultMatrixd(placement->matrix.data());
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : mesh->triangles)
    {
        glNormal3fv(triangle.normal.data());
        for (const auto& vertex : triangle.vertices)
            glVertex3fv(vertex.data());
    }
    glEnd();
    glPopMatrix();
}

void CGLStlMesh::DefineDisplay()
{
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT |
                 GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT | GL_LINE_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    if (displayMode == GLSHADED)
    {
        const GLfloat direction[] = { 0.35f, 0.55f, 1.0f, 0.0f };
        const GLfloat diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        const GLfloat ambient[] = { 0.68f, 0.68f, 0.68f, 1.0f };
        const GLfloat zeroLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, direction);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, zeroLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, zeroLight);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glShadeModel(GL_FLAT);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // Keep visible mesh lines slightly ahead of their own filled faces.
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glColor3ub(itsRed, itsGreen, itsBlue);
        DrawTriangles();
        glDisable(GL_POLYGON_OFFSET_FILL);

        glDisable(GL_LIGHTING);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glColor3ub(38, 42, 50);
        DrawTriangles();
    }
    else
    {
        if (displayMode == GLHLREMOVED)
        {
            GLfloat background[4];
            glGetFloatv(GL_COLOR_CLEAR_VALUE, background);
            glColor4fv(background);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f, 1.0f);
            DrawTriangles();
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3ub(itsRed, itsGreen, itsBlue);
        DrawTriangles();
    }

    glPopAttrib();
}

void CGLStlMesh::Display(const GLDisplayMode& mode)
{
    displayMode = mode;
    DefineDisplay();
}

void CGLStlMesh::Hilight(const GLDisplayMode&)
{
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3ub(0, 255, 255);
    DrawTriangles();
    glPopAttrib();
}
