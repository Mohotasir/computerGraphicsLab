//about origin
#include <GL/glut.h>

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex2f(150, 150);
  glVertex2f(300, 150);
  glVertex2f(225, 225);
  glEnd();
  glFlush();
}
void OptionsMenu(GLint selectedOpt)
{
  switch (selectedOpt)
  {
  case 1:
  /* draw unit square polygon */
  glBegin(GL_POLYGON);
  glVertex2f(150, 150);
  glVertex2f(300, 150);
  glVertex2f(225, 225);
  glEnd();
  /* flush GL buffers */
  glFlush();
  break;
  case 2:
  glRotatef(15, 0, 0, 1);
  glColor3f(1.0f, 0.0f, 0.0f);
  break;
  case 3:
  glScalef(1.5, 1.5, 0);
  glColor3f(0.0f, 1.0f, 0.0f);
  break;
  case 4:
  glTranslatef(100, 100, 0);
  glColor3f(0.0f, 1.0f, 1.0f);
  default: break;
  }
}
void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glPointSize(1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 800.0, 0.0, 800.0);
 }
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("simple");
  glutDisplayFunc(display);
  glutCreateMenu(OptionsMenu);
  glutAddMenuEntry("Polygon ", 1);
  glutAddMenuEntry("Rotate ", 2);
  glutAddMenuEntry("Scale", 3);
  glutAddMenuEntry("Translate ", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  init();
  glutMainLoop();
}
