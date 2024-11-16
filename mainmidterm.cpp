// Thu vien can thiet
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Hang so Pi
const double PI = 3.14159265358979323846;

// Cau truc luu thong tin xoay khoi
struct xoay_khoi {
    GLfloat goc, truc_x, truc_y, truc_z;
};

// Cac bien dieu khien camera va goc nhin
GLfloat goc_quay = 45.0f;                // Goc quay mac dinh
GLfloat ti_le_khung_hinh;                // Ti le khung hinh
GLfloat kich_thuoc_khoi = 30.0f;         // Kich thuoc moi khoi rubik
GLint xoay_ngang = 0, xoay_doc = 0;      // Goc xoay ngang va doc
GLint buoc_xoay = 5;                     // Buoc xoay moi lan
GLint khoang_cach = 5;                   // Khoang cach giua cac khoi
GLint buoc_khoang_cach = 3;              // Buoc tang/giam khoang cach

// Bien dieu khien mat rubik dang duoc chon
GLint mat_x_dau = 0, mat_x_cuoi = 2;     // Vi tri mat theo truc x
GLint mat_y_dau = 0, mat_y_cuoi = 2;     // Vi tri mat theo truc y
GLint mat_z_dau = 0, mat_z_cuoi = 2;     // Vi tri mat theo truc z

// Ma tran luu trang thai xoay cua tung khoi
vector<xoay_khoi> ma_tran_xoay[3][3][3];

// Bien xu ly chuot
GLfloat vi_tri_chuot_x = 0, vi_tri_chuot_y = 0;  // Vi tri chuot
bool chuot_trai_nhan = false;                     // Trang thai nut chuot trai
bool chuot_phai_nhan = false;                     // Trang thai nut chuot phai
bool dang_xoay = false;                           // Trang thai dang xoay

// Ham thuc hien xoay mat rubik
void thuc_hien_xoay(GLfloat goc) {
    if(dang_xoay) return;
    dang_xoay = true;

    vector<xoay_khoi> mat[3][3];
    int chi_muc;
    xoay_khoi phep_xoay;

    // Xoay mat duoc chon
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j) {
            chi_muc = 2 - j%3;

            // Xoay theo truc x
            if(mat_x_dau == mat_x_cuoi) {
                phep_xoay = {goc, 1.0, 0.0, 0.0};
                mat[chi_muc][i] = ma_tran_xoay[mat_x_cuoi][i][j];
            }

            // Xoay theo truc y
            if(mat_y_dau == mat_y_cuoi) {
                phep_xoay = {goc, 0.0, 1.0, 0.0};
                mat[chi_muc][i] = ma_tran_xoay[j][mat_y_cuoi][i];
            }

            // Xoay theo truc z
            if(mat_z_dau == mat_z_cuoi) {
                phep_xoay = {-1 * goc, 0.0, 0.0, 1.0};
                mat[chi_muc][i] = ma_tran_xoay[j][i][mat_z_cuoi];
            }

            mat[chi_muc][i].push_back(phep_xoay);
        }

    // Cap nhat ma tran xoay
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j) {
            if(mat_x_dau == mat_x_cuoi)
                ma_tran_xoay[mat_x_cuoi][i][j] = mat[i][j];

            if(mat_y_dau == mat_y_cuoi)
                ma_tran_xoay[j][mat_y_cuoi][i] = mat[i][j];

            if(mat_z_dau == mat_z_cuoi)
                ma_tran_xoay[j][i][mat_z_cuoi] = mat[i][j];
        }

    dang_xoay = false;
    glutPostRedisplay();
}

// Ham ve mot khoi lap phuong don
void ve_khoi_lap_phuong(int x, int y, int z) {
    vector<xoay_khoi>& xoay_cuc_bo = ma_tran_xoay[x][y][z];

    glPushMatrix();

    // Dich den vi tri
    glTranslatef((x - 1) * kich_thuoc_khoi + x * khoang_cach,
                 (y - 1) * kich_thuoc_khoi + y * khoang_cach,
                 (z - 1) * kich_thuoc_khoi + z * khoang_cach);

    // Ap dung cac phep xoay
    for(int i = xoay_cuc_bo.size() - 1; i >= 0; --i) {
        glRotatef(xoay_cuc_bo[i].goc, 
                 xoay_cuc_bo[i].truc_x,
                 xoay_cuc_bo[i].truc_y,
                 xoay_cuc_bo[i].truc_z);
    }

    // Mat truoc - mau do
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(kich_thuoc_khoi/2, kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, -kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, -kich_thuoc_khoi/2, kich_thuoc_khoi/2);
    glEnd();

    // Mat sau - mau cam
    glColor3f(1.0f, 0.5f, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_QUADS);
        glVertex3f(kich_thuoc_khoi/2, kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, -kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, -kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
    glEnd();

    // Mat trai - mau xanh duong
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-kich_thuoc_khoi/2, kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, -kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, -kich_thuoc_khoi/2, kich_thuoc_khoi/2);
    glEnd();

    // Mat phai - mau xanh la
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(kich_thuoc_khoi/2, kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, -kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, -kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
    glEnd();

    // Mat tren - mau trang
    glColor3f(1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-kich_thuoc_khoi/2, kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
    glEnd();

    // Mat duoi - mau vang
    glColor3f(1.0f, 1.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-kich_thuoc_khoi/2, -kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, -kich_thuoc_khoi/2, -kich_thuoc_khoi/2);
        glVertex3f(kich_thuoc_khoi/2, -kich_thuoc_khoi/2, kich_thuoc_khoi/2);
        glVertex3f(-kich_thuoc_khoi/2, -kich_thuoc_khoi/2, kich_thuoc_khoi/2);
    glEnd();

    glPopMatrix();
}

// Ham khoi tao cac thiet lap ban dau
void khoi_tao() {
    // Thiet lap anh sang
    GLfloat anh_sang_moi_truong[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat anh_sang_khuech_tan[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat anh_sang_phan_xa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat vi_tri_anh_sang[] = {0.0f, 50.0f, 50.0f, 1.0f};

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);

    // Thiet lap vat lieu
    glMaterialfv(GL_FRONT, GL_SPECULAR, anh_sang_phan_xa);
    glMateriali(GL_FRONT, GL_SHININESS, 60);

    // Thiet lap anh sang
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, anh_sang_moi_truong);
    glLightfv(GL_LIGHT0, GL_AMBIENT, anh_sang_moi_truong);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, anh_sang_khuech_tan);
    glLightfv(GL_LIGHT0, GL_SPECULAR, anh_sang_phan_xa);
    glLightfv(GL_LIGHT0, GL_POSITION, vi_tri_anh_sang);

    // Bat cac tinh nang
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    // Khoi tao goc nhin
    goc_quay = 45.0f;
    ti_le_khung_hinh = 1.0f;
}

// Ham hien thi
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Dat camera
    gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);

    // Xoay toan cuc
    glRotatef(xoay_ngang, 1.0f, 0.0f, 0.0f);
    glRotatef(xoay_doc, 0.0f, 1.0f, 0.0f);

    // Ve rubik
    for(int x = 0; x < 3; x++)
        for(int y = 0; y < 3; y++)
            for(int z = 0; z < 3; z++)
                ve_khoi_lap_phuong(x, y, z);

    glutSwapBuffers();
}

// Ham xu ly thay doi kich thuoc cua so
void reshape(int w, int h) {
    if(h == 0) h = 1;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ti_le_khung_hinh = (GLfloat)w / (GLfloat)h;
    gluPerspective(goc_quay, ti_le_khung_hinh, 0.1f, 500.0f);
    
    glMatrixMode(GL_MODELVIEW);
}

// Ham xu ly su kien chuot
void mouse(int button, int state, int x, int y) {
    // Xu ly nut chuot trai
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            chuot_trai_nhan = true;
            vi_tri_chuot_x = x;
            vi_tri_chuot_y = y;
        }
        else {
            chuot_trai_nhan = false;
        }
    }
    // Xu ly nut chuot phai
    else if(button == GLUT_RIGHT_BUTTON) {
        if(state == GLUT_DOWN) {
            chuot_phai_nhan = true;
            vi_tri_chuot_x = x;
            vi_tri_chuot_y = y;
        }
        else {
            chuot_phai_nhan = false;
        }
    }
}

// Ham xu ly chuyen dong chuot
void motion(int x, int y) {
    // Xu ly khi chuot trai duoc giu
    if(chuot_trai_nhan) {
        float dx = x - vi_tri_chuot_x;
        float dy = y - vi_tri_chuot_y;
        
        xoay_doc += dx * 0.5f;
        xoay_ngang += dy * 0.5f;
        
        // Gioi han goc xoay ngang de tranh lat nguoc
        if(xoay_ngang > 80) xoay_ngang = 80;
        if(xoay_ngang < -80) xoay_ngang = -80;
        
        // Chuan hoa goc xoay doc
        while(xoay_doc > 360) xoay_doc -= 360;
        while(xoay_doc < 0) xoay_doc += 360;
    }
    // Xu ly khi chuot phai duoc giu (zoom)
    else if(chuot_phai_nhan) {
        float dy = y - vi_tri_chuot_y;
        if(goc_quay >= 10 && goc_quay <= 130) {
            goc_quay += dy * 0.1f;
            if(goc_quay < 10) goc_quay = 10;
            if(goc_quay > 130) goc_quay = 130;
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(goc_quay, ti_le_khung_hinh, 0.1f, 500.0f);
            glMatrixMode(GL_MODELVIEW);
        }
    }
    
    vi_tri_chuot_x = x;
    vi_tri_chuot_y = y;
    glutPostRedisplay();
}

// Ham xu ly su kien ban phim
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        // Tang/giam khoang cach giua cac khoi
        case '+':
            khoang_cach += buoc_khoang_cach;
            break;
            
        case '-':
            khoang_cach -= buoc_khoang_cach;
            break;
            
        // Xoay goc nhin bang phim
        case 'l':
        case 'L':
            xoay_doc = (xoay_doc - buoc_xoay) % 360;
            break;
            
        case 'j':
        case 'J':
            xoay_doc = (xoay_doc + buoc_xoay) % 360;
            break;
            
        case 'i':
        case 'I':
            xoay_ngang = (xoay_ngang + buoc_xoay) % 360;
            break;
            
        case 'k':
        case 'K':
            xoay_ngang = (xoay_ngang - buoc_xoay) % 360;
            break;
            
        // Chon mat theo truc X
        case 'q':
        case 'Q':
            mat_x_dau = mat_x_cuoi = 0;
            mat_y_dau = 0; mat_y_cuoi = 2;
            mat_z_dau = 0; mat_z_cuoi = 2;
            break;
            
        case 'w':
        case 'W':
            mat_x_dau = mat_x_cuoi = 1;
            mat_y_dau = 0; mat_y_cuoi = 2;
            mat_z_dau = 0; mat_z_cuoi = 2;
            break;
            
        case 'e':
        case 'E':
            mat_x_dau = mat_x_cuoi = 2;
            mat_y_dau = 0; mat_y_cuoi = 2;
            mat_z_dau = 0; mat_z_cuoi = 2;
            break;
            
        // Chon mat theo truc Y
        case 'a':
        case 'A':
            mat_y_dau = mat_y_cuoi = 0;
            mat_x_dau = 0; mat_x_cuoi = 2;
            mat_z_dau = 0; mat_z_cuoi = 2;
            break;
            
        case 's':
        case 'S':
            mat_y_dau = mat_y_cuoi = 1;
            mat_x_dau = 0; mat_x_cuoi = 2;
            mat_z_dau = 0; mat_z_cuoi = 2;
            break;
            
        case 'd':
        case 'D':
            mat_y_dau = mat_y_cuoi = 2;
            mat_x_dau = 0; mat_x_cuoi = 2;
            mat_z_dau = 0; mat_z_cuoi = 2;
            break;
            
        // Chon mat theo truc Z
        case 'z':
        case 'Z':
            mat_z_dau = mat_z_cuoi = 2;
            mat_x_dau = 0; mat_x_cuoi = 2;
            mat_y_dau = 0; mat_y_cuoi = 2;
            break;
            
        case 'x':
        case 'X':
            mat_z_dau = mat_z_cuoi = 1;
            mat_x_dau = 0; mat_x_cuoi = 2;
            mat_y_dau = 0; mat_y_cuoi = 2;
            break;
            
        case 'c':
        case 'C':
            mat_z_dau = mat_z_cuoi = 0;
            mat_x_dau = 0; mat_x_cuoi = 2;
            mat_y_dau = 0; mat_y_cuoi = 2;
            break;
            
        // Xoay mat da chon
        case 'u':
        case 'U':
            thuc_hien_xoay(-90);
            break;
            
        case 'o':
        case 'O':
            thuc_hien_xoay(90);
            break;
            
        // Reset goc nhin
        case 'r':
        case 'R':
            xoay_ngang = 0;
            xoay_doc = 0;
            goc_quay = 45.0f;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(goc_quay, ti_le_khung_hinh, 0.1f, 500.0f);
            glMatrixMode(GL_MODELVIEW);
            break;
            
        // Thoat chuong trinh
        case 27: // ESC
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

// Ham main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rubik's Cube 3D");
    
    khoi_tao();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    
    // In huong dan su dung
    printf("Huong dan su dung:\n");
    printf("- Chuot trai: Xoay goc nhin\n");
    printf("- Chuot phai: Zoom in/out\n");
    printf("- Q,W,E: Chon mat X (trai, giua, phai)\n");
    printf("- A,S,D: Chon mat Y (duoi, giua, tren)\n");
    printf("- Z,X,C: Chon mat Z (sau, giua, truoc)\n");
    printf("- U: Xoay nguoc chieu kim dong ho\n");
    printf("- O: Xoay theo chieu kim dong ho\n");
    printf("- R: Reset goc nhin\n");
    printf("- +/-: Tang/giam khoang cach giua cac khoi\n");
    printf("- I,J,K,L: Xoay goc nhin bang phim\n");
    printf("- Mau do la mat chinh\n");
    printf("- ESC: Thoat\n");
    
    glutMainLoop();
    return 0;
}
