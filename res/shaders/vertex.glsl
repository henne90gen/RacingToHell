
attribute vec4 a_Color;
attribute vec4 a_Position;
attribute vec2 a_TextureCoordinates;

uniform mat4 u_RotationMatrix;
uniform mat4 u_ScaleMatrix;
uniform mat4 u_AspectRatioMatrix;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;

void main() {
	v_TextureCoordinates = a_TextureCoordinates;
	v_Color = a_Color;
    gl_Position = u_ScaleMatrix * u_RotationMatrix * u_AspectRatioMatrix * a_Position;
}
