#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
using namespace  std;

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{

    Hit find;
    find.dist = std::numeric_limits<double>::max();
    find.object = nullptr;
    find.part = 1;

    for(int i = 0; i < objects.size(); ++i){

        Hit Htemp = objects.at(i)->Intersection(ray,1);

        if(Htemp.dist<find.dist && Htemp.dist > small_t)
            find = Htemp;


    }

    //cout << "Closest Intersection: "<<find.dist<<endl;

    return find;

}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{

    //end_point is the camera position
    //direction is a unit vector from camera position to world position of pixel

    //p(t) = e + t ( s - e )
    // e = camera
    // t = fractional distance
    // s = point

    vec3 s = camera.World_Position(pixel_index);
    vec3 e = camera.position;
    vec3 SE = (s-e);

    Ray ray;
    ray.endpoint = e;
    ray.direction = SE.normalized();
    //must be normalized as we only want direction info for that vector
    //endpoint is where t = 0 -> camera's pos
    //direction should be the s - e vector [could be wrong but doubt]
    //this structure might make sense as direction * t would change the function

    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));



}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;

    //get closest hit with Closest_intersec
    //if there is an intersect, use shade surface to get color

    Hit candidate = Closest_Intersection(ray);

    if(candidate.object!=nullptr){

        Shader *mycolor = candidate.object->material_shader;
        vec3 intersect = ray.endpoint + (ray.direction * candidate.dist);
        //intersect = e + t(s-e)
        vec3 normal = candidate.object->Normal(intersect, 1);
        //calculate this right??

        color = mycolor->Shade_Surface(ray, intersect, normal,recursion_depth);

    }
    else{
        color.fill(0);
        color = background_shader->Shade_Surface(ray,color,color,recursion_depth);
    }

    //cout<<"vector direction: "<<ray.direction[0]<<ray.direction[1]<<ray.direction[2]<<endl;
    //cout<<"vector end: "<<ray.endpoint[0]<<ray.endpoint[1]<<ray.direction[2]<<endl;

    return color;



}

void Render_World::Initialize_Hierarchy()
{
   // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
