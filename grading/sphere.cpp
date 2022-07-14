#include "sphere.h"
#include "ray.h"
using namespace std;

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const {

    // p(t) = e + td
    // d = s - e

    vec3 d = ray.direction;
    // s - e
    vec3 e = ray.endpoint;
    // endpoint
    double a = dot(d,d);
    // d*d
    double b = dot((d*2),(e-center));
    //2(d) * (e-c)
    double c = (dot(e-center,e-center))-(radius*radius);
    //[(e-c) * (e-c)]R^2

    /*        Quadratic:
     * -segment1 +- /sqrt(segment2)
     * -----------------------------
     *          segment3
     */

    double segment1 = -b;
    double segment2 = (b*b)-(4*a*c);
    double segment3 = (2*a);

    /*
     * segment2
     * if negative: no solution
     * if positive: 2 solution
     * if 0:        1 solution
     */

    //case there is no intersection
    if(segment2 < 0){
        return {this,0,part};
    }

    double ans1 = (segment1 + sqrt(segment2))/segment3;
    double ans2 = (segment1 - sqrt(segment2))/segment3;



    //case there is 2 intersections, return closest
    if(segment2 > 0){
        if(ans1 < ans2)
            return {this,ans1, part};
        return {this, ans2, part};
    }

    //case only 1 is actual value
    if(segment2 == 0) {
        if (isnan(ans1))
            return {this, ans2, part};
        return {this, ans1, part};
    }

}






vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;

    //(x-h)^2 + (y-k)^2 + (z-l)^2 = r^2 = g
        //h,k,l = center; r = radius
    //partial(fx) = ax, partial(fy) = by, partial (fz) = cz
    //take point = (x,y,z) -> f = (ax, by, cz)
    //gradient(f) / ||gradient(f)|| = normal

    //obtained gradient(f) components:
    double x = 2*(point[0]-center[0]);
    double y = 2*(point[1]-center[1]);
    double z = 2*(point[2]-center[2]);

    //formalize it into a vector:
    vec3 grad(x,y,z);

    //apply gradient(f) / ||gradient(f)||
    normal = grad/grad.magnitude();

    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}

