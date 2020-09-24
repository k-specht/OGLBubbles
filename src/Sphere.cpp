#include "Sphere.h"
#include <vector>
#include <iostream>

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

    vertices =
    {
        // x        y         z        r     b     g
        // First orthogonal rectangle
        -1.0f,     t,        0.0f,   //  1.0f, 0.0f, 0.0f,
         1.0f,     t,        0.0f,   //  1.0f, 0.0f, 0.0f,
        -1.0f,    -1.0f * t, 0.0f,   //  1.0f, 0.0f, 0.0f,
         1.0f,    -1.0f * t, 0.0f,   //  1.0f, 0.0f, 0.0f,

        // Second orthogonal rectangle
         0.0f,    -1.0f,     t,       // 0.0f, 1.0f, 0.0f,
         0.0f,     1.0f,     t,       // 0.0f, 1.0f, 0.0f,
         0.0f,    -1.0f,    -1.0f * t,// 0.0f, 1.0f, 0.0f,
         0.0f,     1.0f,    -1.0f * t,// 0.0f, 1.0f, 0.0f,

        // Third orthogonal rectangle
         t,        0.0f,    -1.0f,//     0.0f, 0.0f, 1.0f,
         t,        0.0f,     1.0f,//     0.0f, 0.0f, 1.0f,
        -1.0f * t, 0.0f,    -1.0f,//     0.0f, 0.0f, 1.0f,
        -1.0f * t, 0.0f,     1.0f//,     0.0f, 0.0f, 1.0f
    };

    /*vV = 
    {
        // x        y         z        r     b     g
        // First orthogonal rectangle
        {-1.0f,     t,        0.0f},   //  1.0f, 0.0f, 0.0f,
        { 1.0f,     t,        0.0f},   //  1.0f, 0.0f, 0.0f,
        {-1.0f,    -1.0f * t, 0.0f},   //  1.0f, 0.0f, 0.0f,
        { 1.0f,    -1.0f * t, 0.0f},   //  1.0f, 0.0f, 0.0f,

        // Second orthogonal rectangle
        { 0.0f,    -1.0f,     t},       // 0.0f, 1.0f, 0.0f,
        { 0.0f,     1.0f,     t},       // 0.0f, 1.0f, 0.0f,
        { 0.0f,    -1.0f,    -1.0f * t},// 0.0f, 1.0f, 0.0f,
        { 0.0f,     1.0f,    -1.0f * t},// 0.0f, 1.0f, 0.0f,

        // Third orthogonal rectangle
        { t,        0.0f,    -1.0f},//     0.0f, 0.0f, 1.0f,
        { t,        0.0f,     1.0f},//     0.0f, 0.0f, 1.0f,
        {-1.0f * t, 0.0f,    -1.0f},//     0.0f, 0.0f, 1.0f,
        {-1.0f * t, 0.0f,     1.0f}//,     0.0f, 0.0f, 1.0f
    };*/

    // Scale vertices to the radius
    //for (int i=0; i<vertices.size(); i++)
    //    vertices[i] *= radius;

    for (int i = 0; i <= vertices.size() - 3; i+=3)
    {
        float scale = std::sqrtf
        (
            vertices[i    ] * vertices[i    ] +
            vertices[i + 1] * vertices[i + 1] +
            vertices[i + 2] * vertices[i + 2]
        );

        vertices[i]     /= (radius * scale);
        vertices[i + 1] /= (radius * scale);
        vertices[i + 2] /= (radius * scale);
    }

    // Generates default indices
    indices = 
    {
        // 5 triangles connected to point 0
        0u, 11u, 5u,
        0u, 5u, 1u,
        0u, 1u, 7u,
        0u, 7u, 10u,
        0u, 10u, 11u,

        // 5 adjacent triangles
        1u, 5u, 9u,
        5u, 11u, 4u,
        11u, 10u, 2u,
        10u, 7u, 6u,
        7u, 1u, 8u,

        // 5 triangles connected to point 3
        3u, 9u, 4u,
        3u, 4u, 2u,
        3u, 2u, 6u,
        3u, 6u, 8u,
        3u, 8u, 9u,

        // 5 more adjacent triangles
        4u, 9u, 5u,
        2u, 4u, 11u,
        6u, 2u, 10u,
        8u, 6u, 7u,
        9u, 8u, 1u
    };

    counter = 0;
}
std::vector<unsigned int> Sphere::GetIndices()
{
    return indices;
}

std::vector<float> Sphere::GetVertices()
{
    return vertices;
}

void Sphere::Divide()
{
    // Creates a new indices set (instead of keeping the old, larger lines)
    std::vector<unsigned int> newInds = {  };
    newInds.resize(indices.size());// * 3);

    // A map of vertex pairs with a unique id; allows better comparison than float == float
    Map* map = new Map();

    float triad[] = {0.0f, 0.0f, 0.0f};

    // Loop through each index triad/triangle
    for (int i = 0; i <= indices.size() - 3; i += 3)
    {
        // Gets the index of the midpoint of each triangle edge
        triad[0] = MidPoint(map, indices[i    ], indices[i + 1]);
        triad[1] = MidPoint(map, indices[i + 1], indices[i + 2]);
        triad[2] = MidPoint(map, indices[i + 2], indices[i    ]);

        /*The triangles themselves seem fine whatttt
        std::cout << "Triangle " << i   << ": (" << vertices[indices[i  ]] << ", " << vertices[triad[0]] << ", " << vertices[triad[2]] << ")." << std::endl;
        std::cout << "Triangle " << i+1 << ": (" << vertices[indices[i+1]] << ", " << vertices[triad[1]] << ", " << vertices[triad[0]] << ")." << std::endl;
        std::cout << "Triangle " << i+2 << ": (" << vertices[indices[i+2]] << ", " << vertices[triad[2]] << ", " << vertices[triad[1]] << ")." << std::endl;
        std::cout << "Triangle " << "X-" << i / 3 << ": (" << vertices[triad[0    ]] << ", " << vertices[triad[1]] << ", " << vertices[triad[2]] << ")." << std::endl;
        */

        // Pushes 1st new triangle
        newInds.push_back(indices[i]  );
        newInds.push_back(triad[0]    );
        newInds.push_back(triad[2]    );

        // Pushes 2nd new triangle
        newInds.push_back(indices[i+1]);
        newInds.push_back(triad[1]    );
        newInds.push_back(triad[0]    );

        // Pushes 3rd new triangle
        newInds.push_back(indices[i+2]);
        newInds.push_back(triad[2]    );
        newInds.push_back(triad[1]    );

        // Pushes 4th new triangle
        newInds.push_back(triad[0]    );
        newInds.push_back(triad[1]    );
        newInds.push_back(triad[2]    );
    }

    indices = newInds;
}

int Sphere::MidPoint(Map* map, unsigned int x, unsigned int y)
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
        // Calculate the new vertex's 3D position
        float newVertex[] =
        {
            // vertex point 1:           vertex point 2:
            (vertices[x    ] + vertices[y    ]) / 2, // x position
            (vertices[x + 1] + vertices[y + 1]) / 2, // y position
            (vertices[x + 2] + vertices[y + 2]) / 2  // z position
        };

        // Scale the new vertices to the unit circle (normalization)
        float scale = std::sqrtf
            (
                newVertex[0] * newVertex[0] + 
                newVertex[1] * newVertex[1] + 
                newVertex[2] * newVertex[2] 
            );
        newVertex[0] /= (radius * scale);
        newVertex[1] /= (radius * scale);
        newVertex[2] /= (radius * scale);

        // Add the new vertex's x, y, and z coordinates to the end of the list
        vertices.push_back(newVertex[0]);
        vertices.push_back(newVertex[1]);
        vertices.push_back(newVertex[2]);
        counter++;
    }

    // Returns the iterator's map id of the (maybe new) vertex
    return inserted.first->second;
}

float Sphere::GetRadius()
{
    return radius;
}