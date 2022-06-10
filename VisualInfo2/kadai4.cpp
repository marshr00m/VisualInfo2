/*
60256309 稲葉 真幸
@、/キーで上昇下降、;、:キーで左右移動、A、Sキーで反時計、時計回りに回転、W、Sで拡大縮小します。
exec_1.png : pointを下記のように設定したときの実行例です。
    {-1.0, 0.0},
    {1.0, 1.0},
    {1.5, -1.0},
    {0.0, 0.0}

exec_2.png : pointを下記のように設定したときの実行例です。
    {-1.0, 0.0},
    {-0.3, 0.3},
    {0.3, 0.4},
    {1.0, 0.1}
*/

#include<GL/glut.h>
#include<math.h>
#include<iostream>

#define PMAX 1000 // 扱うことができる頂点数の最大数
double vertices[PMAX][2]; // 動的に確保すべきだが簡単のためにこのようにする．

double px = 0.0; // 図形の描画位置 x座標
double py = 0.0; // 図形の描画位置 y座標
double ang = 0.0; // 図形の回転角
double scale_x = 1.0, scale_y = 1.0;

double model[PMAX][2];

double point[4][2] = {
    {-1.0, 0.0},
    {1.0, 1.0},
    {1.5, -1.0},
    {0.0, 0.0}
};

double rotation_mat[3][3] = {};
double translation_mat[3][3] = {};
double transformation_mat[3][3] = {};

void display(void);
void keyboard(unsigned char key, int x, int y);
void makeRotation2DMatrix(double mat[3][3], const double r);
void makeTranslation2DMatrix(double mat[3][3], const double tx, const double ty);
void makeScale2DMatrix(double mat[3][3], const double sx, const double sy);
void makeTransformation2DMatrix(double out_mat[3][3], const double in_mat1[3][3], const double in_mat2[3][3]);
void appendTranslation2D(double mat[3][3], const double tx, const double ty);
void appendRotation2D(double mat[3][3], const double r);
void appendScale2D(double mat[3][3], const double sx, const double sy);
void transform2D();
void setIdentity2D(double mat[3][3]);
double getBernstein0(double t);
double getBernstein1(double t);
double getBernstein2(double t);
double getBernstein3(double t);
void getPoints(double model[][2]);

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    // X軸
    glVertex2d(-1.0, 0.0);
    glVertex2d(1.0, 0.0);
    // Y軸
    glVertex2d(0.0, -1.0);
    glVertex2d(0.0, 1.0);
    glEnd();

    //図形
    glColor3f(0.0, 1.0, 0.0);

    getPoints(model);
    setIdentity2D(transformation_mat);
    appendRotation2D(transformation_mat, ang);
    appendScale2D(transformation_mat, scale_x, scale_y);
    appendTranslation2D(transformation_mat, px, py);
    transform2D();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < PMAX; i++) {
        glVertex2d(vertices[i][0], vertices[i][1]);
    }
    glEnd();
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case ':':
        px += 0.1;
        glutPostRedisplay();
        break;
    case ';':
        px -= 0.1;
        glutPostRedisplay();
        break;
    case '@':
        py += 0.1;
        glutPostRedisplay();
        break;
    case '/':
        py -= 0.1;
        glutPostRedisplay();
        break;
    case 'a':
        ang += 0.1;
        glutPostRedisplay();
        break;
    case 's':
        ang -= 0.1;
        glutPostRedisplay();
        break;
    case 'w':
        scale_x += 0.1;
        scale_y += 0.1;
        glutPostRedisplay();
        break;
    case 'z':
        scale_x -= 0.1;
        scale_y -= 0.1;
        glutPostRedisplay();
        break;

    case 27: // ESC
        exit(0);
        break;
    default:
        break;
    }
}


void makeRotation2DMatrix(double mat[3][3], const double r) {
    mat[0][0] = cos(r);
    mat[0][1] = -1.0 * sin(r);
    mat[0][2] = 0;
    mat[1][0] = sin(r);
    mat[1][1] = cos(r);
    mat[1][2] = 0;
    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
}

void makeTranslation2DMatrix(double mat[3][3], const double tx, const double ty) {
    mat[0][0] = 1;
    mat[0][1] = 0;
    mat[0][2] = tx;
    mat[1][0] = 0;
    mat[1][1] = 1;
    mat[1][2] = ty;
    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
}

void makeScale2DMatrix(double mat[3][3], const double sx, const double sy) {
    mat[0][0] = sx;
    mat[0][1] = 0;
    mat[0][2] = 0;
    mat[1][0] = 0;
    mat[1][1] = sy;
    mat[1][2] = 0;
    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
}

void makeTransformation2DMatrix(double out_mat[3][3], const double in_mat1[3][3], const double in_mat2[3][3]) {
    double tmp_mat[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tmp_mat[i][j] =
                in_mat1[i][0] * in_mat2[0][j] +
                in_mat1[i][1] * in_mat2[1][j] +
                in_mat1[i][2] * in_mat2[2][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            out_mat[i][j] = tmp_mat[i][j];
        }
    }
}

void appendTranslation2D(double mat[3][3], const double tx, const double ty) {
    double operation_mat[3][3];
    makeTranslation2DMatrix(operation_mat, tx, ty);
    makeTransformation2DMatrix(mat, operation_mat, mat);
}

void appendRotation2D(double mat[3][3], const double r) {
    double operation_mat[3][3];
    makeRotation2DMatrix(operation_mat, r);
    makeTransformation2DMatrix(mat, operation_mat, mat);
}

void appendScale2D(double mat[3][3], const double sx, const double sy) {
    double operation_mat[3][3];
    makeScale2DMatrix(operation_mat, sx, sy);
    makeTransformation2DMatrix(mat, operation_mat, mat);
}

void transform2D() {
    for (int i = 0; i < PMAX; i++) {
        vertices[i][0] = transformation_mat[0][0] * model[i][0] + transformation_mat[0][1] * model[i][1] + transformation_mat[0][2];
        vertices[i][1] = transformation_mat[1][0] * model[i][0] + transformation_mat[1][1] * model[i][1] + transformation_mat[1][2];
    }
}

void setIdentity2D(double mat[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            if (i == j) mat[i][j] = 1.0;
            else mat[i][j] = 0.0;
    }
}

double getBernstein0(double t) {
    return (1 - t) * (1 - t) * (1 - t);
}
double getBernstein1(double t) {
    return 3 * t * (1 - t) * (1 - t);
}
double getBernstein2(double t) {
    return 3 * t * t * (1 - t);
}
double getBernstein3(double t) {
    return t * t * t;
}

void getPoints(double model[][2]) {
    double b0, b1, b2, b3;
    for (int i = 0; i <= PMAX; i++) {
        double t = i * (1 / (double)PMAX);
        b0 = getBernstein0(t);
        b1 = getBernstein1(t);
        b2 = getBernstein2(t);
        b3 = getBernstein3(t);

        model[i][0] = b0 * point[0][0] + b1 * point[1][0] + b2 * point[2][0] + b3 * point[3][0];
        model[i][1] = b0 * point[0][1] + b1 * point[1][1] + b2 * point[2][1] + b3 * point[3][1];
    }
}