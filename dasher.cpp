#include "raylib.h"

// Animation Data Class
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};
    

int main()
{
    // Window dimensions
    int winDim[2];
    winDim[0] = 512;
    winDim[1] = 380;
    InitWindow(winDim[0], winDim[1], "Dapper Dasher");
 
    // Acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};
    // Jump velocity (pixels/sec)
    const int jumpVel{-600};
    // Determine if scarfy is in the air
    bool isInAir{};
   
    // Add in Scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    // scarfyRec is the box on the sprite sheet highlighting the current frame
    // width is the width of all the iterations of scarfy divided by the number of scarfy itertions (6)
    scarfyData.rec.width = scarfy.width / 6;
    // height is just the height of the row scarfy is on (in this case there's just the one row)
    scarfyData.rec.height = scarfy.height;
    // top left of scarfy is zero, zero
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    // scarfyPos is his position on the screen.
    // x is the width of the window divided by 2 (center of window) minus the width of scarfy divided by 2 (center of scarfy), this should be his left edge.
    scarfyData.pos.x = winDim[0] / 2 - scarfyData.rec.width / 2;
    // y is the height of the window (again, going down from the top) minus the height of scarfy (to get his top edge, otherwise he'd be hanging from the bottom of the screen, off camera)
    scarfyData.pos.y = winDim[1] - scarfyData.rec.height;
    // Scarfy's animation
    // animation frame
    scarfyData.frame = 0; 
    // amount of time before we update the animation frame (1/12 of a second)
    scarfyData.updateTime = 1.0 / 12.0;
    // Keep track of our running time (Delta value)
    scarfyData.runningTime = 0;
    // This is scarfy's rectangle's velocity
    int scarfy_vel{0};

    // Add in Nebula (hazard)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebData {
        {0.0, 0.0, nebula.width / 8, nebula.height / 8},    // Rectangle rec
        {winDim[0], winDim[1] - nebula.height},                     // Vector2 pos
        0,                                                          // Int frame
        1.0 / 12.0,                                                 // Float upTime
        0.0                                                         // Float runTime
    };
    
    AnimData neb2Data {
        {0.0, 0.0, nebula.width / 8, nebula.height / 8},            // Rectangle rec
        {winDim[0] + 300, winDim[1] - nebula.height},               // Vector2 pos
        0,                                                          // Int frame
        1.0 / 16.0,                                                 // Float upTime
        0.0                                                         // Float runTime
    };

    AnimData nebulae[2]{nebData, neb2Data};

    // nebula X velocity (pixels/second)
    int neb_vel{-200};

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
            if (scarfyData.pos.y >= winDim[1] - scarfyData.rec.height)
            {
                isInAir = false;
                scarfy_vel = 0;
                // if running time is greater than update time, we need to update the animation frame
                if (scarfyData.runningTime >= scarfyData.updateTime)
                {
                    // need to reset running time
                    scarfyData.runningTime = 0.0;
                    // update the scarfy frame
                    scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                    // update the frame counter
                    scarfyData.frame++;
                    // if we're on the 6th frame, we need to reset the frame counter to zero
                    if (scarfyData.frame > 5)
                    {
                        scarfyData.frame = 0;
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
            scarfyData.pos.y += scarfy_vel * dT;
            nebulae[0].pos.x += neb_vel * dT;
            nebulae[1].pos.x += neb_vel * dT;
            // update running time
            scarfyData.runningTime += dT;
            nebulae[0].runningTime += dT;
            nebulae[1].runningTime += dT;

            // Nebula animation
            for(int i = 0; i < sizeof(nebulae); i++){
                if (nebulae[i].runningTime >= nebulae[i].updateTime)
                {
                    // need to reset running time
                    nebulae[i].runningTime = 0.0;
                    // update the scarfy frame
                    nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                    // update the frame counter
                    nebulae[i].frame++;
                    // if we're on the 6th frame, we need to reset the frame counter to zero
                    if (nebulae[i].frame > 7)
                    {
                        nebulae[i].frame = 0;
                    }
                }
            }
            
           // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            // Draw Nebula/Hazard
            DrawTextureRec(nebula, nebulae[0].rec, nebulae[0].pos, WHITE);
            DrawTextureRec(nebula, nebulae[1].rec, nebulae[1].pos, WHITE);
       }

        // Game logic ends
        EndDrawing();
        
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}