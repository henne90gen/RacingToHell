
uniform int u_ColorSource;

varying vec4 v_Color;

uniform sampler2D u_TextureUnit;

varying vec2 v_TextureCoordinates;

void main() {
	if (u_ColorSource == 0) {
		gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
	} else if (u_ColorSource == 1) {
		gl_FragColor = v_Color;
		//gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);
	}
}
