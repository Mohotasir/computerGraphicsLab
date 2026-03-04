

#include <iostream>
#include <cmath>
#include <GL/glut.h>

using namespace std;

// ─── Window & Clock Settings ────────────────────────────────────────────────
const int WIN_W   = 600;
const int WIN_H   = 600;
const int CX      = 300;   // clock center X
const int CY      = 300;   // clock center Y
const int R       = 220;   // clock face radius
const int TICK_R  = 200;   // tick mark outer radius
const int TICK_LEN_MAJOR = 20;  // hour tick length
const int TICK_LEN_MINOR = 10;  // minute tick length

// ─── Bresenham's Line Algorithm ──────────────────────────────────────────────
void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (true)
    {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
    glEnd();
}

// ─── Midpoint Circle Algorithm ───────────────────────────────────────────────
void drawCircle(int cx, int cy, int r)
{
    int x = 0, y = r;
    int d = 1 - r;   // decision parameter

    auto plotOct = [&](int px, int py) {
        glBegin(GL_POINTS);
        glVertex2i(cx + px, cy + py);
        glVertex2i(cx - px, cy + py);
        glVertex2i(cx + px, cy - py);
        glVertex2i(cx - px, cy - py);
        glVertex2i(cx + py, cy + px);
        glVertex2i(cx - py, cy + px);
        glVertex2i(cx + py, cy - px);
        glVertex2i(cx - py, cy - px);
        glEnd();
    };

    plotOct(x, y);
    while (x < y)
    {
        if (d < 0)
            d += 2 * x + 3;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        plotOct(x, y);
    }
}

// ─── Draw a thick circle by stacking concentric circles ──────────────────────
void drawThickCircle(int cx, int cy, int r, int thickness)
{
    for (int i = 0; i < thickness; i++)
        drawCircle(cx, cy, r - i);
}

// ─── Draw tick marks (12 hour + 60 minute) ───────────────────────────────────
void drawTicks()
{
    const double PI = 3.14159265358979;

    for (int i = 0; i < 60; i++)
    {
        double angle = (90.0 - i * 6.0) * PI / 180.0;  // 6° per minute
        bool isHour  = (i % 5 == 0);
        int  len     = isHour ? TICK_LEN_MAJOR : TICK_LEN_MINOR;
        int  outerR  = TICK_R;
        int  innerR  = TICK_R - len;

        if (isHour) glLineWidth(3.0f); else glLineWidth(1.5f);

        int x1 = CX + (int)(outerR * cos(angle));
        int y1 = CY + (int)(outerR * sin(angle));
        int x2 = CX + (int)(innerR * cos(angle));
        int y2 = CY + (int)(innerR * sin(angle));

        drawLine(x1, y1, x2, y2);
    }
}

// ─── Draw a clock hand ───────────────────────────────────────────────────────
//   angleDeg : angle from 12-o'clock position, clockwise degrees
//   length   : hand length in pixels
void drawHand(float angleDeg, int length)
{
    const double PI = 3.14159265358979;
    double rad = (90.0 - angleDeg) * PI / 180.0;
    int x2 = CX + (int)(length * cos(rad));
    int y2 = CY + (int)(length * sin(rad));
    drawLine(CX, CY, x2, y2);
}

// ─── Main Display ─────────────────────────────────────────────────────────────
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // --- Clock face (thick border) ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1.5f);
    drawThickCircle(CX, CY, R, 4);

    // --- Tick marks ---
    drawTicks();

    // --- Hour hand (pointing ~10:10 area for demo — classic clock pose) ---
    // Hour hand: 8 o'clock = 240°, Minute hand: 10 min = 60°
    // Showing time: 9:10 (like image — hands pointing left & lower-right)
    glPointSize(4.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    // Hour hand — pointing ~9 o'clock (270°), length 120
    drawHand(265.0f, 120);

    // Minute hand — pointing ~10 min mark (60°), length 160
    drawHand(55.0f, 160);

    // --- Center dot ---
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2i(CX, CY);
    glEnd();

    glFlush();
}

// ─── Init ─────────────────────────────────────────────────────────────────────
void myInit(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(2.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WIN_W, 0.0, WIN_H);
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Analog Clock — Midpoint Circle + Bresenham Line");
    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();
    return 0;
}
