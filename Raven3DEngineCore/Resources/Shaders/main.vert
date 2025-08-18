#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV_0;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
    TexCoord = aUV_0;
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    WorldPos = vec3((uModel * vec4(aPos, 1.0f)).xyz);
}
