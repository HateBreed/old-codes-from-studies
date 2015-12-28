#include "drawobj.h"

void draw_axes()
{
	int coord;
	glBegin(GL_LINES);
	
	glColor3f(1,0,0);
	for(coord = 0; coord < 1000; coord++)
	{
		glVertex3f(0,0,coord);
		glVertex3f(1000,0,coord);
	}
	
	glColor3f(0,1,0);
	glVertex3f(0,-20,0);
	glVertex3f(0,20,0);
	
	glColor3f(0,0,1);
	for(coord = 0; coord < 1000; coord++)
	{
		glVertex3f(coord,0,0);
		glVertex3f(coord,0,1000);
	}
	glEnd();
}

void draw_ground(int fill)
{
	int i;
	
	if(fill == 1) glBegin(GL_POLYGON);
	else glBegin(GL_LINE_LOOP);
	
	glColor3f(0.5,0.4,0.3);
	
	for(i = 0; i < 4; i++)
	{
		glVertex3fv(ground_coords[i]);
	}
	
	glEnd();
}

void draw_pillar(int fill)
{
	int i,j;
	
	for(i = 0; i < 6; i++)
	{
		if(fill == 1) glBegin(GL_POLYGON);
		else glBegin(GL_LINE_LOOP);
		
		for(j = 0; j < 4; j++)
		{
			glVertex3fv(skyscraper_coords[skyscraper_sides[i][j]]);
		}
		glEnd();
	}
}

void draw_pyramid(int fill)
{
	int i,j;
	
	for(i = 0; i < 6; i++)
	{
		if(fill == 1) glBegin(GL_POLYGON);
		else glBegin(GL_LINE_LOOP);
		
		glColor3f(0.0,(0.0 + (0.1*i)),(0.2 + (0.1*i))); // Color variation
		for(j = 0; j < 3; j++)
		{
			glVertex3fv(pyramid_coords[pyramid_sides[i][j]]);
		}
		glEnd();
	}
	glColor3f(1.0,1.0,0.0);
}

void draw_spruce(int fill)
{
	int i,j;
	
	for(i = 0; i < 17; i++)
	{
		if(fill == 1) glBegin(GL_POLYGON);
		else glBegin(GL_LINE_LOOP);
		for(j = 0; j < 4; j++)
		{
			glVertex3fv(spruce_coords[spruce_sides[i][j]]);
		}
		glEnd();
	}
}

void draw_pot(int fill)
{
	int i,j;
	
	for(i = 0; i < 14; i++)
	{
		if(fill == 1) glBegin(GL_POLYGON);
		else glBegin(GL_LINE_LOOP);
		for(j = 0; j < 4; j++)
		{
			if(i < 9) glColor3f(1.0,0.5,1.0-i*0.1);
			else glColor3f(0.2,0.2,1.0-((i-8)*0.1));
			glVertex3fv(pot_coords[pot_sides[i][j]]);
		}
		glEnd();
	}
}
