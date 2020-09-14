/**
 *  A 4 channel vector representing a color.
 *  The alpha channel is used for transparency and blending.
 */
struct Color 
{
    float red;
    float green;
    float blue;
    float alpha;
};

/**
 *  A vertex that represents a point in 2D space.
 */
struct Vertex2D
{
    float x;
    float y;
};

/**
 *  A vertex that represents a point in 3D space.
 */
struct Vertex
{
    float x;
    float y;
    float z;
};