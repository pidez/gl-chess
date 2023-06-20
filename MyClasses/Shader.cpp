
#include <string.h>

#include "Shader.h"


	Shader::Shader(GLenum shader_type) : _type(shader_type) {
		_id = glCreateShader(shader_type);
	}


	Shader::Shader(const Shader &other) {

		_id = other._id;
		_type = other._type;
		_body = other._body;
		_compilation_log = other._compilation_log;
	}

	Shader::~Shader() {}


	void Shader::load_shader(const std::string &shaderpath) {
		std::fstream strm;
		strm.open(shaderpath, std::fstream::in);

		if(strm.is_open()) {
			while(strm){
				std::string line;
				std::getline(strm, line);
				_body.append(line);
				_body.append("\n");
			}
		} else {
			std::cout << "Errore nell'apertura del file.";
		}

		strm.close();

	}


	void Shader::compile(const std::string &shaderpath) {
		load_shader(shaderpath);


		//cast the source string to const GLchar*
		const GLchar *shader_source = (const GLchar *) _body.c_str();

		//Set shader source
		glShaderSource(_id, 1, &shader_source, 0);

		glCompileShader(_id);

		//Check compilation status
		GLint compiled;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);

		if(compiled != GL_TRUE) {
			//get compilation log
			GLchar compilation_message[1024];
			GLsizei log_length = 0;

			glGetShaderInfoLog(_id, 1024, &log_length, compilation_message);

			std::string log(compilation_message);
			std::cout << "Errore nella compilazione dello shader: " << _type << "\n" << log << "\n";

			_compilation_log = log;

		}
	}

	std::string Shader::body() const{
		return _body;
	}

	GLuint Shader::id() const {
		return _id;
	}

	std::string Shader::compilation_log() const {
		return _compilation_log;
	}

	GLenum Shader::type() const {
		return _type;
	}



