#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragNormal;


uniform mat4 projection, modelview;
uniform mat3 normalMatrix;
uniform vec3 disposition;
uniform mat4 scale;


void main()
{
        vec3 pos = (scale*vec4(position, 1.0)).xyz;
        gl_Position = projection * modelview * vec4(pos + disposition, 1.0);
        fragNormal = normalMatrix * normal;
}

