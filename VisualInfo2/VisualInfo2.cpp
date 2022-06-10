#include<GL/glut.h>

double vertices[][2] = {//大きな四角形
	{0.9, 0.9},
	{-0.9, 0.9},
	{-0.9, -0.9},
	{0.9, -0.9}
};

void display(void)
{
	glViewport(0, 0, 320, 120); //元のウインドウに対して歪んだ形で表示される

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; i++)
		glVertex2d(vertices[i][0], vertices[i][1]);
	glEnd();

	glFlush();

}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100); //ウインドウを表示する画面上の左上の位置
	glutInitWindowSize(320, 240); //ウインドウのサイズ

	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutMainLoop();
	return 0;
}
