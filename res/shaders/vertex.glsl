
attribute vec4 a_Position;

attribute vec2 a_TextureCoordinates;

uniform mat4 u_ScaleMatrix;

varying vec2 v_TextureCoordinates;

void main() {
	v_TextureCoordinates = a_TextureCoordinates;
	gl_Position = u_ScaleMatrix * a_Position;
}
