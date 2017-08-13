#include "GLES2_Context.h"

namespace gles2
{
	InternalFormat GLES2_Context::convertToInternalFormat(ImageFormat format)
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
	int GLES2_Context::convertToChannelSize(ImageFormat format)
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
	int GLES2_Context::checkShaderCompilationError(GLuint program, std::string type)
	{
		int success;
		char infoLog[512];
		if (type == "VERTEX_SHADER") {
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(program, 512, NULL, infoLog);
				systemHandle->log(infoLog);
				//std::cout << "ERROR: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		else if (type == "FRAGMENT_SHADER") {
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(program, 512, NULL, infoLog);
				systemHandle->log(infoLog);
				//std::cout << "ERROR: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		else if (type == "SHADER_PROGRAM") {
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(program, 512, NULL, infoLog);
				systemHandle->log(infoLog);
				//std::cout << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
				return -1;
			}
		}
		return 0;
	}


	GLES2_Context::GLES2_Context(sys::PlatformHandle* systemHandle)
		: systemHandle(systemHandle)
	{
		systemHandle->createContext(&width, &height);

		makeCurrent();

		// --- Internal initialization ---
		GLES2_UnitShaderTemplate UnitShaderTemplate;
		unitShader = compileShader(UnitShaderTemplate.vertexCode, UnitShaderTemplate.fragmentCode, UnitShaderTemplate.shaderAttributes);
		unitShader.setUniform("screenTexture", 0);
		unitDrawable = createDrawable(UnitShaderTemplate.vertices, UnitShaderTemplate.vertexLayout, UnitShaderTemplate.mode);
		
		glState.window_width = width;
		glState.window_height = height;
	}
	GLES2_Context::~GLES2_Context()
	{

	}


	void GLES2_Context::makeCurrent()
	{
		systemHandle->makeCurrent();
		
	}
	void GLES2_Context::swapBuffers()
	{
		systemHandle->swapBuffers();
	}


	void GLES2_Context::setActiveViewport(const GLES2_Viewport & viewport)
	{
		glViewport(viewport.x, glState.renderTarget_height - (viewport.y + viewport.height), viewport.width, viewport.height);
		glState.viewport = viewport;
	}
	void GLES2_Context::setActiveRenderTarget(const GLES2_Framebuffer & framebuffer)
	{
		if (glState.renderTarget != framebuffer.FBO) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.FBO);
			glState.renderTarget_height = framebuffer.height;
			glState.renderTarget_width = framebuffer.width;
			glState.renderTarget = framebuffer.FBO;
		}
	}
	void GLES2_Context::setActiveRenderTarget(GLuint FBO)
	{
		if (glState.renderTarget != FBO) {
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glState.renderTarget_height = glState.window_height;
			glState.renderTarget_width = glState.window_width;
			glState.renderTarget = FBO;
		}
	}
	void GLES2_Context::setPackAlignment(int value)
	{
		if (value != glState.pack_alignment) {
			glPixelStorei(GL_PACK_ALIGNMENT, value);
		}
	}
	void GLES2_Context::setUnpackAlignment(int value)
	{
		if (value != glState.unpack_alignment) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, value);
		}
	}
	void GLES2_Context::enableDepthTest()
	{
		if (glState.isDepthTestEnabled == false) {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glState.isDepthTestEnabled = true;
		}
	}
	void GLES2_Context::disableDepthTest()
	{
		if (glState.isDepthTestEnabled) {
			glDisable(GL_DEPTH_TEST);
			glState.isDepthTestEnabled = false;
		}
	}
	void GLES2_Context::enableStencilTest()
	{
		if (glState.isStencilTestEnabled == false) {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glState.isStencilTestEnabled = true;
		}
	}
	void GLES2_Context::disableStencilTest()
	{
		if (glState.isStencilTestEnabled) {
			glDisable(GL_STENCIL_TEST);
			glState.isStencilTestEnabled = true;
		}
	}
	void GLES2_Context::enableBlending()
	{
		if (glState.isBlendingEnabled == false) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glState.isBlendingEnabled = true;
		}
	}
	void GLES2_Context::disableBlending()
	{
		if (glState.isBlendingEnabled) {
			glDisable(GL_BLEND);
			glState.isBlendingEnabled = false;
		}
	}


	GLES2_Framebuffer GLES2_Context::createFramebuffer(GLint width, GLint height)
	{
		GLES2_Framebuffer buffer;
		// Create Framebuffer Object
		glGenFramebuffers(1, &buffer.FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.FBO);

		// Create empty Texture
		glGenTextures(1, &buffer.colorTexture);
		glBindTexture(GL_TEXTURE_2D, buffer.colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach Texture to FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.colorTexture, 0);

		// Depth & Stencil Buffers
		//glGenRenderbuffers(1, &buffer.RBO_D);
		//glBindRenderbuffer(GL_RENDERBUFFER, buffer.RBO_D);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer.RBO_D);

		//glGenRenderbuffers(1, &buffer.RBO_S);
		//glBindRenderbuffer(GL_RENDERBUFFER, buffer.RBO_S);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.RBO_S);

		// Completeness check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::stringstream msg;
			msg << (int)glCheckFramebufferStatus(GL_FRAMEBUFFER);
            std::string what = msg.str();
            systemHandle->log(what);
			throw;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		buffer.glContext = this;
		buffer.id = buffer.FBO;

		buffer.height = height;
		buffer.width = width;
		return buffer;
	}

	GLES2_Drawable GLES2_Context::createDrawable(std::vector<GLfloat> & vertices, std::vector<int> vertexLayout, DrawMode usage, VertexFormat format)
	{
		GLES2_Drawable drawable;
		// Generate
		glGenBuffers(1, &drawable.VBO);
		// Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, drawable.VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], (GLenum)usage);
		// Config Vertex Attributes
		int vertexSize = std::accumulate(vertexLayout.begin(), vertexLayout.end(), 0);
		int nVertices = (int)vertices.size() / vertexSize;

		if (format == VertexFormat::INTERLEAVED) {
			int attribOffset = 0;
			for (unsigned int i = 0; i < vertexLayout.size(); i++) {
				VertexAttrib attrib = {(GLint)i, (GLint)vertexLayout[i], GL_FLOAT, GL_FALSE, (GLsizei)(vertexSize * sizeof(GLfloat)), (int)(attribOffset * sizeof(GLfloat)) };
				drawable.VAO.attributes.push_back(attrib);
				attribOffset += vertexLayout[i];
			}
		}
		else if (format == VertexFormat::BATCHED) {
			int attribOffset = 0;
			for (unsigned int i = 0; i < vertexLayout.size(); i++) {
				VertexAttrib attrib = { (GLint)i, (GLint)vertexLayout[i], GL_FLOAT, GL_FALSE, (GLsizei)(vertexLayout[i] * sizeof(GLfloat)), (int)(attribOffset * sizeof(GLfloat)) };
				drawable.VAO.attributes.push_back(attrib);
				attribOffset += vertexLayout[i] * nVertices;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		drawable.glContext = this;
		drawable.id = drawable.VBO;

		drawable.nVertices = nVertices;
		drawable.usage = usage;
		drawable.primitive = PrimitiveType::TRIANGLES;
		return drawable;
	}
	void GLES2_Context::createDrawable(GLES2_Drawable & drawable, std::vector<GLfloat> & vertices, std::vector<int> vertexLayout, DrawMode usage, VertexFormat format)
	{
		drawable = createDrawable(vertices, vertexLayout, usage, format);;
	}

	GLES2_Texture GLES2_Context::create2DTexture(unsigned char * image, int width, int height, ImageFormat format, int rowAlignment)
	{
		// Generate & Bind
		GLES2_Texture texture;
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
	void GLES2_Context::create2DTexture(GLES2_Texture & texture, unsigned char * image, int width, int height, ImageFormat format, int rowAlignment)
	{
		texture = create2DTexture(image, width, height, format, rowAlignment);
	}

	GLES2_Texture GLES2_Context::createCubemapTexture(std::array<unsigned char*, 6> images, int width, int height)
	{
		// Generate & Bind
		GLES2_Texture texture;
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

		for (GLuint i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]);
		}

		// Set Image Wrapper
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Set Image Filter
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Unbind
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		texture.glContext = this;
		texture.target = TextureType::TEXTURE_CUBEMAP;
		texture.height = height;
		texture.width = width;

		return texture;
	}
	void GLES2_Context::createCubemapTexture(GLES2_Texture & texture, std::array<unsigned char*, 6> images, int width, int height)
	{
		texture = createCubemapTexture(images, width, height);
	}

	GLES2_Shader GLES2_Context::compileShader(std::string vertexCode, std::string fragmentCode, const std::vector<std::string> & attributes)
	{
		GLES2_Shader shader;

		GLuint vertexShader, fragmentShader;
		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

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

		// Link Program
		shader.id = glCreateProgram();
		glAttachShader(shader.id, vertexShader);
		glAttachShader(shader.id, fragmentShader);

		// Bind vertex attributes to location
		for (unsigned int i = 0; i < attributes.size(); ++i)
		{
			glBindAttribLocation(shader.id, i, attributes[i].c_str());
		}

		glLinkProgram(shader.id);
		checkShaderCompilationError(shader.id, "SHADER_PROGRAM");
		// Delete Shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		shader.glContext = this;

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


	void GLES2_Context::draw(const GLES2_Drawable & drawable)
	{
		glBindBuffer(GL_ARRAY_BUFFER, drawable.VBO);

		for (auto &attrib : drawable.VAO.attributes)
		{
			glVertexAttribPointer(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, (GLfloat*)attrib.pointer);
			glEnableVertexAttribArray(attrib.index);
		}

		glDrawArrays((GLenum)drawable.primitive, 0, drawable.nVertices);

		for (auto &attrib : drawable.VAO.attributes)
		{
			glDisableVertexAttribArray(attrib.index);
		}
	}
	void GLES2_Context::draw(const GLES2_Framebuffer & buffer)
	{
		setActiveRenderTarget(0);
		setActiveViewport(glState.viewport);

		useShader(unitShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buffer.colorTexture);

		draw(unitDrawable);
	}

	void GLES2_Context::useShader(const GLES2_Shader & shader)
	{
		if (glState.shader != shader.id) {
			glUseProgram(shader.id);
			glState.shader = shader.id;
		}
	}

	void GLES2_Context::bindTexture(const GLES2_Texture & texture, int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture((GLenum)texture.target, texture.id);
	}
}