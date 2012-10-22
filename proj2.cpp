// Riley Moses

#include <cmath>
#include <cstdlib>
#include <cassert>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Vector.h"
#include "IOstuff.h"
#include "functions.h"

using namespace std;

// OpenGL variables
static const int VIEWPORT_DEFAULT = 800;
static const int MENU_TOGGLE_SHADER = 1;
static const int MENU_ZOOM_IN = 2;
static const int MENU_ZOOM_OUT = 3;
static const int MENU_TOGGLE_CULLING = 4;
static const int MENU_RESET_VIEW = 5;
static const int MENU_RESET_ZOOM = 6;
static const int MENU_RESET_ROTATION = 7;
static const int MENU_SLOWER = 8;
static const int MENU_FASTER = 9;
static const int MENU_STOP_RUN = 10;
static const double TWOPI = (2.0 * M_PI);
// Counters
int numTriangles = 0;
int numVertices = 0;
// STL vectors to hold the triangles, vertices, triangle normals, and vertex normals
vector<triangle> triangleTable;
vector<vertex> vertexTable;
vector<Vector> triangleNormals;
vector<Vector> vertexNormals;
// Bounding box
double xmin, xmax, ymin, ymax, zmin, zmax, maxdim;
// Field of view angle
double clippingFov = 8.0;
double initialFov = 10.0;
double currentFov = initialFov;
double d = (1.0 / tan(toRadians(clippingFov / 2.0)));
// Whether or not smooth shading is being used
bool smoothShading = true;
// Whether or not back-face culling is being used
bool backFaceCulling = true;
// Centered at the origin
const Vector center(0, 0, 0);
// Point under the pixel
Vector p;
Vector q;
double tempX = 0;
double tempY = 0;
// Rotation matrices
double R[16];	// The superposition of all ﬁnished rotations
double R0[16];	// The rotation that is currently being speciﬁed
// Coordinates of the last mouse button down event
int i_0 = 0;
int j_0 = 0;
// Flag to ignore stray mouse events
bool buttonDown = false;

// GLUT window id; value asigned in main() and should stay constant
GLint windowID;
// Viewport dimensions; change when window is resized (via resize callback)
GLint viewport_width = VIEWPORT_DEFAULT;
GLint viewport_height = VIEWPORT_DEFAULT;

// Angles used in animation
GLfloat angle1 = 0;
GLfloat angle2 = 0;
GLfloat dangle1 = 0.0057;
GLfloat dangle2 = 0.0071;
// Whether or not to animate
bool animate = false;

Vector getPointUnderPixel(double mouseX, double mouseY)
{
	tempX = ((2.0 * mouseX) / (viewport_width - 1.0)) - 1.0;
	tempY = -(((2.0 * mouseY) / (viewport_height - 1.0)) - 1.0);
	double possibleZ = (1.0 - pow(tempX, 2) - pow(tempY, 2));
	if (possibleZ >= 0)
	{
		return Vector(tempX, tempY, sqrt(possibleZ));
	}
	else
	{
		return Vector((tempX / sqrt(pow(tempX, 2) + pow(tempY, 2))), (tempY / sqrt(pow(tempX, 2) + pow(tempY, 2))), 0);
	}
}

void toggleCulling()
{
	if (backFaceCulling)
	{
		glCullFace(GL_FRONT);
		backFaceCulling = false;
	}
	else
	{
		glCullFace(GL_BACK);
		backFaceCulling = true;
	}
	glutPostRedisplay();
}

void toggleShading()
{
	smoothShading = !smoothShading;
	glutPostRedisplay();
}

void toggleAnimation()
{
	animate = !animate;
	glutPostRedisplay();
}

void zoomIn()
{
	if (currentFov / 1.025 > 0.1)
	{
		currentFov /= 1.025;
		glutPostRedisplay();
	}
}

void zoomOut()
{
	if (currentFov * 1.025 < 179.0)
	{
		currentFov *= 1.025;
		glutPostRedisplay();
	}
}

void speedUp()
{
	dangle1 *= 1.5;
	dangle2 *= 1.5;
}

void slowDown()
{
	dangle1 *= .5;
	dangle2 *= .5;
}

void resetZoom()
{
	currentFov = initialFov;
}

void resetRotation()
{
	setIdentity(R);
	setIdentity(R0);
	animate = false;
	angle1 = angle2 = 0;
}

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
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

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

// Flat shading: One normal for the surface
GLuint draw_model_flat()
{
	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangleTable.size(); ++i)
	{
		glNormal3f(triangleNormals.at(i).x, triangleNormals.at(i).y, triangleNormals.at(i).z);
		triangle t(triangleTable.at(i).v1, triangleTable.at(i).v2, triangleTable.at(i).v3);
		glVertex3f(vertexTable.at(t.v1).x, vertexTable.at(t.v1).y, vertexTable.at(t.v1).z);
		glVertex3f(vertexTable.at(t.v3).x, vertexTable.at(t.v3).y, vertexTable.at(t.v3).z);
		glVertex3f(vertexTable.at(t.v2).x, vertexTable.at(t.v2).y, vertexTable.at(t.v2).z);
	}
	glEnd();
}

// Smooth shading: One normal per vertex
GLuint draw_model_smooth()
{
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangleTable.size(); ++i)
	{
		triangle t(triangleTable.at(i).v1, triangleTable.at(i).v2, triangleTable.at(i).v3);

		glNormal3f(vertexNormals.at(t.v1).x, vertexNormals.at(t.v1).y, vertexNormals.at(t.v1).z);
		glVertex3f(vertexTable.at(t.v1).x, vertexTable.at(t.v1).y, vertexTable.at(t.v1).z);

		glNormal3f(vertexNormals.at(t.v3).x, vertexNormals.at(t.v3).y, vertexNormals.at(t.v3).z);
		glVertex3f(vertexTable.at(t.v3).x, vertexTable.at(t.v3).y, vertexTable.at(t.v3).z);

		glNormal3f(vertexNormals.at(t.v2).x, vertexNormals.at(t.v2).y, vertexNormals.at(t.v2).z);
		glVertex3f(vertexTable.at(t.v2).x, vertexTable.at(t.v2).y, vertexTable.at(t.v2).z);
	}
	glEnd();
}

// All cubes
GLuint draw_scene()
{
	set_material_properties(1.0,1.0,1.0);

	// REVERSE ORDER
	glPushMatrix();
		// Translation forward
		glTranslatef(0, 0, -1.0 - d);
		// Scale
		glScalef((2.0 / maxdim), (2.0 / maxdim), (2.0 / maxdim));
		// Rotation matrix (trackball)
		glMultMatrixd(R0);
		glMultMatrixd(R);
		glRotatef(angle1,1,2,3);
		glRotatef(angle2,-2,-1,0);
		// Translate by minus center of bounding box
		glTranslatef(-((xmin + xmax) / 2.0), -((ymin + ymax) / 2.0), -((zmin + zmax) / 2.0));
		if (smoothShading)
		{
			draw_model_smooth();
		}
		else
		{
			draw_model_flat();
		}
	glPopMatrix();
}

// Draw callback: clear window, set up matrices, draw all cubes
void draw()
{
	// Set the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(currentFov, 1.0, d - 1.0, d + 3.0);

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

	// Ensure we're drawing to the correct GLUT window
	glutSetWindow(windowID);

	// Clear the color buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw; smooth shading is the default (use glShadeModel to change)
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
void mouse_button(GLint btn, GLint state, GLint mouseX, GLint mouseY)
{
	switch (btn)
	{
		case GLUT_LEFT_BUTTON:
			switch (state)
			{
				case GLUT_DOWN:
					buttonDown = true;
					// Begin dragging trackball, assign the event coordinates to i_0 and j_0
					i_0 = mouseX;
					j_0 = mouseY;
					// Point p is the point on the sphere underneath (i_0, j_0)
					p = getPointUnderPixel(i_0, j_0);
					break;
				case GLUT_UP:
					if (buttonDown)
					{
						glMatrixMode(GL_MODELVIEW);
						// Save the current matrix
						glPushMatrix();
							// Load R0
							glLoadMatrixd(R0);
							// Apply R
							glMultMatrixd(R);
							// Modelview → R
							glGetDoublev(GL_MODELVIEW_MATRIX, R);
						// Restore the original modelview matrix
						glPopMatrix();
						setIdentity(R0);
						buttonDown = false;
					}
					break;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			switch (state)
			{
				case GLUT_DOWN: 
					break;
				case GLUT_UP:   
					break;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			switch (state)
			{
				case GLUT_DOWN: 
					break;
				case GLUT_UP:   
					break;
			}
			break;
	}
}

// Mouse moves with button down
GLvoid button_motion(GLint mouseX, GLint mouseY)
{
	if (buttonDown)
	{
		if (mouseX == i_0 && mouseY == j_0)
		{
			setIdentity(R0);
		}
		else
		{
			q = getPointUnderPixel(mouseX, mouseY);

			Vector rotAxis = p.crossWith(q);
			double rotAngle = toDegrees(acos(p.dotWith(q)));

			glMatrixMode(GL_MODELVIEW);
			// Save the current matrix
			glPushMatrix();
				// Set modelview matrix to identity
				glLoadIdentity();
				// Apply the rotation
				glRotated(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);
				// Modelview → R0
				glGetDoublev(GL_MODELVIEW_MATRIX, R0);
			// Restore the original modelview matrix
			glPopMatrix();
		}

		glutPostRedisplay();
	}
	return;
}

// Mouse moves with button up
GLvoid passive_motion(GLint mouseX, GLint mouseY)
{
	return;
}

// Handle keyboard events
void keyboard(GLubyte key, GLint x, GLint y)
{
	switch (key)
	{
		// Exit when escape is pressed
		case 27:
			exit(0);
		case 's':
		case 'S':
			toggleShading();
			break;
		case 'c':
		case 'C':
			toggleCulling();
			break;
		case 'a':
		case 'A':
			toggleAnimation();
			break;
		case 'q':
		case 'Q':
			speedUp();
			break;
		case 'w':
		case 'W':
			slowDown();
			break;
		default:
			break;
	}
}

// Handle special keys
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			zoomIn();
			break;
		case GLUT_KEY_DOWN:
			zoomOut();
			break;
		default:
			break;
	}
}

// Menu callback
void menu(int value)
{
	switch (value)
	{
		case MENU_TOGGLE_SHADER:
			toggleShading();
			break;
		case MENU_ZOOM_IN:
			zoomIn();
			break;
		case MENU_ZOOM_OUT:
			zoomOut();
			break;
		case MENU_TOGGLE_CULLING:
			toggleCulling();
			break;
		case MENU_RESET_ZOOM:
			resetZoom();
			glutPostRedisplay();
			break;
		case MENU_RESET_ROTATION:
			resetRotation();
			glutPostRedisplay();
			break;
		case MENU_RESET_VIEW:
			resetZoom();
			resetRotation();
			glutPostRedisplay();
			break;
		case MENU_SLOWER:
			slowDown();
			break;
		case MENU_FASTER:
			speedUp();
			break;
		case MENU_STOP_RUN:
			toggleAnimation();
			break;
	}
}

// Handle resizing the window
GLvoid reshape(GLint sizex, GLint sizey)
{
	glutSetWindow(windowID);

	viewport_width = sizex;
	viewport_height = sizey;

	glViewport(0, 0, viewport_width, viewport_height);
	glutReshapeWindow(viewport_width, viewport_height);

	glutPostRedisplay();
}

// Set up GLUT
GLint init_glut(GLint *argc, char **argv)
{
	GLint id;

	glutInit(argc, argv);

	// Size and placement hints to the window system
	glutInitWindowSize(viewport_width, viewport_height);
	glutInitWindowPosition(10,10);

	// Double-buffered, RGB color mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create a GLUT window (not drawn until glutMainLoop() is entered)
	id = glutCreateWindow("CSCI441 Project 2");    

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

	// Handling of special keyboard keys (F-keys, arrowkeys, shift, alt, etc.)
	glutSpecialFunc(specialKeys);

	// What to do when mouse cursor enters/exits the current window
	glutEntryFunc(NULL);

	// What to do on each display loop iteration
	glutDisplayFunc(draw);

	// Create menus
	GLint resetMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Zoom", MENU_RESET_ZOOM);
	glutAddMenuEntry("Rotation", MENU_RESET_ROTATION);
	glutAddMenuEntry("All", MENU_RESET_VIEW);

	GLint animMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Faster (Q)", MENU_FASTER);
	glutAddMenuEntry("Slower (W)", MENU_SLOWER);
	glutAddMenuEntry("Start/Stop (A)", MENU_STOP_RUN);

	GLint mainMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Toggle Shading Model (S)", MENU_TOGGLE_SHADER);
	glutAddMenuEntry("Zoom In (Up Arrow)", MENU_ZOOM_IN);
	glutAddMenuEntry("Zoom Out (Down Arrow)", MENU_ZOOM_OUT);
	glutAddMenuEntry("Toggle Culling Orientation (C)", MENU_TOGGLE_CULLING);
	glutAddSubMenu("Animation", animMenu);
	glutAddSubMenu("Reset", resetMenu);

	glutSetMenu(mainMenu);
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

GLint main(GLint argc, char *argv[])
{
	// Check for input file argument
	string filename = "";
	if (argc != 2)
	{
		filename = "input.t";
	}
	else
	{
		filename = argv[1];
	}

	// Read from input file
	readInputFile(filename, numTriangles, numVertices, triangleTable, vertexTable);
	// Calculate normals
	calcNormals(triangleTable, vertexTable, triangleNormals, vertexNormals);
	// Calculate bounding box
	calcBoundingBox(vertexTable, xmin, xmax, ymin, ymax, zmin, zmax, maxdim);
	// Initialize R and R0 to identity
	setIdentity(R);
	setIdentity(R0);

	// Initialize GLUT: register callbacks, etc.
	windowID = init_glut(&argc, argv);

	// Take care of any OpenGL state initialization
	init_opengl();

	// Start loop
	glutMainLoop();

	return 0;
}