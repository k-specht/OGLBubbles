#include "Sphere.hpp"

#include <vector>
#include <iostream>
#include <array>
#include <map>

#include <glm/glm.hpp>

#include "OGLBLOG.hpp"

using namespace glm;
//using Map = std::map<std::pair<unsigned int, unsigned int>, unsigned int>;

Sphere::Sphere(float r)
{
    // Generates default vertices
    radius  = r;
    float t = (1.0 + std::sqrtf(5.0)) / 2.0;

    // Fix the radius so each side is of length sqrt(t^2 +1)
    radius /= sqrt( t * t + 1.0 );

    // Creates the original icosahedron (aka ternary operator heaven)
    // (see the wikipedia page of icosahedron for details on coordinates)
    vertices.resize(12);
    std::array<float,3> element;
    for (int i = 0; i < 12; i++)
    {
        if ( i < 4 )
        {
            element = 
            {
                ( i % 2 == 0 ) ? -1.0f :  1.0f,
                ( i     <= 1 ) ? t     : -1.0f * t,
                0.0f
            };
            vertices[i] = element;
        }
        else if ( i < 8 )
        {
            element = 
            {
                0.0f,
                ( i % 2 == 0 ) ? -1.0f :  1.0f,
                ( i     <= 5 ) ? t     : -1.0f * t
            };
            vertices[i] = element;
        }
        else
        {
            element = 
            {
                ( i     <= 9 ) ? t     : -1.0f * t,
                0.0f,
                ( i % 2 == 0 ) ? -1.0f :  1.0f
            };
            vertices[i] = element;
        }
    }

    // Scale vertices to the radius
    for (int i = 0; i <= vertices.size(); i++)
    {
        float scale = radius / std::sqrtf
        (
            (vertices[i][0] * vertices[i][0]) +
            (vertices[i][1] * vertices[i][1]) +
            (vertices[i][2] * vertices[i][2])
        );

        vertices[i][0] *= scale;
        vertices[i][1] *= scale;
        vertices[i][2] *= scale;
    }

    // Generates default indices
    indices.resize(20);
    indices = 
    {
        // 5 triangles connected to point 0
        {0u, 11u, 5u },
        {0u, 5u,  1u },
        {0u, 1u,  7u },
        {0u, 7u,  10u},
        {0u, 10u, 11u},

        // 5 adjacent triangles
        {1u,  5u,  9u},
        {5u,  11u, 4u},
        {11u, 10u, 2u},
        {10u, 7u,  6u},
        {7u,  1u,  8u},

        // 5 triangles connected to point 3
        {3u, 9u, 4u},
        {3u, 4u, 2u},
        {3u, 2u, 6u},
        {3u, 6u, 8u},
        {3u, 8u, 9u},

        // 5 more adjacent triangles
        {4u, 9u, 5u },
        {2u, 4u, 11u},
        {6u, 2u, 10u},
        {8u, 6u, 7u },
        {9u, 8u, 1u }
    };

    counter = 11;
}

std::vector<unsigned int> Sphere::GetIndices()
{
    std::vector<unsigned int> indOutput;

    // Converts indices vector of 3-arrays into a single-dimension array vector
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

    // Converts vertices vector of 3-arrays into a single-dimension array vector
    for (int i = 0; i < vertices.size(); i++)
    {
        vertOutput.push_back(vertices[i][0]);
        vertOutput.push_back(vertices[i][1]);
        vertOutput.push_back(vertices[i][2]);
    }

    return vertOutput;
}

void Sphere::Divide(int divisions)
{
    for (int i = 0; i < divisions; i++)
        Subdivision();
}

void Sphere::Subdivision()
{
    // Creates a new indices set (instead of keeping the old, larger lines)
    std::vector<std::array<unsigned int, 3>> oldInds  = indices;
    std::vector<std::array<float,        3>> oldVerts = vertices;
    indices.clear();
    vertices.clear();

    // A map of vertex pairs with a unique id; allows better comparison than float == float (currently unchecked)
    //Map map;
    std::array<unsigned int, 3> oldTriad  = {0u, 0u, 0u};
    std::array<unsigned int, 3> triad     = {0u, 0u, 0u};
    std::array<unsigned int, 3> tempTriad = {0u, 0u, 0u};
    std::vector<std::array<float, 3>> triangle;

    // Loop through each index triad/triangle
    for (auto tri : oldInds)
    {
        // Adds the old vertices/indices to the new list (for indexing purposes)
        for (int i = 0; i < 3; i++)
        {
            //auto res = AddVertex(triangle[j]);
            oldTriad[i] = AddVertex(oldVerts[tri[i]]/*, map*/).first; // Unchecked addition
        }

        // Gets the midpoint of each triangle edge
        //        o
        //      o   o
        //     o  o  o
        triangle.clear();
        triangle.push_back(MidPoint(oldTriad[0], oldTriad[1])); // Vertex 1
        triangle.push_back(MidPoint(oldTriad[1], oldTriad[2])); // Vertex 2
        triangle.push_back(MidPoint(oldTriad[2], oldTriad[0])); // Vertex 3

        // We're done with the old triangle indices, so we set index to point to the first new one
        //index += 3u;

        // Adds the new vertices to the new list
        for (int j = 0; j < 3; j++)
        {
            //auto res = AddVertex(triangle[j]);
            triad[j] = AddVertex(triangle[j]/*, map*/).first; // Unchecked addition
        }
        
        // Pushes the four new triangles to the end of the new indices list
        tempTriad = { oldTriad[0], triad[0], triad[2] };
        indices.push_back(tempTriad);

        tempTriad = { oldTriad[1], triad[1], triad[0] };
        indices.push_back(tempTriad);

        tempTriad = { oldTriad[2], triad[2], triad[1] };
        indices.push_back(tempTriad);

        tempTriad = { triad[0], triad[1], triad[2] };
        indices.push_back(tempTriad);
        //index += 3u;
    }
}

std::pair<unsigned int, bool> Sphere::AddVertex(std::array<float,3> vertex/*, Map map*/)
{
    bool         hasV = false;
    unsigned int vInd = 0u;

    // Find vertex if it exists
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] == vertex)
        {
            hasV = true;
            vInd = static_cast<unsigned int>(i);
            break;
        }
    }

    // Add the vertex if it doesn't exist (possible source for index errors)
    if ( !hasV )
    {
        vertices.push_back(vertex);
        vInd = vertices.size() - 1;
    }

    return std::make_pair(vInd, hasV);

    /*
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
    std::pair<Map::iterator, bool> inserted = map->insert({key, vertices.size()});

    // Checks the boolean value ^
    if ( inserted.second )
    {
        std::array<float,3> newVertex = {0.0f, 0.0f, 0.0f};

        // Calculate the new vertex's 3D position
        for (int i = 0; i < 3; i++)
            newVertex[i] = (vertices[x][i] + vertices[y][i]) / 2.0f;

        // Scale the new vertices to the unit circle (normalization)
        float scale = radius / std::sqrtf
            (
                newVertex[0] * newVertex[0] + 
                newVertex[1] * newVertex[1] + 
                newVertex[2] * newVertex[2] 
            );
        
        for (int i = 0; i < 3; i++)
            newVertex[i] *= scale;

        // Add the new vertex to the vertices list
        vertices.push_back(newVertex);
        counter++;
    }

    // Returns the iterator's map id of the (maybe new) vertex
    return inserted.first->second;*/
}

std::array<float,3> Sphere::MidPoint(unsigned int x, unsigned int y)
{
    std::array<float,3> newVertex = {0.0f, 0.0f, 0.0f};

    // Calculate the new vertex's 3D position (TODO: assert check bounds here)
    for (int i = 0; i < 3; i++)
        newVertex[i] = (vertices[x][i] + vertices[y][i]) / 2.0f;

    // Scale the new vertices to the unit circle (normalization)
    float scale = radius / std::sqrtf
        (
            (newVertex[0] * newVertex[0]) + 
            (newVertex[1] * newVertex[1]) + 
            (newVertex[2] * newVertex[2]) 
        );
    
    for (int i = 0; i < 3; i++)
        newVertex[i] *= scale;

    // Returns the new vertex
    return newVertex;
}

float Sphere::GetRadius()
{
    return radius;
}

void Sphere::Collision(std::array<float,3> vertex, float magnitude)
{
    // Calculate vertex's unit vector to get pure direction
    float mag = std::sqrt(std::pow(vertex[0], 2) + std::pow(vertex[1], 2) + std::pow(vertex[2], 2));
    vertex[0] /= mag;
    vertex[1] /= mag;
    vertex[2] /= mag;
    //std::cout << "Vertex: [" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "]." << std::endl;

    // Apply a fraction of the collision magnitude based on distance to vertex
    for (int i = 0; i < vertices.size(); i++)
    {
        float vMag   = std::sqrt(std::pow(vertices[i][0], 2) + std::pow(vertices[i][1], 2) + std::pow(vertices[i][2], 2));
        float euDist = std::sqrt(std::pow(vertices[i][0] / vMag - vertex[0], 2) + std::pow(vertices[i][1] / vMag - vertex[1], 2) + std::pow(vertices[i][2] / vMag - vertex[2], 2));
        //std::cout<< "Dist: [" << euDist << "]." << std::endl;
        //if (euDist > 2.0f || euDist < 0.5f) euDist = 1.0f;
        float fraction = 1.0f + (euDist / 10);

        for (int j = 0; j < 3; j++)
        {
            vertices[i][j] *= (euDist * (1 + magnitude));
        }
    }
}

std::array<float,3> Sphere::FindCenter()
{
    std::array<float,3> center = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < vertices.size(); i++)
    {
        center[0] += vertices[i][0];
        center[1] += vertices[i][1];
        center[2] += vertices[i][2];
    }

    center[0] /= vertices.size();
    center[1] /= vertices.size();
    center[2] /= vertices.size();

    return center;
}