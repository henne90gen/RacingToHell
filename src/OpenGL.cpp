void setTexturePixels(GLuint texture_object_id, VideoBuffer *videoBuffer) {
	glBindTexture(GL_TEXTURE_2D, texture_object_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, videoBuffer->width,
			videoBuffer->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			videoBuffer->content);
	glGenerateMipmap (GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint loadTexture(VideoBuffer * videoBuffer) {
	GLuint texture_object_id;
	glGenTextures(1, &texture_object_id);

	setTexturePixels(texture_object_id, videoBuffer);
	return texture_object_id;
}

GLuint compileShader(const GLenum type, const GLchar *source,
		const GLint length) {
	GLuint shader_object_id = glCreateShader(type);
	GLint compile_status;

	glShaderSource(shader_object_id, 1, (const GLchar **) &source, &length);
	glCompileShader(shader_object_id);
	glGetShaderiv(shader_object_id, GL_COMPILE_STATUS, &compile_status);

	if (compile_status == GL_FALSE) {
		int infoLogLength;
		glGetShaderiv(shader_object_id, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shader_object_id, infoLogLength, NULL,
					&VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}
		std::string message = "Failed to compile shader. "
				+ std::to_string(type) + "\n" + std::string(source);
		abort(message);
	}

	return shader_object_id;
}

GLuint linkProgram(const GLuint vertex_shader, const GLuint fragment_shader) {
	GLuint program_object_id = glCreateProgram();
	GLint link_status;

	glAttachShader(program_object_id, vertex_shader);
	glAttachShader(program_object_id, fragment_shader);
	glLinkProgram(program_object_id);
	glGetProgramiv(program_object_id, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) {
		abort("Couldn't link shader program.");
	}

	return program_object_id;
}

GLuint buildProgram() {
	// FIXME don't hard code the shaders
	const GLchar *vertex_shader_source =
			"attribute vec4 a_Position;attribute vec2 a_TextureCoordinates;varying vec2 v_TextureCoordinates;void main() {v_TextureCoordinates = a_TextureCoordinates;gl_Position = a_Position;}";
	const GLint vertex_shader_source_length = 179;
	const GLchar *fragment_shader_source =
			"uniform sampler2D u_TextureUnit;varying vec2 v_TextureCoordinates;void main() {gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);}";
	const GLint fragment_shader_source_length = 166;

	GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_shader_source,
			vertex_shader_source_length);
	GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER,
			fragment_shader_source, fragment_shader_source_length);
	return linkProgram(vertex_shader, fragment_shader);
}

GLuint buildProgram(const GLchar *vertex_shader_source,
		const GLint vertex_shader_source_length,
		const GLchar *fragment_shader_source,
		const GLint fragment_shader_source_length) {

	GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_shader_source,
			vertex_shader_source_length);
	GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER,
			fragment_shader_source, fragment_shader_source_length);
	return linkProgram(vertex_shader, fragment_shader);
}

GLuint buildProgramFromAssets(const char *vertex_shader_path,
		const char *fragment_shader_path) {
	const File vertex_shader_source = readFile(vertex_shader_path);
	const File fragment_shader_source = readFile(fragment_shader_path);
	const GLuint program_object_id = buildProgram(vertex_shader_source.content,
			vertex_shader_source.size, fragment_shader_source.content,
			fragment_shader_source.size);

	return program_object_id;
}

GLuint createVertexBufferObject(const GLsizeiptr size, const GLvoid *data,
		const GLenum usage) {
	GLuint vbo_object;
	glGenBuffers(1, &vbo_object);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo_object;
}

void setupOpenGL(GraphicsData *graphics) {
	graphics->gl_texture = loadTexture(&graphics->videoBuffer);
	graphics->gl_buffer = createVertexBufferObject(sizeof(rect), rect, GL_STATIC_DRAW);

	graphics->gl_program = buildProgram();

	graphics->gl_position_location = glGetAttribLocation(graphics->gl_program, "a_Position");
	graphics->gl_texture_coordinates_location = glGetAttribLocation(graphics->gl_program,
			"a_TextureCoordinates");
	graphics->gl_texture_unit_location = glGetUniformLocation(graphics->gl_program, "u_TextureUnit");
}
