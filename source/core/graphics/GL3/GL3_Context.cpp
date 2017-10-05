#include "GL3_Context.h"

namespace gl3
{
	void error_callback(int error, const char* description)
	{
		throw GlfwException(GLFW_ERROR, description);
	}
	int GL3_Context::checkShaderCompilationError(GLuint program, std::string type)
	{
		int success;
		char infoLog[512];
		if (type == "VERTEX_SHADER") {
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(program, 512, NULL, infoLog);
				std::cout << "ERROR: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		else if (type == "FRAGMENT_SHADER") {
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(program, 512, NULL, infoLog);
				std::cout << "ERROR: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		else if (type == "GEOMETRY_SHADER") {
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(program, 512, NULL, infoLog);
				std::cout << "ERROR: Geometry Shader Compilation Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		else if (type == "SHADER_PROGRAM") {
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(program, 512, NULL, infoLog);
				std::cout << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		return 0;
	}

	InternalFormat GL3_Context::convertToInternalFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::R:
			return InternalFormat::R8;
		case ImageFormat::RG:
			return InternalFormat::RG8;
		case ImageFormat::RGB:
			return InternalFormat::RGB8;
		case ImageFormat::RGBA:
			return InternalFormat::RGBA8;
		default:
			return InternalFormat::UNDEFINED;
		}
	}
	int GL3_Context::convertToChannelSize(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::R:
			return 1;
		case ImageFormat::RG:
			return 2;
		case ImageFormat::RGB:
			return 3;
		case ImageFormat::RGBA:
			return 4;
		default:
			return 0;
		}
	}


	GL3_Context::GL3_Context(sys::PlatformHandle* systemHandle, int width, int height, std::string title)
		: systemHandle(systemHandle), width(width), height(height), title(title)
	{
		systemHandle->createContext(width, height, title);

		// --- GLAD ---
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			throw GlewException(0, "GL::ERROR: Failed to initialize GLEW");
		}

		makeCurrent();

		// --- Internal initialization ---
		GL3_UnitShaderTemplate UnitShaderTemplate;
		unitDrawable = createDrawable(UnitShaderTemplate.vertices, UnitShaderTemplate.vertexLayout, UnitShaderTemplate.mode);
		unitShader = compileShader(UnitShaderTemplate.vertexCode, UnitShaderTemplate.fragmentCode);
		unitShader.setUniform("screenTexture", 0);
		GL3_UnitTransformShaderTemplate unitTransformShaderTemplate;
		unitTransformShader = compileShader(unitTransformShaderTemplate.vertexCode, unitTransformShaderTemplate.fragmentCode);
		unitTransformShader.setUniform("screenTexture", 0);
		unitTransformShader.setUniform("transform", gml::Matrix4D<float>());

		glState.window_width = width;
		glState.window_height = height;
		setActiveRenderTarget(0);
		setActiveViewport(Viewport(0, 0, width, height));
	}
	GL3_Context::~GL3_Context()
	{
		
	}


	void GL3_Context::makeCurrent()
	{
		glState.window_width = width;
		glState.window_height = height;
		systemHandle->makeCurrent();
	}
	void GL3_Context::showWindow()
	{
		systemHandle->showWindow();
	}
	void GL3_Context::hideWindow()
	{
		systemHandle->hideWindow();
	}
	void GL3_Context::swapBuffers()
	{
		systemHandle->swapBuffers();
	}


	void GL3_Context::setActiveViewport(const Viewport & viewport)
	{
		glViewport(viewport.x, glState.renderTarget_height - (viewport.y + viewport.height), viewport.width, viewport.height);
		glState.viewport = viewport;
	}
	void GL3_Context::setActiveRenderTarget(const GL3_Framebuffer & framebuffer)
	{
		if (glState.renderTarget != framebuffer.FBO) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.FBO);
			glState.renderTarget_height = framebuffer.height;
			glState.renderTarget_width = framebuffer.width;
			glState.renderTarget = framebuffer.FBO;
		}
	}
	void GL3_Context::setActiveRenderTarget(GLuint FBO)
	{
		if (glState.renderTarget != FBO) {
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glState.renderTarget_height = glState.window_height;
			glState.renderTarget_width = glState.window_width;
			glState.renderTarget = FBO;
		}
	}
	void GL3_Context::setPackAlignment(int value)
	{
		if (value != glState.pack_alignment) {
			glPixelStorei(GL_PACK_ALIGNMENT, value);
		}
	}
	void GL3_Context::setUnpackAlignment(int value)
	{
		if (value != glState.unpack_alignment) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, value);
		}
	}
	void GL3_Context::enableDepthTest()
	{
		if (glState.isDepthTestEnabled == false) {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glState.isDepthTestEnabled = true;
		}
	}
	void GL3_Context::disableDepthTest()
	{
		if (glState.isDepthTestEnabled) {
			glDisable(GL_DEPTH_TEST);
			glState.isDepthTestEnabled = false;
		}
	}
	void GL3_Context::enableStencilTest()
	{
		if (glState.isStencilTestEnabled == false) {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glState.isStencilTestEnabled = true;
		}
	}
	void GL3_Context::disableStencilTest()
	{
		if (glState.isStencilTestEnabled) {
			glDisable(GL_STENCIL_TEST);
			glState.isStencilTestEnabled = true;
		}
	}
	void GL3_Context::enableBlending()
	{
		if (glState.isBlendingEnabled == false) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glState.isBlendingEnabled = true;
		}
	}
	void GL3_Context::disableBlending()
	{
		if (glState.isBlendingEnabled) {
			glDisable(GL_BLEND);
			glState.isBlendingEnabled = false;
		}
	}
	void GL3_Context::enableWireframe()
	{
		if (glState.isWireframeEnabled == false) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glState.isWireframeEnabled = true;
		}
	}
	void GL3_Context::disableWireframe()
	{
		if (glState.isWireframeEnabled) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glState.isWireframeEnabled = false;
		}
	}
	void GL3_Context::enableVSync()
	{
		glfwSwapInterval(1);
	}
	void GL3_Context::disableVSync()
	{
		glfwSwapInterval(0);
	}


	GL3_Framebuffer GL3_Context::createFramebuffer(GLint width, GLint height, ImageFormat format)
	{
		GL3_Framebuffer buffer;
		// Create Framebuffer Object
		glGenFramebuffers(1, &buffer.FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.FBO);

		// Create empty Texture
		buffer.texture = create2DTexture(nullptr, width, height, format);
		buffer.texture.configTextureFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);

		// Attach Texture to FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture.texture, 0);

		// Depth & Stencil Buffers
		glGenRenderbuffers(1, &buffer.RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, buffer.RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.RBO);

		// Completeness check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Error" << std::endl;
			throw;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		buffer.glContext = this;
		buffer.id = buffer.FBO;

		buffer.height = height;
		buffer.width = width;
		return buffer;
	}

	GL3_Drawable GL3_Context::createDrawable(std::vector<GLfloat> & vertices, std::vector<int> vertexLayout, DrawMode usage, VertexFormat format)
	{
		GL3_Drawable drawable;
		// Generate
		glGenBuffers(1, &drawable.VBO);
		glGenVertexArrays(1, &drawable.VAO);
		// Bind VAO
		glBindVertexArray(drawable.VAO);
		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, drawable.VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], (GLenum)usage);
		// Config Vertex Attributes
		int vertexSize = std::accumulate(vertexLayout.begin(), vertexLayout.end(), 0);
		int nVertices = (int)vertices.size() / vertexSize;

		if (format == VertexFormat::INTERLEAVED) {
			int attribOffset = 0;
			for (unsigned int i = 0; i < vertexLayout.size(); i++) {
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, vertexLayout[i], GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (GLvoid*)(attribOffset * sizeof(GLfloat)));
				attribOffset += vertexLayout[i];
			}
		}
		else if (format == VertexFormat::BATCHED) {
			int attribOffset = 0;
			for (unsigned int i = 0; i < vertexLayout.size(); i++) {
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, vertexLayout[i], GL_FLOAT, GL_FALSE, vertexLayout[i] * sizeof(GLfloat), (GLvoid*)(attribOffset * sizeof(GLfloat)));
				attribOffset += vertexLayout[i] * nVertices;
			}
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		drawable.id = drawable.VAO;
		drawable.glContext = this;
		drawable.nVertices = nVertices;
		drawable.vertexSize = vertexSize;
		drawable.layout = vertexLayout;
		drawable.format = format;
		drawable.usage = usage;
		drawable.primitive = PrimitiveType::TRIANGLES;
		return drawable;
	}
	void GL3_Context::createDrawable(GL3_Drawable & drawable, std::vector<GLfloat> & vertices, std::vector<int> vertexLayout, DrawMode usage, VertexFormat format)
	{
		drawable = createDrawable(vertices, vertexLayout, usage, format);;
	}

	GL3_Texture GL3_Context::create2DTexture(unsigned char * image, int width, int height, ImageFormat format, int rowAlignment)
	{
		// Generate & Bind
		GL3_Texture texture;
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// Select pixel pack format
		InternalFormat internalFormat = convertToInternalFormat(format);

		// Set unpack alignment
		setUnpackAlignment(rowAlignment);

		// Attach Texture
		glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)internalFormat, width, height, 0, (GLenum)format, (GLenum)PixelType::U_BYTE, image);

		// Set Default Filter & Wrapper Options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		// Build Texture
		texture.texture = texture.id;
		texture.glContext = this;

		texture.height = height;
		texture.width = width;

		texture.target = TextureType::TEXTURE_2D;
		texture.format = internalFormat;

		texture.px_type = PixelType::U_BYTE;
		texture.px_format = format;
		texture.rowAlignment = rowAlignment;
		texture.nChannels = convertToChannelSize(format);

		return texture;
	}
	void GL3_Context::create2DTexture(GL3_Texture & texture, unsigned char * image, int width, int height, ImageFormat format, int rowAlignment)
	{
		texture = create2DTexture(image, width, height, format, rowAlignment);
	}

	GL3_Texture GL3_Context::createCubemapTexture(std::array<unsigned char*, 6> images, int width, int height, ImageFormat format, int rowAlignment)
	{
		// Generate & Bind
		GL3_Texture texture;
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

		// Select pixel pack format
		InternalFormat internalFormat = convertToInternalFormat(format);

		// Set unpack alignment
		setUnpackAlignment(rowAlignment);

		// Attach Textures
		for (GLuint i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]);
		}

		// Set Image Wrapper
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// Set Image Filter
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Unbind
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		// Build Texture
		texture.texture = texture.id;
		texture.glContext = this;
		
		texture.height = height;
		texture.width = width;

		texture.target = TextureType::TEXTURE_CUBEMAP;
		texture.format = internalFormat;

		texture.px_type = PixelType::U_BYTE;
		texture.px_format = format;
		texture.rowAlignment = rowAlignment;
		texture.nChannels = convertToChannelSize(format);

		return texture;
	}
	void GL3_Context::createCubemapTexture(GL3_Texture & texture, std::array<unsigned char*, 6> images, int width, int height, ImageFormat format, int rowAlignment)
	{
		texture = createCubemapTexture(images, width, height, format, rowAlignment);
	}

	GL3_Shader GL3_Context::compileShader(std::string vertexCode, std::string fragmentCode, std::string geometryCode)
	{
		GL3_Shader shader;

		GLuint vertexShader, fragmentShader, geometryShader;
		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();
		const char* geometrySource = geometryCode.c_str();
		// Vertex Shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		checkShaderCompilationError(vertexShader, "VERTEX_SHADER");
		// Fragment Shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		checkShaderCompilationError(fragmentShader, "FRAGMENT_SHADER");
		// Geometry Shader
		if (geometryCode != "") {
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &geometrySource, NULL);
			glCompileShader(geometryShader);
			checkShaderCompilationError(geometryShader, "GEOMETRY_SHADER");
		}
		// Link Program
		shader.id = glCreateProgram();
		glAttachShader(shader.id, vertexShader);
		glAttachShader(shader.id, fragmentShader);
		if (geometryCode != "") {
			glAttachShader(shader.id, geometryShader);
		}
		glLinkProgram(shader.id);
		checkShaderCompilationError(shader.id, "SHADER_PROGRAM");
		// Delete Shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryCode != "") {
			glDeleteShader(geometryShader);
		}

		shader.glContext = this;
		shader.program = shader.id;

		// Save uniform locations
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		GLsizei bufSize; // maximum name length
		GLchar* name; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(shader.id, GL_ACTIVE_UNIFORMS, &count);
		glGetProgramiv(shader.id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);

		name = new GLchar[bufSize];

		for (int i = 0; i < count; i++) {
			glGetActiveUniform(shader.id, (GLuint)i, bufSize, &length, &size, &type, name);
			shader.uniformLocations[name] = glGetUniformLocation(shader.id, name);
		}
		delete name;

		return shader;
	}

	void GL3_Context::draw(const GL3_Drawable & drawable)
	{
		if (drawable.id != 0) {
			glBindVertexArray(drawable.VAO);
			glDrawArrays((GLenum)drawable.primitive, 0, drawable.nVertices);
			glBindVertexArray(0);
		}
	}
	void GL3_Context::draw(const GL3_Framebuffer & buffer)
	{
		setActiveRenderTarget(0);
		setActiveViewport(glState.viewport);

		useShader(unitShader);
		bindTexture(buffer.texture);
		draw(unitDrawable);
	}
	void GL3_Context::draw(const GL3_Texture & texture)
	{
		useShader(unitShader);
		bindTexture(texture);
		draw(unitDrawable);
	}
	void GL3_Context::useShader(const GL3_Shader & shader)
	{
		if (glState.shader != shader.id) {
			glUseProgram(shader.id);
			glState.shader = shader.id;
		}
	}
	void GL3_Context::bindTexture(const GL3_Texture & texture, int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture((GLenum)texture.target, texture.id);
	}

	void GL3_Context::useUnitShader()
	{
		useShader(unitShader);
	}
	void GL3_Context::useUnitTransformShader(gml::Matrix4D<float> transform)
	{
		useShader(unitTransformShader);
		unitTransformShader.setUniform("transform", transform);
	}
	void GL3_Context::drawUnitDrawable()
	{
		draw(unitDrawable);
	}
}