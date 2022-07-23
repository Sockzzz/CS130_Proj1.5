#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{

    vec3 color;

    double spec = reflectivity;

    vec3 d = ray.direction;

    //reflection = d - 2(d /dot n) n
    vec3 reflect = (d-2*(dot(d,normal))*normal);

    Ray myreflect(intersection_point,reflect);

    //reflectivity is a percentage of light to be from other objects
    //reflectivity - 1 is percentage of objects color that should show up
    color = (1-spec)*(shader->Shade_Surface(ray, intersection_point, normal, recursion_depth));

    //Hit candidate = world.Closest_Intersection(myreflect);


    if(recursion_depth!=world.recursion_depth_limit){
        recursion_depth++;
        //color c = current color + (reflectivity * other_object_color)
        //vec3 intersect = myreflect.endpoint + (myreflect.direction * candidate.dist);
        vec3 otherObject = (reflectivity * world.Cast_Ray(myreflect,recursion_depth));
        return color + otherObject;
    }

    return color;// + mycolor;


}
