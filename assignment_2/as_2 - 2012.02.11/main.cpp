/**********************************************************************
    Spring 2012
    CS 4554 Computer Graphics I
    Assignment 2 : Pong Game
    Student Name : YiHung Lee
    Gw ID        : G42957795
    Implement :
        1: balls collision
        2: balls change color whenever ball collision
        3: at most two balls in a game
    Note :
        1: press keyboard 'd' to create second ball
        2: when lose a ball, third ball doesn't be created even if press 'd'
        3: game doesn't restart automatically, press 'r' to restart game
        4: press 's' to start game
        5: second ball runs faster than first ball, be careful
        6: it's better to use control paddle by mouse than keyboard
 **********************************************************************/

#include <GL/glut.h>
#include <time.h>
/* ascii codes for various special keys */
#define ESCAPE 27

/**********************************************************************
 * Configuration
 **********************************************************************/

#define INITIAL_X_POS    50
#define INITIAL_Y_POS    50
#define INITIAL_WIDTH    640
#define INITIAL_HEIGHT   480
#define MARGIN_SIZE      10
#define BORDER_SIZE      18
#define PADDLE_LENGTH    100
#define BALL_SIZE        15
#define BALL_STEP        7  /* how far ball travels per time step */
#define BALL_STEP_2      12
#define WINDOW_NAME     "Assignment 2 Pong Game"

#define PI				 3.1415926535 /* used for calculating angles */

#define INITIAL_BALL_X_POS 100
#define INITIAL_BALL_Y_POS 240
#define INITIAL_PADDLE_Y_POS 240
#define INITIAL_BALL_ANGLE 45


/**********************************************************************
* Globals
**********************************************************************/

GLsizei window_width;
GLsizei window_height;

GLfloat paddle_y_pos;
GLfloat ball_x_pos;
GLfloat ball_y_pos;
GLfloat ball_x1_pos;
GLfloat ball_y1_pos;
GLfloat ball_trajectory;
GLuint  ball_delay = 30;  /* time between ball position updates in msecs */
GLint BALL_NUMBER = 1;
int game_over = 1;
int current_game = 0;
int misses = 0;				/* to keep track of the misses */
int left_button_state = GLUT_UP;
int left_button_lasty;
int ball_state = 0;
int ball_pre_state = 0;
int ball_1_state = 1;
int ball_1_pre_state = 0;
/**********************************************************************
 * Set the new size of the window
 **********************************************************************/

void resize_scene(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);  /* reset the current viewport and`
                                       * perspective transformation */

    window_width  = width;
    window_height = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* for the purposes of this assignment, we are making the world
     * coordinate system have a 1-1 correspondence with screen space
     * (in pixels).  The origin maps to the lower-left corner of the screen.*/
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);

}

/**********************************************************************
 * The main drawing functions.
 **********************************************************************/

void draw_border(void)
{
    const GLfloat border = MARGIN_SIZE + BORDER_SIZE;
    // 10  + 18
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
    /* top bar */
    glVertex3f(MARGIN_SIZE,(window_height-MARGIN_SIZE),0);
    //10 ,
    glVertex3f(MARGIN_SIZE,(window_height-border),0);
    glVertex3f((window_width-MARGIN_SIZE),(window_height-border),0);
    glVertex3f((window_width-MARGIN_SIZE),(window_height-MARGIN_SIZE),0);
    /* side bar */
    glVertex3f(MARGIN_SIZE,(window_height-border),0);
    glVertex3f(MARGIN_SIZE,border,0);
    glVertex3f(border,border,0);
    glVertex3f(border,(window_height-border),0);
    /* bottom bar */
    glVertex3f(MARGIN_SIZE,border,0);
    glVertex3f(MARGIN_SIZE,MARGIN_SIZE,0);
    glVertex3f((window_width-MARGIN_SIZE),MARGIN_SIZE,0);
    glVertex3f((window_width-MARGIN_SIZE),border,0);
    glEnd();
}

void draw_paddle(void)
{
    const GLfloat xpos = window_width - (MARGIN_SIZE+3*BORDER_SIZE);
    const GLfloat yoff = PADDLE_LENGTH / 2;

    /* the paddle is always drawn at a fixed location within its own
     * coordinate space. its actual world-space position is determined
     * by the modelview matrix (as affected by any glTranslatef(), glRotatef(),
     * or other function calls) */

    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_QUADS);
    glVertex3f(xpos,-yoff+paddle_y_pos,0);
    glVertex3f(xpos,yoff+paddle_y_pos,0);
    glVertex3f(xpos+BORDER_SIZE,yoff+paddle_y_pos,0);
    glVertex3f(xpos+BORDER_SIZE,-yoff+paddle_y_pos,0);
    glEnd();
}
float r =0,g=0,b=1;

void draw_ball()
{
    /* the ball is always drawn at the origin of its own coordinate
     * system.  its real position in world-space is determined by the
     * modelview matrix, just as with the paddle */


    glColor3f(r, g,b);
    glBegin(GL_POINTS);
    glVertex3f(ball_x_pos,ball_y_pos,0);
    glEnd();
}
float r1 =0,g1=0,b1=1;
void draw_ball_2()
{
    glColor3f(r1, g1,b1);
    glBegin(GL_POINTS);
    glVertex3f(ball_x1_pos,ball_y1_pos,0);
    glEnd();
}
void draw_scene(void)
{
    /* clear the screen and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* reset modelview matrix */
    glLoadIdentity();

    /* draw the border */
    draw_border();

    /* draw the paddle */
    draw_paddle();

    draw_ball();

    if(BALL_NUMBER == 2)
        draw_ball_2();

    /* since this is double buffered, swap the
     * buffers to display what just got drawn */
    glutSwapBuffers();
}

void move_ball(int game)
{

    const GLfloat border = MARGIN_SIZE + BORDER_SIZE + 8; //10 + 18
    /* size of the margin + size of border */
    const GLfloat paddle_start =
        window_width - (MARGIN_SIZE + 3*BORDER_SIZE) - 11;
    /* position of paddle */
    /* this allows a game reset in the middle of a game, since
     * once a ball move timer function is registered on the old
     * game, it cannot be cancelled */

    float by = ball_y_pos;
    float py = paddle_y_pos;
    if (game != current_game)
        return;

    if(ball_y_pos > window_height - border)//top
    {
        if(ball_pre_state == 3)
        {
            ball_state = 2;
            r = (float)rand()/RAND_MAX;
        }
        else
        {
            ball_state = 1;
            g = (float)rand()/RAND_MAX;
        }
    }
    else if(ball_x_pos > window_width)//die
        ball_state = 4;
    else if(ball_x_pos > paddle_start  && ball_x_pos < paddle_start + BORDER_SIZE
            && (abs(by - py)<=50 ))//right paddle
    {
        if(ball_pre_state == 0)
        {
            b = (float)rand()/RAND_MAX;
            ball_state = 3;
        }
        else
        {
            r = (float)rand()/RAND_MAX;
            ball_state = 2;
        }
    }
    else if(ball_x_pos < border)//left
    {

        if(ball_pre_state == 2)
        {
            g = (float)rand()/RAND_MAX;
            ball_state = 1;
        }
        else
        {
            b = (float)rand()/RAND_MAX;
            ball_state = 0;
        }
    }
    else if(ball_y_pos < border)//bottom
    {
        r = (float)rand()/RAND_MAX;
        if(ball_pre_state == 1)
        {
            ball_state = 0;
        }
        else
        {
            ball_state = 3;
        }
    }

    ball_pre_state = ball_state;


    switch (ball_state)
    {
        case 0:
            ball_x_pos = ball_x_pos + BALL_STEP;
            ball_y_pos = ball_y_pos + BALL_STEP;
            break;
        case 1:
            ball_x_pos = ball_x_pos + BALL_STEP;
            ball_y_pos = ball_y_pos - BALL_STEP;
            break;
        case 2:
            ball_x_pos = ball_x_pos - BALL_STEP;
            ball_y_pos = ball_y_pos - BALL_STEP;
            break;
        case 3:
            ball_x_pos = ball_x_pos - BALL_STEP;
            ball_y_pos = ball_y_pos + BALL_STEP;
            break;
        case 4:
            ball_x_pos = ball_x_pos + BALL_STEP;
            break;
    }
    if (!game_over)
    {
        /* tell glut when next to update the ball's position */
        glutTimerFunc(ball_delay, move_ball, current_game);
    }
    glutPostRedisplay();
}
void move_ball_2(int game)
{

    const GLfloat border = MARGIN_SIZE + BORDER_SIZE +8; //10 + 18
    /* size of the margin + size of border */
    const GLfloat paddle_start =
        window_width - (MARGIN_SIZE + 3*BORDER_SIZE) - 11;
    /* position of paddle */
    /* this allows a game reset in the middle of a game, since
     * once a ball move timer function is registered on the old
     * game, it cannot be cancelled */

    float by = ball_y1_pos;
    float py = paddle_y_pos;
    if (game != current_game)
        return;

    if(ball_y1_pos > window_height - border)//top
    {
        if(ball_1_pre_state == 3)
        {
            ball_1_state = 2;
            r1 = (float)rand()/RAND_MAX;
        }
        else
        {
            ball_1_state = 1;
            g1 = (float)rand()/RAND_MAX;
        }
    }
    else if(ball_x1_pos > window_width)//die
        ball_1_state = 4;
    else if(ball_x1_pos > paddle_start  && ball_x1_pos < paddle_start + BORDER_SIZE
            && (abs(by - py)<=50 ))//right paddle
    {
        if(ball_1_pre_state == 0)
        {
            b1 = (float)rand()/RAND_MAX;
            ball_1_state = 3;
        }
        else
        {
            r1 = (float)rand()/RAND_MAX;
            ball_1_state = 2;
        }
    }
    else if(ball_x1_pos < border)//left
    {
        if(ball_1_pre_state == 2)
        {
            g1 = (float)rand()/RAND_MAX;
            ball_1_state = 1;
        }
        else
        {
            b1 = (float)rand()/RAND_MAX;
            ball_1_state = 0;
        }
    }
    else if(ball_y1_pos < border)//bottom
    {
        r1 = (float)rand()/RAND_MAX;
        if(ball_1_pre_state == 1)
        {
            ball_1_state = 0;
        }
        else
        {
            ball_1_state = 3;
        }
    }

    ball_1_pre_state = ball_1_state;
    switch (ball_1_state)
    {
        case 0:
            ball_x1_pos = ball_x1_pos + BALL_STEP_2;
            ball_y1_pos = ball_y1_pos + BALL_STEP_2;
            break;
        case 1:
            ball_x1_pos = ball_x1_pos + BALL_STEP_2;
            ball_y1_pos = ball_y1_pos - BALL_STEP_2;
            break;
        case 2:
            ball_x1_pos = ball_x1_pos - BALL_STEP_2;
            ball_y1_pos = ball_y1_pos - BALL_STEP_2;
            break;
        case 3:
            ball_x1_pos = ball_x1_pos - BALL_STEP_2;
            ball_y1_pos = ball_y1_pos + BALL_STEP_2;
            break;
        case 4:
            ball_x1_pos = ball_x1_pos + BALL_STEP_2;
            break;
    }

    if (!game_over)
    {
        /* tell glut when next to update the ball's position */
        glutTimerFunc(ball_delay, move_ball_2, current_game);
    }
    glutPostRedisplay();
}
/**********************************************************************
 * BEWARE: in the following functions, y=0 is at the TOP of the screen,
 * which is the exact opposite of our world which has y=0 at the
 * bottom of the screen.  It shouldn't matter much for the purposes of
 * this assignment, but you have been warned.
 **********************************************************************/

/**********************************************************************
 * the function called whenever a normal key is pressed.
 **********************************************************************/

void key_press(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's': /* start game */
        if (game_over)
        {
            game_over = 0;
            move_ball(current_game);
            if(BALL_NUMBER == 2)
                move_ball_2(current_game);

        }
        break;
    case 'r': /* reset game */
//        current_game++;
        misses			= 0;

        ball_x_pos      = INITIAL_BALL_X_POS  ;
        ball_y_pos      = INITIAL_BALL_Y_POS  ;
        ball_x1_pos      = INITIAL_BALL_X_POS  ;
        ball_y1_pos      = INITIAL_BALL_Y_POS  ;
        paddle_y_pos    = INITIAL_PADDLE_Y_POS;
        ball_trajectory = INITIAL_BALL_ANGLE  ;
        game_over       = 1;
        ball_state      = 0;
        ball_1_state    = 1;
        BALL_NUMBER     = 1;
        glutPostRedisplay();
        break;
    case 'd':
        if(BALL_NUMBER == 1)
        {

            BALL_NUMBER++;
            move_ball_2(current_game);
        }
        break;
    case ESCAPE: /* exit the program...normal termination. */
        exit(0);
    }
}

/**********************************************************************
 * this function is called whenever a mouse button is pressed and moved
 **********************************************************************/

void handle_mouse_motion(int x, int y)
{
    if (left_button_state == GLUT_DOWN)
    {

        /* FIXME: you must make sure that the paddle does not go off
         * the court. (the paddle may touch the walls, but should not
         * intersect them). */

        if(paddle_y_pos < 80)
            paddle_y_pos = 80;
        else if(paddle_y_pos > 400)
            paddle_y_pos = 400;
        else
        {
            paddle_y_pos += left_button_lasty - y;
            left_button_lasty = y;
        }
        glutPostRedisplay();

    }
}

/**********************************************************************
 * this function is called whenever a mouse button is pressed or released
 **********************************************************************/

void handle_mouse_click(int btn, int state, int x, int y)
{
    switch (btn)
    {
    case GLUT_LEFT_BUTTON:
        left_button_state = state;
        left_button_lasty = y;
        break;
    }
}

void special_key(int key, int x, int y)
{

    switch (key)
    {

    case GLUT_KEY_UP://up
        if(paddle_y_pos < 400)
        {
            paddle_y_pos = paddle_y_pos + 10;
            glutPostRedisplay();
        }
        break;
    case GLUT_KEY_DOWN://down
        if(paddle_y_pos > 80)
        {
            paddle_y_pos = paddle_y_pos - 10;
            glutPostRedisplay();
        }

        break;
    default:

        break;
    }


}


int main(int argc, char ** argv)
{
//    srand(int(time(0)));
//     set ball_trajectory, ball position, and paddle
//     position based on command line arguments
    paddle_y_pos    = INITIAL_PADDLE_Y_POS  ;//240
    ball_x_pos      = INITIAL_BALL_X_POS  ; //100
    ball_y_pos      = INITIAL_BALL_Y_POS;   //240
    ball_x1_pos      = INITIAL_BALL_X_POS  ; //100
    ball_y1_pos      = INITIAL_BALL_Y_POS;   //240
    ball_trajectory = INITIAL_BALL_ANGLE  ;//45

    /* Initialize GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(INITIAL_WIDTH, INITIAL_HEIGHT);//640 x 480
    glutInitWindowPosition(INITIAL_X_POS, INITIAL_Y_POS);
    glutCreateWindow(WINDOW_NAME);

    /* Register callback functions */
    glutDisplayFunc(draw_scene);
    glutReshapeFunc(resize_scene);       //Initialize the viewport when the window size changes.
    glutKeyboardFunc(key_press);         //handle when the key is pressed
    glutMouseFunc(handle_mouse_click);   //check the Mouse Button(Left, Right and Center) status(Up or Down)
    glutMotionFunc(handle_mouse_motion); //Check the Current mouse position when mouse moves
    glutSpecialFunc(special_key);        //Special Keyboard Key fuction(For Arrow button and F1 to F10 button)


    /* Initialize GL */
    glPointSize(BALL_SIZE); //BALL_SIZE = 15

    /* Enter event processing loop */
    glutMainLoop();

    return 0;
}

