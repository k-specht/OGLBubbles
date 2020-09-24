#include "Sphere.h"
#include <vector>
#include <iostream>
#include <array>

#include <glm/glm.hpp>
#include <map>

using namespace std;
using Map = std::map<std::pair<unsigned int, unsigned int>, unsigned int>;

Sphere::Sphere(float r)
{
    // Generates default vertices TODO: change this to accept radius
    radius = r;
    float t = (1.0 + std::sqrtf(5.0)) / 2.0;

    // Fix the radius so each side is of length sqrt(t^2 +1)
    radius /= sqrt( t*t + 1.0 );

    // Creates the original icosahedron 
    // (see the wikipedia page of icosahedron for details on coordinates)
    vertices.resize(12);
    std::array<float,3> element;
    for (int i = 0; i < 12; i++)
    {
        if (i < 4)
        {
            element = 
            {
                (i % 2 == 0) ? -1.0f : 1.0f,
                (i <= 1) ? t : -1.0f * t,
                0.0f
            };
            vertices[i] = element;
        }
        else if ( i < 8 )
        {
            element = 
            {
                0.0f,
                (i % 2 == 0) ? -1.0f : 1.0f,
                (i <= 5) ? t : -1.0f * t
            };
            vertices[i] = element;
        }
        else
        {
            element = 
            {
                (i <= 9) ? t : -1.0f * t,
                0.0f,
                (i % 2 == 0) ? -1.0f : 1.0f
            };
            vertices[i] = element;
        }
    }

    // Scale vertices to the radius

    for (int i = 0; i <= vertices.size(); i++)
    {
        float scale = std::sqrtf
        (
            vertices[i][0] * vertices[i][0] +
            vertices[i][1] * vertices[i][1] +
            vertices[i][2] * vertices[i][2]
        );

        vertices[i][0] /= (radius * scale);
        vertices[i][1] /= (radius * scale);
        vertices[i][2] /= (radius * scale);
    }

    // Generates default indices
    indices.resize(20);
    indices = 
    {
        // 5 triangles connected to point 0
        {0u, 11u, 5u},
        {0u, 5u, 1u},
        {0u, 1u, 7u},
        {0u, 7u, 10u},
        {0u, 10u, 11u},

        // 5 adjacent triangles
        {1u, 5u, 9u},
        {5u, 11u, 4u},
        {11u, 10u, 2u},
        {10u, 7u, 6u},
        {7u, 1u, 8u},

        // 5 triangles connected to point 3
        {3u, 9u, 4u},
        {3u, 4u, 2u},
        {3u, 2u, 6u},
        {3u, 6u, 8u},
        {3u, 8u, 9u},

        // 5 more adjacent triangles
        {4u, 9u, 5u},
        {2u, 4u, 11u},
        {6u, 2u, 10u},
        {8u, 6u, 7u},
        {9u, 8u, 1u}
    };

    counter = 0;
}
std::vector<unsigned int> Sphere::GetIndices()
{
    std::vector<unsigned int> indOutput;
    //indOutput.resize(indices.size() * 3);

    for (int i = 0; i < indices.size(); i++)
    {
        indOutput.push_back(indices[i][0]);
        indOutput.push_back(indices[i][1]);
        indOutput.push_back(indices[i][2]);
    }

    return indOutput;
}

std::vector<float> Sphere::GetVertices()
{
    std::vector<float> vertOutput;
    //vertOutput.resize(vertices.size() * 3);

    for (int i = 0; i < vertices.size(); i++)
    {
        vertOutput.push_back(vertices[i][0]);
        vertOutput.push_back(vertices[i][1]);
        vertOutput.push_back(vertices[i][2]);
    }

    return vertOutput;
}

void Sphere::Divide()
{
    // Creates a new indices set (instead of keeping the old, larger lines)
    std::vector<std::array<unsigned int,3>> newInds = {  };
    newInds.resize(indices.size());// * 3);

    // A map of vertex pairs with a unique id; allows better comparison than float == float
    Map* map = new Map();

    unsigned int triad[] = {0, 0, 0};

    // Loop through each index triad/triangle
    for (auto tri : indices)
    {
        // Gets the index of the midpoint of each triangle edge
        //        o
        //      o   o
        //     o  o  o
        triad[0] = MidPoint(map, tri[0], tri[1]);
        triad[1] = MidPoint(map, tri[1], tri[2]);
        triad[2] = MidPoint(map, tri[2], tri[0]);

        // Pushes the four new triangles to the end of the new indices list
        newInds.push_back({ tri[0],   triad[0], triad[2] });
        newInds.push_back({ tri[1],   triad[1], triad[0] });
        newInds.push_back({ tri[2],   triad[2], triad[1] });
        newInds.push_back({ triad[0], triad[1], triad[2] });
    }

    // Might want to check that this does the right thing...
    indices = newInds;
}

unsigned int Sphere::MidPoint(Map* map, unsigned int x, unsigned int y)
{
    // NOTE: indices[x] doesn't follow this pattern:
    // 0    1    2    3    4    5    6    7    8
    // 0              1              2
    // so you'll need to adjust by multiplying each by 3
    x *= 3u;
    y *= 3u;

    // Creates a unique key out of the two entered values
    Map::key_type key (x, y);

    // Ensures that two keys will be equal even if the vertex order is swapped
    if (key.first > key.second)
    {
        std::swap(key.first, key.second);
    }

    // Check if vertex pair was inserted, if so create a new one since it's new
    std::pair<Map::iterator, bool> inserted = map->insert({key, counter});

    if ( inserted.second )
    {
        std::array<float,3> newVertex = {0.0f, 0.0f, 0.0f};

        // Calculate the new vertex's 3D position
        for (int i = 0; i < 3; i++)
            newVertex[i] = (vertices[x][i] + vertices[y][i]) / 2.0f;

        // Scale the new vertices to the unit circle (normalization)
        float scale = std::sqrtf
            (
                newVertex[0] * newVertex[0] + 
                newVertex[1] * newVertex[1] + 
                newVertex[2] * newVertex[2] 
            );
        
        for (int i = 0; i < 3; i++)
            newVertex[i] /= (radius * scale);

        // Add the new vertex to the vertices list
        vertices.push_back(newVertex);
        counter++;
    }

    // Returns the iterator's map id of the (maybe new) vertex
    return inserted.first->second;
}

float Sphere::GetRadius()
{
    return radius;
}