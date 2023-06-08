#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <vector>

#include "Shader.h"
#include "Material.h"

#include "glm/glm.hpp"

class Program {

public:

	Program();

	Program(const Program &other);

	~Program();

private:

	GLuint _id; ///< ID openGL del programma.
 
	std::vector<Shader> _shaders; ///< Vettore contente gli shader che saranno attaccati al programma.

public:

	/***********************************************
	 * Funzione che aggiunge un oggetto di tipo Shader
	 * alla lista di shaders _shaders.
	 * NOTA: gli oggetti Shader sono già compilati e pronti all'uso.
	 ***********************************************/
	void addShader(Shader shader);


	/************************************************
	 * Funzione che aggiunge gli shader specificati in _shaders
	 * al programma, linka il programma e poi 
	 ************************************************/
	void link();


	/***********************************************
	 * Funzione che rende il programma corrente attivo 
	 * per l'uso chiamando glUseProgram(GLint program)
	 ***********************************************/
	void use();

	/* scollega il programma dal contesto opengl */
	void unbind();


	/************************************************
	 * Funzione che elimina il programma dal contesto openGL
	 ***********************************************/
	void del();

	GLuint id() const;

		/*******************************************************
	 * Funzione che imposta una varibile uniform di tipo intero
	 * per lo shader.
	*******************************************************/
	void setInt(const std::string& name, int value) const;

	void setMat4(const std::string& name, const glm::mat4& mat) const;

	void setVec3(const std::string& name, const glm::vec3& vec) const;

	void setFloat(const std::string& name, float value) const;

	//Si settano gli uniform contenenti le informazioni riguardo al materiale
	// i nomi per le variabili uniform sono:
	// 	materialDiffuse, materialAmbient, materialSpecular
	void setMaterial(const Material& material) const;

	//Set shader lightIntensity
	void setLightIntensity(float intensity) const;

	//Set shader lightColor
	void setLightColor(const glm::vec3& color) const;

	void setLightPosition(const glm::vec3& position) const;
};

#endif