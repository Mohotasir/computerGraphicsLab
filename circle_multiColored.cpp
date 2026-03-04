#include <iostream>
#include <GL/glut.h>
using namespace std;

int pntx1, pnty1, r;

void plot(int x, int y, int pntX1, int pntY1, int octant)
{
    // Set different colors for each octant
    switch(octant) {
        case 0: glColor3f(1.0, 0.0, 0.0); break; // Red
        case 1: glColor3f(0.0, 1.0, 0.0); break; // Green
        case 2: glColor3f(0.0, 0.0, 1.0); break; // Blue
        case 3: glColor3f(1.0, 1.0, 0.0); break; // Yellow
        case 4: glColor3f(1.0, 0.0, 1.0); break; // Magenta
        case 5: glColor3f(0.0, 1.0, 1.0); break; // Cyan
        case 6: glColor3f(0.5, 0.0, 0.5); break; // Purple
        case 7: glColor3f(1.0, 0.5, 0.0); break; // Orange
    }
    
    glBegin(GL_POINTS);
    glVertex2i(x + pntX1, y + pntY1);
    glEnd();
}

void myInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void midPointCircleAlgo(int pntX1, int pntY1, int r)
{
    int x = 0;
    int y = r;
    float decision = 5.0 / 4.0 - r;
    
    plot(x, y, pntX1, pntY1, 0);
    
    while (y > x)
    {
        if (decision < 0)
        {
            x++;
            decision += 2 * x + 3;
        }
        else
        {
            y--;
            x++;
            decision += 2 * (x - y) + 5;
        }
        
        // Draw with different colors for each octant
        plot(x, y, pntX1, pntY1, 0);
        plot(x, -y, pntX1, pntY1, 1);
        plot(-x, y, pntX1, pntY1, 2);
        plot(-x, -y, pntX1, pntY1, 3);
        plot(y, x, pntX1, pntY1, 4);
        plot(-y, x, pntX1, pntY1, 5);
        plot(y, -x, pntX1, pntY1, 6);
        plot(-y, -x, pntX1, pntY1, 7);
    }
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glPointSize(5.0);
    midPointCircleAlgo(pntx1, pnty1, r);
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Colored Circle - Midpoint Algorithm");
    
    printf("Enter the center of the Circle (x y): ");
    scanf("%d %d", &pntx1, &pnty1);
    printf("Enter the radius of the Circle: ");
    scanf("%d", &r);
    
    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();
    
    return 0;
}
