
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include<GL/gl.h>
#include<GL/glut.h>
#include<bits/stdc++.h>
#define SZ 100000
#define SFI(x)              scanf("%d",&x)
#define PFI(x)              printf("%d",x)
#define SSZ(a)              strlen(a)
#define ABS(x)              ((x)<0?-(x):(x))
#define READ()              freopen("output.txt", "r", stdin)
#define WRITE()             freopen("output.txt", "w", stdout)
#define MEM(n,val)          memset((n), val, sizeof(n))
#define FOR(I,J,K)          for(I = J; I < K; I++)
#define REV(I,J,K)          for(I = J; I > K; I--)

using namespace std;

double enemy=0.1f;
double enemy_x[SZ];
double enemy_y[SZ];
double enemy_vel[SZ];
double enemy_stat[SZ]={0};
double enemy_col[SZ];

double shooter_x=0.0f;
double shooter_y=-1.9f;
double shooter_min_x=-1.9f;
double shooter_max_x=1.9f;
double angle=0.0f;

double bullet_x[SZ];
double bullet_y[SZ];
double bullet_vel=0.07f; ///bullet velocity,greater faster
int bullet_stat[SZ]={0};
double bullet_vel_x[SZ];
double bullet_vel_y[SZ];

double tip_x[SZ];
double tip_y[SZ];
int bullet=0;

int points=0;
int on=1000;
int frontCount=-1,cnt=0,high=-1;
int game_over=0;
int game_play=0;
int level=1;
int game_level=0;

double mousex;
double mousey;

int mainmenu=1;
int high_score = 0;
int howtoplay=0;
int about = 0;
int life = 3;
int flag=0;
int a=0;

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void MainMenu(){
    glLineWidth(4);
    glClearColor(0.122, 0.149, 0.15,0.656);
    //glColor3f(1.0,1.0,1.0);
    glColor3ub(255,255,255);

    renderBitmapString(-1.7f,-0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"SHOOTING GAME");




	renderBitmapString(0.5f,0.8f,GLUT_BITMAP_TIMES_ROMAN_24,"NEW GAME");



	renderBitmapString(0.5f,0.4f,GLUT_BITMAP_TIMES_ROMAN_24,"HIGH SCORE");



	renderBitmapString(0.5f,0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"HOW TO PLAY");



	renderBitmapString(0.5f,-0.4f,GLUT_BITMAP_TIMES_ROMAN_24,"ABOUT");



	renderBitmapString(0.5f,-0.8f,GLUT_BITMAP_TIMES_ROMAN_24,"EXIT");
}

void drawBullet(){
	glPushMatrix();
	glColor3ub(255,0,0);
        glTranslatef(-170/1200,220/800,0);
        glutSolidCube(0.1);
        //glutSolidSphere(0.03,50,50);
    glPopMatrix();
}

void drawenemy(){
	int i;
    if(frontCount==-1){
        cnt=0;
    }
    if(frontCount==0){
        cnt=1;
    }
	if(frontCount<100000){
		for(i=0;cnt<=1;i++){ //number of enemy appearing
			if(i>frontCount || enemy_y[i]<=-5.0f){
                enemy_x[i]=((rand()%10)*1.0f/100.0)+((rand()%4)*1.0f)-2;
                enemy_y[i]=2.3f;
				enemy_vel[i]=.003f + 3*((rand()%10*1.0f)/10000);
				enemy_stat[i]=0;
				enemy_col[i]=rand()%3;
				frontCount++;
				cnt++;
			}
		}
	}
}

void drawenemy(int i){
	glColor3ub(255,255,255);

    glPushMatrix();
        glTranslatef(0.0,0.07,0.0);
        glutSolidSphere(0.04,50,50);
    glPopMatrix();

    glLineWidth(5);
    glColor3f(0.0,1.0,0.7);

    glBegin(GL_LINES);
        glVertex2f(-0.02,-0.05);
        glVertex2f(-0.09,-0.14);
        glVertex2f(0.02,-0.05);
        glVertex2f(0.09,-0.14);
    glEnd();

    glColor3f(0.0,1.0,0.7);
    glLineWidth(5);

    glBegin(GL_LINES);
        glVertex2f(-0.04,0.01);
        glVertex2f(-0.1,-0.04);
        glVertex2f(0.04,0.01);
        glVertex2f(0.1,-0.04);
    glEnd();

    glColor3f(0.0,1.0,0.7);
    glPointSize(15.0);

    glBegin(GL_POINTS);
        glVertex2f(-0.00,-0.01);
    glEnd();
}

void gameOver(){
    glColor3ub(255,255,255);
    renderBitmapString(-0.5f,0.7f,GLUT_BITMAP_TIMES_ROMAN_24,"GAME OVER");
    renderBitmapString(-0.5f,0.5f,GLUT_BITMAP_TIMES_ROMAN_24,"SCORE : ");

    char str[80];
	sprintf(str,"%d",points);
	for(int i=0;i<(int)(strlen(str));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);

    glBegin(GL_LINES);
        glVertex2f(-0.6f,-0.5f);
        glVertex2f(-0.6f,-0.2f);
        glVertex2f(-0.6f,-0.5f);
        glVertex2f(0.6f,-0.5f);
        glVertex2f(-0.6f,-0.2f);
        glVertex2f(0.6f,-0.2f);
        glVertex2f(0.6f,-0.5f);
        glVertex2f(0.6f,-0.2f);
	glEnd();

	renderBitmapString(-0.2f,-0.4f,GLUT_BITMAP_TIMES_ROMAN_24,"MENU");

    frontCount=-1;
    cnt=0;
    high=-1;
}

void drawShooter(){
    glColor3ub(255,255,255);
	glBegin(GL_POLYGON);
        glVertex2f(-0.2f,-0.2f);
        glVertex2f(0.2f,-0.2f);
        glVertex2f(0.2f,-0.15f);
        glVertex2f(-0.2f,-0.15f);
	glEnd();

	glBegin(GL_POLYGON);
        glVertex2f(-0.1f,-0.5f);
        glVertex2f(0.1f,-0.5f);
        glVertex2f(0.1f,0.16f);
        glVertex2f(-0.1f,0.16f);
	glEnd();

	glBegin(GL_POLYGON);
        glVertex2f(0.1f,-0.15f);
        glVertex2f(0.4f,-0.15f);
        glVertex2f(0.4f,-0.05f);
        glVertex2f(0.1f,-0.05f);
	glEnd();

	glBegin(GL_POLYGON);
        glVertex2f(-0.1f,-0.15f);
        glVertex2f(-0.4f,-0.15f);
        glVertex2f(-0.4f,-0.05f);
        glVertex2f(-0.1f,-0.05f);
	glEnd();

	glBegin(GL_POLYGON);
        glVertex2f(-0.3f,-0.07f);
        glVertex2f(-0.4f,-0.07f);
        glVertex2f(-0.4f,0.15f);
        glVertex2f(-0.3f,0.1f);
	glEnd();

	glBegin(GL_POLYGON);
        glVertex2f(0.3f,-0.07f);
        glVertex2f(0.4f,-0.07f);
        glVertex2f(0.4f,0.15f);
        glVertex2f(0.3f,0.1f);
	glEnd();

	glBegin(GL_POLYGON);
        glVertex2f(-0.1f,0.16f);
        glVertex2f(0.1f,0.16f);
        glVertex2f(0.005f,0.22f);
	glEnd();
}

void new_update(int value){
	for(int i=0;i<=frontCount;i++){
		if(enemy_y[i]<=-1.6f){
			enemy_vel[i]=0;
			enemy_y[i]=-5.0f;
			enemy_stat[i]=0;
			frontCount--;
			points--;
		}
		if(shooter_x>enemy_x[i]-0.46f && shooter_x<enemy_x[i]+0.46f && enemy_y[i]-0.05f<=-1.7f){
			life--;
		}
		if(life==0){
            game_over=1;
            game_play=0;
            mainmenu=0;
		}
	}

	for(int i=0;i<=high;i++){
		if(bullet_stat[i]==1){
			bullet_x[i]-=bullet_vel_x[i];
			bullet_y[i]+=bullet_vel_y[i];
			tip_x[i]-=bullet_vel_x[i];
			tip_y[i]+=bullet_vel_y[i];
			if(bullet_y[i]>=2.0f){
				bullet_stat[i]=0;
			}
		}
	}

	for(int i=0;i<=frontCount;i++){
		for(int j=0;j<=high;j++){
			if(tip_x[j]>enemy_x[i]-0.05f && tip_x[j]<enemy_x[i]+0.05f && tip_y[j]>enemy_y[i]-0.05f && tip_y[j]<enemy_y[i]+0.05f && bullet_stat[j]==1){
				enemy_vel[i]=0;
				enemy_y[i]=-5.0f;
				enemy_stat[i]=0;
				frontCount--;
				bullet_stat[j]=0;
				points++;
			}
		}
	}

	for(int i=0;i<=frontCount;i++){
		if(enemy_y[i]<0.0f && enemy_stat[i]==0){
			cnt--;
			enemy_stat[i]=1;
		}
		if(enemy_y[i]>=-1.85f){
			enemy_y[i]-=enemy_vel[i];
		}
	}

	on+=100;
	game_level+=10;

	if(game_level>=10000){
		level++;
		game_level=1;
	}

    glutTimerFunc(10, new_update, 0);
}

void score(){
	glColor3ub(255,255,255);
    renderBitmapString(2.2f,1.8f,GLUT_BITMAP_HELVETICA_18,"SCORE :  ");
	char str[80];
	sprintf(str,"%d",points);
	for(int i=0;i<(int)(strlen(str));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}

void showLife(){
	glColor3ub(255,255,255);
    renderBitmapString(-2.6f,1.8f,GLUT_BITMAP_HELVETICA_18,"LIFE :  ");
	char str[80];
	sprintf(str,"%d",life);
	for(int i=0;i<(int)(strlen(str));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}

void storeHighScore(){
    ifstream infile("output.txt");
    infile>>a;
    infile.close();

    ofstream outfile("output.txt");
    if(points>a){
        outfile<<points<<endl;
    }
    outfile.close();
}

void showHighScore(){
    ifstream infile("output.txt");
    infile>>a;
    infile.close();

    glColor3ub(255,255,255);
    renderBitmapString(-0.7f,0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"HIGH SCORE :  ");
	char str[80];
	sprintf(str,"%d",a);
	for(int i=0;i<(int)(strlen(str));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);

    glBegin(GL_LINES);
        glVertex2f(-0.6f,-0.5f);
        glVertex2f(-0.6f,-0.2f);
        glVertex2f(-0.6f,-0.5f);
        glVertex2f(0.6f,-0.5f);
        glVertex2f(-0.6f,-0.2f);
        glVertex2f(0.6f,-0.2f);
        glVertex2f(0.6f,-0.5f);
        glVertex2f(0.6f,-0.2f);
	glEnd();

	renderBitmapString(-0.2f,-0.4f,GLUT_BITMAP_TIMES_ROMAN_24,"BACK");
}

void howToPlay(){
    glColor3ub(255,255,255);
    renderBitmapString(-0.4f,1.7f,GLUT_BITMAP_TIMES_ROMAN_24,"HOW TO PLAY");

    glBegin(GL_LINES);
        glVertex2f(-0.6f,-1.5f);
        glVertex2f(-0.6f,-1.2f);
        glVertex2f(-0.6f,-1.5f);
        glVertex2f(0.6f,-1.5f);
        glVertex2f(-0.6f,-1.2f);
        glVertex2f(0.6f,-1.2f);
        glVertex2f(0.6f,-1.5f);
        glVertex2f(0.6f,-1.2f);
	glEnd();

	renderBitmapString(-2.7f,1.4f,GLUT_BITMAP_HELVETICA_18,"1. Use Left arrow key to move the shooter Left");
	renderBitmapString(-2.7f,1.2f,GLUT_BITMAP_HELVETICA_18,"2. Use Right arrow key to move the shooter Right");
	renderBitmapString(-2.7f,1.0f,GLUT_BITMAP_HELVETICA_18,"3. Press Space to key to Shoot");
	renderBitmapString(-2.7f,0.8f,GLUT_BITMAP_HELVETICA_18,"4. Kill the object and 1 point will be added to the score");
	renderBitmapString(-2.7f,0.6f,GLUT_BITMAP_HELVETICA_18,"5. If object are missed and cross the shooter 1 life be lost");
	renderBitmapString(-2.7f,0.4f,GLUT_BITMAP_HELVETICA_18,"6. Press ESC to exit the game");
	renderBitmapString(-0.2f,-1.4f,GLUT_BITMAP_TIMES_ROMAN_24,"BACK");
}


void showAbout(){
    glColor3ub(255,255,255);
    renderBitmapString(-0.2f,1.7f,GLUT_BITMAP_TIMES_ROMAN_24,"ABOUT");
    renderBitmapString(-0.9f,0.7f,GLUT_BITMAP_TIMES_ROMAN_24,"The game is Designed and Developed by");
    renderBitmapString(-0.9f,0.3f,GLUT_BITMAP_HELVETICA_18,"Chowdhury, S. M. Habibul Mursaleen -  (16-32741-3)");
    renderBitmapString(-0.9f,0.0f,GLUT_BITMAP_HELVETICA_18,"Mostafa, Mayel -  (16-32721-3)");

    glBegin(GL_LINES);
        glVertex2f(-0.6f,-1.5f);
        glVertex2f(-0.6f,-1.2f);
        glVertex2f(-0.6f,-1.5f);
        glVertex2f(0.6f,-1.5f);
        glVertex2f(-0.6f,-1.2f);
        glVertex2f(0.6f,-1.2f);
        glVertex2f(0.6f,-1.5f);
        glVertex2f(0.6f,-1.2f);
	glEnd();

	renderBitmapString(-0.2f,-1.4f,GLUT_BITMAP_TIMES_ROMAN_24,"BACK");
}

void handleResize(int w, int h) {
	int winw=w;
	int winh=h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)w / (double)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y){
	if (key == 113) { //game will be closed if q pressed //q==113
		exit(0);
	}
	else if(key==' ' && on>=1000){  //if space is pressed on=1000 means intensity of bullet
                                    //higher value means low intensity
		int i;
		for(i=0;i<high;i++){
			if(bullet_stat[i]==0){
				bullet_x[i]=shooter_x; ///bullet x position will be same as guns x position
				bullet_y[i]=shooter_y; ///bullet y position will be same as guns y position

				tip_x[i]=bullet_x[i]; ///minimum range between bullet and enemy for
				tip_y[i]=bullet_y[i]+0.2; ///perfect shotting
				bullet_vel_x[i]=0; ///set zero as bullet will be fired on y axis only
				bullet_vel_y[i]=bullet_vel;///bullet position will be changed to y axis
				bullet_stat[i]=1;
				bullet=1;
               break; ///so that one bullet can kill one enemy at a time
                      ///it wont overlap
			}
		}
            if(bullet==0){
			high++;
			bullet_x[high]=shooter_x;
			bullet_y[high]=shooter_y;
			tip_x[high]=shooter_x;
			tip_y[high]=shooter_y+0.2;
			bullet_vel_x[high]=0;
			bullet_vel_y[high]=bullet_vel;
			bullet_stat[high]=1;
		}
		on=0;
		bullet=0;
	}
}


void getCoord( int x, int y){
	GLint viewport[8];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	winZ=0;
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	mousex=posX*100/2;
	mousey=(posY*100)/2;
	glutPostRedisplay();
}

void handleKeypress2(int key, int x, int y){
    if (key == GLUT_KEY_LEFT){
        if(shooter_x>shooter_min_x){
            shooter_x-=0.04;  ///left movement of gun
        }
    }
    else if (key == GLUT_KEY_RIGHT){
        if(shooter_x<shooter_max_x){
            shooter_x+=0.04;///right  movement of gun
        }
    }
}

void handleMouseclick(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && game_play==0 && mainmenu==1){
        getCoord(x,y);
        if(mousex>=0.3 && mousex<=1.5 && mousey>=0.7 && mousey<=1.0){
            game_play=1;
            game_over=0;
        }

        else if(mousex>=0.3 && mousex<=1.5 && mousey>=0.3 && mousey<=0.6){
            mainmenu=0;
            high_score=1;
        }

        else if(mousex>=0.3 && mousex<=1.5 && mousey>=-0.1 && mousey<=0.2){
            mainmenu=0;
            howtoplay=1;
        }

        else if(mousex>=0.3 && mousex<=1.5 && mousey>=-0.5 && mousey<=-0.2){
            mainmenu=0;
            about=1;
        }
        else if(mousex>=0.3 && mousex<=1.5 && mousey>=-0.9 && mousey<=-0.6){
            exit(0);
        }
    }

    else if (button == GLUT_LEFT_BUTTON && game_play==0 && mainmenu==0 && high_score==1){
        getCoord(x,y);
        if(mousex>=-0.6 && mousex<=0.6 && mousey>=-0.5 && mousey<=-0.2){
            mainmenu=1;
            high_score=0;
        }
    }

    else if (button == GLUT_LEFT_BUTTON && game_play==0 && mainmenu==0 && howtoplay==1){
        getCoord(x,y);
        if(mousex>=-0.6 && mousex<=0.6 && mousey>=-1.5 && mousey<=-1.2){
            mainmenu=1;
            howtoplay=0;
        }
    }

    else if (button == GLUT_LEFT_BUTTON && game_play==0 && mainmenu==0 && about==1){
        getCoord(x,y);
        if(mousex>=-0.6 && mousex<=0.6 && mousey>=-1.5 && mousey<=-1.2){
            mainmenu=1;
            about=0;
        }
    }

    else if (button == GLUT_LEFT_BUTTON && game_play==0 && mainmenu==0 && game_over==1){
        getCoord(x,y);
        if(mousex>=-0.6 && mousex<=0.6 && mousey>=-0.5 && mousey<=-0.2){
            mainmenu=1;
            game_over=0;
            storeHighScore();
            life = 3;
            points=0;

        }
    }
}

void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(game_play==0 && mainmenu==1){
		glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f,0.0f,0.0f);
            MainMenu();
		glPopMatrix();
	}

	else if(game_play==0 && mainmenu==0 && game_over==1){
		glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f,0.0f,0.0f);
            gameOver();
		glPopMatrix();
	}

	else if(mainmenu==0 && high_score==1 && game_play==0){
        glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f,0.0f,0.0f);
            showHighScore();
		glPopMatrix();
	}


	else if(mainmenu==0 && howtoplay==1 && game_play==0){
        glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f,0.0f,0.0f);
            howToPlay();
		glPopMatrix();
	}

	else if(mainmenu==0 && about==1 && game_play==0){
        glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f,0.0f,0.0f);
            showAbout();
		glPopMatrix();
	}

	else if(mainmenu==0 && game_play==0 && game_over==1){
        glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f,0.0f,0.0f);
            gameOver();
		glPopMatrix();
	}

	else if(game_over==0 && game_play==1){
		drawenemy();
		glPushMatrix();
            glTranslatef(0.0f, 0.0f,-5.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            for(int i=0;i<=high;i++){
                if(bullet_stat[i]==1){
                    glPushMatrix();
                        glTranslatef(bullet_x[i],bullet_y[i],0.0f);
                        glRotatef(0.0f,0.0f,0.0f,1.0f);
                        glColor3f(0.0f,0.0f,0.0f);
                        drawBullet();
                    glPopMatrix();
                }
            }

            glPushMatrix();
                glTranslatef(shooter_x,shooter_y,0.0f);
                drawShooter();
            glPopMatrix();

            for(int i=0;i<=frontCount;i++){
                glPushMatrix();
                glTranslatef(enemy_x[i],enemy_y[i],0.0f);
                if(enemy_vel[i]!=0)
                    drawenemy(i);
                glPopMatrix();
            }
            glPushMatrix();
                glColor3f(1.0f,0.0f,0.0f);
                score();
                showLife();
            glPopMatrix();
        }
        else{
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, -5.0f);
                glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
            glPushMatrix();
                glColor3f(1.0f,0.0f,0.0f);
            glPopMatrix();
        }
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1200,800);
	glutInitWindowPosition(220,50);
	glutCreateWindow("Enemy Shooting Game");
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutReshapeFunc(handleResize);
	glutTimerFunc(5, new_update, 0);
	glutMainLoop();
	return 0;
}
