#include "plane.h"
#include "hit.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{

    /*
     *      (p1 - e) /dot n
     *  t = ----------------
     *          d /dot n
     *
     * n = normal
     * p1 = point
     *
     */


    double denom = dot(ray.direction,normal);
    double t = (dot(x1 - ray.endpoint,normal)) / denom;




    if(t >= 0) {
        return {this, t, part};
    }
    return {this, std::numeric_limits<double>::max(), part};





}

vec3 Plane::Normal(const vec3& p, int part) const
{
    //todo
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}

