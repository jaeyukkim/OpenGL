#include <stdio.h>
#include <GL/glut.h>
#include <gl/gl.h>						// standard OpenGL include
#include <gl/glu.h>						// OpenGL utilties

////// Global Variables
float angle = 0.0f;						// current angle of the rotating triangle
bool fullScreen = false;


////// Robot variables
float legAngle[2] = { 0.0f, 0.0f };		// each leg's current angle
float armAngle[2] = { 0.0f, 0.0f };
float footAngle[2] = { 0.0f, 0.0f };
float handAngle[2] = { 0.0f, 0.0f };

GLfloat Delta = 0.1f;



// DrawCube
// desc: since each component of the robot is made up of
//       cubes, we will use a single function that will
//		 draw a cube at a specified location.
void DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glBegin(GL_POLYGON);
	glVertex3f(0.0f, 0.0f, 0.0f);	// top face
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);	// front face
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	// right face
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);


	glVertex3f(-1.0f, 0.0f, 0.0f);	// left face
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);


	glVertex3f(0.0f, 0.0f, 0.0f);	// bottom face
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	// back face
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
	DrawCube(0.0f, 0.0f, 0.0f);
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
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void DrawHair(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.0f);	// white
	glTranslatef(xPos, yPos, zPos);
	glScalef(2.0f, 0.5f, 2.0f);		// head is a 2x2x2 cube
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void DrawEye(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);	// white
	glTranslatef(xPos, yPos, zPos);
	glScalef(0.5f, 0.2f, 0.1f);		// head is a 2x2x2 cube
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void DrawMouse(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);	// white
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 0.2f, 0.1f);		// head is a 2x2x2 cube
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

// DrawTorso
// desc: draws the robot torso
void DrawTorso(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.0f, 0.87f, 0.0f);	// blue
	glTranslatef(xPos, yPos, zPos);
	glScalef(3.0f, 5.0f, 2.0f);		// torso is a 3x5x2 cube
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void DrawPants(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.6f, 0.821f, 0.98f);	// yellow
	glTranslatef(xPos, yPos, zPos);
	glScalef(3.0f, 1.0f, 2.0f);		// torso is a 3x5x2 cube
	DrawCube(0.0f, 0.0f, 0.0f);
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
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void DrawFoot(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.9f, 0.2f, 0.2f);	// yellow
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 1.0f, 2.0f);		// leg is a 1x5x1 cube
	DrawCube(0.0f, 0.0f, 0.0f);
	glPopMatrix();
}

void DrawHand(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);	// 
	glTranslatef(xPos, yPos, zPos);
	glScalef(1.0f, 1.0f, 1.0f);		// arm is a 1x4x1 cube
	DrawCube(0.0f, 0.0f, 0.0f);
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

	static bool foot1 = true;
	static bool foot2 = false;

	static bool hand1 = true;
	static bool hand2 = false;

	glPushMatrix();

	glTranslatef(xPos, yPos, zPos);	// draw robot at desired coordinates

// draw components
	DrawHead(1.0f, 2.0f, 0.0f);
	DrawHair(1.0f, 2.5f, 0.0f);
	DrawEye(0.7f, 1.3f, 0.1f);
	DrawEye(-0.3f, 1.3f, 0.1f);
	DrawMouse(0.5f, 0.5f, 0.1f);
	DrawTorso(1.5f, 0.0f, 0.0f);
	DrawPants(1.5f, -5.0f, 0.0f);
	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (arm1)
		armAngle[0] = armAngle[0] + 0.01f;
	else
		armAngle[0] = armAngle[0] - 0.01f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (armAngle[0] >= 15.0f)
		arm1 = false;
	if (armAngle[0] <= -15.0f)
		arm1 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
	DrawArm(2.5f, 0.0f, -0.5f);
	glPopMatrix();

	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (hand1)
		handAngle[0] = handAngle[0] + 0.01f;
	else
		handAngle[0] = handAngle[0] - 0.01f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (handAngle[0] >= 15.0f)
		hand1 = false;
	if (handAngle[0] <= -15.0f)
		hand1 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
	DrawHand(2.5f, -4.0f, -0.5f);
	glPopMatrix();


	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (arm2)
		armAngle[1] = armAngle[1] + 0.01f;
	else
		armAngle[1] = armAngle[1] - 0.01f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (armAngle[1] >= 15.0f)
		arm2 = false;
	if (armAngle[1] <= -15.0f)
		arm2 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
	DrawArm(-1.5f, 0.0f, -0.5f);
	glPopMatrix();

	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (hand2)
		handAngle[1] = handAngle[1] + 0.01f;
	else
		handAngle[1] = handAngle[1] - 0.01f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (handAngle[1] >= 15.0f)
		hand2 = false;
	if (handAngle[1] <= -15.0f)
		hand2 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
	DrawHand(-1.5f, -4.0f, -0.5f);
	glPopMatrix();

	glPushMatrix();
	// if leg is moving forward, increase angle, else decrease angle
	if (leg1)
		legAngle[0] = legAngle[0] + 0.01f;
	else
		legAngle[0] = legAngle[0] - 0.01f;

	// once leg has reached its maximum angle in a direction,
	// reverse it
	if (legAngle[0] >= 15.0f)
		leg1 = false;
	if (legAngle[0] <= -15.0f)
		leg1 = true;

	// move the leg away from the torso and rotate it to give "walking" effect
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);

	// draw the leg
	DrawLeg(-0.5f, -5.0f, -0.5f);

	glPopMatrix();

	// do the same as above with leg 2, the robot's left leg
	glPushMatrix();

	if (leg2)
		legAngle[1] = legAngle[1] + 0.01f;
	else
		legAngle[1] = legAngle[1] - 0.01f;

	if (legAngle[1] >= 15.0f)
		leg2 = false;
	if (legAngle[1] <= -15.0f)
		leg2 = true;

	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
	DrawLeg(1.5f, -5.0f, -0.5f);

	glPopMatrix();

	glPushMatrix();

	if (foot1)
		footAngle[1] = footAngle[1] + 0.01f;
	else
		footAngle[1] = footAngle[1] - 0.01f;

	if (footAngle[1] >= 15.0f)
		foot1 = false;
	if (footAngle[1] <= -15.0f)
		foot1 = true;

	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
	DrawFoot(1.5f, -10.0f, 0.5f);

	glPopMatrix();

	glPushMatrix();

	if (foot1)
		footAngle[0] = footAngle[0] + 0.01f;
	else
		footAngle[0] = footAngle[0] - 0.01f;

	if (footAngle[0] >= 15.0f)
		foot1 = false;
	if (footAngle[0] <= -15.0f)
		foot1 = true;

	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);
	DrawFoot(-0.5f, -10.0f, 0.5f);

	glPopMatrix();


	glPopMatrix();
}

// Render
// desc: handles drawing of scene
void MyDisplay()
{
	glEnable(GL_DEPTH_TEST);				// enable depth testing

	// do rendering here
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// clear to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen and depth buffer
	glLoadIdentity();										// reset modelview matrix

	angle = angle + 0.05f;					// increase our rotation angle counter
	if (angle >= 360.0f)					// if we've gone in a circle, reset counter
		angle = 0.0f;

	glPushMatrix();							// put current matrix on stack
		glLoadIdentity();					// reset matrix
		glTranslatef(0.0f, 0.0f, -30.0f);	// move to (0, 0, -30)
		glRotatef(angle, 0.0f, 1.0f, 0.0f);	// rotate the robot on its y-axis
		DrawRobot(0.0f, 0.0f, 0.0f);		// draw the robot
	glPopMatrix();							// dispose of current matrix

	glFlush();

	
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Drawing Example");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;



	return 0;
}