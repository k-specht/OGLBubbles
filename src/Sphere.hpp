#ifndef SPHERE
#define SPHERE

#include <vector>
#include <map>
#include <array>
#include <glm/glm.hpp>

/**
 *  A class that represents a Spherical drawable.
 *  Contains member functions for icosahedron generation and spherical division.
 */
class Sphere
{
    public:
        /**
         *  Initializes a Sphere generated using icosahedron division.
         *  @param radius - The desired radius of this spherical icosahedron.
         */
        Sphere(float radius);

        /**
         *  A basic copy operation on the Sphere object.
         *  Copying an object doesn't make sense, so we just delete it.
         */
        Sphere(const Sphere&) = delete;

        /**
         *  A basic move operation on the Sphere object.
         *  Moving an object doesn't make much sense, so we just delete it.
         */
        Sphere& operator=(const Sphere&) = delete;

        /** Deconstructor for the Sphere object. */
        ~Sphere() { };

        /** Retrieves the indices generated by this Sphere object. */
        std::vector<unsigned int> GetIndices();

        /** Retrieves the vertices generated by this Sphere object. */
        std::vector<float> GetVertices();

        /**
         *  Returns this sphere's radius.
         *  @return The radius of the current sphere; defined at creation.
         */
        float GetRadius();

        /**
         *  Subdivides the icosahedron the given amount of times.
         *  @param iterations - The number of times you wish to divide this shape.
         */
        void Divide(int iterations);

        /** Divides the current icosahedron into more triangles. */
        void Subdivision();

        /**
         *  Adds the provided vertex to the list or returns its id.
         *  TODO: Add hashmap checking instead of sequential search.
         *  @param vertex - The vertex to be added to the end of the list.
         *  @param map    - The vertex pair map to be checked for existing index pairings.
         *  @return A pairing of the existing vertex's index and false, or 0u and true if it is added.
         */
        std::pair<unsigned int,bool> AddVertex( std::array<float,3> vertex, std::map<std::pair<unsigned int, unsigned int>, unsigned int> map );

        /**
         *  Adds a new vertex to the back of the vertices list.
         *  @param one - The start vertex of the mid-point to be calculated.
         *  @param two - The end vertex of the mid-point to be calculated.
         *  @return The index of the existing midpoint, or size() - 1 if a new vertex was added to the end.
         */
        std::array<float,3> MidPoint
        (
            unsigned int one, 
            unsigned int two
        );

        /**
         *  Distributes a collision across the vertices from the provided vector.
         *  Physics assumptions: Assume direct collision, assume no fluid resistance/friction.
         *  @param vertex    - The point in space where the collision occurs.
         *  @param magnitude - The magnitude of the collision force.
         */
        void Collision( std::array<float,3> vertex, float magnitude);

        /** Finds the center point in 3D space of this sphere. */
        std::array<float,3> FindCenter();
        
    private:

        std::vector<std::array<float,3>> vertices;       // The list of unique vertices for the current shape.
        std::vector<std::array<unsigned int,3>> indices; // A list of the triangle indices formed from this shape's vertices.

        float radius;         // The spherical radius of this icosahedron.
        unsigned int counter; // Counter for unique keys
};

#endif