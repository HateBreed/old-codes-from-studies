#include "project.h"
#include "drawobj.h"

#define INIT_TIMER 2
#define SPRUCES 1000

static int pillar, pyr, spruc, pot,ground;
static GLfloat pos_x = 10.0, pos_y = 1.8, pos_z = 20.0;
static GLfloat rot_x = 800.0, rot_y = 350.0;
static int mouse_last_x = 0, mouse_last_y = 0;
static int mouse_x = 0, mouse_y = 0;
static int menu_mx = 0, menu_my = 0;
static int fillpoly = 1, make_axes = 0,flymode = 1, foggy = 1;
static int controlmode = 1, keyb_toggles = 1;
static int movement = 0;
static int mousereset = 0, doreset = 1;

static GLfloat speed = 1.0;

static int light1 = 1, light2 = 1;

static GLfloat pyramid_x = 20.0, pyramid_y = 20.0, pyramid_z = 15.0;
static GLfloat pyramid_r = 0.0;
static GLfloat temp = 0.0;

static GLfloat gen_scaling_factor = 0.2;
static GLfloat pillar_height_scale = 1.0;
static GLfloat pot_scaling_factor = 0.05;

GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };

int spruce_pos_x[SPRUCES], spruce_pos_z[SPRUCES];

GLfloat light = -0.5, spot = 0.0;

int main(int argc, char *argv[])
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutCreateWindow("Project");
	
	// Callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutPassiveMotionFunc(mouse_movement);
	
	// Change crosshair
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutMenuStatusFunc(menu_state);
	
	// Set mouse to center
	mousereset = 1;
	reset_mouse();
	
	create_coords(spruce_pos_x,spruce_pos_z,SPRUCES);
	
	init();
	
	glutTimerFunc(10,move_pyramid,0);
	lightness(INIT_TIMER);
	spot_lightness(INIT_TIMER);
	glutMainLoop();
	
	return 0;
}

static void init()
{	
	GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = {0.5,0.5,0.5,1.0};
	GLfloat mat_shininess[] = { 50.0 };
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_LIGHTING);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	
	ground = glGenLists(1);
	glNewList(ground,GL_COMPILE);
	make_ground();
	glEndList();
	
	pillar = glGenLists(1);
	glNewList(pillar,GL_COMPILE);
	make_pillar();
	glEndList();
	
	glEnable(GL_CULL_FACE);
	pyr = glGenLists(1);
	glNewList(pyr,GL_COMPILE);
	make_pyramid();
	glEndList();
	
	spruc = glGenLists(1);
	glNewList(spruc,GL_COMPILE);
	make_spruce();
	glEndList();
	
	pot = glGenLists(1);
	glNewList(pot,GL_COMPILE);
	make_pot();
	glEndList();
	
	// If fog enabled
	if(foggy == 1)
	{
		// Set color to be same as fog color
		glClearColor(fogColor[0],fogColor[1],fogColor[2],fogColor[3]);
		glFogfv(GL_FOG_COLOR, fogColor);
	
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogf(GL_FOG_DENSITY, 0.01);
	
		glHint(GL_FOG_HINT, GL_NICEST);
	
		glEnable(GL_FOG);
	}
	else
	{
		glClearColor(0.0,0.0,0.0,1.0);
		glDisable(GL_FOG);
	}
	
	create_menu();
}

static void display(void)
{
	int i,j;
	GLfloat pos1[] = {85,45,135,1.0f};
	
	GLfloat dir1[] = {85,40,135,1.0f};
	
	GLfloat pos2[] = {500,200,500,0.0f};
	GLfloat dir2[] = {500,0,500,1.0};
	
	GLfloat ambl[]= { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat difl[]= { light, light, light, 1.0f };
	GLfloat spcl[] = { 1.0, 1.0, 1.0, 1.0 };
	
	GLfloat ambl1[]= { 0.0f, 0.0f, 0.2f, 1.0f };
	GLfloat difl1[]= { 0.0f, 0.0f, 0.5f, 1.0f };
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// Rotate scenario
	glRotatef(rot_y,1.0,0.0,0.0);
	glRotatef(rot_x,0.0,1.0,0.0);
	
	// Translate coordinates
	glTranslatef(-pos_x,-pos_y,-pos_z);
	
	// Set blue blinking light
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambl1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difl1);
	glLightfv(GL_LIGHT1,GL_POSITION,pos1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF,360*spot); // Blinking
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir1);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,1.0f);
	
	// Set ambient light
	glLightfv(GL_LIGHT2,GL_AMBIENT, difl);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, ambl);
	glLightfv(GL_LIGHT2,GL_POSITION,pos2);
	
	// Check which light to enable
	if(light1 == 1)
	{
		if(!glIsEnabled(GL_LIGHT1)) glEnable(GL_LIGHT1);
	}
	else
	{
		if(glIsEnabled(GL_LIGHT1)) glDisable(GL_LIGHT1);
	}


	if(light2 == 1)
	{
		if(!glIsEnabled(GL_LIGHT2)) glEnable(GL_LIGHT2);
	}
	else
	{
		if(glIsEnabled(GL_LIGHT2)) glDisable(GL_LIGHT2);
	}
	
	// Axes and ground
	if(make_axes == 1) draw_axes();
	
	glPushMatrix();
	glCallList(ground);
	glPopMatrix();
	
	// Create towers for building
	for(j = 0; j < 10; j++)
	{
		for(i = 0; i < 8; i++)
		{
			if(((j==0) || (j==9)) || ((i == 0) || (i==7)))
			{
				glPushMatrix();
				glTranslatef(50+(10*j),20,100+(i*10));
				glColor3f(0.0,0.8-(i*0.1),0.8-(j*0.1));
				glCallList(pillar);
				glPopMatrix();
			}
		}
	}
	
	// Create pyramid
	glPushMatrix();
	glTranslatef(pyramid_x,pyramid_y,pyramid_z);
	if(movement != 2 && movement != 3)glRotatef(pyramid_r,0.0,1.0,0.0);
	glColor3f(1.0,0.0,1.0);
	glCallList(pyr);
	glPopMatrix();
	
	
	// Roof
	glPushMatrix();
	glTranslatef(95,50,135);
	glColor3f(1.0,0.0,1.0);
	glScalef(25.0,5.0,20.0);
	glCallList(pyr);
	glPopMatrix();
	
	// Create spruces
	for(i = 0; i < 200; i++)
	{
		glPushMatrix();
		glTranslatef(spruce_pos_x[i],2,spruce_pos_z[i]);
		glColor3f(0.0,1.0,0.0);
		glCallList(spruc);
		glPopMatrix();
	}
	
	// Create flowerpots inside the building
	for(j = 0; j < 7; j++)
	{
		for(i = 0; i < 16; i++)
		{
			glPushMatrix();
			glTranslatef(55 + (i*5),0.5,105 + (j*10) + (i % 2));
			glColor3f(0.0,0.8,0.3);
			glCallList(pot);
			glPopMatrix();
		}
	}
#ifdef LINUX
	if((doreset == 1) && (mousereset == 2)) glutPassiveMotionFunc(mouse_movement); // linux 
#endif
	glutSwapBuffers();
}

// Reshape the screen
static void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)width / (GLfloat)height, 1.0, 1500.0);
	glMatrixMode(GL_MODELVIEW);
	
}


// Controls for enabling 
static void keyboard(unsigned char key, int x, int y)
{
	float rot_x_rad = 0.0, rot_y_rad = 0.0;
	
	if(controlmode == 1)
	{
		switch(key)
		{
			// Step Forward
			case 'w':
				rot_x_rad = (rot_x / 180 * 3.141592654f);
				
				// Set position based on rotation x - axis 
				if((pos_x + (sin(rot_x_rad)) > 0) && (pos_x + (sin(rot_x_rad)) < FLOORMAX))
					pos_x += (sin(rot_x_rad));
				
				// Set position based on rotation z - axis
				if((pos_z - (cos(rot_x_rad)) > 0) && (pos_z - (cos(rot_x_rad)) < FLOORMAX))
					pos_z -= (cos(rot_x_rad));
				
				// If flying
				if(flymode == 1)
				{
					// Set position based on rotation y - axis
					rot_y_rad = (rot_y / 180 * 3.141592654f);
					pos_y -= ((sin(rot_y_rad)));
				}
				break;
			// Step back
			case 's':
				rot_x_rad = (rot_x / 180 * 3.141592654f);
				// Set position based on rotation x - axis 
				if((pos_x - (sin(rot_x_rad)) > 0) && (pos_x - (sin(rot_x_rad)) < FLOORMAX))
					pos_x -= ((sin(rot_x_rad)));
				// Set position based on rotation z - axis
				if((pos_z + (cos(rot_x_rad)) > 0) && (pos_z + (cos(rot_x_rad)) < FLOORMAX))
					pos_z += ((cos(rot_x_rad)));
				
				// If flying
				if(flymode == 1)
				{
					// Set position based on rotation y - axis
					rot_y_rad = (rot_y / 180 * 3.141592654f);
					pos_y += ((sin(rot_y_rad)));
				}
				
				break;
			// Strafe right
			case 'd':
				rot_x_rad = (rot_x / 180 * 3.141592654);
				if((pos_x + (cos(rot_x_rad)) > 0) && (pos_x + (cos(rot_x_rad)) < FLOORMAX) &&
					(pos_z + (sin(rot_x_rad)) > 0) && (pos_z + (sin(rot_x_rad)) < FLOORMAX))
					{
						pos_x += cos(rot_x_rad) * 0.8;
						pos_z += sin(rot_x_rad) * 0.8;
					}
				break;
			// Strafe left
			case 'a':
				rot_x_rad = (rot_x / 180 * 3.141592654);
				if((pos_x - (cos(rot_x_rad)) > 0) && (pos_x - (cos(rot_x_rad)) < FLOORMAX) &&
					(pos_z - (sin(rot_x_rad)) > 0) && (pos_z - (sin(rot_x_rad)) < FLOORMAX))
					{
						pos_x -= cos(rot_x_rad) * 0.8;
						pos_z -= sin(rot_x_rad) * 0.8;
					}
				break;
		}
	}
	// Used in "Arrowkey controls"
	else
	{
		switch(key)
		{
			// Strafe right
			case '.':
				rot_x_rad = (rot_x / 180 * 3.141592654);
				if((pos_x + (cos(rot_x_rad)) > 0) && (pos_x + (cos(rot_x_rad)) < FLOORMAX) &&
					(pos_z + (sin(rot_x_rad)) > 0) && (pos_z + (sin(rot_x_rad)) < FLOORMAX))
					{
						pos_x += cos(rot_x_rad) * 0.8;
						pos_z += sin(rot_x_rad) * 0.8;
					}
				break;
			// Strafe left
			case ',':
				rot_x_rad = (rot_x / 180 * 3.141592654);
				if((pos_x - (cos(rot_x_rad)) > 0) && (pos_x - (cos(rot_x_rad)) < FLOORMAX) &&
					(pos_z - (sin(rot_x_rad)) > 0) && (pos_z - (sin(rot_x_rad)) < FLOORMAX))
					{
						pos_x -= cos(rot_x_rad) * 0.8;
						pos_z -= sin(rot_x_rad) * 0.8;
					}
				break;
		}
	}
	
	// Keyboard modificators
	if(keyb_toggles == 1)
	{
		// Filling
		switch(key)
		{
			case 'f':
				toggle_fill();
				break;
			
			// Axes
			case 't':
				toggle_axes();
				break;
			
			// Reset pyramid
			case 'r':
				reset_pyramid();
				break;
			
			// Fog
			case 'v':
				toggle_fog();
				break;
			// Space - up or jump
			
			// Flymode
			case 'x':
				toggle_flymode();
				break;
			// Controls
			case 'c':
			{
				toggle_controls();
				break;
			}
			case '1':
				toggle_light(GL_LIGHT1);
				break;glutPassiveMotionFunc(mouse_movement);
		
			case '2':
				toggle_light(GL_LIGHT2);
				break;
		}
	}
	
	// Always on
	switch(key)
	{
		case 32:
			if(flymode == 0) jump(INIT_TIMER);
			else pos_y += 1;
			break;
		// z - down or crouch
		case 'z':
			if(flymode == 0) crouch(INIT_TIMER);
			else pos_y -= 1;
			break;
		// Decrease animation speed
		case '-':
			if(speed > 0.2) speed -= 0.1;
			break;
		// Increase animation speed
		case '+':
			if(speed < 10.0) speed += 0.1;
			break;
		case 'p':
			print_keyb_style();
			break;
		// Quit
		case 'q':
			exit(0);
	}	
	
	glutPostRedisplay();
}

// Special keys - used only in classic fps control configuration
// "Arrowkey control"
static void special(int key, int x, int y)
{
	float rot_x_rad = 0.0, rot_y_rad = 0.0;
	
	if(controlmode == 0)
	{
		switch(key)
		{
			// Look up
			case GLUT_KEY_PAGE_UP:
				if((rot_y - 1 < 420) && (rot_y - 1 > 300)) rot_y -= 1;
				break;
			// Look down
			case GLUT_KEY_PAGE_DOWN:
				if((rot_y + 1 < 420) && (rot_y + 1 > 300)) rot_y += 1;
				break;
			// Turn left
			case GLUT_KEY_LEFT:
				rot_x -= 2;
				break;
			// Turn right
			case GLUT_KEY_RIGHT:
				rot_x += 2;
				break;
			// Step back
			case GLUT_KEY_DOWN:
				rot_x_rad = (rot_x / 180 * 3.141592654f);
				
				// limit movement from 0 to FLOORMAX
				if((pos_x - (sin(rot_x_rad)) > 0) && (pos_x - (sin(rot_x_rad)) < FLOORMAX))
					pos_x -= ((sin(rot_x_rad)));
				
				if((pos_z + (cos(rot_x_rad)) > 0) && (pos_z + (cos(rot_x_rad)) < FLOORMAX))
					pos_z += ((cos(rot_x_rad)));
				
				// If flying - fly
				if(flymode == 1)
				{
					rot_y_rad = (rot_y / 180 * 3.141592654f);
					pos_y += ((sin(rot_y_rad)));
				}
				break;
			// Step forward
			case GLUT_KEY_UP:
				rot_x_rad = (rot_x / 180 * 3.141592654f);
				
				// limit movement from 0 to FLOORMAX
				if((pos_x + (sin(rot_x_rad)) > 0) && (pos_x + (sin(rot_x_rad)) < FLOORMAX))
					pos_x += (sin(rot_x_rad));
				
				if((pos_z - (cos(rot_x_rad)) > 0) && (pos_z - (cos(rot_x_rad)) < FLOORMAX))
					pos_z -= (cos(rot_x_rad));
				
				// If flying - fly
				if(flymode == 1)
				{
					rot_y_rad = (rot_y / 180 * 3.141592654f);
					pos_y -= ((sin(rot_y_rad)));
				}
				break;
		}
		glutPostRedisplay();
	}
}

static void mouse_movement(int x, int y)
{
	// Remember coordinates before this happened
	mouse_x = mouse_last_x;
	mouse_y = mouse_last_y;
	
	// Remember current coordinates
	mouse_last_x = x;
	mouse_last_y = y;

	// If caused by resetting function, do nothing
	if(mousereset == 2) mousereset = 0;
	// Otherwise call reset - sets rotation and centers mouse
	else
	{
		mousereset = 1;
		reset_mouse();
	}
}

// Reset mouse poisition and calculate rotation 
static void reset_mouse()
{
	// If set to reset position 
	if(mousereset == 1)
	{
		// No recalling of reset - caused by "warping" the mouse pointer
		mousereset = 2;
		
#ifdef LINUX
		if(doreset == 1) glutPassiveMotionFunc(NULL); // linux
#endif
		// Center mouse
		if(doreset == 1) glutWarpPointer( glutGet( GLUT_WINDOW_WIDTH )/2, glutGet( GLUT_WINDOW_HEIGHT )/2 );

		// Set rotation
		
		if(flymode == 1)
		{
			if(((rot_y + (mouse_last_y - mouse_y)) < 450) && 
				((rot_y + (mouse_last_y - mouse_y)) > 280)) rot_y += (mouse_last_y - mouse_y);
		}
		else
		{
			if(((rot_y + (mouse_last_y - mouse_y)) < 420) && 
				((rot_y + (mouse_last_y - mouse_y)) > 300)) rot_y += (mouse_last_y - mouse_y);
		}
		rot_x += (mouse_last_x - mouse_x);

		glutPostRedisplay();
	}
}

// Create menus
static void create_menu()
{
	int menuarea,drawmenu,movemenu,animmenu,lightmenu;
	
	// Drawing menu
	drawmenu= glutCreateMenu(menu_selection);
	glutAddMenuEntry("Enable / Disable wireframe",0);
	glutAddMenuEntry("Enable / Disable floor axes",1);
	glutAddMenuEntry("Enable / Disable fog",3);
	
	// Movement menu
	movemenu = glutCreateMenu(menu_selection);
	glutAddMenuEntry("Enable / Disable flymode",2);
	glutAddMenuEntry("Enable / Disable classic controls",4);
	glutAddMenuEntry("Enable / Disable keyboard togglekeys",6);
	glutAddMenuEntry("Print current keyboard setting to console",9);
	glutAddMenuEntry("Toggle mouse capture (reset)",10);
	
	// Animation menu
	animmenu = glutCreateMenu(menu_selection);
	glutAddMenuEntry("Reset flying pyramid",5);
	
	lightmenu = glutCreateMenu(menu_selection);
	glutAddMenuEntry("Enable / Disable blinking light",7);
	glutAddMenuEntry("Enable / Disable ambient light",8);
	// Main
	menuarea = glutCreateMenu(menu_selection);
	glutAddSubMenu("Drawing",drawmenu);
	glutAddSubMenu("Movement",movemenu);
	glutAddSubMenu("Lights",lightmenu);
	glutAddSubMenu("Animation",animmenu);
	glutAddMenuEntry("Quit",11);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

// Check menu selections
static void menu_selection(int s)
{
	switch(s)
	{
		case 0:
			toggle_fill();
			break;
		case 1:
			toggle_axes();
			break;
		case 2:
			toggle_flymode();
			break;
		case 3:
			toggle_fog();
			break;
		case 4:
			toggle_controls();
			break;
		case 5:
			reset_pyramid();
			break;
		case 6:
			toggle_keys();
			break;
		case 7:
			toggle_light(GL_LIGHT1);
			break;
		case 8:
			toggle_light(GL_LIGHT2);
			break;
		case 9:
			print_keyb_style();
			break;
		case 10:
			toggle_mousereset();
			break;
		case 11:
			exit(0);
			break;
	}
	glutPostRedisplay();
}


// Called when entering / exiting MAIN manu, submenus 
// do not call this. Required for keeping the mouse
// position before menu is entered
static void menu_state(int state, int x, int y)
{
	if(state == GLUT_MENU_IN_USE)
	{
		// Get the position when entering menu
		menu_mx = x;
		menu_my = y;
		// Disable mouse movement detection
		glutPassiveMotionFunc(NULL);
	}
	else if (state == GLUT_MENU_NOT_IN_USE)
	{
		// Enable mouse movement detection
		glutPassiveMotionFunc(mouse_movement);
		
		// Reset position
		if(doreset == 1) glutWarpPointer( menu_mx, menu_my );
	}
}

static void make_pillar()
{
	glPushMatrix();
	glScalef(gen_scaling_factor,pillar_height_scale,gen_scaling_factor);
	draw_pillar(fillpoly);
	glPopMatrix();
}

static void make_pyramid()
{
	glPushMatrix();
	glScalef(gen_scaling_factor,gen_scaling_factor,gen_scaling_factor);
	draw_pyramid(fillpoly);
	glPopMatrix();
}

static void make_spruce()
{
	glPushMatrix();
	glScalef(gen_scaling_factor,gen_scaling_factor,gen_scaling_factor);
	draw_spruce(fillpoly);
	glPopMatrix();
}

static void make_pot()
{
	glPushMatrix();
	glScalef(pot_scaling_factor,pot_scaling_factor,pot_scaling_factor);
	draw_pot(fillpoly);
	glPopMatrix();
}

static void make_ground()
{
	glPushMatrix();
	glScalef(1.0,1.0,1.0);
	draw_ground(fillpoly);
	glPopMatrix();
}

// Pyramid animation
static void move_pyramid()
{	
	switch(movement)
	{
		case 0:
			pyramid_x += 0.1*speed;
			if(pyramid_x > 240.0) movement = 1;
			break;
	
		case 1:
			pyramid_z += 0.1*speed;
			if(pyramid_z > 20.0) movement = 2;
			break;
		case 2:
			pyramid_y -= 0.2*speed;
			if(pyramid_y < 2.0) movement = 3;
			break;
		case 3:
			temp += 0.01*speed;
			if(temp >= 10.0)
			{
				movement = 4;
				temp = 0.0;
			}
			break;
		case 4:
			pyramid_y += 0.1*speed;
			if(pyramid_y > 4.0) movement = 5;
		case 5:
			pyramid_z += 0.2*speed;
			if(pyramid_z >= 300.0) movement = 6;
			break;
		case 6:
			pyramid_y += 0.1*speed;
			if(pyramid_y < 50.0) pyramid_z -= 0.01;
			
			else if(pyramid_y < 80.0) pyramid_z += 0.01;
			
			if(pyramid_y >= 80.0) movement = 7;
			break;
		case 7:
			pyramid_x += 0.3*speed;
			pyramid_z += 0.3*speed;
			if(pyramid_x > 800 || pyramid_z > 800) movement = 8;
			
			break;
		case 8:
			pyramid_y += 0.8*speed;
			if(pyramid_y > 200) movement = 9;
			break;
		case 9:
			if(pyramid_x > 20.0) pyramid_x -= 0.2*speed;
			if(pyramid_z > 10.0) pyramid_z -= 0.2*speed;
			
			if(pyramid_x <= 20.0 && pyramid_z <= 10.0)
			{
				pyramid_x = 20.0;
				pyramid_z = 10.0;
				movement = 10;
			}
			break;
		case 10:
			if(pyramid_y > 20.0) pyramid_y -= 0.2*speed;
			else
			{
				pyramid_y = 20.0;
				movement = 0;
			}
			break;
		default:
			break;
	}
	// Set a new timer
	glutTimerFunc(10,move_pyramid,0);
	if(pyramid_r >= 360.0) pyramid_r = 1.0;
	else pyramid_r += 1.0*speed; // add rotation
	glutPostRedisplay();
}


// Drop "player" (camera position) to the ground
static void fall_to_ground(int up)
{
	// Initialize timer
	if(up == INIT_TIMER) glutTimerFunc(10,fall_to_ground,0);
	
	// Direction down to ground
	else if(up == 0)
	{
		if(pos_y > 1.0)
		{
			pos_y -= 0.2;
			glutTimerFunc(10,fall_to_ground,0);
		}
		// Dropped on your knees, get up
		else glutTimerFunc(20,fall_to_ground,1);
	}
	// Get back on your feet
	else
	{
		if(pos_y < 1.8)
		{
			pos_y += 0.05;
			glutTimerFunc(20,fall_to_ground,1);
		}
		// Stand straight
		else pos_y = 1.8;
	}
	
}

// Jump, similar to falling
static void jump(int up)
{
	// Init
	if(up == INIT_TIMER) glutTimerFunc(10,jump,1);
	
	// Jump
	else if(up == 1)
	{
		pos_y += 0.1;
		// Reached your limit
		if(pos_y < 5.0) glutTimerFunc(10,jump,1);
		// Drop
		else glutTimerFunc(10,jump,0);
	}
	
	// Drop
	else
	{
		pos_y -= 0.2;
		if(pos_y > 1.8) glutTimerFunc(10,jump,0);
		// Adjust your balance
		else glutTimerFunc(10,crouch,INIT_TIMER);
	}
}

// Crouch similar to jump and falling
static void crouch(int down)
{
	// Init
	if(down == INIT_TIMER) glutTimerFunc(10,crouch,1);
	
	// Down
	else if(down == 1)
	{
		pos_y -= 0.1;
		if(pos_y > 1.0) glutTimerFunc(10,crouch,1);
		else glutTimerFunc(10,crouch,0);
	}
	// Stand straight
	else
	{
		pos_y += 0.1;
		if(pos_y < 1.8) glutTimerFunc(10,crouch,0);
		else pos_y == 1.8;
	}
}

// Toggles between filling / wireframe
static void toggle_fill()
{
	if(fillpoly == 1) fillpoly = 0;
	else fillpoly = 1;
	init();
}

// Toggles axes on the ground
static void toggle_axes()
{
	if(make_axes == 1) make_axes = 0;
	else make_axes = 1;
}

// Toggles flying
static void toggle_flymode()
{
	if(flymode == 1)
	{
		flymode = 0;
		fall_to_ground(INIT_TIMER);
	}
	else flymode = 1;
}

// Toggles fog
static void toggle_fog()
{
	if(foggy == 1) foggy = 0;
	else foggy = 1;
	init();
}
// Toggles between control configurations
static void toggle_controls()
{
	if(controlmode == 1) controlmode = 0;
	else controlmode = 1;
}

// Toggles modificator keys on keyboard
static void toggle_keys()
{
	if(keyb_toggles == 1) keyb_toggles = 0;
	else keyb_toggles = 1;
}

// Toggle lights 1- blue blink, 2- ambient
static void toggle_light(int light)
{
	switch(light)
	{
		case GL_LIGHT1:
			if(light1 == 1) light1 = 0;
			else light1 = 1;
			break;
		case GL_LIGHT2:
			if(light2 == 1) light2 = 0;
			else light2 = 1;
			break;
	}
}

static void toggle_mousereset()
{
	if(doreset == 1) doreset = 0;
	else doreset = 1;
}
// Resets pyramid animation
static void reset_pyramid()
{
	movement = 0;
	pyramid_x = 20.0;
	pyramid_y = 20.0;
	pyramid_z = 10.0;
	glutTimerFunc(10,move_pyramid,0);
}

// Create random coordinates for objects
static void create_coords(int x[], int z[], int amount)
{
	int i = 0;
	int r;
	
	for(i = 0; i < amount; i++)
	{	
		r = rand() % 900 + 10;
		while((r > 50) && (r < 140)) r = rand() % 900 + 10;
		x[i] = r;
	}
	for(i = 0; i < amount; i++)
	{
		r = rand() % 900 + 10;
		while((r > 100) && (r < 170)) r = rand() % 900 + 10;
		z[i] = r;
	}
}

// Daylight variation
static void lightness(int var)
{
	if(var == INIT_TIMER) glutTimerFunc(10,lightness,1);
	
	// from dawn till noon
	else if(var == 1)
	{
		if(light < 1.0)
		{
			light += 0.001;
			glutTimerFunc(10,lightness,1);
		}
		else glutTimerFunc(10000,lightness,0); // day
	}
	// noon to dusk and into the night
	else
	{
		if(light > -0.5)
		{
			light -= 0.001;
			glutTimerFunc(10,lightness,0);
		}
		else glutTimerFunc(10,lightness,1); 
	}
}

// Blinking for the spotlight
static void spot_lightness(int var)
{
	if(var == INIT_TIMER) glutTimerFunc(10,spot_lightness,1);
	
	else if(var == 1)
	{
		if(spot < 1.0)
		{
			spot += 0.1;
			glutTimerFunc(10,spot_lightness,1);
		}
		else glutTimerFunc(10,spot_lightness,0);
	}
	else
	{
		if(spot > -0.5)
		{
			spot -= 0.1;
			glutTimerFunc(10,spot_lightness,0);
		}
		else glutTimerFunc(10,spot_lightness,1);
	}
}

static void print_keyb_style()
{
	if(controlmode == 0)
	{
		printf("Arrowkey controls:\n");
		printf("Move forward                Arrow up\n");
		printf("Move backward               Arrow down\n");
		printf("Turn left                   Arrow left\n");
		printf("Turn right                  Arrow right\n");
		printf("Strafe left                 Comma (,)\n");
		printf("Strafe right                Dot (.)\n");
		printf("Look up                     Page up\n");
		printf("Look down                   Page down\n");
	}
	else
	{
		printf("WSAD controls:\n");
		printf("Move forward                w\n");
		printf("Move backward               s\n");
		printf("Strafe left                 a\n");
		printf("Strafe right                d\n");
	}
	
	if(flymode == 1)
	{
		printf("Add height                  space\n");
		printf("Decrease height             z\n");
	}
	else
	{
		printf("Jump                        space\n");
		printf("Crouch                      z\n");
	}
	
	printf("Quit                        q\n");
	
	printf("\nModify environment:\n");
	if(keyb_toggles == 1)
	{
		printf("Toggle fog                  v\n");
		printf("Toggle wireframe and fill   f\n");
		printf("Toggle blinking light       1\n");
		printf("Toggle ambient light        2\n");
		printf("Toggle walk / flymode       x\n");
		printf("Change control style        c\n");
		printf("Draw ground axes            t\n");
		printf("Reset pyramid animation     r\n");
	}
	printf("Speed up animation          +\n");
	printf("Slow down animation         -\n");
	printf("\n\n");
}




