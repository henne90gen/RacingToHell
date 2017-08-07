
uniform int u_ColorSource;

varying vec4 v_Color;

uniform sampler2D u_TextureUnit;

varying vec2 v_TextureCoordinates;

void main() {
	if (u_ColorSource == 0) {
		gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
	} else if (u_ColorSource == 1) {
		vec4 textureColor = texture2D(u_TextureUnit, v_TextureCoordinates);
		gl_FragColor = vec4(v_Color.r, v_Color.g, v_Color.b, textureColor.a);
	} else if (u_ColorSource == 2) {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);//v_Color;
	}
}
