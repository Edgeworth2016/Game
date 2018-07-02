/* Assignment 4
Kai Bernstein
kdbernst@syr.edu
Visual Studio 2017
2 April 2018
No references

Interactions:
Press w, a, s, d to move forward, left, back, and right respectively.
Press the left and right arrow keys to turn.
Press the up and down arrow keys to look up and down.
Press 0 to turn on the flashlight.
Press 1,2,3 and 4 to toggle between flashlight colors (white, red, green, blue).
Press o to turn the room light on and off.  You can also press the red switch on the wall for this.
Press z to open the door.  You can also click on the doorknob for this.
Click on the key (in the trashcan in the bottom right corner) to have it move to the chest.
Press l or r to turn the key left or right in the lock.  Left is the correct direction.
Turn off both the room light and flashlight to see the cube on the table glow.
*/

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159265

static double width, height;
static bool isSelecting = 0;
static int hits;
static unsigned int buffer[1024];
static unsigned int closestName = 0;
static double psx = 0, psy = 0, psz = 0;
static double theta = PI;
static double phi = 0;
static double psi = 0;
static double chi = PI / 2;
static double omicron = PI / 2;
static double lambda = PI / 2;
static int open = 0;
static double cy= -10.5;
static double ty = -14.8;
static double keyx = 13, keyy = -14, keyz = -33;
static double keyangle = 0;
static int key = 0;
static int stage = 0;
static int stage2 = 0;
static bool lighton = 1;
static bool spoton = 0;
static int chest = 0;
static double red = 1, green = 1, blue = 1;
static bool treasure = 0;

void findClosestHit(int hits, unsigned int buffer[])
{
	unsigned int *ptr, minZ;

	minZ = 0xffffffff; // 2^32 - 1
	ptr = buffer;
	closestName = 0;
	for (int i = 0; i < hits; i++)
	{
		ptr++;
		if (*ptr < minZ)
		{
			minZ = *ptr;
			ptr += 2;
			closestName = *ptr;
			ptr++;
		}
		else ptr += 3;
	}
}

void drawGrass()
{
	//Grass
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(0, -15, -100);
	glScalef(1, 0.00001, 1);
	glutSolidCube(400);
	glPopMatrix();
}

void drawDoor()
{
	//Door
	glColor3f(0.2, 0.2, 0.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(0, -5.5, -30);
	glTranslatef(5 * sin(psi), 0, 5 * sin(psi));
	glRotatef(psi, 0, 1, 0);
	glScalef(1, 1.9, 0.001);
	glutSolidCube(10);
	glPopMatrix();
	glColor3f(1.0, 1.0, 0.0);
}

void drawDoorknob()
{
	//Doorknob
	glPushMatrix();
	if (isSelecting) {
		if (open == 0) glLoadName(1);
		else glLoadName(0);
	}
	glTranslatef(-3, -5.5, -29);
	glTranslatef(9.1 * sin(psi), 0, 7 * sin(psi));
	glutSolidSphere(.7, 30, 30);
	glPopMatrix();
}

void drawHouse()
{
	//Front
	glColor3f(.5, .5, .5);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-11, 0, -30);
	glScalef(0.4, 1, 0.001);
	glutSolidCube(30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(11, 0, -30);
	glScalef(0.4, 1, 0.001);
	glutSolidCube(30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 9.5, -30);
	glScalef(3, 1, 0.001);
	glutSolidCube(11);
	glPopMatrix();
	//Left Side
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-17, 0, -47);
	glScalef(0.001, .88, 1);
	glutSolidCube(34);
	glPopMatrix();
	//Right Side
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(17, 0, -47);
	glScalef(0.001, .88, 1);
	glutSolidCube(34);
	glPopMatrix();
	//Back Side
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-11, 0, -64);
	glScalef(0.4, 1, 0.001);
	glutSolidCube(30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(11, 0, -64);
	glScalef(0.4, 1, 0.001);
	glutSolidCube(30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -9.5, -64);
	glScalef(1, .32, 0.001);
	glutSolidCube(34);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 9.5, -64);
	glScalef(1, .32, 0.001);
	glutSolidCube(34);
	glPopMatrix();
	//Roof
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(0, 14.9, -47);
	glScalef(1, 0.001, 1);
	glutSolidCube(34);
	glPopMatrix();
	//Floor
	glColor3f(0.1, 0.1, 0.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(0, -14.9, -47);
	glScalef(1, 0.00001, 1);
	glutSolidCube(34);
	glPopMatrix();
}

void drawTable()
{
	//Legs
	glColor3f(0.4, 0.4, 0.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-16, -11, -63);
	glScalef(.1, .8, .1);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-6, -11, -63);
	glScalef(.1, .8, .1);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-16, -11, -57);
	glScalef(.1, .8, .1);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-6, -11, -57);
	glScalef(.1, .8, .1);
	glutSolidCube(10);
	glPopMatrix();
	//Top
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-11, -6.5, -60);
	glScalef(1.1, .1, .7);
	glutSolidCube(10);
	glPopMatrix();
}

void drawChest()
{
	//Body
	glColor3f(0.2, 0.2, 0.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(13, -12.9, -59);
	glScalef(.7, .4, .1);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(16, -12.9, -61);
	glScalef(.1, .4, .4);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(10, -12.9, -61);
	glScalef(.1, .4, .4);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(13, -12.9, -63);
	glScalef(.7, .4, .1);
	glutSolidCube(10);
	glPopMatrix();
	glColor3f(0.3, 0.3, 0.0);
	//Top
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(13, cy, -61);
	glScalef(.7, .1, .5);
	glutSolidCube(10);
	glPopMatrix();
}

void drawKeyhole()
{
	//Keyhole
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	static GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_FLAT);
	gluQuadricDrawStyle(qobj, GLU_LINE);
	glTranslatef(13, -12.4, -58);
	gluDisk(qobj, .45, .9, 30, 30);
	glPopMatrix();
}

void drawSky()
{
	//Moon
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(0, 30, -299);
	glutSolidSphere(12, 30, 30);
	glPopMatrix();
}

void drawTrashcan()
{
	//Can
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	static GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glTranslatef(14, -14.5, -33);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(qobj, 2, 2.5, 3, 30, 30);
	glPopMatrix();
	//Key
	glColor3f(0.8, 0.8, 0.8);
	glPushMatrix();
	if (isSelecting)  glLoadName(2);
	static GLUquadricObj *qobj2;
	qobj2 = gluNewQuadric();
	gluQuadricNormals(qobj2, GLU_SMOOTH);
	gluQuadricDrawStyle(qobj2, GLU_FILL);
	glTranslatef(keyx, keyy, keyz);
	glRotatef(keyangle, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	gluDisk(qobj, .3, .5, 30, 30);
	glPopMatrix();
	glPushMatrix();
	if (isSelecting)  glLoadName(2);
	static GLUquadricObj *qobj3;
	qobj3 = gluNewQuadric();
	gluQuadricNormals(qobj3, GLU_FLAT);
	gluQuadricDrawStyle(qobj3, GLU_FILL);
	glTranslatef(keyx, keyy, keyz - 1.2);
	glRotatef(keyangle, 0, 0, 1);
	gluCylinder(qobj, .1, .1, .8, 30, 30);
	glPopMatrix();
}

void drawSwitch()
{
	glColor3f(1, 0, 0);
	if (isSelecting)  glLoadName(3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3f(-16.9, 1, -35);
	glVertex3f(-16.9, 1, -34);
	glVertex3f(-16.9, 0, -34);
	glVertex3f(-16.9, 0, -35);
	glEnd();
}

void drawTriforce()
{
	glColor3f(1, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (isSelecting)  glLoadName(0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(11, ty, -61);
	glVertex3f(13, ty, -61);
	glVertex3f(12, ty + sqrt(3), -61);
	glEnd();
	if (isSelecting)  glLoadName(0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(13, ty, -61);
	glVertex3f(15, ty, -61);
	glVertex3f(14, ty + sqrt(3), -61);
	glEnd();
	if (isSelecting)  glLoadName(0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(12, ty + sqrt(3), -61);
	glVertex3f(14, ty + sqrt(3), -61);
	glVertex3f(13, ty + 2*sqrt(3), -61);
	glEnd();
}

void drawCube()
{
	glColor3f(1, 1, 1);
	glPushMatrix();
	if (isSelecting)  glLoadName(0);
	glTranslatef(-15, -5, -62);
	glutSolidCube(2);
	glPopMatrix();
}

void drawClock()
{
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (isSelecting)  glLoadName(0);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, 1);
	glVertex3f(-12, 10, -63.9);
	for (int i = 0; i <= 12; i++)
		glVertex3f(3 * cos(i * PI / 6) - 12, 3 * sin(i * PI / 6) + 10, -63.9);
	glEnd();
	glColor3f(0, 0, 0);
	glLineWidth(.01);
	if (isSelecting)  glLoadName(0);
	glBegin(GL_LINES);
	glNormal3f(0, 0, 1);
	glVertex3f(-12, 10, -63.8);
	glVertex3f(-12 + 3*cos(chi), 10 + 3*sin(chi), -63.8);
	glEnd();
	glLineWidth(1); 
	if (isSelecting)  glLoadName(0);
	glBegin(GL_LINES);
	glNormal3f(0, 0, 1);
	glVertex3f(-12, 10, -63.8);
	glVertex3f(-12 + 3*cos(omicron), 10 + 3*sin(omicron), -63.8);
	glEnd();
	glLineWidth(2);
	if (isSelecting)  glLoadName(0);
	glBegin(GL_LINES);
	glNormal3f(0, 0, 1);
	glVertex3f(-12, 10, -63.8);
	glVertex3f(-12 + 2 * cos(lambda), 10 + 2 * sin(lambda), -63.8);
	glEnd();
}

void animateClock()
{
	chi -= PI / 7680;
	omicron -= PI / 460800;
	lambda -= PI / 27648000;
	glutPostRedisplay();
}

void animateDoor(int value)
{
	if (psi < 90)
		psi += 3;
	else
		open = 2;
	glutPostRedisplay();
	glutTimerFunc(1000, animateDoor, 1);
}

void animateKey(int value)
{
	if (stage == 0 && keyy < -10) keyy += .01;
	else if (stage == 0 && keyy >= -10) { keyz -= .01; stage = 1; }
	else if (stage == 1 && keyz > -55.5) keyz -= .01;
	else if (stage == 1 && keyz <= -55.5) { keyy -= .01; stage = 2; }
	else if (stage == 2 && keyy > -12.2) keyy -= .01;
	else if (stage == 2 && keyy <= -12.2) { keyz -= .01; stage = 3; }
	else if (stage == 3 && keyz > -56.7) keyz -= .01;
	else if (stage == 3 && keyz <= -56.7) { key = 2; treasure = 1; }
	glutPostRedisplay();
	glutTimerFunc(1000, animateKey, 1);
}

void animateChest(int value)
{
	if (chest == 1) {
		if (stage2 == 0 && keyangle < 90) keyangle += 1;
		else if (stage2 == 0 && keyangle >= 90) stage2 = 1;
		else if (stage2 == 1 && cy < 10) cy += .01;
		else if (stage2 == 1 && cy >= 10) stage2 = 2;
		else if (stage2 == 2 && ty < 0) ty += .01;
		else if (stage2 == 2 && ty >= 0) key = 4;
	}
	else if (chest == 2) {
		if (stage2 == 0 && keyangle > -90) keyangle -= 1;
		else if (stage2 == 0 && keyangle <= -90) stage2 = 1;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, animateChest, 1);
}

void projectionStuff()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1, 1, 300);
	glMatrixMode(GL_MODELVIEW);
}

void changeView()
{
	gluLookAt(psx, psy, psz, psx + sin(theta), psy + sin(phi), psz + cos(theta), 0, 1, 0);
}


void drawEverything() {
	if (chest == 2 && stage2 == 1) {}
	else {
		float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
		float lightDifAndSpec1[] = { red, green, blue, 1.0 };
		float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };
		float globAmb[] = { 0.1, 0.1, 0.1, 1.0 };

		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

		glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		changeView();

		glDisable(GL_LIGHTING);
		drawSky();
		glEnable(GL_LIGHTING);

		if (spoton) glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);
		float lightPos1[] = { psx, psy, psz, 1.0 };
		float spotDirection[] = { sin(theta), sin(phi), cos(theta) };
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);

		glEnable(GL_LIGHTING);
		drawGrass();
		glDisable(GL_LIGHTING);

		if (lighton) glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);
		float lightPos0[] = { 0.0, 15, -47, 1.0 };

		float matAmbandDif[] = { 0.5, 0.5, 0.5, 1.0 };
		float matAmbandDifOff[] = { 0.0, 0.0, 0.0, 0.0 };
		float matDif[] = { 1.0, 1.0, 1.0, 1.0 };
		float matDifOff[] = { 0.0, 0.0, 0.0, 0.0 };
		float matSpec[] = { 1.0, 1.0, 0.0, 1.0 };
		float matSpecOff[] = { 0.0, 0.0, 0.0, 0.0 };
		float matShine[] = { 120.0 };
		float matShineOff[] = { 0.0 };
		float matEmission[] = { 0.0, 0.0, 1.0, 1.0 };
		float matEmission2[] = { 1.0, 1.0, 0.0, 1.0 };
		float matEmissionOff[] = { 0.0, 0.0, 0.0, 0.0 };

		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
		if (lighton) glColor3f(1.0, 1.0, 1.0);
		else glColor3f(0.0, 0.0, 0.0);
		glutSolidSphere(1, 8, 8);
		glPopMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbandDif);
		drawHouse();
		drawDoor();
		drawDoorknob();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbandDifOff);
		glDisable(GL_LIGHTING);
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDif);
		drawTable();
		drawChest();
		drawTrashcan();
		drawSwitch();
		drawClock();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDifOff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
		drawKeyhole();
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecOff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShineOff);
		if (!lighton && !spoton) glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
		drawCube();
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmissionOff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission2);
		drawTriforce();
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmissionOff);
		glDisable(GL_LIGHT0);
		glutIdleFunc(animateClock);
		if (open == 1)
			animateDoor(1);
		if (key == 1)
			animateKey(1);
		if (key == 2)
			animateChest(1);
	}
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projectionStuff();
	glLoadIdentity();
	isSelecting = 0;
	drawEverything();
	glutSwapBuffers();
}

void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	width = w;
	height = h;
}

void pickFunction(int button, int state, int x, int y)
{
	int viewport[4];
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(1024, buffer);
		(void)glRenderMode(GL_SELECT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);

		gluPerspective(100, 1, 1, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glInitNames();
		glPushName(0);
		isSelecting = 1;
		drawEverything();

		hits = glRenderMode(GL_RENDER);
		findClosestHit(hits, buffer);

		if (closestName == 1) open = 1;
		else if (closestName == 2 && key == 0) key = 1;
		else if (closestName == 3) lighton = !lighton;

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glutPostRedisplay();
	}
}

void mouseMotion(int x, int y)
{

}

bool checkCollision(int x)
{
	double xx = psx + sin(theta);
	double xx2 = psx + sin(theta + PI);
	double xx3 = psx + sin(theta + PI/2);
	double xx4 = psx + sin(theta - PI/2);
	double zz = psz + cos(theta);
	double zz2 = psz + cos(theta + PI);
	double zz3 = psz + cos(theta + PI / 2);
	double zz4 = psz + cos(theta - PI / 2);
	if (x == 1) {
		if (xx >= -17.5 && xx <= 17.5) {
			if (zz >= -64.5 && zz <= -63.5)
				return 1;
		}
		if ((xx >= -17.5 && xx <= -5) || (xx >= 5 && xx <= 17.5)) {
			if (zz >= -30.5 && zz <= -29.5)
				return 1;
		}
		if (xx >= -5 && xx <= 5 && open == 0) {
			if (zz >= -30.5 && zz <= -29.5)
				return 1;
		}
		if (zz >= -64.5 && zz <= -30.5) {
			if ((xx >= -17.5 && xx <= -16.5) || (xx >= 16.5 && xx <= 17.5))
				return 1;
		}
		if (xx >= -16 && xx <= -6) {
			if (zz >= -63.5 && zz <= -56.5)
				return 1;
		}
		if (xx >= 9.5 && xx <= 16.5) {
			if (zz >= -63.5 && zz <= -58.5)
				return 1;
		}
		if (xx <= -100 || xx >= 100 || zz <= -150 || zz >= 50)
			return 1;
	}
	if (x == 2) {
		if (xx2 >= -17.5 && xx2 <= 17.5) {
			if (zz2 >= -64.5 && zz2 <= -63.5)
				return 1;
		}
		if ((xx2 >= -17.5 && xx2 <= -5) || (xx2 >= 5 && xx2 <= 17.5)) {
			if (zz2 >= -30.5 && zz2 <= -29.5)
				return 1;
		}
		if (xx2 >= -5 && xx2 <= 5 && open == 0) {
			if (zz2 >= -30.5 && zz2 <= -29.5)
				return 1;
		}
		if (zz2 >= -64.5 && zz2 <= -30.5) {
			if ((xx2 >= -17.5 && xx2 <= -16.5) || (xx2 >= 16.5 && xx2 <= 17.5))
				return 1;
		}
		if (xx2 >= -16 && xx2 <= -6) {
			if (zz2 >= -63.5 && zz2 <= -56.5)
				return 1;
		}
		if (xx2 >= 9.5 && xx2 <= 16.5) {
			if (zz2 >= -63.5 && zz2 <= -58.5)
				return 1;
		}
		if (xx2 <= -100 || xx2 >= 100 || zz2 <= -150 || zz2 >= 50)
			return 1;
	}
	if (x == 3) {
		if (xx3 >= -17.5 && xx3 <= 17.5) {
			if (zz3 >= -64.5 && zz3 <= -63.5)
				return 1;
		}
		if ((xx3 >= -17.5 && xx3 <= -5) || (xx3 >= 5 && xx3 <= 17.5)) {
			if (zz3 >= -30.5 && zz3 <= -29.5)
				return 1;
		}
		if (xx3 >= -5 && xx3 <= 5 && open == 0) {
			if (zz3 >= -30.5 && zz3 <= -29.5)
				return 1;
		}
		if (zz3 >= -64.5 && zz3 <= -30.5) {
			if ((xx3 >= -17.5 && xx3 <= -16.5) || (xx3 >= 16.5 && xx3 <= 17.5))
				return 1;
		}
		if (xx3 >= -16 && xx3 <= -6) {
			if (zz3 >= -63.5 && zz3 <= -56.5)
				return 1;
		}
		if (xx3 >= 9.5 && xx3 <= 16.5) {
			if (zz3 >= -63.5 && zz3 <= -58.5)
				return 1;
		}
		if (xx3 <= -100 || xx3 >= 100 || zz3 <= -150 || zz3 >= 50)
			return 1;
	}
	if (x == 4) {
		if (xx4 >= -17.5 && xx4 <= 17.5) {
			if (zz4 >= -64.5 && zz4 <= -63.5)
				return 1;
		}
		if ((xx4 >= -17.5 && xx4 <= -5) || (xx4 >= 5 && xx4 <= 17.5)) {
			if (zz4 >= -30.5 && zz4 <= -29.5)
				return 1;
		}
		if (xx4 >= -5 && xx4 <= 5 && open == 0) {
			if (zz4 >= -30.5 && zz4 <= -29.5)
				return 1;
		}
		if (zz4 >= -64.5 && zz4 <= -30.5) {
			if ((xx4 >= -17.5 && xx4 <= -16.5) || (xx4 >= 16.5 && xx4 <= 17.5))
				return 1;
		}
		if (xx4 >= -16 && xx4 <= -6) {
			if (zz4 >= -63.5 && zz4 <= -56.5)
				return 1;
		}
		if (xx4 >= 9.5 && xx4 <= 16.5) {
			if (zz4 >= -63.5 && zz4 <= -58.5)
				return 1;
		}
		if (xx4 <= -100 || xx4 >= 100 || zz4 <= -150 || zz4 >= 50)
			return 1;
	}
	return 0;
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		if (!checkCollision(1)) {
			psx += sin(theta);
			psz += cos(theta);
			glutPostRedisplay();
		}
		break;
	case 's':
		if (!checkCollision(2)) {
			psx += sin(theta + PI);
			psz += cos(theta + PI);
			glutPostRedisplay();
		}
		break;
	case 'a':
		if (!checkCollision(3)) {
			psx += sin(theta + PI / 2);
			psz += cos(theta + PI / 2);
			glutPostRedisplay();
		}
		break;
	case 'd':
		if (!checkCollision(4)) {
			psx += sin(theta - PI / 2);
			psz += cos(theta - PI / 2);
			glutPostRedisplay();
		}
		break;
	case 'o':
		lighton = !lighton;
		glutPostRedisplay();
		break;
	case 'l':
		if (treasure) {
			chest = 1;
			//key = 3;
		}
		glutPostRedisplay();
		break;
	case 'r':
		if (treasure) {
			chest = 2;
			//key = 3;
		}
		glutPostRedisplay();
		break;
	case '0':
		spoton = !spoton;
		glutPostRedisplay();
		break;
	case '1':
		red = 1; green = 1; blue = 1;
		glutPostRedisplay();
		break;
	case '2':
		red = 1; green = 0; blue = 0;
		glutPostRedisplay();
		break;
	case '3':
		red = 0; green = 1; blue = 0;
		glutPostRedisplay();
		break;
	case '4':
		red = 0; green = 0; blue = 1;
		glutPostRedisplay();
		break;
	case 'z':
		if (open == 0) open = 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN) {
		if (phi > (-5 * PI) / 12)
			phi -= (PI / 12);
	}
	if (key == GLUT_KEY_UP) {
		if (phi < (5 * PI) / 12)
			phi += (PI / 12);
	}
	if (key == GLUT_KEY_LEFT) {
		theta += (PI / 12);
	}
	if (key == GLUT_KEY_RIGHT) {
		theta -= (PI / 12);
	}
	glutPostRedisplay();
}

void printInteraction(void)
{
	cout << "Interactions:" << endl
		<< "Press w, a, s, d to move forward, left, back, and right respectively." << endl
		<< "Press the left and right arrow keys to turn." << endl
		<< "Press the up and down arrow keys to look up and down." << endl
		<< "Press 0 to turn on the flashlight." << endl
		<< "Press 1, 2, 3 and 4 to toggle between flashlight colors(white, red, green, blue)." << endl
		<< "Press o to turn the room light on and off.You can also press the red switch on the wall for this." << endl
		<< "Press z to open the door.You can also click on the doorknob for this." << endl
		<< "Click on the key(in the trashcan in the bottom right corner) to have it move to the chest." << endl
		<< "Press l or r to turn the key left or right in the lock.Left is the correct direction." << endl
		<< "Turn off both the room light and flashlight to see the cube on the table glow." << endl;
}

int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 4");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutMouseFunc(pickFunction);
	glutMotionFunc(mouseMotion);
	setup();
	glutMainLoop();
	return 0;
}