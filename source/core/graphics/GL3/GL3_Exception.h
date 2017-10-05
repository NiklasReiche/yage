#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

#include "gl3.h"

namespace gl3
{
	enum GlfwErrorCodes {
		GLFW_ERROR
	};

	class GL3_Exception : public std::runtime_error {
	private:
		std::string message;
		std::string buildMessage(std::string message) {
			std::stringstream s;
			s << message << std::endl;
			return s.str();
		}
	public:
		GL3_Exception(std::string message)
			: runtime_error(""), message(buildMessage(message)) {};

		virtual const char* what() const throw() {
			return message.c_str();
		};
	};

	class GlewException : public GL3_Exception {
	private:
		std::string buildMessage(GLenum err, std::string message) {
			std::stringstream s;
			s << message << std::endl;
			s << (stderr, "Error: %s\n", (err)) << std::endl;
			return s.str();
		}
	public:
		GlewException(GLenum err, std::string message)
			: GL3_Exception(buildMessage(err, message)) {};
	};

	class GlfwException : public GL3_Exception {
	private:
		std::string buildMessage(GlfwErrorCodes err, std::string message) {
			std::stringstream s;
			s << message << std::endl;
			s << (stderr, "Error: %s\n", (err)) << std::endl;
			return s.str();
		}
	public:
		GlfwException(GlfwErrorCodes err, std::string message)
			: GL3_Exception(buildMessage(err, message)) {};
	};
}