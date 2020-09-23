#include "Sphere.h"
#include <vector>
#include <iostream>

#include <glm/glm.hpp>

using namespace std;

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

    // Scale vertices to the radius
    //for (int i=0; i<vertices.size(); i++)
    //    vertices[i] *= radius;

    for (int i=0; i<=vertices.size()-3; i+=3)
    {
        float scale = std::sqrtf
        (
            vertices[i  ] * vertices[i  ] + 
            vertices[i+1] * vertices[i+1] + 
            vertices[i+2] * vertices[i+2] 
        );

        vertices[i  ] /= (radius * scale);
        vertices[i+1] /= (radius * scale);
        vertices[i+2] /= (radius * scale);
    }

    // Generates default indices
    indices = 
    {
        // 5 triangles connected to point 0
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,

        // 5 adjacent triangles
        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,

        // 5 triangles connected to point 3
        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        // 5 more adjacent triangles
        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1
    };
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
    // Mmmm triangle tesselation (make sure the vector has enough space)
    std::vector<unsigned int> newInds = {  };
    newInds.resize(indices.size());// * 3);

    // TODO: Add key mapping system (good) instead of comparing floats (bad)

    float triad[] = {0.0f, 0.0f, 0.0f};

    // Loop through each index triad/triangle
    for (int i=0; i<=(20*3)-3; i+=3)
    {
        // Gets the index of the midpoint of each triangle edge
        for (int j=0; j<3; j++)
        {
            triad[j] = MidPoint(indices[j], indices[( j + 1 ) % 3]);
        }

        // First new triangle
        newInds.push_back(indices[i]  );
        newInds.push_back(triad[0]    );
        newInds.push_back(triad[2]    );

        // Second new triangle
        newInds.push_back(indices[i+1]);
        newInds.push_back(triad[1]    );
        newInds.push_back(triad[0]    );

        // Third new triangle
        newInds.push_back(indices[i+2]);
        newInds.push_back(triad[2]    );
        newInds.push_back(triad[1]    );

        // Fourth new triangle
        newInds.push_back(triad[0]    );
        newInds.push_back(triad[1]    );
        newInds.push_back(triad[2]    );
    }

    indices = newInds;
}

int Sphere::MidPoint(int x, int y)
{
    // TODO: factor in the unit circle to this

    // NOTE: indices[x] doesn't follow this pattern:
    // 0    1    2    3    4    5    6    7    8
    // 0              1              2
    // so you'll need to adjust by multiplying each by 3


    // Calculate the new vertex's 3D position (assumption: indices[i]*3 points to vertices[i]'s x-coordinate value)
    x *= 3;
    y *= 3;
    float newVertex[] =
    {
        // vertex point 1:           vertex point 2:
        vertices[indices[x]    ] + vertices[indices[y]    ] / 2, // x position
        vertices[indices[x] + 1] + vertices[indices[y] + 1] / 2, // y position
        vertices[indices[x] + 2] + vertices[indices[y] + 2] / 2  // z position
    };

    // Scale the new vertices to the unit circle
    float scale = std::sqrtf
        (
            newVertex[0] * newVertex[0] + 
            newVertex[1] * newVertex[1] + 
            newVertex[2] * newVertex[2] 
        );
    newVertex[0] /= (radius * scale);
    newVertex[1] /= (radius * scale);
    newVertex[2] /= (radius * scale);

    // Checks if vertex exists already (might want to change data structure later to make this better)
    for (int j = 0; j <= vertices.size(); j+=3)
    {
        if 
        ( 
            vertices[j  ] == newVertex[0] && 
            vertices[j+1] == newVertex[1] &&
            vertices[j+2] == newVertex[2]
        )
            return j / 3;
    }
    
    std::cout << "Adding new vertex: (" << newVertex[0] << ", " << newVertex[1] << ", " << newVertex[2] << ")." << std::endl;

    // Add the new vertex's x, y, and z coordinates to the end of the list
    vertices.push_back(newVertex[0]);
    vertices.push_back(newVertex[1]);
    vertices.push_back(newVertex[2]);

    return vertices.size() - 3;
}

float Sphere::GetRadius()
{
    return radius;
}