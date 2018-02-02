#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "pool.h"
#include "trampoline.h"
#include "spikes.h"
#include "enemy.h"
#include "magnet.h"
#include "helper.h"
#include "enemytype.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

const int balls = 100;

Ball player;
Ground ground[10];
Pool pool[10];
Trampoline trampoline[10];
Spikes spike[10];
Enemy ball[balls];
Magnet magnet;
EnemyType etype[3];

// float eps = 0.3f;
int spikecnt;
int SPIKE_TIME = 120;
int spikeTime;
glm::mat4 initVP;
int magnetDir;
bool magnetActive;
int CHANGE_TIME = 600;
int ACTIVE_TIME = 300;
int cttimer;
int activetimer;
int jumps;
int MAX_JUMPS = 2;
int level;
int points;
int goal;
int timeLeft;
int grounds = 3;
int pools = 2;
int trampolines = 1;
int spikes = 1;
int plankProb;
bool dragging;
double posX, posY;
int INF = 1000;
int ballTime;
int last;
int MAP_LEFT = -13;
int MAP_RIGHT = 13;



float ball_ratio[] = {5, 2, 2};
// 5 2 2
// 4 2 2
// 3 2 2
// 2 2 2 
// 1 1 2
// 1 1 3
// 1 1 4
// 1 1 5



float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
		// clear the color and depth in the frame buffer
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use the loaded shader program
		// Don't change unless you know what you are doing
		glUseProgram (programID);

		// Eye - Location of camera. Don't change unless you are sure!!
		// glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
		// Target - Where is the camera looking at.  Don't change unless you are sure!!
		// glm::vec3 target (0, 0, 0);
		// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
		// glm::vec3 up (0, 1, 0);

		// Compute Camera matrix (view)
		// Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
		// Don't change unless you are sure!!
		Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

		// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
		// Don't change unless you are sure!!
		glm::mat4 VP = Matrices.projection * Matrices.view;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// For each model you render, since the MVP will be different (at least the M part)
		// Don't change unless you are sure!!
		glm::mat4 MVP;  // MVP = Projection * View * Model

		// Scene render
		for (int i=0 ; i<pools ; ++i)
			pool[i].draw(VP);

		for (int i=0 ; i<balls ; ++i)
			ball[i].draw(VP);

		for (int i=0 ; i<grounds ; ++i)
			ground[i].draw(VP);

		if (magnetActive)
			magnet.draw(initVP);
		
		for (int i=0 ; i<trampolines ; ++i)
			trampoline[i].draw(VP);

		for (int i=0 ; i<spikes ; ++i)
			spike[i].draw(VP);

		player.draw(VP);
}

glm::vec3 detectCollision(Ball player, Ground ground) {
		if (ground.x <= player.position.x && player.position.x <= ground.y && abs(player.position.y - PLAYER_SIZE + 1) < abs(player.speed.y))
				return glm::vec3(0, 1, 0);
		else if ((player.position.x - ground.x) * (player.position.x - ground.x) + (player.position.y - ground.h) * (player.position.y - ground.h) <= PLAYER_SIZE * PLAYER_SIZE)
				return glm::normalize(glm::vec3((player.position.x - ground.x), (player.position.y - ground.h), 0));
		else if ((player.position.x - ground.y) * (player.position.x - ground.y) + (player.position.y - ground.h) * (player.position.y - ground.h) <= PLAYER_SIZE * PLAYER_SIZE)
				return glm::normalize(glm::vec3((player.position.x - ground.y), (player.position.y - ground.h), 0));
		return glm::vec3(0, 0, -1);
}

glm::vec3 detectCollision(Ball player, Pool pool) {
		if (player.position.y < ground[0].h + PLAYER_SIZE && player.position.x <= pool.position.x + pool.r - PLAYER_SIZE && pool.position.x - pool.r + PLAYER_SIZE <= player.position.x)
				if ((player.position.x - pool.position.x) * (player.position.x - pool.position.x) + (player.position.y - pool.position.y) * (player.position.y - pool.position.y) >= (pool.r - PLAYER_SIZE) * (pool.r - PLAYER_SIZE))
						return glm::normalize(glm::vec3((pool.position.x - player.position.x), (pool.position.y - player.position.y), 0));
		return glm::vec3(0, 0, -1);
}

glm::vec3 detectCollision(Ball player, Trampoline trampoline) {
		if (player.speed.y <= 0 && trampoline.x <= player.position.x && player.position.x <= trampoline.y && abs(player.position.y - PLAYER_SIZE - trampoline.h) < abs(player.speed.y))
				return glm::vec3(0, 1, 0);
		return glm::vec3(0, 0, -1);
}

glm::vec3 detectCollision(Ball player, Spikes spike) {
		if (player.speed.y <= 0 && spike.position.x <= player.position.x && player.position.x <= spike.position.x + spike.width && abs((player.position.y - PLAYER_SIZE) - (spike.position.y + spike.h)) < abs(player.speed.y))
				return glm::vec3(0, 1, 0);
		return glm::vec3(0, 0, -1);
}

// ground
// pool
// porc
//// magnet

// speeds...

// 1 0 0 1 0
// 2 1 0 1 5
// 2 1 1 1 10
// 2 1 2 1 15

void generateLevel() {
	// -INF = ground = pool = ground = INF

	cerr << "Generating level " << level << "..." << endl;

	int poolSize = randomNum(2, 4);
	float halfPool = poolSize / 2.0f;

	if (grounds == 1)
		ground[0].newShape(-INF, (INF << 1));
	else if (grounds == 2 && pools == 1) {
		ground[0].newShape(-INF, INF - halfPool);
		pool[0].newShape(-halfPool, poolSize);
		ground[1].newShape(halfPool , INF - halfPool);
	}

	int spikeWidth = randomNum(4, 8);
	spike[0].newShape(-halfPool-spikeWidth-randomNum(0, 2), spikeWidth-3, spikeWidth, 0.001f * randomNum(1, 3));

	int spikeWidth2 = randomNum(4, 8);
	spike[1].newShape(halfPool + randomNum(0, 2), spikeWidth2-3, spikeWidth2, 0.001f * randomNum(1, 3));

	trampoline[0].newShape(-halfPool - 3 - randomNum(0, 2), randomNum(1, 3));
	trampoline[1].newShape(halfPool + randomNum(0, 2), randomNum(1, 3));
}

void setLevel(int newLevel) {
	level = newLevel;

	if (level > 3)
		ACTIVE_TIME = 300;
	else {
		magnetActive = 0;
		ACTIVE_TIME = 1;
	}

	grounds = (level <= 1 ? 1 : 2);
	pools = (level <= 1 ? 0 : 1);
	spikes = (level > 3 ? 2 : 0);
	if (level == 3)	spikes = 1;
	plankProb = min((level - 1) * 5, 80);
	trampolines = ((2 <= level && level <= 6) ? 1 : 0);
	if (4 <= level && level <= 5)
		trampolines = 2;

	points = 0;
	goal = 200 + (level*50);
	ball_ratio[0] = max(6.0f - level, 1.0f);
	ball_ratio[1] = (level >= 5 ? 1.0f : 2.0f);
	ball_ratio[2] = max(level - 3.0f, 2.0f);
	float total = 0;
	for (int i=0 ; i<3 ; ++i)
		total += ball_ratio[i];
	// cerr << "RATIOS : ";
	for (int i=0 ; i<3 ; ++i) {
		ball_ratio[i] /= total;
		// cerr << ball_ratio[i] << ' ';
	}
	// cerr << endl;
	generateLevel();
}

// 0
// 1
// 1
// 2
// 2
// 1
// 0
// 0
// 0
// 0
// 0
// 0

// void tick_level() {

// }

void tick_input(GLFWwindow *window) {
		double currX, currY;
		glfwGetCursorPos(window, &currX, &currY);

		int a = glfwGetKey(window, GLFW_KEY_A) || (dragging && currX - posX < -70);
		int d = glfwGetKey(window, GLFW_KEY_D) || (dragging && currX - posX > 70);
		int left  = glfwGetKey(window, GLFW_KEY_LEFT);
		int right = glfwGetKey(window, GLFW_KEY_RIGHT);

		if (dragging && posY - currY > 70)
			jump();

		if (a && !d)
			player.move('l', 0.004f);
		else if (d && !a)
			player.move('r', 0.004f);
		else if (magnetActive) {
			if (magnet.direction == 1)
				player.move('r', 0.001f);
			else
				player.move('l', 0.001f);
		}
		else
			player.move('s', 0.004f);

		setScreenX(max((float)screen_center_x, player.position.x - 2));
		setScreenX(min((float)screen_center_x, player.position.x + 2));

		if (left && !right)
				setScreenX(max((float)screen_center_x - 0.02f, player.position.x - 2));
		else if (right && !left)
				setScreenX(min((float)screen_center_x + 0.02f, player.position.x + 2));
		reset_screen();
}

void tick_elements() {
		ballTime--;
		if (ballTime == 0) {
			cerr << "THROW";
			throwBall();
		}
		if (points >= goal)
			setLevel(level+1);

		activetimer = (activetimer + 1) % ACTIVE_TIME;
		cttimer = (cttimer + 1) % CHANGE_TIME;

		if (cttimer == 0)
			magnet.changeDir(-1 * magnet.direction);

		if (activetimer == 1)
			magnetActive ^= 1;

		player.tick();
		for (int i=0 ; i<pools ; ++i)
			pool[i].tick();

		last = 0;
		for (int i=0 ; i<balls ; ++i) {
			ball[i].tick();
			if (ball[i].position.x > ball[last].position.x)
				last = i; 
		}

		for (int i=0 ; i<spikes ; ++i)
			spike[i].tick();

		int collided = 0;
		glm::vec3 normal;
		for (int i=0 ; i<grounds ; ++i) {
		glm::vec3 ret = detectCollision(player, ground[i]);
				if (ret.z == 0) {
						player.position.y = max(player.position.y, -0.8f);
						collided = 1;
						normal = ret;
				}
		}

		bool inside = (player.position.y < ground[0].h + PLAYER_SIZE);
		if (!collided) {
				for (int i=0 ; i<pools ; ++i) {
						glm::vec3 ret = detectCollision(player, pool[i]);
						if (ret.z == 0) {
								player.position = pool[i].position - ((pool[i].r - PLAYER_SIZE) * ret);
								collided = 1;
								normal = ret;
						}
				}
		}

		glm::vec3 boost = glm::vec3(1, 1, 0);
		if (!collided) {
				for (int i=0 ; i<trampolines ; ++i) {
						glm::vec3 ret = detectCollision(player, trampoline[i]);
						if (ret.z == 0) {
							player.position.y = max(player.position.y, trampoline[i].h);
							collided = 1;
							boost = glm::vec3(1, min(4.0f, -0.6f/player.speed.y), 0);
							normal = ret;
						}
				}
		}

		int spiked = 0;
		if (!collided) {
				for (int i=0 ; i<spikes ; ++i) {
						glm::vec3 ret = detectCollision(player, spike[i]);
						if (ret.z == 0) {
							// player.position.y = max(player.position.y, trampoline[i].h);
							collided = 1;
							spiked = 1;
							// boost = glm::vec3(1, min(4.0f, -0.6f/player.speed.y), 0);
							normal = ret;
						}
				}
		}

		if (spiked && spikeTime == 0) {
			spikeTime = SPIKE_TIME;
			// spiked();
			cerr << "SPIKE! " << spikecnt++;
		}
		else if (spikeTime)
			spikeTime--;


		if (!collided) {
				for (int i=0 ; i<balls ; ++i) {
						glm::vec3 ret = ball[i].detectCollision(player.position, 0.2f, player.speed);
						if (ret.z == 0) {
							cerr << "POINTS = " << points << endl;
							if (!ball[i].hasPlank || ret.x == 0) {	
								points += 20;
								ball[i].position.x = INF;
							}
							collided = 1;
							boost = glm::vec3(1, 1.0/0.3f, 0);
							normal = ret;
						}
				}

		}

		if (player.inside && !inside)
			player.speed = glm::vec3(0.5, 0.5, 0) * player.speed;

		player.inside = inside;

		if (inside)
			jumps = 0;
		else
			jumps = max(jumps, 1);

		if (collided) {
			jumps = 0;
			if (inside)
				player.speed = glm::reflect(player.speed, normal) - 0.9f * glm::dot(-player.speed, normal) * normal;
			else
				player.speed = boost * glm::vec3(1, 0.3, 0) * glm::reflect(player.speed, normal);
		}
		else
			jumps = max(jumps, 1);
}

void throwBall() {
	ballTime = max(randomNum(1, 50), randomNum(1, 50));
	float rnd = randomNum(0, 999) / 1000.0f;
	int ty = 0;
	for (int i=0 ; i<3 ; ++i) {
		if (rnd > ball_ratio[i])
			rnd -= ball_ratio[i];
		else {
			ty = i;
			break;
		}
	}
	float y, speed, sz;
	color_t color;
	etype[ty].getVals(y, sz, speed, color);
	ball[last].newShape(screen_center_x - 7, y, color, sz, speed, randomNum(1, 100) <= plankProb, randomNum(-45, 45));
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
		/* Objects should be created before any other gl function and shaders */

		// Initialise State
		spikecnt = 1;
		ballTime = 0;
		level = 1;
		points = 0;
		goal = 250;
		ACTIVE_TIME = 1;
		ballTime = 5;

		etype[0] = EnemyType(-0.8, 1, 0.25f, 0.3f, 0.005f, 0.01f, COLOR_YELLOW);
		etype[1] = EnemyType(1, 2.5, 0.2f, 0.3f, 0.015f, 0.025f, COLOR_PINK);
		etype[2] = EnemyType(2, 4, 0.2f, 0.25f, 0.02f, 0.04f, COLOR_BLACK);

		// for (int i=0 ; i<3 ; ++i)
		// 	etype[i].print();

		initVP = Matrices.projection * Matrices.view;

		// Create the models

		player = Ball(0, 0, COLOR_RED);

		for (int i=0 ; i<10 ; ++i) {
			ground[i] = Ground(0, 0);
			pool[i] = Pool(0, 0);
			trampoline[i] = Trampoline(0, 0);
			spike[i] = Spikes(0, 0, 0, 0);
		}

		// ground[0] = Ground(-100, 90);
		// pool[0] = Pool(-10, 3);
		// ground[1] = Ground(-7, 10);
		// pool[1] = Pool(3, 4);
		// trampoline[0] = Trampoline(13, 2);
		// trampoline[1] = Trampoline(-INF, 2);
		// ground[2] = Ground(7, 100);
		// spike[0] = Spikes(7, 3, 4, 0.4f);
		// spike[1] = Spikes(-INF, 3, 4, 0.4f);
		magnet = Magnet(1);


		for (int i=0 ; i<balls ; ++i) {
			ball[i] = Enemy(INF, 0, COLOR_RED, 0, 0, 0, 0);
		}

		setLevel(4);

		// Create and compile our GLSL program from the shaders
		programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
		// Get a handle for our "MVP" uniform
		Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


		reshapeWindow (window, width, height);

	//   // Enable blending
		// glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Background color of the scene
		glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
		glClearDepth (1.0f);

		glEnable (GL_DEPTH_TEST);
		glDepthFunc (GL_LEQUAL);

		cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
		cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
		cout << "VERSION: " << glGetString(GL_VERSION) << endl;
		cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
		srand(time(0));
		int width  = 600;
		int height = 600;

		window = initGLFW(width, height);

		initGL (window, width, height);

		/* Draw in loop */
		while (!glfwWindowShouldClose(window)) {
				// Process timers

				if (t60.processTick()) {
						// 60 fps
						draw();
						// OpenGL Draw commands
						// Swap Frame Buffer in double buffering
						glfwSwapBuffers(window);

						tick_elements();
						tick_input(window);
				}

				// Poll for Keyboard and mouse events
				glfwPollEvents();
		}

		quit(window);
}

void setInitialMousePos(double xpos, double ypos, bool clicked) {
	posX = xpos;
	posY = ypos;
	dragging = clicked;
}


void reset_screen() {
		float top    = screen_center_y + 4 / screen_zoom;
		float bottom = screen_center_y - 4 / screen_zoom;
		float left   = screen_center_x - 4 / screen_zoom;
		float right  = screen_center_x + 4 / screen_zoom;
		Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

bool jump() {
	if (jumps < MAX_JUMPS) {
		jumps++;
		player.jump();
	}
}

void translateBalls(float offset) {
	for (int i=0 ; i<balls ; ++i)
		ball[i].position.x += offset;
	player.position.x += offset;
}

void setScreenX(float posX) {
	cerr << screen_center_x << " = ";
	screen_center_x = posX;
	int MAP_SIZE = MAP_RIGHT - MAP_LEFT;
	if (posX > MAP_RIGHT) {
		screen_center_x -= MAP_SIZE;
		translateBalls(-MAP_SIZE);
	}
	else if (posX < MAP_LEFT) {
		screen_center_x += MAP_SIZE;
		translateBalls(MAP_SIZE);
	}
	cerr << screen_center_x << endl;
}

void scroll(int xoffset, int yoffset) {
		if (yoffset == 1)
				screen_zoom = min(screen_zoom + 0.02, 1.1);
		else if (yoffset == -1)
				screen_zoom = max(screen_zoom - 0.02, 0.7);
		if (xoffset == 1)
			setScreenX(max((float)screen_center_x - 0.02f, player.position.x - 2));
		else if (xoffset == -1)
			setScreenX(min((float)screen_center_x + 0.02f, player.position.x + 2));

		reset_screen();
}