/******************************************


  Lectura de 

  julian.flores@usc.es

*/
#pragma warning(disable:4996)
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLint vUniformeUmbral;
GLuint vertexShader,fragmentShader, progShader;



//Lectura de Ficheros
char *textFileRead(const char *fn);
int textFileWrite(const char *fn, const char *s);

char *textFileRead(const char *fn) {


	FILE *fp;
	char *content = nullptr;

	int count = 0;
	
	if (fn != nullptr) {
		fp = fopen(fn, "rt");

		if (fp != nullptr) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else { printf("NO HE ENCONTRADO EL SHADER, PULSA UNA TECLA"); getchar(); exit (0); }
	}
	 	
	return content;
}

int textFileWrite(const char *fn, const char *s) {

	FILE *fp;
	int status = 0;

	if (fn != nullptr) {
		fp = fopen(fn, "w");

		if (fp != nullptr) {

			if (fwrite(s, sizeof(char), strlen(s), fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}



#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d\n", file, line);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}



GLuint setShaders(const char *nVertx, const char *nFrag) {

	char *ficherovs = nullptr;
	char *ficherofs = nullptr;
	const char * codigovs = nullptr;
	const char * codigofs = nullptr;

	//Creo el vertexShader y el FragmentShader
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//Leo el codigo del ficheo
	ficherovs = textFileRead(nVertx), 
	ficherofs = textFileRead(nFrag);

	//Lo igual al puntero para cargarlos
	codigovs = ficherovs;
	codigofs = ficherofs;
    //Los cargo
	glShaderSource(vertexShader, 1, &codigovs,nullptr);
	glShaderSource(fragmentShader, 1, &codigofs,nullptr);

	//Libero los ficheros
	free(ficherovs);free(ficherofs);

	// Copio vertex y Fragment
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//Miro si hay algun error
	printShaderInfoLog(vertexShader);
	printShaderInfoLog(fragmentShader);

	//Creo el programa asociado
	progShader = glCreateProgram();

	// Le attacheo shaders al programa
	glAttachShader(progShader,vertexShader);
	glAttachShader(progShader,fragmentShader);

	// Lo linko
	glLinkProgram(progShader);
	// A ver si hay errores
	printProgramInfoLog(progShader);

	//Lo uso
	//glUseProgram(progShader);
	return (progShader);

}



