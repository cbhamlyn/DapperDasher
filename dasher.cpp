#include "raylib.h"
int main()
{
    // Window dimensions
    int win_w = 512;
    int win_h = 380;
    InitWindow(win_w, win_h, "Dapper Dasher");
 
    // Acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};
    // Jump velocity (pixels/sec)
    const int jumpVel{-600};
    // Determine if scarfy is in the air
    bool isInAir{};
   
    // Add in Scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // scarfyRec is the box on the sprite sheet highlighting the current frame
    Rectangle scarfyRec; 
    // width is the width of all the iterations of scarfy divided by the number of scarfy itertions (6)
    scarfyRec.width = scarfy.width / 6;
    // height is just the height of the row scarfy is on (in this case there's just the one row)
    scarfyRec.height = scarfy.height;
    // top left of scarfy is zero, zero
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    // scarfyPos is his position on the screen.
    Vector2 scarfyPos;
    // x is the width of the window divided by 2 (center of window) minus the width of scarfy divided by 2 (center of scarfy), this should be his left edge.
    scarfyPos.x = win_w / 2 - scarfyRec.width / 2;
    // y is the height of the window (again, going down from the top) minus the height of scarfy (to get his top edge, otherwise he'd be hanging from the bottom of the screen, off camera)
    scarfyPos.y = win_h - scarfyRec.height;
    // This is scarfy's rectangle's velocity
    int scarfy_vel{0};

    // Add in Nebula (hazard)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebRec {0.0, 0.0, nebula.width / 8, nebula.height / 8};
    Vector2 nebPos{win_w, win_h - nebRec.height};
    // nebula X velocity (pixels/second)
    int neb_vel{-200};

    // Scarfy's animation
    // animation frame
    int frame{0}; 
    // amount of time before we update the animation frame (1/12 of a second)
    const float updateTime{1.0 / 12.0};
    // Keep track of our running time (Delta value)
    float runningTime{0};

    // Nebula's animation
    int nebFrame{0};
    const float nebUpTime{1.0 / 12.0};
    float nebRunTime{0};

    SetTargetFPS(60); 
    while (!WindowShouldClose())
    {
        // Delta Time (time since last frame)
        const float dT{GetFrameTime()};
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
                scarfy_vel = 0;
                // if running time is greater than update time, we need to update the animation frame
                if (runningTime >= updateTime)
                {
                    // need to reset running time
                    runningTime = 0.0;
                    // update the scarfy frame
                    scarfyRec.x = frame * scarfyRec.width;
                    // update the frame counter
                    frame++;
                    // if we're on the 6th frame, we need to reset the frame counter to zero
                    if (frame > 5)
                    {
                        frame = 0;
                    }
                }
            }
            else
            // apply gravity
            {
                isInAir = true;
                scarfy_vel += gravity * dT;
            }
            // check for jumping
            if (IsKeyPressed(KEY_SPACE) && !isInAir)
            {
                scarfy_vel += jumpVel;
            }
            // update Scarfy and Nebula position
            scarfyPos.y += scarfy_vel * dT;
            nebPos.x += neb_vel * dT;
            // update running time
            runningTime += dT;
            nebRunTime += dT;

            // Nebula animation
            if (nebRunTime >= nebUpTime)
            {
                // need to reset running time
                nebRunTime = 0.0;
                // update the scarfy frame
                nebRec.x = nebFrame * nebRec.width;
                // update the frame counter
                nebFrame++;
                // if we're on the 6th frame, we need to reset the frame counter to zero
                if (nebFrame > 7)
                {
                    nebFrame = 0;
                }
            }
           // Draw Scarfy
            DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);
            // Draw Nebula/Hazard
            DrawTextureRec(nebula, nebRec, nebPos, WHITE);
       }

        // Game logic ends
        EndDrawing();
        
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}