#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;
const int rectangleWidth = 10;
const int rectangleHeight = 100;
const Color screenBackgroundColor = BLACK;
const Color entityColor = DARKGRAY;
const char* winnerText = nullptr;
const char* textHint = nullptr;

struct Ball
{
    float x, y;
    float speedX, speedY;
    float radius;

    void Draw()
    {
        DrawCircle((int)x, (int)y, radius, entityColor);
    }
};

struct Paddle
{
    float x, y;
    float speed;
    float width, height;

    Rectangle GetRect()
    {
        return Rectangle{ x - width / 2, y - height / 2, rectangleWidth, rectangleHeight };
    }

    void Draw()
    {
        DrawRectangleRec(GetRect(), entityColor);
    }
};

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Retro Game:Pong!");
    
    SetTargetFPS(60);

    Ball ball;
    ball.x = GetScreenWidth() / 2.0f;
    ball.y = GetScreenHeight() / 2.0f;
    ball.radius = 5;
    ball.speedX = 300;
    ball.speedY = 300;

    Paddle leftPaddle;
    leftPaddle.x = 50;
    leftPaddle.y = GetScreenHeight() / 2;
    leftPaddle.width = 10;
    leftPaddle.height = 100;
    leftPaddle.speed = 500;

    Paddle rightPaddle;
    rightPaddle.x = GetScreenWidth() - 50;
    rightPaddle.y = GetScreenHeight() / 2;
    rightPaddle.width = 10;
    rightPaddle.height = 100;
    rightPaddle.speed = 500;

    while (!WindowShouldClose())
    {
        ball.x += ball.speedX * GetFrameTime();
        ball.y += ball.speedY * GetFrameTime();

        if (ball.y < 0)
        {
            ball.y = 0;
            ball.speedY *= -1;
        }

        // Check if ball is offset of the screen height and bounce it in the oposite direction
        if (ball.y > GetScreenHeight())
        {
            ball.y = GetScreenHeight();
            ball.speedY *= -1;
        }

        // move the paddles
        if (IsKeyDown(KEY_W))
        {
            leftPaddle.y -= leftPaddle.speed * GetFrameTime();
        }

        if (IsKeyDown(KEY_S))
        {
            leftPaddle.y += leftPaddle.speed * GetFrameTime();
        }

        if (IsKeyDown(KEY_UP))
        {
            rightPaddle.y -= rightPaddle.speed * GetFrameTime();
        }

        if (IsKeyDown(KEY_DOWN))
        {
            rightPaddle.y += rightPaddle.speed * GetFrameTime();
        }

        // Check collisions and bounce the ball in the oposite direction
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
        {
            // If ball speed is going in negative, reverse it
            if (ball.speedX < 0)
            {
                ball.speedX *= -1.1f;
                ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
            }
        }

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
        {
            // Same we did before in the other if statement, but in the oposite direction
            if (ball.speedX > 0)
            {
                ball.speedX *= -1.1f;
                ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
            }
        }

        if (ball.x < 0)
        {
            winnerText = "Player 2 wins!";
        }

        if (ball.x > GetScreenWidth())
        {
            winnerText = "Player 1 wins!";
        }

        if (winnerText)
        {
            textHint = " Press SPACEBAR to play again.";
        }

        if (winnerText && IsKeyPressed(KEY_SPACE))
        {
            ball.x = GetScreenWidth() / 2;
            ball.y = GetScreenHeight() / 2;
            ball.speedX = 300;
            ball.speedY = 300;
            winnerText = nullptr;
        }

        BeginDrawing();
        ClearBackground(screenBackgroundColor);
        ball.Draw();
        leftPaddle.Draw();
        rightPaddle.Draw();

        if (winnerText)
        {
            int textWidth = MeasureText(winnerText, 60);
            DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, WHITE);

            int textHintWidth = MeasureText(textHint, 18);
            DrawText(textHint, GetScreenWidth() / 2 - (-10) - textHintWidth / 2, GetScreenHeight() / 2 - (-40), 18, WHITE);
        }

       
        EndDrawing();
    }

    CloseWindow();

    return 0;
}