#include <GL/glut.h>

// current state
int option = 0;

// previous state (for undo)
int prev_option = 0;

// fixed pivot point
float px = 225, py = 175;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move to pivot
    glTranslatef(px, py, 0);

    // apply transformation
    if(option == 2)
        glRotatef(15, 0, 0, 1);
    else if(option == 3)
        glScalef(1.5, 1.5, 1);
    else if(option == 4)
        glTranslatef(100, 100, 0);

    // move back
    glTranslatef(-px, -py, 0);

    // draw polygon
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex2f(150, 150);
    glVertex2f(300, 150);
    glVertex2f(225, 225);
    glEnd();

    // draw fixed pivot point
    glPointSize(6);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex2f(px, py);
    glEnd();

    glFlush();
}

// menu function
void OptionsMenu(int selectedOpt)
{
    if(selectedOpt == 5) // Undo
    {
        option = prev_option;
    }
    else
    {
        prev_option = option; // save previous
        option = selectedOpt;
    }

    glutPostRedisplay(); // instant update
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Undo Menu Transform");

    glutDisplayFunc(display);

    glutCreateMenu(OptionsMenu);
    glutAddMenuEntry("Original", 1);
    glutAddMenuEntry("Rotate", 2);
    glutAddMenuEntry("Scale", 3);
    glutAddMenuEntry("Translate", 4);
    glutAddMenuEntry("Undo", 5);   // 🔥 undo option
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();
    glutMainLoop();
}
