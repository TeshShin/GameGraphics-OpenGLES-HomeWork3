#version 300 es

precision mediump float;

uniform sampler2D textureDiff;

in vec3 v_normal;
in vec2 v_texCoord;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 normal = normalize(v_normal);
    vec3 color = texture(textureDiff, v_texCoord).rgb;

    fragColor = vec4(color, 1.0);
}