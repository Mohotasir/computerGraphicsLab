#include <GL/glut.h>
#include <stdio.h>


int x1, y1, x2, y2;

void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);   // White background
    glColor3f(0.2, 0.4, 0.9);           // Blue line color
    glPointSize(1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

void draw_pixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham with decreasing thickness
void draw_tapered_line(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int incx = (x2 > x1) ? 1 : -1;
    int incy = (y2 > y1) ? 1 : -1;

    int x = x1;
    int y = y1;

    int steps = (dx > dy) ? dx : dy;

    int max_thickness = 20;
    int e;

    if (dx > dy)
    {
        e = 2 * dy - dx;

        for (int i = 0; i <= dx; i++)
        {
            // thickness decreases gradually
            int thickness = max_thickness - (i * max_thickness / steps);
            if (thickness < 1) thickness = 1;

            // Draw vertical thickness
            for (int t = -thickness/2; t <= thickness/2; t++)
            {
                draw_pixel(x, y + t);
            }

            if (e >= 0)
            {
                y += incy;
                e -= 2 * dx;
            }

            e += 2 * dy;
            x += incx;
        }
    }
    else
    {
        e = 2 * dx - dy;

        for (int i = 0; i <= dy; i++)
        {
            int thickness = max_thickness - (i * max_thickness / steps);
            if (thickness < 1) thickness = 1;

            // Draw horizontal thickness
            for (int t = -thickness/2; t <= thickness/2; t++)
            {
                draw_pixel(x + t, y);
            }

            if (e >= 0)
            {
                x += incx;
                e -= 2 * dy;
            }

            e += 2 * dx;
            y += incy;
        }
    }
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    draw_tapered_line(x1, y1, x2, y2);

    glFlush();
}

int main(int argc, char **argv)
{
    printf("Enter (x1 y1 x2 y2): ");
    scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Thick to Thin Line");

    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();

    return 0;
}
