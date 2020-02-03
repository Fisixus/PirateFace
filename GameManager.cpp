
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define PI 3.14159265
const int NumVertices = 90;
GLfloat radius = 0.4;

point4 points[NumVertices];
color4 colors[NumVertices];

GLuint ww = 500;
GLuint wh = 500;

point4 PirateFace[NumVertices] = {
	point4(0.0, 0.0 ,0.0, 1.0) //a0

	/*

		point4(0.0, 0.0 ,0.0, 1.0), //a0
		point4(0.0, 0.4 ,0.0, 1.0), //a1
		point4(0.1, 0.4 ,0.0, 1.0), //a2
		point4(0.1, 0.1 ,0.0, 1.0), //a3
		point4(0.2, 0.1 ,0.0, 1.0), //a4
		point4(0.2, 0.0 ,0.0, 1.0), //a5
		*/
};

point4 centerPoint = point4(0.0, 0.0, 0.0, 1.0);

// RGBA colors
color4 vertex_colors[6] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
};

GLfloat  Theta[3] = { 0.0, 0.0, 0.0 };
GLuint  theta;  // The location of the "theta" shader uniform variable

//----------------------------------------------------------------------------

int Index = 0;
void quad(int a, int b, int c)
{
	colors[Index] = vertex_colors[2]; points[Index] = PirateFace[a]; Index++;
	colors[Index] = vertex_colors[2]; points[Index] = PirateFace[b]; Index++;
	colors[Index] = vertex_colors[2]; points[Index] = PirateFace[c]; Index++;
}

//----------------------------------------------------------------------------

void fillPointsandColors()
{
	GLfloat angle;
	for(int i=0;i<30;i++)
	{
		angle = 2 * PI * i / 30;
		PirateFace[i].x = (centerPoint.x + cos(angle)) * radius;
		PirateFace[i].y = (centerPoint.y + sin(angle)) * radius;
		quad(0, i + 1, i + 2);
		//printf("Pressing left, respectively. %f\n", PirateFace[i].x);
	}
/*
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);*/
}

//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
	fillPointsandColors();

	//printf("Pressing left, respectively.\n");;
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	theta = glGetUniformLocation(program, "theta");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:     break;
		case GLUT_MIDDLE_BUTTON:   break;
		case GLUT_RIGHT_BUTTON:    break;
		}
	}
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);

	// set OpenGL context to 3.1 or 3.2 and 
	// set profile to core
	// deprecated functions will not be available in core profile
	// the other option is GLUT_COMPATIBILITY PROFILE which let's us use deprecated functionality if
	// vendor implementation provide it
	//glutInitContextVersion(3, 1);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	//create graphics window
	glutCreateWindow("Pirate Face");

	//include the following statement due to an error in GLEW library
	glewExperimental = GL_TRUE;

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}
