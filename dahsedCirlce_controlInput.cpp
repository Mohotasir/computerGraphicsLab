#include <iostream>
#include <GL/glut.h>
using namespace std;

int pntx1, pnty1, r;
int dash_length = 5;
int gap_length = 3;

void plot(int x, int y, int pntX1, int pntY1, bool should_draw)
{
    if (should_draw) {
        glBegin(GL_POINTS);
        glVertex2i(x + pntX1, y + pntY1);
        glEnd();
    }
}

void myInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
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
    int pixel_count = 0;
    bool should_draw = true;
    
    plot(x, y, pntX1, pntY1, should_draw);
    pixel_count++;
    
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
        
        // Toggle dash pattern
        if (should_draw) {
            if (pixel_count >= dash_length) {
                should_draw = false;
                pixel_count = 0;
            }
        } else {
            if (pixel_count >= gap_length) {
                should_draw = true;
                pixel_count = 0;
            }
        }
        
        // Draw all 8 octants with dash pattern
        plot(x, y, pntX1, pntY1, should_draw);
        plot(x, -y, pntX1, pntY1, should_draw);
        plot(-x, y, pntX1, pntY1, should_draw);
        plot(-x, -y, pntX1, pntY1, should_draw);
        plot(y, x, pntX1, pntY1, should_draw);
        plot(-y, x, pntX1, pntY1, should_draw);
        plot(y, -x, pntX1, pntY1, should_draw);
        plot(-y, -x, pntX1, pntY1, should_draw);
        
        pixel_count++;
    }
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
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
    glutCreateWindow("Dashed Circle - Midpoint Algorithm");
    
    printf("Enter the center of the Circle (x y): ");
    scanf("%d %d", &pntx1, &pnty1);
    printf("Enter the radius of the Circle: ");
    scanf("%d", &r);
    printf("Enter dash length (default 5): ");
    scanf("%d", &dash_length);
    printf("Enter gap length (default 3): ");
    scanf("%d", &gap_length);
    
    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();
    
    return 0;
}
