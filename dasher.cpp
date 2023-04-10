#include "raylib.h"
int main()
{
    // Window dimensions
    int win_w = 512;
    int win_h = 380;
    InitWindow(win_w, win_h, "Dapper Dasher");
 
    // Acceleration due to gravity (pixels/frame)/frame
    const int gravity{1};
    const int jumpVel{-22};
    bool isInAir{};
   
    // Add in Scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec; 
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = win_w/2 - scarfyRec.width/2;
    scarfyPos.y = win_h - scarfyRec.height;

    int rec_vel{0};

    SetTargetFPS(60); 
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        if (false)
        {
            DrawText("Game Over!", 400, 200, 20, RED);
        }
        else
        {
            // Game logic starts
            // check for ground
            if (scarfyPos.y >= win_h - scarfyRec.height)
            {
                isInAir = false;
                rec_vel = 0;
            }
            else
            // apply gravity
            {
                isInAir = true;
                rec_vel += gravity;
            }
            // check for jumping
            if (IsKeyPressed(KEY_SPACE) && !isInAir)
            {
                rec_vel += jumpVel;
            }
            
            // update position
            scarfyPos.y += rec_vel;

            DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);
       }

        // Game logic ends
        EndDrawing();
        
    }
    UnloadTexture(scarfy);
    CloseWindow();
}