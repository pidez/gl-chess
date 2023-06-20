#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

class Shader {

public:

	/************************************************************
	 * Costruttore che crea uno shader del tipo specificato
	 * da shader_type e ne memorizza l'id 
	 ************************************************************/
	Shader(GLenum shader_type);

	/************************************************************
	 * Costruttore di copia
	 * Quando uno shader viene copiato, viene copiato anche il log
	 * dell'ultima compilazione.
	 ***********************************************************/
	Shader(const Shader &other);

	~Shader();

private:

	GLuint _id; ///< ID openGL dello shader.
	GLenum _type; ///< tipo openGL dello shader.

	std::string _body; ///< Sorgente dello shader.
	std::string _compilation_log; ///< Log dell'ultima compilazione dello shader.

	/****************************************************
	 * Funzione ausiliaria che legge il sorgente dello shader
	 * dal percorso specificato e lo memorizza in _body
	 ****************************************************/
	void load_shader(const std::string &shaderpath);

public:

	
	/******************************************************************************
	 * Metodo che carica il sorgente dello shader da shaderpath e 
	 * compila lo shader. In caso di errori di compilazione il log
	 * viene stampato su std::cout e memorizzato in _compilation_log.
	 *****************************************************************************/
	void compile(const std::string &shaderpath);


	/******************************************************
	* Funzione che ritorna il sorgente dello shader
	******************************************************/
	std::string body() const;


	/******************************************************
	* Funzione che ritorna l'id openGL dello shader.
	******************************************************/
	GLuint id() const;

	GLenum type() const;


	/******************************************************
	* Funzione che ritorna il log dell'ultima compilazione
	* dello shader.
	******************************************************/
	std::string compilation_log() const;



};

#endif