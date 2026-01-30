#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
uniform float time;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

//vec3 transform = vec3(0.0f, 0.0f, 2.5f);


vec3 rotateAxis(vec3 p, vec3 axis, float angle)
{
    axis = normalize(axis);
    float c = cos(angle);
    float s = sin(angle);

    return p * c +
           cross(axis, p) * s +
           axis * dot(axis, p) * (1.0 - c);
}

void main()
{
    vec3 p = pos;
    p = rotateAxis(p, vec3(0,1,0), time/1500.0f);
    p = rotateAxis(p, vec3(1,0,0), time/500.0f);
    p += vec3(0.0f, 0.0f, -10.0f);
    gl_Position = vec4(p, -p.z);
    //gl_Position = vec4(p.xyz, -p.z);//projection*view*model*vec4(pos,1.0f);
}
