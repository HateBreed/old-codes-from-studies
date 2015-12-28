#ifndef __PROJECT_H
#define __PROJECT_H

#include "drawobj.h"

static void init();
static void display(void);
static void reshape(int,int);
static void keyboard(unsigned char, int, int);
static void special(int,int,int);
static void mouse_movement(int,int);
static void create_menu();
static void menu_selection();
static void menu_state(int,int,int);

static void make_pillar();
static void make_pyramid();
static void make_spruce();
static void make_pot();
static void move_pyramid();
static void make_ground();

static void fall_to_ground(int);
static void jump(int);
static void crouch(int);

static void reset_mouse();

static void toggle_fill();
static void toggle_axes();
static void toggle_flymode();
static void toggle_fog();
static void toggle_controls();
static void toggle_keys();
static void toggle_light(int);
static void toggle_mousereset();

static void reset_pyramid();

static void create_coords(int[],int[],int);

static void lightness(int);

static void spot_lightness(int);

static void print_keyb_style();
static void print_to_screen(char *);
#endif

