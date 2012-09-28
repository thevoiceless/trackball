// Riley Moses

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cassert>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

/* --------------------------------------------- */
/* ----- GLOBAL VARIABLES  --------------------- */
/* --------------------------------------------- */

static const int VPD_DEFAULT = 800;

static const int MENU_SLOWER = 1;
static const int MENU_FASTER = 2;
static const int MENU_STOP_RUN = 3;

static const double TWOPI = (2.0 * M_PI);

/* --------------------------------------------- */

GLint wid;               /* GLUT window id; value asigned in main() and should stay constant */
GLint vpw = VPD_DEFAULT; /* viewport dimensions; change when window is resized (resize callback) */
GLint vph = VPD_DEFAULT;

/* --------------------------------------------- */

GLfloat angle1 = 0;  /* angles used in animation */
GLfloat angle2 = 0;
GLfloat dangle1 = 0.0057;
GLfloat dangle2 = 0.0071;

bool animate = true;   /* animate or not? */

/* --------------------------------------------- */
/* ----- SET LIGHT SOURCE PROPERTIES ----------- */
/* --------------------------------------------- */

void init_lightsource ( )
{
  GLfloat light_ambient[] = { .0, .0, .0, 1.0 };
  GLfloat light_diffuse[] = { .9, .9, .9, 1.0 };
  GLfloat light_specular[] = { 0, 0, 0, 1.0 };
  GLfloat light_position[] = { 200.0, 200.0, 200.0, 1.0 };
  GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);   // 
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
  glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0);
  glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0);

  // ambient light intensity: global constant
  // In OpenGL there is global ambient intensity (like Ia in the Phong model we discussed 
  // in class.
  // Additionally each light can add some amount to that ambient light.
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

/* --------------------------------------------- */
/* ------... AND MATERIAL PROPERTIES ----------- */
/* --------------------------------------------- */

void set_material_properties ( GLfloat r, GLfloat g, GLfloat b )
{
  GLfloat mat_specular[4] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat mat_ambient_and_diffuse[4] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat mat_shininess[1] = { 0.0 };
  
  mat_specular[0] = mat_ambient_and_diffuse[0] = r;
  mat_specular[1] = mat_ambient_and_diffuse[1] = g;
  mat_specular[2] = mat_ambient_and_diffuse[2] = b;

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient_and_diffuse);
 
}

/* --------------------------------------------- */
/* ------ RENDERING ---------------------------- */
/* --------------------------------------------- */

// a single cube extending from -1 to 1 in all dimensions

GLuint draw_cube ( )
{
  glBegin(GL_QUADS);

  glNormal3f(0.0,0.0,-1.0);

  glVertex3f(1.0,1.0,-1.0);
  glVertex3f(1.0,-1.0,-1.0);
  glVertex3f(-1.0,-1.0,-1.0);
  glVertex3f(-1.0,1.0,-1.0);
  
  glNormal3f(0.0,0.0,1.0);

  glVertex3f(1.0,1.0,1.0);
  glVertex3f(-1.0,1.0,1.0);
  glVertex3f(-1.0,-1.0,1.0);
  glVertex3f(1.0,-1.0,1.0);

  glNormal3f(1.0,0.0,0.0);

  glVertex3f(1.0,-1.0,1.0);
  glVertex3f(1.0,-1.0,-1.0);
  glVertex3f(1.0,1.0,-1.0);
  glVertex3f(1.0,1.0,1.0);

  glNormal3f(-1.0,0.0,0.0);

  glVertex3f(-1.0,-1.0,1.0);
  glVertex3f(-1.0,1.0,1.0);
  glVertex3f(-1.0,1.0,-1.0);
  glVertex3f(-1.0,-1.0,-1.0);

  glNormal3f(0.0,1.0,0.0);

  glVertex3f(-1.0,1.0,1.0);
  glVertex3f(1.0,1.0,1.0);
  glVertex3f(1.0,1.0,-1.0);
  glVertex3f(-1.0,1.0,-1.0);

  glNormal3f(0.0,-1.0,0.0);

  glVertex3f(-1.0,-1.0,-1.0);
  glVertex3f(1.0,-1.0,-1.0);
  glVertex3f(1.0,-1.0,1.0);
  glVertex3f(-1.0,-1.0,1.0);

  glEnd();
}

/* --------------------------------------------- */

// all cubes

GLuint draw_scene ( )
{
  set_material_properties(1.0,1.0,1.0);

  draw_cube();

  set_material_properties(1.0,0.0,0.0);

  glPushMatrix();
    glTranslatef(-1.0,-1.0,-1.0);
    glScalef(.4,.4,.4);
    draw_cube();
  glPopMatrix();

  set_material_properties(0.0,1.0,0.0);
  
  glPushMatrix();
    glTranslatef(-1.0,-1.0,1.0);
    glScalef(.4,.4,.4);
    draw_cube();
  glPopMatrix();

  set_material_properties(0.0,0.0,1.0);
  
  glPushMatrix();
    glTranslatef(-1.0,1.0,-1.0);
    glScalef(.4,.4,.4);
    draw_cube();
  glPopMatrix();
  
  set_material_properties(0.5,0.0,0.5);

  glPushMatrix();
    glTranslatef(1.0,-1.0,-1.0);
    glScalef(.4,.4,.4);
    draw_cube();
  glPopMatrix();
}

/* --------------------------------------------- */

/* draw callback: clear window, set up matrices, draw all cubes */

void draw()
{
  /* set the projection matrix */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(8.0,1.0,15.0,25.0);

  // ... and modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* initialize light -- note that modelview matrix is identity here
     so light source coordinates won't be transformed --
     this is equivalent to specifying light source location in 
     the world coordinates */
  init_lightsource();

  // update angles
  if (animate)
    {
      angle1 += dangle1;
      angle2 += dangle2;
    }

  // build modelview matrix....
  glTranslatef(0,0,-20);
  glRotatef(angle1,1,2,3);
  glRotatef(angle2,-2,-1,0);

  /* ensure we're drawing to the correct GLUT window */
  glutSetWindow(wid);

  /* clear the color buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* draw cubes; note that flat shading is the default -- use glShadeModel to change */
  draw_scene();

  /* flush the pipeline */
  glFlush();

  /* look at our handiwork */
  glutSwapBuffers();

  // make sure everything is redrawn in the animation mode
  if (animate)
    glutPostRedisplay();  
}

/* --------------------------------------------- */
/* ------- INTERACTION ------------------------- */
/* --------------------------------------------- */

/* handle mouse events */

void mouse_button(GLint btn, GLint state, GLint mx, GLint my)
{
  switch( btn ) {
    case GLUT_LEFT_BUTTON:
      cout << "Left Button"; // remove this line from your final submission
      switch( state ) {
        case GLUT_DOWN: 
	  cout << " down" << endl; // remove this line from your final submission
	  break;
        case GLUT_UP:   
	  cout << " up" << endl; // remove this line from your final submission
	  break;
      }
      break;
    case GLUT_MIDDLE_BUTTON:
      cout << "Middle Button"; // remove this line from your final submission
      switch( state ) {
        case GLUT_DOWN: 
	  cout << " down" << endl; // remove this line from your final submission
	  break;
        case GLUT_UP:   
	  cout << " up" << endl; // remove this line from your final submission
	  break;
      }
      break;
    case GLUT_RIGHT_BUTTON:
      cout << "Right Button"; // remove this line from your final submission
      switch( state ) {
        case GLUT_DOWN: 
	  cout << " down" << endl; // remove this line from your final submission
	  break;
        case GLUT_UP:   
	  cout << " up" << endl; // remove this line from your final submission
	  break;
      }
      break;
  }
}

/* --------------------------------------------- */

// mouse moves with button down

GLvoid button_motion(GLint mx, GLint my)
{
  cout << "Motion with button down: " << mx << "," << my << endl; // remove this line from your final submission
  return;
}

/* --------------------------------------------- */

// mouse moves with button up

GLvoid passive_motion(GLint mx, GLint my)
{
  cout << "Passive Motion: " << mx << "," << my << endl; // remove this line from your final submission
  return;
}

/* --------------------------------------------- */

/* handle keyboard events; here, just exit if ESC is hit */

void keyboard(GLubyte key, GLint x, GLint y)
{
  switch(key) {
    case 27:  /* ESC */
              exit(0);

    default:  break;
  }
}

/* --------------------------------------------- */

/* menu callback */

void menu ( int value )
{
  switch(value)
    {
    case MENU_SLOWER:
      dangle1 *= .5;
      dangle2 *= .5;
      break;
    case MENU_FASTER:
      dangle1 *= 1.5;
      dangle2 *= 1.5;
      break;
    case MENU_STOP_RUN:
      animate = !animate;
      break;
    }
}

/* --------------------------------------------- */

/* handle resizing the glut window */

GLvoid reshape ( GLint sizex, GLint sizey )
{
  glutSetWindow(wid);

  vpw = sizex;
  vph = sizey;

  glViewport(0, 0, vpw, vph);
  glutReshapeWindow(vpw, vph);

  glutPostRedisplay();
}

/* --------------------------------------------- */
/* -------- SET UP GLUT  ----------------------- */
/* --------------------------------------------- */

GLint init_glut(GLint *argc, char **argv)
{
  GLint id;

  glutInit(argc,argv);

  /* size and placement hints to the window system */
  glutInitWindowSize(vpw, vph);
  glutInitWindowPosition(10,10);

  /* double buffered, RGB color mode */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  /* create a GLUT window (not drawn until glutMainLoop() is entered) */
  id = glutCreateWindow("MACS441 OpenGL Sample code");    

  /* register callbacks */

  /* window size changes */
  glutReshapeFunc(reshape);

  /* keypress handling when the current window has input focus */
  glutKeyboardFunc(keyboard);

  /* mouse event handling */
  glutMouseFunc(mouse_button);           /* button press/release        */
  glutMotionFunc(button_motion);         /* mouse motion w/ button down */
  glutPassiveMotionFunc(passive_motion); /* mouse motion with button up */

  /* window obscured/revealed event handler */
  glutVisibilityFunc(NULL);

  /* handling of keyboard SHIFT, ALT, CTRL keys */
  glutSpecialFunc(NULL);

  /* what to do when mouse cursor enters/exits the current window */
  glutEntryFunc(NULL);

  /* what to do on each display loop iteration */
  glutDisplayFunc(draw);

  /* create menu */
  GLint menuID = glutCreateMenu(menu);
  glutAddMenuEntry("slower",MENU_SLOWER);
  glutAddMenuEntry("faster",MENU_FASTER);
  glutAddMenuEntry("stop/run",MENU_STOP_RUN);
  glutSetMenu(menuID);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  return id;
}

/* --------------------------------------------- */
/* ----- TYPICAL OPENGL SETTINGS FOR 3D -------- */
/* --------------------------------------------- */

void init_opengl( )
{
  /* back-face culling on */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  /* automatically scale normals to unit length after transformation */
  glEnable(GL_NORMALIZE);

  /* clear to BLACK */
  glClearColor(0.0, 0.0, 0.0, 1.0);

  /* Enable depth test  */
  glEnable(GL_DEPTH_TEST);
}

/* --------------------------------------------- */
/* --------------------------------------------- */
/* --------------------------------------------- */

GLint main(GLint argc, char **argv)
{  
  /* initialize GLUT: register callbacks, etc */
  wid = init_glut(&argc, argv);

  /* any OpenGL state initialization we need to do */
  init_opengl();

  glutMainLoop();

  return 0;
}

/* --------------------------------------------- */
