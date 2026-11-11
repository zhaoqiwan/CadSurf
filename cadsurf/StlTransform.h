#pragma once

#include <array>
#include <cmath>

using StlPosition = std::array<double, 3>;

struct StlTransform
{
    // Column-major matrix for homogeneous column vectors and glMultMatrixd.
    std::array<double, 16> matrix = {{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }};

    static StlTransform Translation(const StlPosition& offset)
    {
        StlTransform result;
        for (int axis = 0; axis < 3; ++axis)
            result.matrix[12 + axis] = offset[axis];
        return result;
    }

    static StlTransform Scale(const StlPosition& center, double factor)
    {
        StlTransform result;
        for (int axis = 0; axis < 3; ++axis)
            result.matrix[5 * axis] = factor;
        return result.Around(center);
    }

    static bool Rotation(const StlPosition& center, const StlPosition& direction,
                         double degrees, StlTransform& result)
    {
        const double length = std::hypot(std::hypot(direction[0], direction[1]), direction[2]);
        if (!(length > 0.0) || !std::isfinite(length) || !std::isfinite(degrees))
            return false;

        const double x = direction[0] / length;
        const double y = direction[1] / length;
        const double z = direction[2] / length;
        const double radians = std::fmod(degrees, 360.0) * (std::acos(-1.0) / 180.0);
        const double c = std::cos(radians);
        const double s = std::sin(radians);
        const double t = 1.0 - c;
        StlTransform rotation;
        rotation.matrix = {{
            c + x*x*t,     y*x*t + z*s, z*x*t - y*s, 0,
            x*y*t - z*s,   c + y*y*t,   z*y*t + x*s, 0,
            x*z*t + y*s,   y*z*t - x*s, c + z*z*t,   0,
            0,             0,           0,           1
        }};
        result = rotation.Around(center);
        return true;
    }

    StlPosition Point(const StlPosition& point) const
    {
        StlPosition result;
        for (int row = 0; row < 3; ++row)
            result[row] = matrix[row] * point[0] + matrix[4 + row] * point[1]
                        + matrix[8 + row] * point[2] + matrix[12 + row];
        return result;
    }

    StlTransform operator*(const StlTransform& other) const
    {
        StlTransform result;
        for (int column = 0; column < 4; ++column)
        {
            for (int row = 0; row < 4; ++row)
            {
                double value = 0.0;
                for (int term = 0; term < 4; ++term)
                    value += matrix[4 * term + row] * other.matrix[4 * column + term];
                result.matrix[4 * column + row] = value;
            }
        }
        return result;
    }

private:
    StlTransform Around(const StlPosition& center) const
    {
        // Move the pivot to the origin, transform, then move it back.
        return Translation(center) * *this *
               Translation({{-center[0], -center[1], -center[2]}});
    }
};
