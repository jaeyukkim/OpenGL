#include <stdio.h>
#include <windows.h>					// standard Windows app include
#include <GL/glut.h>
#include <gl/gl.h>						// standard OpenGL include
#include <gl/glu.h>						// OpenGL utilties
#include <stdlib.h>
#include <math.h>
#include <GL/glaux.h>
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")


int change_moniter = 1;

unsigned int MyTextureObject[1];
AUX_RGBImageRec* pTextureImage[1];  //텍스쳐 저장 공간을 가리키는 포인터

char tmp[20] = "sky.bmp";

////// Global Variables
float angle = 0.0f;						// current angle of the rotating triangle
static int SpinAngle = 0;
bool fullScreen = false;


////// Robot variables
float legAngle[2] = { 0.0f, 0.0f };		// each leg's current angle
float armAngle[2] = { 0.0f, 0.0f };


GLfloat DeltaX = 0.1f;
GLfloat DeltaY = 0.1f;
GLfloat DeltaZ = 0.1f;
GLfloat Speed_DeltaX = 0.1f;
GLfloat Speed_DeltaZ = 0.1f;
GLfloat Speed_DeltaY = 0.1f;


void draw_floor(float xPos, float yPos, float zPos);
AUX_RGBImageRec* LoadBMP(char* szFilename) {
	FILE* pFile = NULL;
	if (!szFilename) {
		return NULL;
	}
	pFile = fopen(szFilename, "r");
	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(szFilename);     //파일로부터 메모리로
	}
	return NULL;
}

int LoadGLTextures(char* szFilePath) {       //파일을 로드하고 텍스쳐로 변환
	int Status = FALSE;
	glClearColor(0.0, 0.0, 0.0, 0.5);
	memset(pTextureImage, 0, sizeof(void*) * 1);    //포인터를 널로

	if (pTextureImage[0] = LoadBMP(szFilePath)) {   //비트맵을 로드하고 오류확인
		Status = TRUE;                              //상태 플랙을 True로
		glGenTextures(1, &MyTextureObject[0]);      //텍스쳐 생성
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3,
			pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}
	if (pTextureImage[0]) {                 //텍스쳐가 존재하면
		if (pTextureImage[0]->data) {       //텍스쳐 영상이 존재하면
			free(pTextureImage[0]->data);   //텍스쳐 영상공간 반납
		}
		free(pTextureImage[0]);             //텍스쳐 반납
	}
	return Status;
}




void DrawSun()
{
	GLfloat LightPosition[] = { 0.0, 0.0, 50.0, 1.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glTranslatef(0.0, 0.0, -5.0);
		glPushMatrix();
			glColor3f(1.0, 0.0, 0.0);
			glRotatef(SpinAngle, 1.0, 0.0, 0.0);
			glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
			glTranslatef(0.0, 0.0, 50.0);
			glDisable(GL_LIGHTING);
			glColor3f(0.9, 0.9, 0.9);
			glutWireSphere(3.0, 10, 10);
			glEnable(GL_LIGHTING);
		glPopMatrix();
	glPopMatrix();
	glFlush();
}


void DrawCube(float xPos, float yPos, float zPos, float xScale, float yScale, float zScale)
{

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f * yScale, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);	// top face
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f * zScale);
	glVertex3f(0.0f, 0.0f, 0.0f);	// front face
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();

	// right face
	glBegin(GL_POLYGON);
	glNormal3f(1.0f * xScale, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-1.0f * xScale, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);	// left face
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f * yScale, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);	// bottom face
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();

	// back face
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f * zScale);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glEnd();
	glPopMatrix();
}

// DrawArm
// desc: draws one arm
void DrawArm(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.95f, 0.7f, 0.56f);	// red
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 4.0f, 1.0f);		// arm is a 1x4x1 cube
	DrawCube(0.0f, 0.0f, 0.0f, 1.0f, 4.0f, 1.0f);
	glPopMatrix();
}


// DrawHead
// desc: draws the robot head
void DrawHead(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.95f, 0.7f, 0.56f);	// white
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 2.0f, 2.0f);		// head is a 2x2x2 cube
	DrawCube(0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f);
	glPopMatrix();
}

void DrawHair(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.0f);	// white
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 0.5f, 2.0f);		// head is a 2x2x2 cube
	DrawCube(0.0f, 0.0f, 0.0f, 2.0f, 0.5f, 2.0f);
	glPopMatrix();
}

void DrawEye(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);	
	glTranslatef(xPos, yPos, zPos);
	glScalef(0.5f, 0.2f, 0.1f);		
	DrawCube(0.0f, 0.0f, 0.0f, 0.5f, 0.2f, 0.1f);
	glPopMatrix();
}

void DrawMouse(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);	
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 0.2f, 0.1f);		
	DrawCube(0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.1f);
	glPopMatrix();
}

// DrawTorso
// desc: draws the robot torso
void DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
		glColor3f(0.8f, 0.6f, 0.6f);	
		glTranslatef(xPos, yPos, zPos);
		glScalef(3.0f, 5.0f, 2.0f);		// torso is a 3x5x2 cube
		DrawCube(0.0f, 0.0f, 0.0f, 3.0f, 5.0f, 2.0f);
	glPopMatrix();
}

void DrawPants(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.6f, 0.821f, 0.98f);	// yellow
	glTranslatef(xPos, yPos, zPos);
	glScalef(3.0f, 1.0f, 2.0f);		// torso is a 3x5x2 cube
	DrawCube(0.0f, 0.0f, 0.0f, 3.0f, 1.0f, 2.0f);
	glPopMatrix();
}

// DrawLeg
// desc: draws a single leg
void DrawLeg(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.6f, 0.821f, 0.98f);	// yellow
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 5.0f, 1.0f);		// leg is a 1x5x1 cube
	DrawCube(0.0f, 0.0f, 0.0f, 1.0f, 5.0f, 1.0f);
	glPopMatrix();
}

void DrawFoot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.9f, 0.2f, 0.2f);	// yellow
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 1.0f, 2.0f);		// leg is a 1x5x1 cube
	DrawCube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f);
	glPopMatrix();
}

void DrawHand(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);	// 
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 1.0f, 1.0f);		// arm is a 1x4x1 cube
	DrawCube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void DrawBackPack(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.9f, 0.1f, 0.1f);	// 
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 4.0f, 1.5f);		// arm is a 1x4x1 cube
	DrawCube(0.0f, 0.0f, 0.0f, 2.0f, 4.0f, 1.5f);
	glPopMatrix();
}


// DrawRobot
// desc: draws the robot located at (xpos,ypos,zpos)
void DrawRobot(float xPos, float yPos, float zPos)
{
	static bool leg1 = true;		// robot's leg states
	static bool leg2 = false;		// true = forward, false = back

	static bool arm1 = true;
	static bool arm2 = false;



	glPushMatrix();
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glTranslatef(xPos, yPos, zPos);	// draw robot at desired coordinates
// draw components
	

	DrawHead(1.0f, 2.0f, 0.0f);
	DrawHair(1.0f, 2.5f, 0.0f);
	DrawEye(0.7f, 1.3f, 0.1f);
	DrawEye(-0.3f, 1.3f, 0.1f);
	DrawMouse(0.5f, 0.5f, 0.1f);
	DrawTorso(1.5f, 0.0f, 0.0f);
	DrawPants(1.5f, -5.0f, 0.0f);
	DrawBackPack(1.5f, 0.0f, -2.0f);
	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (arm1)
		armAngle[0] = armAngle[0] + 0.06f;
	
	else
		armAngle[0] = armAngle[0] - 0.06f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (armAngle[0] >= 50.0f)
		arm1 = false;
	if (armAngle[0] <= -50.0f)
		arm1 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
	DrawArm(2.5f, 3.0f, -0.5f);
	glPopMatrix();



	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (arm2)
		armAngle[1] = armAngle[1] + 0.06f;
	else
		armAngle[1] = armAngle[1] - 0.06f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (armAngle[1] >= 50.0f)
		arm2 = false;
	if (armAngle[1] <= -50.0f)
		arm2 = true;

	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
	DrawArm(-1.5f, 3.0f, -0.5f);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
	DrawHand(-1.5f, 4.0f, -0.5f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
	DrawHand(2.5f, 4.0f, -0.5f);
	glPopMatrix();



	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (leg1)
		legAngle[0] = legAngle[0] + 0.03f;
	else
		legAngle[0] = legAngle[0] - 0.03f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (legAngle[0] >= 15.0f)
		leg1 = false;
	if (legAngle[0] <= -15.0f)
		leg1 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);
	DrawLeg(-0.5f, -5.0f, -0.5f);
	glPopMatrix();




	glPushMatrix();

	if (leg2)
		legAngle[1] = legAngle[1] + 0.03f;
	else
		legAngle[1] = legAngle[1] - 0.03f;

	if (legAngle[1] >= 15.0f)
		leg2 = false;
	if (legAngle[1] <= -15.0f)
		leg2 = true;

	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
	DrawLeg(1.5f, -5.0f, -0.5f);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
	DrawFoot(1.5f, -10.0f, 0.5f);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);
	DrawFoot(-0.5f, -10.0f, 0.5f);
	glPopMatrix();


	glPopMatrix();
}

void draw_floor(float xPos, float yPos, float zPos)
{
	
	glPushMatrix();
		
		glTranslatef(xPos, yPos, zPos);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, -80.0f, 80.0f);  //앞면
			glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, -80.0f, 80.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(80.0f, 80.0f, 80.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-80.0f, 80.0f, 80.0f);
		glEnd();

		glBegin(GL_QUADS);

			glNormal3f(-0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-80.0f, -80.0f, -80.0f);  //뒷면
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-80.0f, 80.0f, -80.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(80.0f, 80.0f, -80.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, -80.0f, -80.0f);
		glEnd();

		glBegin(GL_QUADS);

			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, -80.0f, -80.0f);  //우측면
			glTexCoord2f(1.0f, 1.0f); glVertex3f(80.0f, 80.0f, -80.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(80.0f, 80.0f, 80.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, -80.0f, 80.0f);
		glEnd();

		glBegin(GL_QUADS);

			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-80.0f, 80.0f, -80.0f);  //윗면
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, 80.0f, 80.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, 80.0f, 80.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(80.0f, 80.0f, -80.0f);
		glEnd();


		glBegin(GL_QUADS);

			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-80.0f, -80.0f, -80.0f);  //아랫면
			glTexCoord2f(0.0f, 1.0f); glVertex3f(80.0f, -80.0f, -80.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, -80.0f, 80.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-80.0f, -80.0f, 80.0f);
		glEnd();

		glBegin(GL_QUADS);

			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, -80.0f, -80.0f);  //좌측면
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-80.0f, -80.0f, 80.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-80.0f, 80.0f, 80.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-80.0f, 80.0f, -80.0f);
		glEnd();
	glPopMatrix();
	
}

void MyDisplay()
{
	glEnable(GL_DEPTH_TEST);				// enable depth testing

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen and depth buffer
	glLoadIdentity();										// reset modelview matrix

	if (change_moniter == 1)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();							// put current matrix on stack

			glLoadIdentity();					// reset matrix
			gluPerspective(90, (GLfloat)800 / (GLfloat)600, 1.0f, 1000.0f);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				gluLookAt(30.0, 20.0, 30.0, -30.0, 0.0, -30.0, 0.0, 1.0, 0.0);
				DrawSun();
				draw_floor(-40.0f, 11.0f, -40.0f);
				DrawRobot(-20.0f + DeltaX, 5.0 + DeltaY, -40.0f + DeltaZ);		// draw the robot

			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();							// dispose of current matrix
	}

	else
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();							// put current matrix on stack

			glLoadIdentity();					// reset matrix
			gluPerspective(90, (GLfloat)800 / (GLfloat)600, 1.0f, 1000.0f);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				gluLookAt(1.0, -40.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
				DrawSun();
				draw_floor(-40.0f, 11.0f, -40.0f);
				DrawRobot(-20.0f + DeltaX, 5.0 + DeltaY, -40.0f + DeltaZ);		// draw the robot

			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	glutSwapBuffers();
	glutPostRedisplay();
	
}




void SetupRC()
{
	
	
	GLfloat light1_ambient[] = {0.4, 0.4, 0.4, 1.0};     //해 조명
	GLfloat light1_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specref[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 512);
	
}

void MyTimer(int Value) {

	if (DeltaX <= -30 || DeltaX >= 30)
		Speed_DeltaX = -Speed_DeltaX;
	if (DeltaZ <= -30 || DeltaZ >= 30)
		Speed_DeltaZ = -Speed_DeltaZ;
	if (DeltaY <= -30 || DeltaY >= 30)
		Speed_DeltaY = -Speed_DeltaY;

	DeltaX += (1.5 * Speed_DeltaX);
	DeltaZ += (2.5 * Speed_DeltaZ);
	DeltaY += (2.5 * Speed_DeltaY);
	SpinAngle = (SpinAngle + 1) % 360;
	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 1);
}


void Keyboard(unsigned char KeyPressed, int x, int y) {
	switch (KeyPressed) {
	case '1':
		change_moniter = 1;
		break;

	case '2':
		change_moniter = 0;
		break;

	}
}

int main(int argc, char** argv)
{
	PlaySound(TEXT("wind_sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	
	
	glutCreateWindow("OpenGL Drawing Example");
	
	glClearColor(0.0, 0.0, 0.0, 0.5);
	
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	if (LoadGLTextures(tmp)) {
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
	}
	glPopMatrix();
	glLoadIdentity();
	
	glutTimerFunc(10, MyTimer, 1);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(MyDisplay);
	SetupRC();
	
	glutMainLoop();
	return 0;

}