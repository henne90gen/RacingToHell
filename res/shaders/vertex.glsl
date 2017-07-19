
attribute vec4 a_Position;

attribute vec2 a_TextureCoordinates;

attribute vec4 a_Color;

uniform mat4 u_ScaleMatrix;

varying vec2 v_TextureCoordinates;

varying vec4 v_Color;

void main() {
	v_TextureCoordinates = a_TextureCoordinates;
	v_Color = a_Color;
	gl_Position = u_ScaleMatrix * a_Position;
}
