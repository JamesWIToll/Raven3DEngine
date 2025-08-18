#version 460
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

struct Material {
    vec3 diffuseColor;
    vec3 specColor;
    vec3 ambientColor;
    vec3 emissiveColor;
    float opacity;
    float shininess;
    float shininessStrength;
    sampler2D diffTex;
    sampler2D specTex;
    sampler2D ambTex;
    sampler2D emisTex;
    sampler2D normTex;
};

uniform Material uMaterial;

void main() {
    FragColor = vec4(uMaterial.diffuseColor, uMaterial.opacity) * texture(uMaterial.diffTex, TexCoord.xy);
}
