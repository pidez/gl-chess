#include "Program.h"
#include "glm/glm.hpp"


	Program::Program() {
		_id = glCreateProgram();
	}

	Program::Program(const Program &other) {

		_id = other._id;

		for (auto s : other._shaders) {
			_shaders.push_back(s);
		}
	}

	Program::~Program() {
	}



	void Program::addShader(Shader shader) {
		_shaders.push_back(shader);
	}

	void Program::link() {

		//attach all the shaders in _shaders;
		for(auto it = _shaders.begin(); it != _shaders.end(); it++) {
			glAttachShader(_id, it->id());
		}


		//link program and check for linking errors
		glLinkProgram(_id);

		GLint isLinked = 0;
		glGetProgramiv(_id, GL_LINK_STATUS, &isLinked);

		if(isLinked != GL_TRUE) {
			GLint maxLength = 0;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

			GLchar message[maxLength];

			glGetProgramInfoLog(_id, maxLength, &maxLength, message);

			std::string log(message);
			std::cout << "Errore nel linking del programma:\n" << log << "\n";
		}

		//delete shaders
		//nota: questo ha effetto sul contesto openGL, non sul contenuto di _shaders
		for(auto it = _shaders.begin(); it != _shaders.end(); it++) {
			glDeleteShader(it->id());
		}
	}

	void Program::use() {
		glUseProgram(_id);
	}

	void Program::unbind() {
		glUseProgram(0);
	}

	void Program::del() {
		glDeleteProgram(_id);
	}

	GLuint Program::id() const {
		return _id;
	}

	void Program::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
	}

	void Program::setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Program::setVec3(const std::string& name, const glm::vec3& vec) const {
		glUniform3f(glGetUniformLocation(_id, name.c_str()), vec.x, vec.y, vec.z);
	}

	void Program::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
	}

	void Program::setMaterial(const Material& material) const {

		setVec3("materialAmbient", material.ambient());
		setVec3("materialDiffuse", material.diffuse());
		setVec3("materialSpecular", material.specular());
		setFloat("shininess", material.shininess());

	}

	void Program::setLightIntensity(float intensity) const{
		setFloat("lightIntensity", intensity);
	}

	void Program::setLightColor(const glm::vec3& color) const {
		setVec3("lightColor", color);
	}

	void Program::setLightPosition(const glm::vec3& position) const {
		setVec3("lightPosition", position);
	}


