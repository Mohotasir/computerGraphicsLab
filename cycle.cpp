#include <iostream>
#include <GL/glut.h>
#include <cmath>
using namespace std;

void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

/* -------- Midpoint Circle Algorithm -------- */
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
        {
            p = p + 2 * x + 3;
        }
        else
        {
            p = p + 2 * x - 2 * y + 5;
            y--;
        }

        x++;
    }
}

/* -------- Bresenham Line Algorithm -------- */
void drawLine(int x1, int y1, int x2, int y2)
{
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x, y;

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    incx = 1;
    if (x2 < x1) incx = -1;

    incy = 1;
    if (y2 < y1) incy = -1;

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
            {
                e += inc2;
            }

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
            {
                e += inc2;
            }

            y += incy;
            plot(x, y);
        }
    }
}

/* -------- Draw Bicycle -------- */
void drawBicycle()
{
    int r = 60;

    int x1 = 200, y1 = 200; // left wheel
    int x2 = 400, y2 = 200; // right wheel

    // Wheels
    midPointCircle(x1, y1, r);
    midPointCircle(x2, y2, r);

    // Frame
    drawLine(x1, y1, 300, 280);
    drawLine(300, 280, x2, y2);
    drawLine(x1, y1, 340, 200);
    drawLine(340, 200, 300, 280);

    // Seat
    drawLine(300, 280, 300, 310);
    drawLine(280, 310, 320, 310);

    // Handle
    drawLine(x2, y2, 420, 280);
    drawLine(420, 280, 450, 300);
}

/* -------- Display -------- */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glPointSize(2);

    drawBicycle();

    glFlush();
}

/* -------- Initialization -------- */
void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
}

/* -------- Main -------- */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Bicycle using Line and Circle");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
