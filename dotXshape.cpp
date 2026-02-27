#include <GL/glut.h>
#include <stdio.h>

int x1, y1, x2, y2;

void myInit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
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

void draw_dashed_line(int x1, int x2, int y1, int y2)
{
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x, y;
    int pixel_count = 0;

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
        if (pixel_count % 2 == 0)
            draw_pixel(x, y);

        pixel_count++;

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

            if (pixel_count % 2 == 0)
                draw_pixel(x, y);

            pixel_count++;
        }
    }
    else
    {
        if (pixel_count % 2 == 0)
            draw_pixel(x, y);

        pixel_count++;

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

            if (pixel_count % 2 == 0)
                draw_pixel(x, y);

            pixel_count++;
        }
    }
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // First diagonal
    draw_dashed_line(x1, x2, y1, y2);

    // Second diagonal (reverse Y)
    draw_dashed_line(x1, x2, y2, y1);

    glFlush();
}

int main(int argc, char **argv)
{
    printf("Enter (x1, y1, x2, y2)\n");
    scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham X Shape");

    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();

    return 0;
}
