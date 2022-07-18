#include "light.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{


//phong = diffuse + ambient + specular

    //diffuse = (k_d)(I)(max[0, n /dot l])
    //ambient = (k_a)(I_a)
    //specular = (k_s)(I)(max[0,n /dot h]) // different than his notes which have v /dot h
    //h = (v+1) / ||v+1||

    vec3 color(0,0,0);
    //red = ob_color[0];
    //green = ob_color[1];
    //blue = ob_color[2];
    color = (world.ambient_color*color_ambient)*world.ambient_intensity;


    for(int i = 0; i < world.lights.size(); ++i) {

        Ray check;
        vec3 light_loc = world.lights.at(i)->position;
        vec3 s = light_loc;
        vec3 e = intersection_point;
        vec3 SE = (s-e);

        check.endpoint = e;
        check.direction = SE.normalized();

        Hit found = world.Closest_Intersection(check);

        double lightDistance = SE.magnitude();

        if (found.object != nullptr && found.dist <= lightDistance && world.enable_shadows) {
            continue;
        }

        vec3 l = light_loc-intersection_point; //switched around
        vec3 intensity = world.lights.at(i)->Emitted_Light(l);
        vec3 v = ray.direction;
        vec3 r = ((2* dot(l,normal))*normal-l).normalized();

        //vec3 k_a = this -> color_ambient;
        vec3 k_d = this -> color_diffuse;
        vec3 k_s = this -> color_specular;


        double zero = 0;

        //if we need to check for shadows ->
        //find the distance from the point to the light source
        //check if the closest intersection is the light source

        vec3 tempC(0,0,0);
        for(int j = 0; j < 3; ++j){
            double diffuse = k_d[j]*intensity[j]*(std::max(zero, dot(normal,l.normalized())));
            double specular = k_s[j]*intensity[j]* pow(std::max(zero, dot(-v,r)), this->specular_power);
            tempC[j] = diffuse + specular;
        }
        color = color + tempC;
    }

    return color;
}
