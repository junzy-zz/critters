#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include "SOIL/SOIL.h"
//#include "SDL/SDL.h"
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

// Function Declarations
void texParams();
char *  itoa ( int value, char * str, int base );
void drawScene();
void drawSpider(float len,int color);
void drawBasket(float len);
void GetOGLPos(int x, int y);
void update(int value);
void handlemouse_drag(int x,int y);
void drawBox(float len);
void drawBall(float rad);
void initLazer(int value);
void drawLazer(int i);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void initspider(int value);

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24; 
GLuint loadImage(string fileName);
/*unsigned int loadtexture(const char* filename)
{
	SDL_Surface* img=SDL_LoadBMP(filename);
	unsigned int id;
	glGenTextures(1,&id);//gen text at id
	glBindTexture(GL_TEXTURE_2D,id);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_SHORT_5_6_5,img->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	SDL_FreeSurface(img);
	return id;

}*/

//texture array
	GLuint	texture[10];
// Global Variables
	float box_len = 8.0f;
	float tri_x = 0.0f;
	float tri_y = -3.0f;
	float theta = 0.0f; 
	float RedB_x=-5.0f;
	float RedB_y=-3.0f;
	float GreenB_x=5.0f;
	float GreenB_y=-3.0f;
	char LastKey='b';
	float randx;
	float they=3.0f;
	float fbound_r=6.0f;
	float fbound_l=-6.0f;
	int turn;
	float mouseposx,mouseposy,click,mouseposz;
	int abc = 1000;
//points
	int points;

	typedef struct spider
	{
		float velocity;
	float color;//color
	int state;
	float x;
	float y;
}spider;

typedef struct lazer
{
	float x,y;
	float velocity_x,velocity_y;
	int state;
	float angle;

}lazer;
spider arr[10000];
lazer lazer_array[10000];
spider floored_spiders[10000];//spiders that hit the floor

int flend;
int end;
int lend;
int nspiders;
int p=0;
int exit_flag=0;
int t=0;
int main(int argc, char **argv) {

	// SDL_Init( SDL_INIT_EVERYTHING ); 

	//initialize background


	// Initialize GLUT

	for(int i=0;i<100;i++)
	{
		arr[i].y=3.0f;
		arr[i].velocity=0.1;
		int random=rand() % 3;
		if(random==0)
		{
			arr[i].velocity=0.007;
		//	cout<<"vel is"<<arr[i].velocity<<endl;
		}
		else if(random==1)
			arr[i].velocity=0.009;
		else
			arr[i].velocity=0.011;
		//	lazer_array[i].y=-3.0f;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 1 ;
	int windowHeight = h * 1;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("Spider Game");  // Setup the window
	initRendering();

	texture[0]=loadImage(string("images/temp.png"));
	cout<<"LOADING:"<<texture[0]<<endl;
	texture[1]=loadImage(string("images/cage.png"));
	texture[2]=loadImage(string("images/Rspider.png"));
	texture[3]=loadImage(string("images/Gspider.png"));
	texture[4]=loadImage(string("images/Bspider.png"));
	texture[5]=loadImage(string("images/gun.png"));
	texture[6]=loadImage(string("images/Rcage.png"));
	texture[7]=loadImage(string("images/Gcage.png"));
	//cout<<"LOADIMG:"<<loadImage(string("image/temp.png"))<<endl;
//exit(0);
	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutMotionFunc(handlemouse_drag);
	glutReshapeFunc(handleResize);
	glutTimerFunc(10, update, 0);
	glutTimerFunc(2000,initspider,0);
	//	glutTimerFunc(500,initLazer,0);
	glutMainLoop();
	//Quit SDL 
//	SDL_Quit();
	return 0;
}

void drawScore(int value)
{ 
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f (4.0f, 3.0f);
	char a[]={"Your Score is: "};
	char text[100];
	snprintf(text,100,"Your score is:%d",value);
//	cout<<text;
		//char* text= (char*) temp_str.c_str(); 
	int k;
		//for (k = 0; a[k] != '\0'; k++)
		//	glutBitmapCharacter(font_style, a[k]);
	for (k = 0; text[k] != '\0'; k++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[k]);
}

unsigned int tex;
// Function to draw objects on the screen
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);

	
	//tex=loadtexture("background.bmp");
	glPushMatrix();

	//Draw background
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0.0f,0.0f,0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	texParams();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-6.0f, -6.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(6.0f, -6.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(6.0f, 6.0f, -15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-6.0f, 6.0f, -15.0f);
	glEnd();
	glPopMatrix(); 
	glDisable(GL_TEXTURE_2D);

	//  exit screen
	char *game_over="Game Over";
	if(exit_flag==1)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glRasterPos2f (0.0f, 0.0f);
		
		//char* text= (char*) temp_str.c_str(); 
		int k;
		//for (k = 0; a[k] != '\0'; k++)
		//	glutBitmapCharacter(font_style, a[k]);
		for (k = 0; game_over[k] != '\0'; k++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game_over[k]);
		if(t==1000)
			exit(0);
	}

	// Draw Box
	glBindTexture(GL_TEXTURE_2D,tex);
	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		//glTexCoord2f(0.0,1.0);
	glVertex2f(-box_len/2 , -box_len/2 );
		//glTexCoord2f(0.0,0.0);
	glVertex2f(box_len/2 , -box_len / 2);
		//glTexCoord2f(1.0,0.0);
	glVertex2f(box_len/2 , box_len / 2);
		//glTexCoord2f(1.0,1.0);
	glVertex2f(-box_len/2, box_len / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw red basket


	glEnable(GL_TEXTURE_2D)	;
	glPushMatrix();
	glTranslatef(RedB_x/2,RedB_y,0.0f);
//	glColor3f(1.0f,0.0f,0.0f);
//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	texParams();

	glBegin(GL_QUADS);
	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-0.25,-0.25,-0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(0.25,-0.25,-0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(0.25,0.25,-0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-0.25,0.25,-0.0f);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//Draw Green Basket

	glEnable(GL_TEXTURE_2D)	;
	glPushMatrix();
	glTranslatef(GreenB_x/2,GreenB_y,0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	texParams();

	glBegin(GL_QUADS);
	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(0.0,0.0);
	glVertex2f(-0.25,-0.25);
	glTexCoord2f(1.0,0.0);
	glVertex2f(0.25,-0.25);
	glTexCoord2f(1.0,1.0);
	glVertex2f(0.25,0.25);
	glTexCoord2f(0.0,1.0);
	glVertex2f(-0.25,0.25);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	drawScore(points);
	/*glPushMatrix();
	glTranslatef(GreenB_x/2,GreenB_y,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	drawBasket(0.5f);
	glPopMatrix();*/

	for(int i=0;i<end;i++)
	{
		glPushMatrix();
		glTranslatef(arr[i].x,arr[i].y,0.0f);
		
		if(arr[i].state==0 || arr[i].state==2)
		{

			if(arr[i].color==0)
				drawSpider(0.3f,0);
			if(arr[i].color==1)
				drawSpider(0.3f,1);
			if(arr[i].color==2)
				drawSpider(0.3f,2);
			

			
		}
		glPopMatrix();
	}

	for(int i=0;i<lend;i++)
	{
		if(lazer_array[i].y<4.5f)
		{
			glPushMatrix();
			glTranslatef(lazer_array[i].x,lazer_array[i].y,0.0f);
			glColor3f(1.0f,0.0f,0.0f);
			if(lazer_array[i].state==0)
			{
				
				drawLazer(i);

			}
			glPopMatrix();
		}
	}
	// Draw cannon
	glPushMatrix();
	glTranslatef(tri_x, tri_y, 0.0f);
	glRotatef(theta, 0.0f, 0.0f, 1.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	drawTriangle();
	glPopMatrix();

	/*	//draw spider
		glPushMatrix();
		randx=rand() % 8 - 4;
		glTranslatef(randx, 3.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		drawSpider(1.0f);
		glPopMatrix();
	 */

		glPopMatrix();
		glutSwapBuffers();
	}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
	void update(int value) {
		if(exit_flag)
			t+=1;
		if(!p)
		{
			for(int i=0;i<end;i++)
			{

				if(arr[i].state==0 && (arr[i].y>-3.0f))
				{
					arr[i].y-=arr[i].velocity;
			//cout<<arr[i].velocity<<endl;
				}
			//if(arr[i].y<tri_y+0.2 && arr[i].state!=2 && arr[i].color==2 && arr[i].x<=tri_x+0.2 && arr[i].x>=trix-0.2)
				if(abs(arr[i].x-tri_x)<=0.2f && arr[i].y<tri_y+0.4f)
				{
					exit_flag=1;
					p=1;
					exit(0);
				}
				if(arr[i].x>(RedB_x/2)-0.25 && arr[i].x<(RedB_x/2)+0.25f && arr[i].y <RedB_y+0.25f && arr[i].state!=2 )
				{
					arr[i].state=1;
					if(arr[i].color==0 && arr[i].y>-4.0f)
					{
						points++;
						arr[i].y=-10.0f;
					}
					else if(arr[i].y>-4.0f)
					{
						points --;
						arr[i].y=-10.0f;
					}
			//cout<<points;
				}

				if(arr[i].x>(GreenB_x/2)-0.25 && arr[i].x<(GreenB_x/2)+0.25f && arr[i].y <GreenB_y+0.25f && arr[i].state!=2)
				{
					arr[i].state=1;
					if(arr[i].color==1 && arr[i].y>-3.0f)
					{
						points++;
						arr[i].y=-10.0f;
					}
					else if(arr[i].y>-4.0f)
					{
						points --;
						arr[i].y=-10.0f;
					}
			//cout<<points;
				}

				if(arr[i].y<-3.0f && arr[i].state!=2)
				{
					floored_spiders[flend++]=arr[i];
					nspiders--;
					cout<<"floored";
					arr[i].state=2;
				//floored_spiders[i].state=2;
					for(int k=0;k<flend;k++)
					{
						if(floored_spiders[k].x>tri_x && floored_spiders[k].x<fbound_r)
						{
							fbound_r=floored_spiders[k].x-0.2;
							cout<<"fbound Setup"<<endl;
						}	
						else if(floored_spiders[k].x<tri_x && floored_spiders[k].x>fbound_l)
							fbound_l=floored_spiders[k].x+0.2;
					}

				}
			//basket capturing

			//dormant state on floor

				for(int j=0;j<lend;j++)
				{

					if(lazer_array[j].y>4.5f)
						lazer_array[j].state=1;



					if((lazer_array[j].x<(arr[i].x+0.15)) && (lazer_array[j].x>(arr[i].x-0.15)) && (lazer_array[j].y>(arr[i].y-0.2) && (lazer_array[j].y<arr[i].y+0.2))
						&& (lazer_array[j].state==0) && (arr[i].state==0))
					{
						cout<<"spider shot";
						system("aplay explosion.wav &");
						if(arr[i].color==2)
							points++;
						else 
							points--;
						arr[i].state=1;
						lazer_array[j].state=1;
						nspiders--;
					}


				}

			}
			for(int i=0;i<lend;i++)
			{

				lazer_array[i].y+=(0.1*cos(DEG2RAD(lazer_array[i].angle)));

				if(lazer_array[i].x>4.0f)
				{	
					lazer_array[i].x-=0.1;
			//	cout<<"lol";
					lazer_array[i].angle*=-1;
				}
				else if(lazer_array[i].x<-4.0f)
				{
					lazer_array[i].x+=0.1;
			//	cout<<"arrayx"<<lazer_array[i].x<<endl<<"boxlen"<<box_len/2<<endl;
					lazer_array[i].angle*=-1;
				}
				else
					lazer_array[i].x+=(-0.1*sin(DEG2RAD(lazer_array[i].angle)));
			}
		}
		abc = abc +100;
		glutTimerFunc(10, update, 0);
	//check boundary conditions
	}

	void drawLazer(int temp)
	{


		glRotatef(lazer_array[temp].angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,-0.50);
		glEnd();
	}
	void initLazer(int value)
	{

		lazer_array[lend].x=tri_x;
		lazer_array[lend].y=tri_y;
		lazer_array[lend++].angle=theta;
	//	glutTimerFunc(500,initLazer,0);
	}
	void initspider( int value)
	{
		if(nspiders<8)
		{
			int x=rand() % 8 - 4;
			arr[end].x=x;
			arr[end].y-=arr[end].velocity;
			arr[end].color=rand() % 3;
			end++;
			nspiders++;
		}
		glutTimerFunc(2000,initspider,0);
	}


	void drawSpider(float len,int color)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable (GL_BLEND); 
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f,0.0f,0.0f,0.1f);
		if(color==0)
			glBindTexture(GL_TEXTURE_2D,texture[2]);
		else if(color==1)
			glBindTexture(GL_TEXTURE_2D,texture[3]);
		else if(color==2)
			glBindTexture(GL_TEXTURE_2D,texture[4]);
		texParams();
		glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(-len , -len/2 );
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(len , -len / 2);
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(len , len / 2);
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(-len, len / 2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	//void drawBox(float len) {


//	}

	

/*void drawBall(float rad) {

  glBegin(GL_TRIANGLE_FAN);
  for(int i=0 ; i<360 ; i++) {
  glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
  }
  glEnd();
  }*/

  void drawTriangle() {


  	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D,texture[5]);
  	texParams();
  	glBegin(GL_QUADS);
  	glNormal3f(0.0f,0.0f,1.0f);
  	glTexCoord2f(0.0f,0.0f);
  	glVertex2f(-1.0f , -1.0f );
  	glTexCoord2f(1.0f,0.0f);
  	glVertex2f(1.0f , -1.0f);
  	glTexCoord2f(1.0f,1.0f);
  	glVertex2f(1.0f , 1.0f);
  	glTexCoord2f(0.0f,1.0f);
  	glVertex2f(-1.0f, 1.0f);
  	glEnd();
  	glDisable(GL_TEXTURE_2D);

  }

// Initializing some openGL 3D rendering options
  void initRendering() {


	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);     // escape key is pressed
	}
	cout<<"Key pressed\n"<<key;
	if(key=='r')
	{
		LastKey=key;
		turn=3;
	}
	if(key=='b')
	{
		LastKey=key;
		turn=1;
	}
	if(key=='g')
	{
		LastKey=key;
		turn=2;
	}
	if(key==' ' && abc > 1000)
	{
		abc = 0;
		LastKey=key;
		initLazer(0);
		system("aplay LASER1.WAV &");
		LastKey='b';
	}
	if(key=='p')
		p=p^1;
}

void handleKeypress2(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT && LastKey=='b' && (tri_x)-0.1>(-1*(box_len/2)) && tri_x-0.1>fbound_l)
		tri_x -= 0.3;
	if (key == GLUT_KEY_RIGHT && LastKey=='b' && (tri_x)+0.1<(box_len/2) && tri_x+0.1<fbound_r)
		tri_x += 0.3;
	if(key== GLUT_KEY_RIGHT && LastKey=='g' && (GreenB_x)+0.1<(1*(box_len)))
		GreenB_x += 0.3;
	if(key== GLUT_KEY_LEFT && LastKey=='g' && (GreenB_x)-0.1>(-1*(box_len)))
		GreenB_x -= 0.3;
	if(key== GLUT_KEY_RIGHT && LastKey=='r' && (RedB_x)+0.1<(1*(box_len)))
		RedB_x += 0.3;
	if(key== GLUT_KEY_LEFT && LastKey=='r' && (RedB_x)-0.1>(-1*(box_len)))
		RedB_x -= 0.3;

}


void texParams() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

GLuint loadImage(string fileName) {
	GLuint tex_ID;

	tex_ID = SOIL_load_OGL_texture(
		fileName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_MULTIPLY_ALPHA
		| SOIL_FLAG_COMPRESS_TO_DXT
		| SOIL_FLAG_DDS_LOAD_DIRECT
		| SOIL_FLAG_INVERT_Y
		);

	if( tex_ID > 0 ) {
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, tex_ID );

		return tex_ID;
	}
	else {
		cout << "Bad image" << endl;
		return 0;
	}
}

//mouse drag handling	

void GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	mouseposx=posX*230/2;
	mouseposy=(posY*200)/2;
	mouseposz=posZ;
}

void handlemouse_drag(int x,int y)
{
	GetOGLPos(x,  y);
	if(click==1)
	{
		if(turn==1 &&  mouseposx>fbound_l && mouseposx<fbound_r && mouseposx>(-box_len/2) && mouseposx<(box_len/2))
		{
			tri_x=mouseposx;
		}
		else if(turn==2 && (mouseposx)<(box_len) && mouseposx>(-box_len))
		{
			GreenB_x=mouseposx;
		}
		else if(turn==3 && mouseposx<(box_len) && mouseposx>(-box_len))
			RedB_x=mouseposx;
		
	}
	else if (click==2)
	{      
		turn=1;
		float s1=mouseposx-tri_x;
		float s2=mouseposy-tri_y;
		float temp_angle=-atan(float(s1)/s2)*180/PI;
		if(temp_angle>=-60 && temp_angle<=60)
		{
			theta=-atan(float(s1)/s2)*180/PI;
		}

	}
}
void handleMouseclick(int button, int state, int x, int y) {

/*	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
			theta += 15;
		else if (button == GLUT_RIGHT_BUTTON)
			theta -= 15;
	}	

*/



	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{      
                            //              if(theta<=60)
                            //                      theta += 15;
			click=1;
			GetOGLPos(x,y);
			float x1=tri_x-0.2;
			float x2=tri_x+0.2;
			float y1=tri_y-0.2;
			float y2=tri_y+0.2;
			cout<<mouseposx<<" *"<<mouseposy<<" "<<endl;
			if(mouseposx>=x1 && mouseposx<=x2 && mouseposy>=y1 &&  mouseposy<=y2 )
			{
				turn=1;
			}
			float x11=GreenB_x-0.2;
			float x22=GreenB_x+0.2;      
			float y11=-3.0f;
			float y22=GreenB_y+0.25;
			//cout << mouseposx << endl;     
			if(mouseposx>=x11 && mouseposx<=x22 && mouseposy>=y11 &&  mouseposy<=y22 )
				turn=3;
			float x111=RedB_x-0.2;
			float x222=RedB_x+0.2;       
			float y111=RedB_y-0.2;
			float y222=RedB_y+0.2;
			cout << mouseposx << endl;     
			if(mouseposx>=x111 && mouseposx<=x222 && mouseposy>=y111 &&  mouseposy<=y222 )
				turn=2;

                            //cout << x << " " <<  y << endl;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
                            //              if(theta>=-60)
                            //                      theta -= 15;
			click=2;
		}
	}
	//else
//	{
//		turn=1;
//		cout<<"cant";
//	}
}




