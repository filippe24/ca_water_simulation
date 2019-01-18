#version 330 core


in vec3 fragNormal;
in vec4 vPos;

out vec4 fragColor;


uniform vec4 color;
uniform vec3 disposition;


void main()
{
    vec4 final_color = vec4(color.x, color.y, color.z, color);
    fragColor = final_color * normalize(fragNormal).z;
}
