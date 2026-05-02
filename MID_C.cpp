#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int x1_, y1_, x2_, y2_, x3_, y3_;
int cx, cy, r;

float scaleFactor = 1.0;
float angle = 0.0;
float tx = 0.0, ty = 0.0;

float fillCol[3] = {0.0, 1.0, 0.0};
float borderCol[3] = {0.0, 0.0, 1.0};

// ---------------- PIXEL ----------------
void setPixel(int x, int y, float color[3]) {
    glBegin(GL_POINTS);
    glColor3fv(color);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixel(int x, int y, float pixels[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
}

void draw_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// ---------------- BRESENHAM LINE ----------------
void drawLine(int x1, int y1, int x2, int y2) {
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x, y;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    incx = (x2 > x1) ? 1 : -1;
    incy = (y2 > y1) ? 1 : -1;

    x = x1;
    y = y1;

    if (dx > dy) {
        draw_pixel(x, y);
        e = 2 * dy - dx;
        inc1 = 2 * (dy - dx);
        inc2 = 2 * dy;

        for (i = 0; i < dx; i++) {
            if (e >= 0) {
                y += incy;
                e += inc1;
            } else {
                e += inc2;
            }
            x += incx;
            draw_pixel(x, y);
        }
    } else {
        draw_pixel(x, y);
        e = 2 * dx - dy;
        inc1 = 2 * (dx - dy);
        inc2 = 2 * dx;

        for (i = 0; i < dy; i++) {
            if (e >= 0) {
                x += incx;
                e += inc1;
            } else {
                e += inc2;
            }
            y += incy;
            draw_pixel(x, y);
        }
    }
}

// ---------------- TRIANGLE ----------------
void drawTriangle() {
    glColor3f(0, 0, 1);
    drawLine(x1_, y1_, x2_, y2_);
    drawLine(x2_, y2_, x3_, y3_);
    drawLine(x3_, y3_, x1_, y1_);
}

// ---------------- MIDPOINT CIRCLE ----------------
void plotCircle(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(cx + x, cy + y);
    glVertex2i(cx - x, cy + y);
    glVertex2i(cx + x, cy - y);
    glVertex2i(cx - x, cy - y);
    glVertex2i(cx + y, cy + x);
    glVertex2i(cx - y, cy + x);
    glVertex2i(cx + y, cy - x);
    glVertex2i(cx - y, cy - x);
    glEnd();
}

void drawCircle() {
    int x = 0;
    int y = r;
    int d = 1 - r;

    glColor3f(0, 0, 1);

    while (x <= y) {
        plotCircle(x, y);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// ---------------- INCIRCLE CALCULATION ----------------
void computeInCircle() {
    // Side lengths
    float a = sqrt((x2_ - x3_) * (x2_ - x3_) + (y2_ - y3_) * (y2_ - y3_));
    float b = sqrt((x1_ - x3_) * (x1_ - x3_) + (y1_ - y3_) * (y1_ - y3_));
    float c = sqrt((x1_ - x2_) * (x1_ - x2_) + (y1_ - y2_) * (y1_ - y2_));

    // Incenter formula
    cx = (a * x1_ + b * x2_ + c * x3_) / (a + b + c);
    cy = (a * y1_ + b * y2_ + c * y3_) / (a + b + c);

    // Area using determinant
    float area = fabs(
        x1_ * (y2_ - y3_) +
        x2_ * (y3_ - y1_) +
        x3_ * (y1_ - y2_)
    ) / 2.0;

    float s = (a + b + c) / 2.0;

    // Inradius
    r = area / s;
}

// ---------------- BOUNDARY FILL ----------------
void boundaryFill(int x, int y) {
    float color[3];
    getPixel(x, y, color);

    if ((color[0] != borderCol[0] || color[1] != borderCol[1] || color[2] != borderCol[2]) &&
        (color[0] != fillCol[0] || color[1] != fillCol[1] || color[2] != fillCol[2])) {

        setPixel(x, y, fillCol);

        boundaryFill(x + 1, y);
        boundaryFill(x - 1, y);
        boundaryFill(x, y + 1);
        boundaryFill(x, y - 1);
    }
}

// ---------------- DISPLAY ----------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    // Translation
    glTranslatef(tx, ty, 0);

    // Scaling
    glScalef(scaleFactor, scaleFactor, 1);

    // Rotation
    glRotatef(angle, 0, 0, 1);

    drawTriangle();
    drawCircle();

    glPopMatrix();

    glFlush();
}

// ---------------- MOUSE ----------------
void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int xi = x;
        int yi = 500 - y;

        int dx = xi - (cx + tx);
        int dy = yi - (cy + ty);

        if (dx * dx + dy * dy <= r * r) {
            printf("Valid Click -> Filling Circle\n");
            boundaryFill(xi, yi);
        } else {
            printf("Invalid Click! Outside Circle\n");
        }
    }
}

// ---------------- KEYBOARD TRANSLATION ----------------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': ty += 10; break;
        case 's': ty -= 10; break;
        case 'a': tx -= 10; break;
        case 'd': tx += 10; break;
    }

    glutPostRedisplay();
}

// ---------------- MENU ----------------
void menu(int option) {
    float input;

    switch (option) {
        case 1:
            printf("Enter Scale Up Factor: ");
            scanf("%f", &input);
            scaleFactor *= input;
            break;

        case 2:
            printf("Enter Scale Down Factor: ");
            scanf("%f", &input);
            if (input != 0)
                scaleFactor /= input;
            break;

        case 3:
            printf("Enter AntiClockwise Rotation Angle: ");
            scanf("%f", &input);
            angle += input;
            break;

        case 4:
            printf("Enter Clockwise Rotation Angle: ");
            scanf("%f", &input);
            angle -= input;
            break;
    }

    glutPostRedisplay();
}

// ---------------- INIT ----------------
void init() {
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);

    glMatrixMode(GL_MODELVIEW);
}

// ---------------- MAIN ----------------
int main(int argc, char** argv) {

    printf("Enter Triangle Points (x1 y1 x2 y2 x3 y3):\n");
    scanf("%d %d %d %d %d %d", &x1_, &y1_, &x2_, &y2_, &x3_, &y3_);

    computeInCircle();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Triangle + Incircle + Boundary Fill + Transformations");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutCreateMenu(menu);
    glutAddMenuEntry("Scale Up", 1);
    glutAddMenuEntry("Scale Down", 2);
    glutAddMenuEntry("Rotate AntiClockwise", 3);
    glutAddMenuEntry("Rotate Clockwise", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;
}
