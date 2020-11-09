#version 330 core

in vec3 vertex;
in vec3 normal;
in vec3 color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 normalMatrix;
uniform mat4 TG;

out vec2 vtexCoords;

out vec4 frontColor;

void main()
{		
    vec4 N = normalize(normalMatrix*vec4(normal, 1.0));
    frontColor = vec4(color,1.0) * N.z*10;
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
