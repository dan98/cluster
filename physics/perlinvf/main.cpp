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
	Vector(){}
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
	return n;
}

double magVector(Vector a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
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

double getRandomInRange(double l, double r)
{
	return (1.0L*rand()/RAND_MAX)*(r-l) + l;
}

struct Particle{
	Vector pos, acc, vel;
	Particle(Vector _pos)
	{
		pos=_pos;
	}
	Particle(){}
};

Particle PArray[2066];

void loadParticles(int n)
{	
	for(int i=1; i<=n; ++i)
	{
		PArray[i]=Particle(Vector(getRandomInRange(-0.5,0.5),getRandomInRange(-0.5,0.5),getRandomInRange(-0.5,0.5)));
	}
}

void maxMag(Vector &v, double max)
{
	double mag=magVector(v);
	if(mag>max)
	{
		v.x/=mag;
		v.y/=mag;
		v.z/=mag;
		v.x*=max;
		v.y*=max;
		v.z*=max;
	}
}

int main(int argc, char **argv)
{
	srand(time(0));
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
	int sizeParticles=200;
	loadParticles(sizeParticles);

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
		for(double i=-0.5; i<=0.5; i+=0.1)
			for(double j=-0.5; j<=0.5; j+=0.1)
				for(double k=-0.5; k<=0.5; k+=0.1)
				{
					double a1=SimplexOne.SimplexNoiseInRange4D(i, j, k, t, 0, 10);
					double a2=SimplexTwo.SimplexNoiseInRange4D(i, j, k, t, 0, 10);
					double a3=SimplexThree.SimplexNoiseInRange4D(i, j, k, t, 0, 10);
					Vector vel(0.03, 0, 0);
					vel=multMat3Vector(Qz(a1), vel);
					vel=multMat3Vector(Qy(a2), vel);
					vel=multMat3Vector(Qx(a3), vel);
					glPointSize(1);
					glLineWidth(0.2);
					
					glBegin(GL_LINES);
					   glVertex3f(i, j, k);
					   glVertex3f(i+vel.x, j+vel.y, k+vel.z);
					glEnd();
					/*
					glBegin(GL_POINTS);
					   glVertex3f(i, j, k);
					   glVertex3f(i+vel.x, j+vel.y, k+vel.z);
					glEnd();
					*/
				}
	
		for(int i=1; i<=sizeParticles; ++i)
			for(int j=1; j<=sizeParticles; ++j)
			{
				Vector acc(PArray[j].pos.x-PArray[i].pos.x,PArray[j].pos.y-PArray[i].pos.y,PArray[j].pos.z-PArray[i].pos.z);
				maxMag(acc, 0.000001/(magVector(acc)*magVector(acc)));
				acc.x*=-1;acc.y*=-1;acc.z*=-1;
				PArray[i].acc=addVector(PArray[i].acc, acc);
				//PArray[i].vel=addVector(PArray[i].vel, PArray[i].acc);
				maxMag(PArray[i].vel, 0.005);
				maxMag(PArray[i].acc, 0.005);
			}	
		for(int i=1; i<=sizeParticles; ++i){
			double a1=SimplexOne.SimplexNoiseInRange4D(PArray[i].pos.x, PArray[i].pos.y, PArray[i].pos.z, t, 0, 10);
			double a2=SimplexTwo.SimplexNoiseInRange4D(PArray[i].pos.x, PArray[i].pos.y, PArray[i].pos.z, t, 0, 10);
			double a3=SimplexThree.SimplexNoiseInRange4D(PArray[i].pos.x, PArray[i].pos.y, PArray[i].pos.z, t, 0, 10);
			Vector acc(0.002, 0,0);
			acc=multMat3Vector(Qx(a3), acc);
			acc=multMat3Vector(Qz(a1), acc);
			acc=multMat3Vector(Qy(a2), acc);
			PArray[i].acc=addVector(PArray[i].acc, acc);
			PArray[i].vel=addVector(PArray[i].vel, PArray[i].acc);
			maxMag(PArray[i].vel, 0.005);
			maxMag(PArray[i].acc, 0.005);
			PArray[i].pos=addVector(PArray[i].pos, PArray[i].vel);
			maxMag(PArray[i].vel, 0.005);
			maxMag(PArray[i].acc, 0.005);
			Vector a=PArray[i].pos;
			a.x=min(a.x, 0.5);
			a.x=max(a.x, -0.5);
			a.y=min(a.y, 0.5);
			a.y=max(a.y, -0.5);
			a.z=min(a.z, 0.5);
			a.z=max(a.z, -0.5);
			PArray[i].pos=a;
			glPointSize(8);
			//cout<<PArray[i].pos.x<<" "<<PArray[i].pos.y<<" "<<PArray[i].pos.z<<"\n";
			glBegin(GL_POINTS);
			   glVertex3f(PArray[i].pos.x, PArray[i].pos.y, PArray[i].pos.z);
			glEnd();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
		t+=0.005;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

