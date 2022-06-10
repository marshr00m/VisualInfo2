/*
60256309 ��t �^�K
�����̖̂ʂ��ƂɐF��ς��邽�߂̔z��������display�֐����ŌĂяo���܂����B
�܂��A�v���O�����������Ȃ��Ă��Ă���̂ŁA���t�@�N�^�����O���s���č\������蕪����₷�����܂����B
��������������Ƃő�����������̂ŁA�Q�[�����Ɏg�p����Ă���L�[�z�u�����ɂ��Ē����I�ɑ���ł���悤�ɑO�񂩂�ύX���܂����B

-----���s���̑���-----
"W, S" : X����]
"A, D" : Y����]
"Q, E" : Z����]
";, ]" : X�����s�ړ�
"@, :" : Y�����s�ړ�
"p, [" : Z�����s�ړ�
---------------------
*/

#include<GL/glut.h>
#include<math.h>
#include<iostream>

#define PMAX 1000          // �������Ƃ��ł��钸�_���̍ő吔

// �O���[�o���ϐ� -----------------------------------------------------
// ------------------------------------------------------------------

double vertices[PMAX][3];  // ���I�Ɋm�ۂ��ׂ������ȒP�̂��߂ɂ��̂悤�ɂ���D

double px = 0.0;           // �}�`�̕`��ʒu x���W
double py = 0.0;           // �}�`�̕`��ʒu y���W
double pz = 0.0;           // �}�`�̕`��ʒu z���W

double angx = 0.0;         // X�����̉�]�p
double angy = 0.0;         // Y�����̉�]�p
double angz = 0.0;         // Z�����̉�]�p

int nv = 8;                //���_��

double model[][3] = {                   //���_�̍��W
    {0.0, 0.0, 0.0},
    {0.5, 0.0, 0.0},
    {0.5, 0.5, 0.0},
    {0.0, 0.5, 0.0},
    {0.0, 0.0, 0.5},
    {0.5, 0.0, 0.5},
    {0.5, 0.5, 0.5},
    {0.0, 0.5, 0.5},
};

int model_f[][4] = {
    {3, 2, 1, 0},
    {1, 2, 6, 5},
    {5, 6, 7, 4},
    {4, 7, 3, 0},
    {2, 3, 7, 6},
    {1, 0, 4, 5}
};

double color_mat[][3] = {
    {1, 0, 0},
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 1},
    {0, 1, 0},
    {0, 1, 1}
};

double rotation_mat[4][4] = {};
double translation_mat[4][4] = {};
double transformation_mat[4][4] = {};

// �v���g�^�C�v�錾 ----------------------------------------------------
// ------------------------------------------------------------------

void display(void);

// 3D
void makeRotationMatrix3DX(double mat[4][4], const double r);
void makeRotationMatrix3DY(double mat[4][4], const double r);
void makeRotationMatrix3DZ(double mat[4][4], const double r);

void makeTranslationMatrix3D(double mat[4][4], const double tx, const double ty, const double tz);
void makeTransformationMatrix3D(double out_mat[4][4], const double in_mat1[4][4], const double in_mat2[4][4]);

void appendRotation3DX(double mat[4][4], const double r);
void appendRotation3DY(double mat[4][4], const double r);
void appendRotation3DZ(double mat[4][4], const double r);
void appendTranslation3D(double mat[4][4], const double tx, const double ty, const double tz);

void transform3D();
void setIdentity3D(double mat[4][4]);

void keyboard(unsigned char key, int x, int y);

// ------------------------------------------------------------------
// ------------------------------------------------------------------

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
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
    // X��
    glVertex2d(-1.0, 0.0);
    glVertex2d(1.0, 0.0);
    // Y��
    glVertex2d(0.0, -1.0);
    glVertex2d(0.0, 1.0);
    glEnd();

    //�}�`
    glColor3f(0.0, 1.0, 0.0);

    //���W�ϊ�
    setIdentity3D(transformation_mat);

    appendRotation3DX(transformation_mat, angx);
    appendRotation3DY(transformation_mat, angy);
    appendRotation3DZ(transformation_mat, angz);

    appendTranslation3D(transformation_mat, px, py, pz);

    transform3D();

    double s = 1.0;     // ���_���瓊�e�ʂ܂ł̋���
    double d = 1.9;     // ���_���猴�_�܂ł̋���

    for (int i = 0; i < 6; i++) {
        glColor3f(color_mat[i][0], color_mat[i][1], color_mat[i][2]);
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            glVertex2d(s / (d - vertices[model_f[i][j]][2]) * vertices[model_f[i][j]][0], s / (d - vertices[model_f[i][j]][2]) * vertices[model_f[i][j]][1]);
        }
        glEnd();
    }
    glFlush();
}



// 3D
void makeRotationMatrix3DX(double mat[4][4], const double r) {
    mat[0][0] = 1;
    mat[0][1] = 0;
    mat[0][2] = 0;
    mat[0][3] = 0;

    mat[1][0] = 0;
    mat[1][1] = cos(r);
    mat[1][2] = -1 * sin(r);
    mat[1][3] = 0;

    mat[2][0] = 0;
    mat[2][1] = sin(r);
    mat[2][2] = cos(r);
    mat[2][3] = 0;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;
}

void makeRotationMatrix3DY(double mat[4][4], const double r) {
    mat[0][0] = cos(r);
    mat[0][1] = 0;
    mat[0][2] = sin(r);
    mat[0][3] = 0;

    mat[1][0] = 0;
    mat[1][1] = 1;
    mat[1][2] = 0;
    mat[1][3] = 0;

    mat[2][0] = -1 * sin(r);
    mat[2][1] = 0;
    mat[2][2] = cos(r);
    mat[2][3] = 0;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;
}

void makeRotationMatrix3DZ(double mat[4][4], const double r) {
    mat[0][0] = cos(r);
    mat[0][1] = -1 * sin(r);
    mat[0][2] = 0;
    mat[0][3] = 0;

    mat[1][0] = sin(r);
    mat[1][1] = cos(r);
    mat[1][2] = 0;
    mat[1][3] = 0;

    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
    mat[2][3] = 0;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;
}

void makeTranslationMatrix3D(double mat[4][4], const double tx, const double ty, const double tz) {
    mat[0][0] = 1;
    mat[0][1] = 0;
    mat[0][2] = 0;
    mat[0][3] = tx;

    mat[1][0] = 0;
    mat[1][1] = 1;
    mat[1][2] = 0;
    mat[1][3] = ty;

    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
    mat[2][3] = tz;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;
}

void makeTransformationMatrix3D(double out_mat[4][4], const double in_mat1[4][4], const double in_mat2[4][4]) {
    double tmp_mat[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tmp_mat[i][j] =
                in_mat1[i][0] * in_mat2[0][j] +
                in_mat1[i][1] * in_mat2[1][j] +
                in_mat1[i][2] * in_mat2[2][j] +
                in_mat1[i][3] * in_mat2[3][j];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out_mat[i][j] = tmp_mat[i][j];
        }
    }
}

void appendRotation3DX(double mat[4][4], const double r) {
    double operation_mat[4][4];
    makeRotationMatrix3DX(operation_mat, r);
    makeTransformationMatrix3D(mat, operation_mat, mat);
}

void appendRotation3DY(double mat[4][4], const double r) {
    double operation_mat[4][4];
    makeRotationMatrix3DY(operation_mat, r);
    makeTransformationMatrix3D(mat, operation_mat, mat);
}

void appendRotation3DZ(double mat[4][4], const double r) {
    double operation_mat[4][4];
    makeRotationMatrix3DZ(operation_mat, r);
    makeTransformationMatrix3D(mat, operation_mat, mat);
}

void appendTranslation3D(double mat[4][4], const double tx, const double ty, const double tz) {
    double operation_mat[4][4];
    makeTranslationMatrix3D(operation_mat, tx, ty, tz);
    makeTransformationMatrix3D(mat, operation_mat, mat);
}

void transform3D() {
    for (int i = 0; i < nv; i++) {
        vertices[i][0] = transformation_mat[0][0] * model[i][0] + transformation_mat[0][1] * model[i][1] + transformation_mat[0][2] * model[i][2] + transformation_mat[0][3];
        vertices[i][1] = transformation_mat[1][0] * model[i][0] + transformation_mat[1][1] * model[i][1] + transformation_mat[1][2] * model[i][2] + transformation_mat[1][3];
        vertices[i][2] = transformation_mat[2][0] * model[i][0] + transformation_mat[2][1] * model[i][1] + transformation_mat[2][2] * model[i][2] + transformation_mat[2][3];
    }
}

void setIdentity3D(double mat[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            if (i == j) mat[i][j] = 1.0;
            else mat[i][j] = 0.0;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case ';':
        px -= 0.1;
        glutPostRedisplay();
        break;
    case ']':
        px += 0.1;
        glutPostRedisplay();
        break;

    case '@':
        py += 0.1;
        glutPostRedisplay();
        break;
    case ':':
        py -= 0.1;
        glutPostRedisplay();
        break;

    case 'p':
        pz += 0.1;
        glutPostRedisplay();
        break;
    case '[':
        pz -= 0.1;
        glutPostRedisplay();
        break;

    case 'w':
        angx -= 0.1;
        glutPostRedisplay();
        break;
    case 's':
        angx += 0.1;
        glutPostRedisplay();
        break;

    case 'a':
        angy -= 0.1;
        glutPostRedisplay();
        break;
    case 'd':
        angy += 0.1;
        glutPostRedisplay();
        break;

    case 'q':
        angz += 0.1;
        glutPostRedisplay();
        break;
    case 'e':
        angz -= 0.1;
        glutPostRedisplay();
        break;
    case ' ':
        angx += 0.1;
        angy += 0.1;
        angz += 0.1;
        glutPostRedisplay();
        break;

    case 27: // ESC
        exit(0);
        break;
    default:
        break;
    }
}
