/**
* Bhanupriya Haritwal
* Student ID: 110174278
* CSCI 172
*/

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <SOIL.h>

#include <math.h>
#include<time.h>
#include <bits/stdc++.h>
#define PI 3.1415926535897932384626433832795
using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLUquadricObj *sunObj = NULL,*mercuryObj = NULL,*earthObj = NULL,*moonObj = NULL;
GLuint texSun,texMercury,texEarth,texMoon;

/* Variable initialization */
/* Assuming arbitrary planet as mercury */
/* rotation speeds are arbitrary values */

vector<float> mercuryX,mercuryZ,earthX,earthZ,moonX,moonZ;
int mercuryPoints = 5000, earthPoints = 10000, moonPoints = 5000;
float distSunMercury = 2.0, distSunEarth = 4.0, distEarthMoon = 0.7;
int k = 0;
bool simStarted = false;
double angle = 0.0, scale = 1.0;
float mercuryRotationMultiplier = 360.0/1200.0;
float earthRotationMultiplier = 360.0/700.0;
float moonRotationMultiplier = (-0.7)*360.0/1200.0;

/**
* Utility function load textures for different planets
*/

void loadTexture(char* fileName,GLuint& tex) {

    int width,height;
    glGenTextures(1,&tex);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


    glBindTexture(GL_TEXTURE_2D, tex);
    unsigned char* image = SOIL_load_image(fileName,&width,&height, 0,SOIL_LOAD_RGBA);

    if(!image)cout<<"fail to find image"<<endl;


    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width, height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

/**
* Utility function initialize all planet objects
*/
void initPlanetObjects() {

    sunObj = gluNewQuadric();
    gluQuadricDrawStyle(sunObj, GLU_FILL);
    gluQuadricTexture(sunObj, GL_TRUE);
    gluQuadricNormals(sunObj, GLU_SMOOTH);
    loadTexture("images/sun.jpg",texSun);

    mercuryObj = gluNewQuadric();
    gluQuadricDrawStyle(mercuryObj, GLU_FILL);
    gluQuadricTexture(mercuryObj, GL_TRUE);
    gluQuadricNormals(mercuryObj, GLU_SMOOTH);
    loadTexture("images/mercury.jpg",texMercury);

    earthObj = gluNewQuadric();
    gluQuadricDrawStyle(earthObj, GLU_FILL);
    gluQuadricTexture(earthObj, GL_TRUE);
    gluQuadricNormals(earthObj, GLU_SMOOTH);
    loadTexture("images/earth.jpg",texEarth);

    moonObj = gluNewQuadric();
    gluQuadricDrawStyle(moonObj, GLU_FILL);
    gluQuadricTexture(moonObj, GL_TRUE);
    gluQuadricNormals(moonObj, GLU_SMOOTH);
    loadTexture("images/moon.jpg",texMoon);

}

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    if(simStarted) {
        k++;
    }
    int mercuryIndex = k % (2*mercuryPoints);   // calculation of point index for mercury
    int earthIndex = k % (2*earthPoints);       // calculation of point index for earth

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);
    glRotatef(angle,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In WireFrame Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

//*************************My code**********************************//

    glPushMatrix();
    glScalef(scale,scale,scale);                     // Handling scene => zooming IN/OUT
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texSun);                      // Set texture for sun
        gluSphere(sunObj,0.6,40,40);                              // Create sun
        glPushMatrix();
            glTranslatef(mercuryX[mercuryIndex], 0.0, mercuryZ[mercuryIndex]);   // handling mercury orbit rotation
            glRotatef(k*mercuryRotationMultiplier, 0.0, 1.0, 0.0);    // handling mercury own axis rotation
            glBindTexture(GL_TEXTURE_2D, texMercury);                 // Set texture for mercury
            gluSphere(mercuryObj,0.3,40,40);            // Creating mercury
        glPopMatrix();
        glPushMatrix();
            glTranslatef(earthX[earthIndex], 0.0, earthZ[earthIndex]);   // handling earth orbit rotation
            glRotatef(k*earthRotationMultiplier, 0.0, 1.0, 0.0);     // handling earth own axis rotation
            glBindTexture(GL_TEXTURE_2D, texMoon);                     // Set texture of earth
            glBindTexture(GL_TEXTURE_2D, texEarth);
            gluSphere(earthObj,0.3,40,40);              // Creating earth
            glPushMatrix();
                glRotatef(k*moonRotationMultiplier, 0.0, 1.0, 0.0);   // handling moon rotation
                glTranslatef(distEarthMoon, 0.0, 0.0);                // Setting distance moon-earth distance
                glBindTexture(GL_TEXTURE_2D, texMoon);                 // Set texture of moon
                gluSphere(moonObj,0.1,40,40);               // Creating moon
            glPopMatrix();
            /* To create moon rotation path */
            glPushMatrix();
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINE_LOOP);
                     for(int i=0;i<2*moonPoints;i++) {
                        glVertex3f(moonX[i],0.0,moonZ[i]);
                     }
                glEnd();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    /* To create mercury rotation path */
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
            for(int i=0;i<2*mercuryPoints;i++) {
                glVertex3f(mercuryX[i],0.0,mercuryZ[i]);
            }
        glEnd();
    glPopMatrix();
    /* To create earth rotation path */
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
            for(int i=0;i<2*earthPoints;i++) {
                glVertex3f(earthX[i],0.0,earthZ[i]);
            }
        glEnd();
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'w':
            WireFrame = !WireFrame;  // To toggle WireFrame
            break;
        case ' ':
            simStarted = !simStarted;  // To toggle simulation OR start/pause simulation
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        scale+=0.01;            // performing zoom in
    break;

    case GLUT_KEY_DOWN:
        scale-=0.01;            // performing zoom out
    break;

    case GLUT_KEY_RIGHT:
        angle-=5.0;            // performing right rotation
    break;

    case GLUT_KEY_LEFT:
        angle+=5.0;            // performing left rotation
    break;

   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    initPlanetObjects();

}
/**
* Utility function initialize path points (for orbital motion) of the mercury, earth and moon.
*/
static void initPoints(void) {
    for(double i = 0.0; i < 2*PI; i+= PI/mercuryPoints) {
        mercuryX.push_back(sin(i) * distSunMercury);
        mercuryZ.push_back(cos(i) * distSunMercury);
    }

    for(double i = 0.0; i < 2*PI; i+= PI/earthPoints) {
        earthX.push_back(sin(i) * distSunEarth);
        earthZ.push_back(cos(i) * distSunEarth);
    }

    for(double i = 0.0; i < 2*PI; i+= PI/moonPoints) {
        moonX.push_back(sin(i) * distEarthMoon);
        moonZ.push_back(cos(i) * distEarthMoon);
    }
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Solar System");
    init();
    initPoints();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
