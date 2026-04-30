#include <GL/glut.h>
#include <math.h>
#include <stdio.h>


int x1_, y1_, x2_, y2_, x3_, y3_;
int cx, cy, r;
float scaleFactor = 1.0;
float angle = 0.0;

float fillCol[3] = {0.0, 1.0, 0.0};
float borderCol[3] = {0.0, 0.0, 0.0};

// -------- PIXEL FUNCTIONS --------
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

void draw_pixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}
// -------- BRESENHAM LINE --------
void drawLine(int x1, int y1, int x2, int y2) {
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
	x = x1; y = y1;
	if (dx > dy) {
		draw_pixel(x, y);
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
			draw_pixel(x, y);
		}
	}
	else
	{
		draw_pixel(x, y);
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
			draw_pixel(x, y);
		}
	}
}

// -------- TRIANGLE --------
void drawTriangle() {
    drawLine(x1_, y1_, x2_, y2_);
    drawLine(x2_, y2_, x3_, y3_);
    drawLine(x3_, y3_, x1_, y1_);
}

// -------- MIDPOINT CIRCLE --------
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

    while (x <= y) {
        plotCircle(x, y);
        if (d < 0)
            d += 2 * x + 3;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// -------- CIRCUMCIRCLE CALCULATION --------
void computeCircle() {
    float d = 2 * (x1_*(y2_ - y3_) + x2_*(y3_ - y1_) + x3_*(y1_ - y2_));

    cx = ((x1_*x1_ + y1_*y1_)*(y2_ - y3_) +
          (x2_*x2_ + y2_*y2_)*(y3_ - y1_) +
          (x3_*x3_ + y3_*y3_)*(y1_ - y2_)) / d;

    cy = ((x1_*x1_ + y1_*y1_)*(x3_ - x2_) +
          (x2_*x2_ + y2_*y2_)*(x1_ - x3_) +
          (x3_*x3_ + y3_*y3_)*(x2_ - x1_)) / d;

    r = sqrt((cx - x1_)*(cx - x1_) + (cy - y1_)*(cy - y1_));
}

// -------- BOUNDARY FILL --------
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

// -------- DISPLAY --------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    // Transformations about origin
    glScalef(scaleFactor, scaleFactor, 1);
    glRotatef(angle, 0, 0, 1);

    glColor3f(0, 0, 0);

    drawTriangle();
    drawCircle();

    glPopMatrix();

    glFlush();
}

void mouse(int btn, int state, int x, int y) {

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        int xi = x;
        int yi = 500 - y;   // convert screen to OpenGL

        // Check inside circle
        int dx = xi - cx;
        int dy = yi - cy;

        if (dx*dx + dy*dy <= r*r) {
            printf("Valid click: Filling...\n");
            boundaryFill(xi, yi);
        }
        else {
            printf("Invalid click! Outside circle.\n");
        }
    }
}


// -------- MENU --------
void menu(int option) {
    float inputFactor;
    float inputAngle;
    switch(option) {
        case 1:
            printf("Enter scale factor (e.g. 1.5 to scale up): ");
            fflush(stdout);
            scanf("%f", &inputFactor);
            scaleFactor *= inputFactor;
            break;
        case 2:
            printf("Enter scale factor (e.g. 1.5 to scale down): ");
            fflush(stdout);
            scanf("%f", &inputFactor);
            if (inputFactor != 0)
                scaleFactor /= inputFactor;
            break;
        case 3:
            printf("Enter rotation angle in degrees (anti-clockwise): ");
            fflush(stdout);
            scanf("%f", &inputAngle);
            angle += inputAngle;
            break;
        case 4:
            printf("Enter rotation angle in degrees (clockwise): ");
            fflush(stdout);
            scanf("%f", &inputAngle);
            angle -= inputAngle;
            break;
    }
    glutPostRedisplay();
}

// -------- INIT --------
void init() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// -------- MAIN --------
int main(int argc, char** argv) {

    printf("Enter triangle points (x1 y1 x2 y2 x3 y3):\n");
    scanf("%d %d %d %d %d %d", &x1_, &y1_, &x2_, &y2_, &x3_, &y3_);

    computeCircle();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Graphics Lab Final Task");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutCreateMenu(menu);
    glutAddMenuEntry("Scale Up", 1);
    glutAddMenuEntry("Scale Down", 2);
    glutAddMenuEntry("Rotate Anti-Clockwise", 3);
    glutAddMenuEntry("Rotate Clockwise", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
}
