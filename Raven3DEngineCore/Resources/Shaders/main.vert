#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aUV_0;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;
out mat3 TBN;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;


void main() {

    TexCoord = aUV_0;

    Normal = (transpose(inverse(uModel)) * vec4(aNormal, 1.0f)).xyz;
    WorldPos = vec3((uModel * vec4(aPos, 1.0f)).xyz);

    vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0f)));
    vec3 N = normalize(vec3(uModel * vec4(aNormal, 0.0f)));
    vec3 B = normalize(cross(N, T));

    TBN = mat3(T, B, N);

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
}
