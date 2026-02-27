#include <GL/glut.h>
#include <stdio.h>

int x1, y1, x2, y2;

void myInit()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
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
    int pixel_count = 0;  // Counter to alternate draw/skip
    
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
    
    if (dx > dy) {
        // Draw pixel if even count (0, 2, 4, ...), skip if odd (1, 3, 5, ...)
        if (pixel_count % 2 == 0) {
            draw_pixel(x, y);
        }
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
            
            // Draw pixel if even count, skip if odd
            if (pixel_count % 2 == 0) {
                draw_pixel(x, y);
            }
            pixel_count++;
        }
    }
    else
    {
        // Draw pixel if even count, skip if odd
        if (pixel_count % 2 == 0) {
            draw_pixel(x, y);
        }
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
            
            // Draw pixel if even count, skip if odd
            if (pixel_count % 2 == 0) {
                draw_pixel(x, y);
            }
            pixel_count++;
        }
    }
}

void myDisplay()
{
    draw_dashed_line(x1, x2, y1, y2);
    glFlush();
}

int main(int argc, char **argv)
{
    printf("Enter (x1, y1, x2, y2)\n");
    scanf_s("%d %d %d %d", &x1, &y1, &x2, &y2);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Bresenham's Dashed Line Drawing");
    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();
    
    return 0;
}
