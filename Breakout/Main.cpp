#include "SDL.h"
#include "SDL_ttf.h"
#include "cmath"
#include "cstdlib"
#include "ctime"
#include "sstream"
#include "iostream"
#include "windows.h"

using namespace std;

SDL_Window *windows;
SDL_Renderer *renderer;
SDL_Surface *PlayerPaddle;
SDL_Surface *Ball;
SDL_Rect Paddle_Rect;
SDL_Rect Ball_Rect;
SDL_Rect Bricks_Rect[40];
SDL_Rect Level_Rect;
SDL_Color red = { 255,0,0 };
SDL_Color blue = { 0,0,255 };
SDL_Color green = { 0,255,0 };
SDL_Color white = { 255,255,255 };
TTF_Font *times;
int level = 1, score = 0, a = 0;
int bricksnumber;
int bricksleft = 0;
int brick_i;
int xvel = -1, yvel = -1;
int brickcount[40];
int brickcolor[40];
bool brickstate[40];
bool running = true;
bool winner = false;
const int Paddle_Width = 100;
const int Paddle_Height = 20;
const int Ball_Width = 20;
const int Ball_Height = 20;

void SetUp() {
	Paddle_Rect.x = 350;
	Paddle_Rect.y = 560;
	Paddle_Rect.w = Paddle_Width;
	Paddle_Rect.h = Paddle_Height;

	Ball_Rect.x = 370;
	Ball_Rect.y = 540;
	Ball_Rect.h = Ball_Height;
	Ball_Rect.w = Ball_Width;

	int m = 45, n = 80;

	for (int i = 0; i < 40; ++i) {
		int random = rand() % 3;
		brickstate[i] = true;
		brickcolor[i] = random;
		Bricks_Rect[i].h = 20;
		Bricks_Rect[i].w = 75;
		if (m + Bricks_Rect[i].w < 798)
		{
			Bricks_Rect[i].x = m ;
			Bricks_Rect[i].y = n;
			m = m + 80;
		}
		else {
			n = n + 21;
			m = 45;
			Bricks_Rect[i].x = m;
			Bricks_Rect[i].y = n;
			m = m + 80;
		}
	}
	for (int i = 0; i < 40; ++i) {
		if (brickcolor[i] == 0)
		{
			brickcount[i] = 1;
		}
		else if (brickcolor[i] == 1)
		{
			brickcount[i] = 2;
		}
		else
		{
			brickcount[i] = 3;
		}
	}
	
}

void BallMove() {
	Ball_Rect.x += xvel;
	Ball_Rect.y += yvel;
	SDL_Delay(2);
}

void BrickMove() {
	int n = 21;
	int m = 45;
	for (int i = 0; i < 40; ++i) {
		if (brickstate[i] == true) {
			if (m + Bricks_Rect[i].w < 798)
			{
				Bricks_Rect[i].y += a ;
			}
			else {
				n = n + 21 ;
				Bricks_Rect[i].y += a;
			}
		}
	}
	a = a + 2;
	}

void DrawScore(){
	stringstream score;
	score << bricksleft;
	SDL_Surface *Score_Sur = TTF_RenderText_Solid(times, score.str().c_str(), white);
	SDL_Texture *Score_Text = SDL_CreateTextureFromSurface(renderer, Score_Sur);
	SDL_Rect Score_Rect = { 550,0,20,40 };
	SDL_RenderCopy(renderer, Score_Text, NULL, &Score_Rect);
	SDL_FreeSurface(Score_Sur);
	SDL_DestroyTexture(Score_Text);
	//SDL_RenderPresent(renderer);
}

void DrawLevel() {
	stringstream slevel;
	slevel << level;
	SDL_Surface *Level_Sur = TTF_RenderText_Solid(times, slevel.str().c_str(),white);
	SDL_Texture *Level_Text = SDL_CreateTextureFromSurface(renderer, Level_Sur);
	SDL_Rect Level_Rect = { 30,0,20,40 };
	SDL_RenderCopy(renderer, Level_Text, NULL, &Level_Rect);
	SDL_FreeSurface(Level_Sur);
	SDL_DestroyTexture(Level_Text);

}

void PrintResult() {
	/*SDL_DestroyRenderer(renderer);
	renderer = SDL_CreateRenderer(windows, -1, SDL_RENDERER_ACCELERATED);*/
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	////SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Surface *Result_Sur = NULL;
	SDL_Texture *Result_Text = NULL;
	SDL_Rect Result_Rect = { 250,260,300,80 };
	SDL_RenderDrawRect(renderer, &Result_Rect);
	Result_Sur = TTF_RenderText_Solid(times, "You Win !!", white);
	Result_Text = SDL_CreateTextureFromSurface(renderer, Result_Sur);
	SDL_RenderCopy(renderer, Result_Text, NULL, &Result_Rect);
	SDL_RenderPresent(renderer);
	SDL_FreeSurface(Result_Sur);
	SDL_DestroyTexture(Result_Text);
}

bool BallinRect(int x, int y, SDL_Rect rect) {
		if (x >= rect.x &&
			x <= rect.x + rect.w &&
			y >= rect.y &&
			y <= rect.y + rect.h
			)
		{
			return true;
		}

		return false;
	}

bool CheckCollision(SDL_Rect r1, SDL_Rect r2)
{
	if (BallinRect(r1.x, r1.y, r2) == true ||
		BallinRect(r1.x + r1.w, r1.y, r2) == true ||
		BallinRect(r1.x, r1.y + r1.h, r2) == true ||
		BallinRect(r1.x + r1.w, r1.y + r1.w, r2) == true)
		return true;
	return false;

}

void Collision() {
	if (Ball_Rect.x < 1) {
		Ball_Rect.x = 1;
		xvel = -xvel;
	}
	if (Ball_Rect.x + Ball_Rect.w > 799) {
		Ball_Rect.x = 799 - Ball_Rect.w;
		xvel = -xvel;
	}
	if (Ball_Rect.y < 1) {
		Ball_Rect.y = 1;
		yvel = -yvel;
	}
	if (Ball_Rect.y + Ball_Rect.h > 599) { //sua lai
		Ball_Rect.y = 599 - Ball_Rect.h;
		yvel = -yvel;
	}
	//check paddle
	if (CheckCollision(Ball_Rect, Paddle_Rect) == 1)
	{
		yvel = -yvel;
		Ball_Rect.y = Paddle_Rect.y - Ball_Rect.h;
	}
	int count[40];
	for (int i = 0; i < 40; ++i) {
		count[i] = 0;
	}
	for (int i = 0; i < 40; ++i) {
		if (brickstate[i] == true) {
			if (CheckCollision(Ball_Rect, Bricks_Rect[i]) == 1) {
				brickcount[i] = brickcount[i] - 1;
				if (brickcount[i] == 2) {
					brickcolor[i] = 1;
				}
				else if (brickcount[i] == 1) {
					brickcolor[i] = 0;
				}
				yvel = -yvel;
				if (yvel == 0) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR", "yvel = 0", windows);
				//xvel = xvel;
				//Ball_Rect.y -= 1;
				if (brickcount[i] == 0) {
					brickstate[i] = false;
					bricksleft += 1;
				}
				if (bricksleft % 3 == 0) {
					//BrickMove();
				}
			}
		}
	}
}

void PaddleMove() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RIGHT]) Paddle_Rect.x += 1;
	if (state[SDL_SCANCODE_LEFT]) Paddle_Rect.x -= 1;
	if (Paddle_Rect.x < 1) Paddle_Rect.x = 1;
	if (Paddle_Rect.x + Paddle_Rect.w > 799)
	{
		Paddle_Rect.x = 799 - Paddle_Rect.w;
	}
}

void SetPaddleX(int a) {
	int newx;
	if (a < 1) {
		newx = 1;
	}
	else if (a + Paddle_Rect.w > 799) {
		newx = 799 - Paddle_Rect.w;
	}
	else newx = a;
	Paddle_Rect.x = newx;
}

void PaddleMoveByMouse() {
	int mousex, mousey;
	SDL_GetMouseState(&mousex, &mousey);
	SetPaddleX(mousex + Paddle_Rect.w / 2);
}

void LoadGame(){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	windows = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(windows, -1, SDL_RENDERER_ACCELERATED);
	SetUp();
	times = TTF_OpenFont("times.ttf", 15);
}

void DrawScreen() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &Paddle_Rect);
	SDL_RenderFillRect(renderer, &Ball_Rect);
	for (int i = 0; i < 40; ++i) {
		if (brickstate[i] == true) {
			if (brickcolor[i] == 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else if (brickcolor[i] == 1)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}
			SDL_RenderFillRect(renderer, &Bricks_Rect[i]);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(renderer, &Bricks_Rect[i]);

		}
	}
	DrawScore();
	DrawLevel();
	SDL_RenderPresent(renderer);
}

void Logic()
{
	BallMove();
	Collision();
	if (bricksleft == 40) {
		winner = true;
	}
	PaddleMoveByMouse();
}

void Quit()
{
	SDL_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(windows);
}

int main(int agrc, char *agrs[])
{
	SDL_Event occur;
	LoadGame();
	while (running == true) {
		SDL_PollEvent(&occur);
		if (occur.type == SDL_QUIT) {
			running = false;
			Quit();
		}
		else if (occur.type == SDL_KEYDOWN) {
			if (occur.key.keysym.sym == SDLK_SPACE) {
				Quit();
			}
		}
		Logic();
		if (winner == false) {
			//running = false;
			DrawScreen();
		}
		else {
			PrintResult();
		}
	}
	//system("pause");
	return 0;
}
