
// GAME2012 - Assignment1.cpp by Yaniv Milman 101270509
//
// Assignment 1 submission.
//
// Description:
// Click run to see the results.
//have both maths for any angle as well as 45% angle inly  (commented)

#include <iostream>
#include "stdlib.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
#include <string>
#include "LoadShaders.h"


int squareNum;
float angleNum;
//void timer(int);
#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint Buffers[1];

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

//enum VAO_IDs { Triangles, NumVAOs };
//enum Buffer_IDs { ArrayBuffer, NumBuffers };
//enum Attrib_IDs { vPosition = 0 };

GLuint vao, points_vbo, colors_vbo, modelID;

//const GLfloat scale = 0.5f;

GLfloat points[] = {
	-0.9f,  0.9f,  0.0f,
	0.9f,  0.9f,  0.0f,
	0.9f, -0.9f,  0.0f,
	-0.9f, -0.9f,  0.0f
};

GLfloat colors[] = {
	0.07f, 0.0f, 0.0f,
	0.0f, 0.07f, 0.0f,
	0.0f, 0.0f, 0.07f,
	0.0f, 0.07f, 0.07f
};

static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{
	//ShaderInfo shaders[] = {
	//	{ GL_VERTEX_SHADER, "projection.vert" },
	//	{ GL_FRAGMENT_SHADER, "square.frag" },
	//	{ GL_NONE, NULL }
	//};
	////Loading and compiling shaders
	//GLuint program = LoadShaders(shaders);
	//glUseProgram(program);	//My Pipeline is set up
	

	vertexShaderId = setShader((char*)"vertex", (char*)"cube.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	glClearColor(1.0, 1.0, 1.0, 0.0);

	modelID = glGetUniformLocation(program, "model");	
	/*childID = glGetUniformLocation(program, "child");*/

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.
//	timer(0);
}

//---------------------------------------------------------------------
//
// transformModel
//

void transformObject(float scale, glm::vec3 rotationAxis,
	float rotationAngle, glm::vec3 translation)
{
	//glm::mat4 Child; 
	//Child = glm::mat4(0.7f);
	//Child = glm::translate(Child, translation);
	//Child = glm::rotate(Child, glm::radians(rotationAngle), rotationAxis);
	//Child = glm::scale(Child, glm::vec3(scale));
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
	/*glUniformMatrix4fv(childID, 1, GL_FALSE, &Child[0][0]);*/
}

void recolorSquare(void)
{
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;

	float colors[] = {
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b
	};

	glBindVertexArray(colors_vbo); // Can optionally unbind the vertex array to avoid modification.
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); // Why can we do this without binding again?
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/
	glBindVertexArray(points_vbo); // Can optionally unbind the vertex array to avoid modification.
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);
	
	float ScaleNumber = 1.0f;
	//float ScaleNumber = .1f;  //use this when you want 45% roation only 
	float aSide;
	float angleTan;
	float ScaleNew;
	float oppSide;

		for (int i = 0; i < squareNum; i++)
		{
			angleTan = tan(glm::radians(angleNum));
			aSide = ScaleNumber * angleTan / (angleTan + 1);  //substatution + wolfram alpha to simplify 
			oppSide = ScaleNumber - aSide; //opp + adj  = scalenumber this is a + b  = xoriginal 
			ScaleNew = sqrt(aSide * aSide + oppSide * oppSide);

			//ScaleNew = sqrt((ScaleNumber * ScaleNumber) + (ScaleNumber * ScaleNumber)); //Use this for 45 degree only 
		
			transformObject(ScaleNumber, Z_AXIS, -(angleNum* i), glm::vec3(0.0f, 0.0f, 0.0f));
			recolorSquare();
			
	/*	glDrawArrays(GL_QUADS, 0, 4);*/
			/*ScaleNumber = tempvar;*/
			ScaleNumber = ScaleNew;
		
			
		}
	
	glFlush(); // Instead of double buffering.
}

void idle()
{
	//glutPostRedisplay();
}
//
//void timer(int) {
//	glutPostRedisplay();
//	glutTimerFunc(1000 / FPS, timer, 0); // 60 FPS or 16.67ms.
//}

int main(int argc, char** argv)
{
	std::cout << "How many squares do you want?" << std::endl;
	std::cin >> squareNum;
	std::cout << "Please input an angle of Rotation" << std::endl;
	std::cin >> angleNum;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Yaniv Milman 101270509");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
}
