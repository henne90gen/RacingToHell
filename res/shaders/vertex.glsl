#version 300 es

in vec4 a_Color;
in vec2 a_Position;
in vec2 a_TextureCoordinates;

uniform mat4 u_RotationMatrix;
uniform mat4 u_ScaleMatrix;
uniform mat4 u_AspectRatioMatrix;

out vec4 v_Color;
out vec2 v_TextureCoordinates;

void main() {
    v_Color = a_Color;
    v_TextureCoordinates = a_TextureCoordinates;
    gl_Position = u_ScaleMatrix * u_RotationMatrix * u_AspectRatioMatrix * vec4(a_Position.x, a_Position.y, 0, 1);
}
