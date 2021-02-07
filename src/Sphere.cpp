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

    GenerateNormals();

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

std::vector<float> Sphere::GetNormals()
{
    std::vector<float> normOutput;

    // Converts normals vector of 3-arrays into a single-dimension array vector
    for (int i = 0; i < normals.size(); i++)
    {
        normOutput.push_back(normals[i][0]);
        normOutput.push_back(normals[i][1]);
        normOutput.push_back(normals[i][2]);
    }

    return normOutput;
}

std::vector<float> Sphere::GetVertNorms()
{
    std::vector<float> out;

    // Converts normals vector of 3-arrays into a single-dimension array vector
    assert(normals.size() == vertices.size());
    for (int i = 0; i < normals.size(); i++)
    {
        // Vertices
        out.push_back(vertices[i][0]);
        out.push_back(vertices[i][1]);
        out.push_back(vertices[i][2]);

        // Normals
        out.push_back(normals[i][0]);
        out.push_back(normals[i][1]);
        out.push_back(normals[i][2]);
    }

    return out;
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
            oldTriad[i] = AddVertex(oldVerts[tri[i]]).first; // Unchecked addition

        /*// Normals - INCOMPLETE
        ///Calculate two edge of the triangle
        //Vector3f edge1 = subtract(v0, v1);
        //Vector3f edge2 = subtract(v1, v2);
        

        //Calculate face normal
        //Vector3f normal = normalize(crossProduct(edge1, edge2));
        */

        // Gets the midpoint of each triangle edge
        //        o
        //      o   o
        //     o  o  o
        triangle.clear();
        triangle.push_back(MidPoint(oldTriad[0], oldTriad[1])); // Vertex 1
        triangle.push_back(MidPoint(oldTriad[1], oldTriad[2])); // Vertex 2
        triangle.push_back(MidPoint(oldTriad[2], oldTriad[0])); // Vertex 3

        // Adds the new vertices to the new list
        for (int j = 0; j < 3; j++)
            triad[j] = AddVertex(triangle[j]).first; // Unchecked addition
        
        // Pushes the four new triangles to the end of the new indices list
        tempTriad = { oldTriad[0], triad[0], triad[2] };
        indices.push_back(tempTriad);

        tempTriad = { oldTriad[1], triad[1], triad[0] };
        indices.push_back(tempTriad);

        tempTriad = { oldTriad[2], triad[2], triad[1] };
        indices.push_back(tempTriad);

        tempTriad = { triad[0], triad[1], triad[2] };
        indices.push_back(tempTriad);
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

    /*Hashmap code (OLD), leaving here for future work
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

std::array<float, 3> Sphere::FaceNormal(std::array<float, 3> v1, std::array<float, 3> v2, std::array<float, 3> v3)
{
    static const int x = 0;
    static const int y = 1;
    static const int z = 2;

    std::array<float, 3> vect1, vect2, out;

    // Finds The Vector Between 2 Points
    // Calculate The Vector From Point 1 To Point 0
    vect1[x] = v1[x] - v2[x];
    vect1[y] = v1[y] - v2[y];
    vect1[z] = v1[z] - v2[z];

    // Calculate The Vector From Point 2 To Point 1
    vect2[x] = v2[x] - v3[x];
    vect2[y] = v2[y] - v3[y];
    vect2[z] = v2[z] - v3[z];

    // Uses cross product to get surface normal
    out[x] = vect1[y] * vect2[z] - vect1[z] * vect2[y];
    out[y] = vect1[z] * vect2[x] - vect1[x] * vect2[z];
    out[z] = vect1[x] * vect2[y] - vect1[y] * vect2[x];

    //return Normalize(out);
    return out;
}

std::array<float, 3> Sphere::Normalize(std::array<float, 3> vector)
{
    float length = std::sqrtf(std::powf(vector[0], 2) + std::powf(vector[1], 2) + std::powf(vector[2], 2));

    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;

    return vector;
}

std::array<std::array<float, 3>, 3> Sphere::VertexNormal(std::array<float, 3> v1, std::array<float, 3> v2, std::array<float, 3> v3)
{
    return {v1, v2, v3};
}

void Sphere::GenerateNormals()
{
    if ( normals.size() > 0 )
        normals.clear();

    normals.resize(vertices.size());
    
    for ( auto tri : indices )
    {
        auto result = FaceNormal(vertices[tri[0]], vertices[tri[1]], vertices[tri[2]]);

        normals[tri[0]] = result;
        normals[tri[1]] = result;
        normals[tri[2]] = result;
    }

    // output result
    //for ( auto normal : normals )
    //    std::cout << "Normal: (" << normal[0] << ", " << normal[1] << ", " << normal[2] << ")." << std::endl;
}
