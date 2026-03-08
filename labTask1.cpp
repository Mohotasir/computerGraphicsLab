#include <iostream>
#include <GL/glut.h>
#include <cmath>
using namespace std;


int pivotX = 320;
int pivotY = 380;
int length = 200;
int bobRadius = 15;

float angle = 0.0;
float speed = 0.05;


void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void midPointCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int p = 1 - r;

    while (x <= y)
    {
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);
        plot(xc + x, yc - y);
        plot(xc - x, yc - y);
        plot(xc + y, yc + x);
        plot(xc - y, yc + x);
        plot(xc + y, yc - x);
        plot(xc - y, yc - x);

        if (p < 0)
            p = p + 2 * x + 3;
        else
        {
            p = p + 2 * x - 2 * y + 5;
            y--;
        }
        x++;
    }
}

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x, y;

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    incx = (x2 < x1) ? -1 : 1;
    incy = (y2 < y1) ? -1 : 1;

    x = x1;
    y = y1;

    if (dx > dy)
    {
        plot(x, y);
        e = 2 * dy - dx;
        inc1 = 2 * (dy - dx);
        inc2 = 2 * dy;

        for (i = 0; i < dx; i++)
        {
            if (e >= 0)
            {
                y += incy;
                e += inc1;
            }
            else
                e += inc2;

            x += incx;
            plot(x, y);
        }
    }
    else
    {
        plot(x, y);
        e = 2 * dx - dy;
        inc1 = 2 * (dx - dy);
        inc2 = 2 * dx;

        for (i = 0; i < dy; i++)
        {
            if (e >= 0)
            {
                x += incx;
                e += inc1;
            }
            else
                e += inc2;

            y += incy;
            plot(x, y);
        }
    }
}


void drawPendulum()
{
    
    drawLine(pivotX - 120, pivotY, pivotX + 120, pivotY);

    float maxAngle = 30 * 3.14159 / 180; 
    float theta = maxAngle * sin(angle);

    int bobX = pivotX + length * sin(theta);
    int bobY = pivotY - length * cos(theta);

    
    drawLine(pivotX, pivotY, bobX, bobY);

    
    midPointCircle(bobX, bobY, bobRadius);
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glPointSize(2);

    drawPendulum();

    glFlush();
}


void update(int value)
{
    angle += speed;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Pendulum Animation");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
