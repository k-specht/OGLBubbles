#ifndef Centroid
#define Centroid

#include <vector>
#include <iostream>
#include <array>

/** A structure of x y z coordinates for a singular point. */
struct Point
{
    float x;
    float y;
    float z;
};

/**
 *  Returns the euclidean distance between the two points.
 *  @param one - The first point in xyz space to be compared.
 *  @param two - The second point whose distance to the first should be calculated.
 *  @return The Euclidean distance from point one to two.
 */
inline float distance(Point one, Point two)
{
    return sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2) + pow(one.z - two.z, 2));
}

/** 
 *  Subtracts vector 2 from vector 1.
 *  @param v1 - The first point vector to subtract from.
 *  @param v2 - The second point vector to be subtracted from v1.
 *  @return The resulting vector of the calculation.
 */
inline Point vSub(Point v1, Point v2)
{
    Point p;

    p.x = v1.x - v2.x;
    p.y = v1.y - v2.y;
    p.z = v1.z - v2.z;

    return p;
}

/**
 *  Finds the furthest point and returns it.
 *  @param center - The center point of the circle whose points are being considered.
 *  @param points - The cluster of points to be checked for the greatest element.
 *  @param n      - The number of points in this cluster.
 *  @return The point whose distance from the center is greatest.
 */
inline Point findGreatest(Point center, Point points[], int n)
{
    // Create max point from center
    int maxDistance = -1;
    Point maxPt = center;

    //std::cout << "points: " << sizeof(points[0]) << std::endl;
    // Loop through each point and find the greatest distance
    for (int i = 0; i < n; i++)
    {
        float d = distance(points[i], center);
        // std::cout << "Distance between {" << points[i].x << ", " << points[i].y << ", " << points[i].z << "} and center: " << d << std::endl;
        if ( d > maxDistance )
        {
            maxDistance = d;
            maxPt = points[i];
        }
    }

    // Debug code that's probably equivalent to "if (points.length <= 1)"
    if ( (maxPt.x == center.x) && (maxPt.y == center.y) && (maxPt.z == center.z))
        std::cout << "ERR: greatest distance is center point?" << std::endl;

    std::cout << "Greatest point in dataset of size " << sizeof(points) - (sizeof(points[0]) / sizeof(float)) << ": {" << maxPt.x << ", " << maxPt.y << ", " << maxPt.z << "}." << std::endl;
    return maxPt;
}

/** 
 *  Returns the next greatest point from the center .
 *  @param center - The center point of the points cluster.
 *  @param points - The points cluster to find the kth-furthest point in.
 *  @param k      - A number representing the amount of furthest elements you wish to skip.
 *  @return The kth furthest point from the given center, in the given points cluster.
 */
inline Point findKGreatest(Point center, Point points[], int k)
{
    // Temporary array
    // std::vector<Point> pts = std::vector<Point>(points, points + sizeof(points) / sizeof(points[0]));
    std::vector<Point> pts;
    Point greatest;
    // std::cout << "Size: " << sizeof(points) - (sizeof(points[0]) / sizeof(float)) << std::endl;
    for (int x = 0; x < sizeof(points) - (sizeof(points[0]) / sizeof(float)); x++)
    {
        pts.push_back(points[x]);
    }

    //std::cout << "Point count: " << pts.size() << std::endl;
    // Find k-th furthest point
    for (int i = 0; i < k; i++)
    {
        // i-th greatest
        greatest = findGreatest(center, pts.data(), pts.size());

        // Add all elements but the greatest to the array
        for (int j = 0; j < pts.size(); j++)
        {
            if ( pts[j].x == greatest.x && pts[j].y == greatest.y && pts[j].z == greatest.z )
            {
                pts.erase(pts.begin() + j);
                //std::cout << "Erased num " << j << " from the list, new size is " << pts.size() << "." << std::endl;
                j--;
            }
        }
    }
    
    // Debug message; if there's an error with k this will probably show up
    if ( sizeof(points) - (sizeof(points[0]) / sizeof(float)) == pts.size() ) std::cout << "ERR: Greatest not removed." << std::endl;

    return findGreatest(center, pts.data(), pts.size());
}

/**
 *  Gets the center of the specified point arrays by averaging the max and min points.
 *  @param points - The vector of points to be considered.
 *  @param n      - The number of points in the cluster.
 */
inline Point findCenter(Point points[], int n)
{
    Point center;
    center = { 0.0f, 0.0f, 0.0f };

    // Finds min and max of points
    //std::cout << "Point count: " << sizeof(points) - (sizeof(points[0]) / sizeof(float)) << std::endl;
    for (int i = 0; i < n; i++)
    {
        //std::cout << "Iteration: " << i << ", center: {" << center.x << ", " << center.y << ", " << center.z << "}, point: " << std::endl;
        std::cout << points[i].x << ", " << points[i].y << ", " << points[i].z << "." << std::endl;
        center.x += points[i].x;
        center.y += points[i].y;
        center.z += points[i].z;
    }

    center.x /= (sizeof(points) - (sizeof(points[0]) / sizeof(float)));
    center.y /= (sizeof(points) - (sizeof(points[0]) / sizeof(float)));
    center.z /= (sizeof(points) - (sizeof(points[0]) / sizeof(float)));

    //center.x = (max.x + min.x) / 2;
    //center.y = (max.y + min.y) / 2;
    //center.z = (max.z + min.z) / 2;

    return center;
}

/**
 *  This method calculates the minimum sphere that can hold the points entered in stdin.
 *  @return An pair consisting of {x, y, z} and r, where xyz are the coordinates of the center point and r is the radius.
 */
inline std::pair<std::array<float,3>, float> Center()
{
    // Pointer to a vector containing the points to be used
    std::vector<Point>* points = new std::vector<Point>();

    // The number of points in this cluster
    int n;

    // Read in points
    std::ifstream source("../OGLBubbles/bin/input.txt");
    //source.open("../OGLBubbles/bin/input.txt", ios::in);
    bool first = true;
    for (std::string line; getline(source, line);)
    {
        Point point;
        std::istringstream in(line);

        if ( first )
        {
            in >> n;
            first = false;
            continue;
        }

        in >> point.x;
        in >> point.y;
        in >> point.z;

        //point.x += 10.0f;
        //point.y += 10.0f;
        //point.z += 10.0f;

        points->push_back(point);
    }
    source.close();
    //std::cout << "Point array size: " << points->size() << std::endl;

    // Keeps track of circle state
    float oldRadius, originalR;
    Point oldCenter, oldGreatest;

    // Set up original circle (blue)
    Point center    = findCenter(points->data(), points->size());
    float radius    = distance(findGreatest(center, points->data(), points->size()), center);

    originalR       = radius;
    oldGreatest     = findGreatest(center, points->data(), points->size());

    // Log original
    std::cout << "Original center: {" << center.x << ", " << center.y << ", " << center.z << "}, Original radius: " << radius << std::endl;

    // Loop variables
    bool sameX, sameY;
    float aX, aY, magV, check;
    Point next, v;
    int k = 0; // The current furthest point that's moving the center & radius

    // Move circle for each point
    for (int k = 0; k < n; k++)
    {
        //std::cout << "Index: " << k << endl;
        // Save circle state
        oldRadius = radius;
        oldCenter = center;

        // Find the next greatest point
        next = findKGreatest(center, points->data(), k);
    
        // Difference between next greatest point and center of circle (used in line formula)
        v    = vSub(next, center);
        magV = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

        // Closest point on the circle:
        aX = center.x + v.x / magV * radius;
        aY = center.y + v.y / magV * radius;

        sameX = false;
        sameY = false;

        // Both in same X hemisphere
        if ((next.x >= center.x && oldGreatest.x >= center.x) || (next.x <= center.x && oldGreatest.x <= center.x) )
            sameX = true;
        
        // Both in same Y hemisphere
        if ((next.y >= center.y && oldGreatest.y >= center.y) || (next.y <= center.y && oldGreatest.y <= center.y) )
            sameY = true;

        // Move origin by inverse(circleEdge - nextGreatestPoint) / 2, or not inverse if same hemisphere
        next.x -= aX;
        next.y -= aY;
        next.x /= (sameX) ? 2 : -2;
        next.y /= (sameY) ? 2 : -2;
        center = vSub(center, next);

        // Shrink radius by nextGreatestPoint / 2
        radius -= (radius - distance(next, oldCenter)) / 2;

        // Check if any points are excluded, if so expand the radius to fit them (could instead move center...?)
        check = distance(findGreatest(center, points->data(), points->size()), center);
        if (check > radius) radius = check;
        
        // Revert state to previous if radius increased
        if ( oldRadius < radius )
        {
            //std::cout << "Reverting on k " << k << endl;
            //cout << "Warn: radius size increased during iteration: " << k << endl;
            radius = oldRadius;
            center = oldCenter;
        }
        //if ( radius == oldRadius ) break;
        //if ( k >= points->size() - 1 ) break;
        oldGreatest = findGreatest(center, points->data(), points->size());
    }

    delete points;

    //std::cout << radius << std::endl;
    //std::cout << "Completed circle centering, new center: {" << center.x << ", " << center.z << ", " << center.y << "}, radius: " << radius << std::endl;
    //cout << "Previous center: {" << oldCenter.x << ", " << oldCenter.y << "}, previous radius: " << oldRadius << endl;
    std::array<float, 3> cent = {center.x, center.y, center.z};
    return std::make_pair(cent, radius);
}

#endif