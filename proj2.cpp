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

// Global variables
static const int VPD_DEFAULT = 800;
static const int MENU_SLOWER = 1;
static const int MENU_FASTER = 2;
static const int MENU_STOP_RUN = 3;
static const double TWOPI = (2.0 * M_PI);

// GLUT window id; value asigned in main() and should stay constant
GLint wid;
// Viewport dimensions; change when window is resized (via resize callback)
GLint vpw = VPD_DEFAULT;
GLint vph = VPD_DEFAULT;

// Angles used in animation
GLfloat angle1 = 0;
GLfloat angle2 = 0;
GLfloat dangle1 = 0.0057;
GLfloat dangle2 = 0.0071;
// Whether or not to animate
bool animate = true;

// Set light source properties
void init_lightsource()
{
  GLfloat light_ambient[] = { .0, .0, .0, 1.0 };
  GLfloat light_diffuse[] = { .9, .9, .9, 1.0 };
  GLfloat light_specular[] = { 0, 0, 0, 1.0 };
  GLfloat light_position[] = { 200.0, 200.0, 200.0, 1.0 };
  GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
  glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0);
  glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0);

  // Ambient light intensity: global constant
  // In OpenGL there is global ambient intensity, like Ia in the Phong model
  // Additionally, each light can add some amount to that ambient light
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

// Set material properties
void set_material_properties(GLfloat r, GLfloat g, GLfloat b)
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

// Rendering

// A single cube extending from -1 to 1 in all dimensions
GLuint draw_cube()
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

// All cubes
GLuint draw_scene()
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


// Draw callback: clear window, set up matrices, draw all cubes
void draw()
{
  // Set the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(8.0,1.0,15.0,25.0);

  // Set the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Initialize light
  // Note that modelview matrix is identity here so light source coordinates won't be transformed
  // This is equivalent to specifying light source location in the world coordinates
  init_lightsource();

  // Update angles
  if (animate)
  {
    angle1 += dangle1;
    angle2 += dangle2;
  }

  // Build modelview matrix
  glTranslatef(0,0,-20);
  glRotatef(angle1,1,2,3);
  glRotatef(angle2,-2,-1,0);

  // Ensure we're drawing to the correct GLUT window
  glutSetWindow(wid);

  // Clear the color buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw cubes; note that flat shading is the default (use glShadeModel to change)
  draw_scene();

  // Flush the pipeline
  glFlush();

  // Look at our handiwork
  glutSwapBuffers();

  // Make sure everything is redrawn in the animation mode
  if (animate)
  {
    glutPostRedisplay();
  } 
}

// Interaction
// Handle mouse events
void mouse_button(GLint btn, GLint state, GLint mx, GLint my)
{
  switch(btn)
  {
    case GLUT_LEFT_BUTTON:
      cout << "Left Button"; // remove this line from your final submission
      switch(state)
      {
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
      switch( state )
      {
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
      switch( state )
      {
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

// Mouse moves with button down
GLvoid button_motion(GLint mx, GLint my)
{
  cout << "Motion with button down: " << mx << "," << my << endl; // remove this line from your final submission
  return;
}

// Mouse moves with button up
GLvoid passive_motion(GLint mx, GLint my)
{
  cout << "Passive Motion: " << mx << "," << my << endl; // remove this line from your final submission
  return;
}


// Handle keyboard events
void keyboard(GLubyte key, GLint x, GLint y)
{
  switch(key)
  {
    // Exit when escape is pressed
    case 27:
      exit(0);
    default:
      break;
  }
}


// Menu callback
void menu(int value)
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

// Handle resizing the window
GLvoid reshape(GLint sizex, GLint sizey)
{
  glutSetWindow(wid);

  vpw = sizex;
  vph = sizey;

  glViewport(0, 0, vpw, vph);
  glutReshapeWindow(vpw, vph);

  glutPostRedisplay();
}

// Set up GLUT
GLint init_glut(GLint *argc, char **argv)
{
  GLint id;

  glutInit(argc,argv);

  // Size and placement hints to the window system
  glutInitWindowSize(vpw, vph);
  glutInitWindowPosition(10,10);

  // Double-buffered, RGB color mode
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Create a GLUT window (not drawn until glutMainLoop() is entered)
  id = glutCreateWindow("MACS441 OpenGL Sample code");    

  // Register callbacks
  // Window size changes
  glutReshapeFunc(reshape);

  // Keypress handling when the current window has input focus
  glutKeyboardFunc(keyboard);

  // Mouse event handling
  // Button press/release
  glutMouseFunc(mouse_button);
  // Mouse motion with button down
  glutMotionFunc(button_motion);
  // Mouse motion with button up
  glutPassiveMotionFunc(passive_motion);

  // Window obscured/revealed event handler
  glutVisibilityFunc(NULL);

  // Handling of keyboard SHIFT, ALT, CTRL keys
  glutSpecialFunc(NULL);

  // What to do when mouse cursor enters/exits the current window
  glutEntryFunc(NULL);

  // What to do on each display loop iteration
  glutDisplayFunc(draw);

  // Create menu
  GLint menuID = glutCreateMenu(menu);
  glutAddMenuEntry("slower", MENU_SLOWER);
  glutAddMenuEntry("faster", MENU_FASTER);
  glutAddMenuEntry("stop/run", MENU_STOP_RUN);
  glutSetMenu(menuID);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  return id;
}

// OpenGL settings for 3D
void init_opengl()
{
  // Enable back-face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Automatically scale normals to unit length after transformation
  glEnable(GL_NORMALIZE);

  // Clear to BLACK
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
}

GLint main(GLint argc, char **argv)
{  
  // Initialize GLUT: register callbacks, etc.
  wid = init_glut(&argc, argv);

  // Take care of any OpenGL state initialization
  init_opengl();

  // Start loop
  glutMainLoop();

  return 0;
}