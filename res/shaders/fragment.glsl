#version 330 core

in vec4 v_Color;
in vec2 v_TextureCoordinates;

uniform int u_ColorSource;
uniform sampler2D u_TextureUnit;

out vec4 color;

void main() {
    if (u_ColorSource == 0) {
        color = texture(u_TextureUnit, v_TextureCoordinates);
    } else if (u_ColorSource == 1) {
        vec4 textureColor = texture(u_TextureUnit, v_TextureCoordinates);
        color = vec4(v_Color.r, v_Color.g, v_Color.b, textureColor.a);
    } else if (u_ColorSource == 2) {
        color = v_Color;
    }
}
