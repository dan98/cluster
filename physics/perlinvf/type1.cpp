#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
using namespace std;
#include "simplex.cpp"
#define x first
#define lsb(x) (x&-x)
#define pb push_back
#define y second
const int NMAX=300666; //LOL
const int MOD=1000000007;
typedef long long LL;
typedef long double LD;
typedef pair<int, int> PII;
typedef pair<LD,LD> PLD;
typedef vector<int> VI;

GLFWwindow* window;

void init_frame()
{	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(700, 700, "main", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
}

struct Vector{
	double x, y, z;
	Vector(double _x, double _y, double _z)
	{
		x=_x;
		y=_y;
		z=_z;
	}
};
struct Mat3
{
	double m[3][3];
	Mat3(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33)
	{
		m[0][0]=a11;
		m[0][1]=a12;
		m[0][2]=a13;
		m[1][0]=a21;
		m[1][1]=a22;
		m[1][2]=a23;
		m[2][0]=a31;
		m[2][1]=a32;
		m[2][2]=a33;
	}
};

Vector multMat3Vector(Mat3 A, Vector v)
{
	Vector n(0,0,0);
	
	n.x=v.x*A.m[0][0]+v.y*A.m[0][1]+v.z*A.m[0][2];
	n.y=v.x*A.m[1][0]+v.y*A.m[1][1]+v.z*A.m[1][2];
	n.z=v.x*A.m[2][0]+v.y*A.m[2][1]+v.z*A.m[2][2];

	return n;
}

Vector addVector(Vector a, Vector b)
{
	Vector n(0,0,0);
	n.x=a.x+b.x;
	n.y=a.y+b.y;
	n.z=a.z+b.z;

}

Mat3 Qx(double a)
{
	return Mat3(1, 0, 0, 0, cos(a), sin(a), 0, -sin(a), cos(a));
}
Mat3 Qy(double a)
{
	return Mat3(cos(a), 0, sin(a), 0, 1, 0, -sin(a), 0, cos(a));
}
Mat3 Qz(double a)
{
	return Mat3(cos(a), sin(a), 0, -sin(a), cos(a), 0, 0, 0, 1);
}

Vector poz(0, 0, 0);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	init_frame();
	int simplexseed1, simplexseed2, simplexseed3;
		SimplexNoiseStruct SimplexOne;
		SimplexNoiseStruct SimplexTwo;
		SimplexNoiseStruct SimplexThree;

		SimplexOne.setNoiseSeed(simplexseed1);
		SimplexTwo.setNoiseSeed(simplexseed2);
		SimplexThree.setNoiseSeed(simplexseed3);

	double t=0;

	while(!glfwWindowShouldClose(window))
	{
        glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		GLfloat lightpos[] = {.5, 1., 1., 0.};
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
		GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, cyan);

		glRotatef(0.1, 0, 1, 1);
		for(double i=-0.5; i<=0.5; i+=0.05)
			for(double j=-0.5; j<=0.5; j+=0.05)
				for(double k=-0.5; k<=0.5; k+=0.05)
				{
					glPointSize(1);
					glLineWidth(2);
					glBegin(GL_LINES);
					   glVertex3f(i, j, k);
					glEnd();
					glBegin(GL_POINTS);
					   glVertex3f(i, j, k);
					glEnd();
				}


		double a1=SimplexOne.SimplexNoiseInRange4D(poz.x, poz.y, poz.z, t, 0, 0.5*acos(-1));
		double a2=SimplexTwo.SimplexNoiseInRange4D(poz.x, poz.y, poz.z, t, 0, 0.5*acos(-1));
		double a3=SimplexThree.SimplexNoiseInRange4D(poz.x, poz.y, poz.z, t, 0, 0.5*acos(-1));
		Vector vel(0.1, 0, 0);
		vel=multMat3Vector(Qz(a1), vel);
		vel=multMat3Vector(Qy(a2), vel);
		vel=multMat3Vector(Qx(a3), vel);
		poz=addVector(poz, vel);

		glPointSize(8);
		glBegin(GL_POINTS);
		   glVertex3f(poz.x, poz.y, poz.z);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
		t+=0.001;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

