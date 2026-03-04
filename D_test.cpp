#include <iostream>
#include <GL/glut.h>
#include <cmath>
using namespace std;

int centerX = 320, centerY = 240;
int radius = 150;

/* ---------- Plot Function ---------- */
void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

/* ---------- Midpoint Circle Algorithm ---------- */
void midPointCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int p = 1 - r;   // Correct decision parameter

    while (x <= y)
    {
        // 8-way symmetry
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);
        plot(xc + x, yc - y);
        plot(xc - x, yc - y);
        plot(xc + y, yc + x);
        plot(xc - y, yc + x);
        plot(xc + y, yc - x);
        plot(xc - y, yc - x);

        x++;

        if (p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

/* ---------- DDA Line Algorithm ---------- */
void drawLine(int x1, int y1, int x2, int y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = max(abs(dx), abs(dy));

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++)
    {
        plot(round(x), round(y));
        x += xInc;
        y += yInc;
    }
}

/* ---------- Draw Clock ---------- */
void drawClock()
{
    // Outer Circle
    midPointCircle(centerX, centerY, radius);

    // 4 Main Tick Marks
    drawLine(centerX, centerY + radius, centerX, centerY + radius - 20); // 12
    drawLine(centerX + radius, centerY, centerX + radius - 20, centerY); // 3
    drawLine(centerX, centerY - radius, centerX, centerY - radius + 20); // 6
    drawLine(centerX - radius, centerY, centerX - radius + 20, centerY); // 9

    // Clock Hands (Static Position like image)
    // Hour hand
    drawLine(centerX, centerY, centerX + 40, centerY + 70);

    // Minute hand
    drawLine(centerX, centerY, centerX + 80, centerY - 60);

    // Second hand
    drawLine(centerX, centerY, centerX + 60, centerY + 20);
}

/* ---------- Display ---------- */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glPointSize(2);

    drawClock();

    glFlush();
}

/* ---------- Init ---------- */
void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
}

/* ---------- Main ---------- */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Analog Clock using Midpoint Circle");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
