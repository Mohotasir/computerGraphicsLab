#include <iostream>
#include <GL/glut.h>
using namespace std;

int pntx1, pnty1, num_circles;
int radii[10];  // Maximum 10 circles

void plot(int x, int y, int pntX1, int pntY1)
{
    glBegin(GL_POINTS);
    glVertex2i(x + pntX1, y + pntY1);
    glEnd();
}

void myInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void midPointCircleAlgo(int pntX1, int pntY1, int r)
{
    int x = 0;
    int y = r;
    float decision = 5.0 / 4.0 - r;

    plot(x, y, pntX1, pntY1);

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

        plot(x, y, pntX1, pntY1);
        plot(x, -y, pntX1, pntY1);
        plot(-x, y, pntX1, pntY1);
        plot(-x, -y, pntX1, pntY1);
        plot(y, x, pntX1, pntY1);
        plot(-y, x, pntX1, pntY1);
        plot(y, -x, pntX1, pntY1);
        plot(-y, -x, pntX1, pntY1);
    }
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(3.0);

    // Draw all concentric circles
    for (int i = 0; i < num_circles; i++)
    {
        midPointCircleAlgo(pntx1, pnty1, radii[i]);
    }

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Concentric Circles - Midpoint Algorithm");

    printf("Enter the center of circles (x y): ");
    scanf("%d %d", &pntx1, &pnty1);

    printf("How many concentric circles (max 10)? ");
    scanf("%d", &num_circles);

    if (num_circles > 10) num_circles = 10;

    printf("Enter %d radii (e.g., 30 60 90): \n", num_circles);
    for (int i = 0; i < num_circles; i++)
    {
        scanf("%d", &radii[i]);
    }

    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();

    return 0;
}
