
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define PI 3.14159265

//First 60 for face
//Second 6 for headband
//Third 18 for smile
const int NumVertices = 84;
GLfloat faceRadius = 0.4;

GLfloat smileRadius = 0.3;
GLfloat eyeRadius = 0.1;

point4 points[NumVertices];
color4 colors[NumVertices];

GLuint ww = 500;
GLuint wh = 500;

GLuint vao;

//0-30 for face, 31-34 for headband, 35-43 for smile
point4 PirateFace[45] = {
	point4(0.0, 0.0 ,0.0, 1.0)
};

point4 faceCenterPoint = point4(0.0, 0.0, 0.0, 1.0);
point4 smileCenterPoint = point4(0.0, 0.1, 0.0, 1.0);
//point4 leftEyeCenterPoint = point4(0.0, 0.0, 0.0, 1.0);
//point4 rightEyeCenterPoint = point4(0.0, 0.0, 0.0, 1.0);

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
void lines(int a, int b)
{
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[a]; Index++;
	//colors[Index] = vertex_colors[2]; points[Index] = centerPoint; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[b]; Index++;
}

void quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[a]; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[b]; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[c]; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[a]; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[c]; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[d]; Index++;
}
//----------------------------------------------------------------------------

void storeFace()
{
	GLfloat angle;
	for (int i = 0; i <= 30; i++)
	{
		angle = 2 * PI * (i + 1) / 29;
		PirateFace[i].x = faceCenterPoint.x + cos(angle) * faceRadius;
		PirateFace[i].y = faceCenterPoint.y + sin(angle) * faceRadius;
		PirateFace[i].z = 0.0;
		PirateFace[i].w = 1.0;
		//printf("Pressing left, respectively.X %f\n", PirateFace[i].x);
		//printf("Pressing left, respectively.Y %f\n", PirateFace[i].y);
	}

	for (int i = 0; i < 30; i++)
	{
		lines(i, i + 1);
	}

}

void storeHeadband()
{
	PirateFace[31].x = 0.32f;
	PirateFace[31].y = 0.25f;
	PirateFace[31].z = 0.0f;
	PirateFace[31].w = 1.0f;

	PirateFace[32].x = 0.32f;
	PirateFace[32].y = 0.27f;
	PirateFace[32].z = 0.0f;
	PirateFace[32].w = 1.0f;

	PirateFace[33].x = -0.32f;
	PirateFace[33].y = 0.27f;
	PirateFace[33].z = 0.0f;
	PirateFace[33].w = 1.0f;

	PirateFace[34].x = -0.32f;
	PirateFace[34].y = 0.25f;
	PirateFace[34].z = 0.0f;
	PirateFace[34].w = 1.0f;

	quad(31, 32, 33, 34);

}

void storeSmile()
{
	int counter = 35;
	GLfloat angle;
	for (int i = 17; i <= 25; i++)
	{
		angle = 2 * PI * (i + 1) / 29;
		PirateFace[counter].x = smileCenterPoint.x + cos(angle) * smileRadius;
		PirateFace[counter].y = smileCenterPoint.y + sin(angle) * smileRadius;
		PirateFace[counter].z = 0.0;
		PirateFace[counter++].w = 1.0;
		//printf("Pressing left, respectively.X %f\n", PirateFace[i].x);
		//printf("Pressing left, respectively.Y %f\n", PirateFace[i].y);
	}

	for (int i = 35; i <= 43; i++)
	{
		lines(i, i + 1);
	}
}

void fillPointsandColors()
{
	storeFace();
	storeHeadband();
	storeSmile();
	
}

//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
	fillPointsandColors();

	//printf("Pressing left, respectively.\n");;
	// Create a vertex array object
	
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);
	glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_LINE_LOOP, 0, NumVertices-24);
	glDrawArrays(GL_TRIANGLES, NumVertices - 24, NumVertices-18);
	glDrawArrays(GL_LINES, NumVertices - 18, NumVertices);

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
void reshapeFunc(GLsizei w, GLsizei h)
{
	/* adjust clipping box */


	/* adjust viewport and clear */

	glViewport(0, 0, ww*h / wh, h);

	/* set global size for use by drawing routine */

	//ww = w;
	//wh = h;
	//resetGame();
	glutPostRedisplay();
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
	glutReshapeFunc(reshapeFunc);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}
