#ifndef MARCHING_CUBES_HPP
#define MARCHING_CUBES_HPP

#include "Object.hpp"
#include <vector>
#include <map>

class MarchingCubes : public Object { 
public:
    MarchingCubes(unsigned int xSegs, unsigned int ySegs, unsigned int zSegs, float xDim, float yDim, float zDim, float* data, float surfaceLevel);
    ~MarchingCubes();
private:
    struct Vector3 {
        float x;
        float y;
        float z;

        Vector3 operator+(const Vector3 & other) const {
            return {x + other.x, y + other.y, z + other.z}; 
        }
    };

    struct IVector3 {
        unsigned int x;
        unsigned int y;
        unsigned int z;

        bool operator==(const IVector3& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator<(const IVector3& other) const {
            return x < other.x || (x == other.x && y < other.y) || (x == other.x && y == other.y && z < other.z);
        }
    };

    struct Vertex {
        Vector3 position;
        Vector3 normal;
        unsigned int usageCount;
        unsigned int index;
    };

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    MeshData march_cubes(unsigned int xSegs, unsigned int ySegs, unsigned int zSegs, float xUnit, float yUnit, float zUnit, float* data, float surfaceLevel);

    void fill_triangulations(const std::vector<std::pair<IVector3, IVector3>> & triangulation, std::map<std::pair<IVector3, IVector3>, MarchingCubes::Vertex> & vertexMap, std::vector<unsigned int> & indices, float* data, float surfaceLevel, float xSegs, float ySegs, float zSegs, float xUnit, float yUnit, float zUnit, unsigned int & globalIndex);

    Vector3 point_pair_to_position(std::pair<IVector3, IVector3> positionPair, Vector3 origin, float valueA, float valueB, float surfaceLevel, float xUnit, float yUnit, float zUnit);

    Vector3 triangle_normal(Vector3* positions);

    std::vector<float> m_bufferData;
    std::vector<unsigned int> m_indices;
};
#endif