#pragma once

#include <array>
#include <iosfwd>
#include <string>
#include <vector>

using StlVertex = std::array<float, 3>;

struct StlTriangle
{
    StlVertex normal;
    std::array<StlVertex, 3> vertices;
};

struct StlMesh
{
    std::vector<StlTriangle> triangles;
    StlVertex minimum;
    StlVertex maximum;
};

// The stream must be seekable and opened in binary mode.
bool ReadBinaryStl(std::istream& input, StlMesh& mesh, std::string& error);
