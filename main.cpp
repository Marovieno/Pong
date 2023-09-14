#include "raylib.h"
#include <ctime>
#include <iostream>

int player1score = 0;
int player2score = 0;
Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };
const char* winnerText = nullptr;

class Ball
{
	public:
		float x, y;
		float speedX, speedY;
		float radius;

		void Draw()
		{
			DrawCircle((int) x, (int) y, radius, Yellow);
		}

		void ResetBall()
		{
			if (player1score < 5 && player2score < 5)
			{ 
				x = GetScreenWidth() / 2;
				y = GetScreenHeight() / 2;
				int random[] = { -1, 1, -1, 1 };
				srand(time(NULL));
				int ranIndex = rand() % 4;
				speedX = 300 * random[ranIndex];
				speedY = 300 * random[ranIndex];
				winnerText = nullptr;
			}

			if (winnerText && IsKeyPressed(KEY_SPACE))
			{
				x = GetScreenWidth() / 2;
				y = GetScreenHeight() / 2;
				int random[] = { -1, 1, -1, 1 };
				srand(time(NULL));
				int ranIndex = rand() % 4;
				speedX = 300 * random[ranIndex];
				speedY = 300 * random[ranIndex];
				winnerText = nullptr;
				player1score = 0;
				player2score = 0;
			} 
			
		}
};

class Paddle
{
	public:
		float x, y;
		float speed;
		float width, height;
		Rectangle GetRect()
		{
			return Rectangle{ x - width / 2, y - height / 2, 15, 150 };
		}

		void Draw()
		{
			DrawRectangleRounded(GetRect(), 0.8, 0, WHITE);
		}
};

int main() {
	InitWindow(1000, 600, "PING PONG");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 10;
	int random[] = { -1, 1, -1, 1 };
	srand(time(NULL));
	int ranIndex = rand() % 4;
	ball.speedX = 300 * random[ranIndex];
	ball.speedY = 300 * random[ranIndex];
	
	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 800;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 800;

	const char* tieText = nullptr;

	while (!WindowShouldClose())
	{
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}
		if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}

		if (ball.x + ball.radius >= GetScreenWidth())
		{
			ball.ResetBall();
			player1score++;
		}

		if (ball.x - ball.radius <= 0)
		{
			ball.ResetBall();
			player2score++;
		}

		if (IsKeyDown(KEY_W))
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S))
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_PAGE_UP))
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_PAGE_DOWN))
		{
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (leftPaddle.y - leftPaddle.height / 2 <= 0)
		{
			leftPaddle.y = leftPaddle.height / 2;
		}
		if ((leftPaddle.y + leftPaddle.height) >= GetScreenHeight())
		{
			leftPaddle.y = GetScreenHeight() - leftPaddle.height;
		}

		if (rightPaddle.y - rightPaddle.height / 2 <= 0)
		{
			rightPaddle.y = rightPaddle.height / 2;
		}
		if ((rightPaddle.y + rightPaddle.height) >= GetScreenHeight())
		{
			rightPaddle.y = GetScreenHeight() - rightPaddle.height;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		if (player1score == 5)
		{
			winnerText = "Player 1 Wins!";
			ball.speedX = 0;
			ball.speedY = 0;
			ball.ResetBall();
		}
		if (player2score == 5)
		{
			winnerText = "Player 2 Wins!";
			ball.speedX = 0;
			ball.speedY = 0;
			ball.ResetBall();
		}

		if (winnerText && IsKeyPressed(KEY_SPACE))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			int random[] = { -1, 1, -1, 1 };
			srand(time(NULL));
			int ranIndex = rand() % 4;
			ball.speedX = 300 * random[ranIndex];
			ball.speedY = 300 * random[ranIndex];
			winnerText = nullptr;
		}

		BeginDrawing();
			ClearBackground(Dark_Green);
			DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), Green);
			DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 200, Light_Green);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();
			DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);
			DrawText(TextFormat("%i", player1score), GetScreenWidth() / 4 - 20, 20, 80, WHITE);
			DrawText(TextFormat("%i", player2score), 3 * GetScreenWidth() / 4 - 20, 20, 80, WHITE);

			if (winnerText)
			{
				int textWidth = MeasureText(winnerText, 45);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 45, YELLOW);
			}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
