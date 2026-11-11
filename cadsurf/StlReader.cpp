#include "StlReader.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <istream>
#include <limits>
#include <utility>

namespace
{
constexpr std::uint64_t HeaderSize = 84;
constexpr std::uint64_t TriangleSize = 50;

std::uint32_t ReadUInt32(const unsigned char* bytes)
{
    return std::uint32_t(bytes[0]) |
           (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) |
           (std::uint32_t(bytes[3]) << 24);
}

bool ReadVertex(const unsigned char* bytes, StlVertex& vertex)
{
    static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
                  "Binary STL requires IEEE 754 single-precision coordinates");

    for (size_t axis = 0; axis < vertex.size(); ++axis)
    {
        const auto bits = ReadUInt32(bytes + axis * 4);
        std::memcpy(&vertex[axis], &bits, sizeof(bits));
        if (!std::isfinite(vertex[axis]))
            return false;
    }
    return true;
}
}

bool ReadBinaryStl(std::istream& input, StlMesh& mesh, std::string& error)
{
    input.seekg(0, std::ios::end);
    const std::streamoff size = input.tellg();
    input.seekg(80, std::ios::beg);

    unsigned char countBytes[4];
    if (size < static_cast<std::streamoff>(HeaderSize) ||
        !input.read(reinterpret_cast<char*>(countBytes), sizeof(countBytes)))
    {
        error = "The binary STL header could not be read.";
        return false;
    }

    const auto count = ReadUInt32(countBytes);
    if (count == 0 || HeaderSize + TriangleSize * count != static_cast<std::uint64_t>(size))
    {
        error = "Expected a nonempty binary STL with a valid triangle count and file size.";
        return false;
    }

    StlMesh result;
    result.triangles.reserve(count);
    result.minimum.fill(std::numeric_limits<float>::max());
    result.maximum.fill(std::numeric_limits<float>::lowest());
    for (std::uint32_t index = 0; index < count; ++index)
    {
        unsigned char record[TriangleSize];
        if (!input.read(reinterpret_cast<char*>(record), sizeof(record)))
        {
            error = "A binary STL triangle record could not be read.";
            return false;
        }

        StlTriangle triangle;
        if (!ReadVertex(record, triangle.normal))
        {
            error = "A binary STL normal is not finite.";
            return false;
        }
        for (size_t corner = 0; corner < triangle.vertices.size(); ++corner)
        {
            auto& vertex = triangle.vertices[corner];
            if (!ReadVertex(record + 12 * (corner + 1), vertex))
            {
                error = "A binary STL vertex is not finite.";
                return false;
            }
            for (size_t axis = 0; axis < vertex.size(); ++axis)
            {
                result.minimum[axis] = std::min(result.minimum[axis], vertex[axis]);
                result.maximum[axis] = std::max(result.maximum[axis], vertex[axis]);
            }
        }
        result.triangles.push_back(triangle);
    }

    mesh = std::move(result);
    error.clear();
    return true;
}
