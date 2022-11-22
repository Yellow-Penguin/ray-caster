#include <GL/glut.h>
#include <math.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
// Player position
float playerPosX, playerPosY, playerDirectionX, playerDirectionY, playerAngle, stepSize;

int blockSize = 32;
int map[MAP_WIDTH][MAP_HEIGHT] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void drawMap2D()
{
	int x, y, offsetX, offsetY;
	for (y = 0; y < MAP_WIDTH; ++y) {
		for (x = 0; x < MAP_HEIGHT; ++x) {
			if (map[y][x] == 1) {glColor3f(1, 1, 0);} else {glColor3f(0, 0, 0);}
			offsetX = x * blockSize;
			offsetY = y * blockSize;
			glBegin(GL_QUADS);
			glVertex2i(offsetX + 1, offsetY + 1);
			glVertex2i(offsetX + 1, offsetY + blockSize - 1);
			glVertex2i(offsetX + blockSize - 1, offsetY + blockSize - 1);
			glVertex2i(offsetX + blockSize - 1, offsetY + 1);
			glEnd();
		}
	}
}

void drawPlayer() {
	glColor3f(1, 0, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(playerPosX, playerPosY);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(playerPosX, playerPosY);
	glVertex2i(playerPosX + playerDirectionX * 3, playerPosY + playerDirectionY * 3);
	glEnd();
}

void drawRays() {
	int mapX, mapY, stepX, stepY, hit;
	float rayAngle, deltaX, deltaY, sideX, sideY, rayDirectionX, rayDirectionY;
	
	hit = 0;
	
	mapX = (int) playerPosX;
	mapY = (int) playerPosY;
	
	rayAngle = playerAngle;
	rayDirectionX = cos(rayAngle) * blockSize;
	rayDirectionY = sin(rayAngle) * blockSize;
	
	deltaX = (rayDirectionX == 0) ? 1e30 : sqrt(1 + (rayDirectionY * rayDirectionY) / (rayDirectionX * rayDirectionX));
	deltaY = (rayDirectionY == 0) ? 1e30 : sqrt(1 + (rayDirectionX * rayDirectionX) / (rayDirectionY * rayDirectionY));
	
	if (rayDirectionX < 0) {
		stepX = -1;
		sideX = (playerPosX - mapX) * deltaX;
	} else {
		stepX = 1;	
		sideX = (mapX + blockSize - playerPosX) * deltaX;
	}
	
	if (rayDirectionY < 0) {
		stepY = -1;
		sideY = (playerPosY - mapY) * deltaY;
	} else {
		stepY = 1;	
		sideY = (mapY + blockSize - playerPosY) * deltaY;
	}
	printf("%d\t%d\n", mapX, mapY);
	printf("%f\t%f\n", deltaX, deltaY);
	printf("%f\t%f\n", rayDirectionX, rayDirectionY);
	printf("%f\t%f\n", sideX, sideY);
	printf("%d\n", map[15][7] == 1);
	//DDA
	while (hit == 0) {
		printf("%d\n", hit);
		if (sideX < sideY) {
			sideX += deltaX;
			mapX += stepX * blockSize;
		} else {
			sideY += deltaY;
			mapY += stepY * blockSize;
		}
		//printf("%d\t%d\t%f\t%f\n", mapY / blockSize - 1, mapX / blockSize - 1, sideX,sideY);
		if (map[ (int) (mapY / blockSize - 1)][ (int) (mapX / blockSize - 1)] == 1) {
			hit = 1;
		}
	}
}

void buttonsPressed(unsigned char key, int x, int y) {
	if (key == 'a') {
		playerAngle -= 0.1;
		if (playerAngle < 0) {playerAngle += 2 * M_PI;};
		playerDirectionX = cos(playerAngle) * stepSize;
		playerDirectionY = sin(playerAngle) * stepSize;
	}
	if (key == 'd'){
		playerAngle += 0.1;
		if (playerAngle > 2 * M_PI) {playerAngle -= 2 * M_PI;};
		playerDirectionX = cos(playerAngle) * stepSize;
		playerDirectionY = sin(playerAngle) * stepSize;
	}
	if (key == 'w'){playerPosX += playerDirectionX; playerPosY += playerDirectionY;}
	if (key == 's'){playerPosX -= playerDirectionX; playerPosY -= playerDirectionY;}
	glutPostRedisplay();
}

void display() {
 glClear(GL_COLOR_BUFFER_BIT);
 drawMap2D();
 drawPlayer();
 drawRays();
 glutSwapBuffers();
}

void init() {
	glClearColor(0.2, 0.2, 0.2, 0);
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	playerPosX = 256;
	playerPosY = 256;
	stepSize = 4;
	playerAngle = 1;
	playerDirectionX = cos(playerAngle) * stepSize;
	playerDirectionY = sin(playerAngle) * stepSize;
}

int main(int argc, char** argv) { 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
 glutCreateWindow("Raycaster");
 init();\
 glutDisplayFunc(display);
 glutKeyboardFunc(buttonsPressed);
 glutMainLoop();
 return 0;
}
