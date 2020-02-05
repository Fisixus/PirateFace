
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define PI 3.14159265

enum { InitMode = 0, SingleRotationMode = 1, AnimationMode = 2 };
int Mode = InitMode;

//First 1000 for face
//Second 6 for headband
//Third 16 for smile
//Fourth 60 for left eye
//Fifth 90 for right eye
//Sixth 6 for eye band
const int NumVertices = 1178;

const int n = 6;
bool firstRoll = false;
GLfloat rotatingDegree = 0.0;
GLfloat moveSpeed = 0.0f;

GLfloat faceRadius = 0.4;
GLfloat smileRadius = 0.3;
GLfloat eyeRadius = 0.05;

point4 points[NumVertices];
color4 colors[NumVertices];

GLuint ww = 500;
GLuint wh = 500;

GLuint vao;

//0-500 for face, 501-504 for headband, 505-513 for smile, 514-544 for left eye, 545-575 for right eye, 576-579 for eye band
point4 PirateFace[580] = {
	point4(0.0, 0.0 ,0.0, 1.0)
};

point4 faceCenterPoint = point4(0.0, 0.0, 0.0, 1.0);
point4 smileCenterPoint = point4(0.0, 0.1, 0.0, 1.0);
point4 leftEyeCenterPoint = point4(-0.15, 0.1, 0.0, 1.0);
point4 rightEyeCenterPoint = point4(0.15, 0.1, 0.0, 1.0);

// RGBA colors
color4 vertex_colors[6] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
};


GLuint  theta;  // The location of the "theta" shader uniform variable
GLuint translatebyRadiusValue;
GLuint scale;

GLfloat  ThetaValue[3] = { 0.0, 0.0, 0.0 };
GLfloat  ScaleValue[3] = { 1.0, 1.0, 1.0 };
GLfloat  TranslateByRadiusValue[3] = { 0.0, 0.0, 0.0 };
//----------------------------------------------------------------------------

int Index = 0;
void lines(int a, int b)
{
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[a]; Index++;
	//colors[Index] = vertex_colors[2]; points[Index] = centerPoint; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[b]; Index++;
}

void triangle(int a, int b)
{
	colors[Index] = vertex_colors[0]; points[Index] = PirateFace[a]; Index++;
	colors[Index] = vertex_colors[0]; points[Index] = rightEyeCenterPoint; Index++;
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
	int counter = 0;
	GLfloat angle;
	for (int i = 0; i <= 500; i++)
	{
		angle = 2 * PI * (i + 1) / 499;
		PirateFace[counter].x = faceCenterPoint.x + cos(angle) * faceRadius;
		PirateFace[counter].y = faceCenterPoint.y + sin(angle) * faceRadius;
		PirateFace[counter].z = 0.0;
		PirateFace[counter++].w = 1.0;
		//printf("Pressing left, respectively.X %f\n", PirateFace[i].x);
		//printf("Pressing left, respectively.Y %f\n", PirateFace[i].y);
	}

	for (int i = 0; i < 500; i++)
	{
		lines(i, i + 1);
	}

}

void storeHeadband()
{
	PirateFace[501].x = 0.32f;
	PirateFace[501].y = 0.25f;
	PirateFace[501].z = 0.0f;
	PirateFace[501].w = 1.0f;

	PirateFace[502].x = 0.32f;
	PirateFace[502].y = 0.27f;
	PirateFace[502].z = 0.0f;
	PirateFace[502].w = 1.0f;

	PirateFace[503].x = -0.32f;
	PirateFace[503].y = 0.27f;
	PirateFace[503].z = 0.0f;
	PirateFace[503].w = 1.0f;

	PirateFace[504].x = -0.32f;
	PirateFace[504].y = 0.25f;
	PirateFace[504].z = 0.0f;
	PirateFace[504].w = 1.0f;

	quad(501, 502, 503, 504);

}

void storeSmile()
{
	int counter = 505;
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

	for (int i = 505; i < 513; i++)
	{
		lines(i, i + 1);
	}
}

void storeLeftEye()
{
	int counter = 514;
	GLfloat angle;
	for (int i = 0; i <= 30; i++)
	{
		angle = 2 * PI * (i + 1) / 29;
		PirateFace[counter].x = leftEyeCenterPoint.x + cos(angle) * eyeRadius;
		PirateFace[counter].y = leftEyeCenterPoint.y + sin(angle) * eyeRadius;
		PirateFace[counter].z = 0.0;
		PirateFace[counter++].w = 1.0;
		//printf("Pressing left, respectively.X %f\n", PirateFace[i].x);
		//printf("Pressing left, respectively.Y %f\n", PirateFace[i].y);
	}

	for (int i = 514; i < 544; i++)
	{
		lines(i, i + 1);
	}
}

void storeRightEye()
{
	int counter = 545;
	GLfloat angle;
	for (int i = 0; i <= 30; i++)
	{
		angle = 2 * PI * (i + 1) / 29;
		PirateFace[counter].x = rightEyeCenterPoint.x + cos(angle) * eyeRadius;
		PirateFace[counter].y = rightEyeCenterPoint.y + sin(angle) * eyeRadius;
		PirateFace[counter].z = 0.0;
		PirateFace[counter++].w = 1.0;
		//printf("Pressing left, respectively.X %f\n", PirateFace[i].x);
		//printf("Pressing left, respectively.Y %f\n", PirateFace[i].y);
	}

	for (int i = 545; i < 575; i++)
	{
		triangle(i, i + 1);
	}
}

void storeEyeBand()
{
	//a0
	PirateFace[576].x = 0.18f;
	PirateFace[576].y = 0.25f;
	PirateFace[576].z = 0.0f;
	PirateFace[576].w = 1.0f;
	//a1
	PirateFace[577].x = 0.12f;
	PirateFace[577].y = 0.25f;
	PirateFace[577].z = 0.0f;
	PirateFace[577].w = 1.0f;
	//a2
	PirateFace[578].x = 0.12f;
	PirateFace[578].y = 0.10f;
	PirateFace[578].z = 0.0f;
	PirateFace[578].w = 1.0f;
	//a3
	PirateFace[579].x = 0.18f;
	PirateFace[579].y = 0.10f;
	PirateFace[579].z = 0.0f;
	PirateFace[579].w = 1.0f;

	quad(576, 577, 578, 579);
}

void fillPointsandColors()
{
	storeFace();
	storeHeadband();
	storeSmile();
	storeLeftEye();
	storeRightEye();
	storeEyeBand();
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);
	glUniform3fv(theta, 1, ThetaValue);
	glUniform3fv(translatebyRadiusValue, 1, TranslateByRadiusValue);
	glUniform3fv(scale, 1, ScaleValue);

	glDrawArrays(GL_LINE_LOOP, 0, 1000);
	glDrawArrays(GL_TRIANGLES, 1000, 6);
	glDrawArrays(GL_LINES, 1006, 16);
	glDrawArrays(GL_LINE_LOOP, 1022, 60);
	glDrawArrays(GL_TRIANGLES, 1082, 90);
	glDrawArrays(GL_TRIANGLES, 1172, 6);

	glutSwapBuffers();
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
	translatebyRadiusValue = glGetUniformLocation(program, "translatebyRadiusValue");
	scale = glGetUniformLocation(program, "scale");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
void animationMode(int id)
{
	if (Mode != AnimationMode) return;
	rotatingDegree += 360.0 / n;
	moveSpeed += faceRadius/2;
	if (firstRoll) rotatingDegree = 0.0f;
	if (rotatingDegree > 360) return;

	//Scale 1/2
	ScaleValue[0] = 0.5f;
	ScaleValue[1] = 0.5f;
	ScaleValue[2] = 0.5f;

	//Rotate
	ThetaValue[0] = 0;
	ThetaValue[1] = 0;
	ThetaValue[2] = rotatingDegree * -1;

	//Translate Left Screen
	TranslateByRadiusValue[0] = moveSpeed;
	TranslateByRadiusValue[1] = 0;
	TranslateByRadiusValue[2] = 0;

	firstRoll = false;
	glutPostRedisplay();
	glutTimerFunc(250, animationMode, 0);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'a': case 'A':
/*
		faceRadius = 0.4;
		smileRadius = 0.3;
		eyeRadius = 0.05;
		faceRadius *= 0.5f;
		smileRadius *= 0.5f;
		eyeRadius *= 0.5f;
*/

		firstRoll = true;
		rotatingDegree = 0;
		moveSpeed = -0.9;
		Mode = AnimationMode;
		glutTimerFunc(250, animationMode, 0);
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
	glutTimerFunc(250, animationMode, 0);

	glutMainLoop();
	return 0;
}
