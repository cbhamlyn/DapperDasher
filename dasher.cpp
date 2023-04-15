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
    const int winDim[2]{512, 380};
    
    InitWindow(winDim[0], winDim[1], "Dapper Dasher");
 
    // Acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    // Add in Nebula (hazard)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        // Rectangle rec
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        // Vector Pos
        nebulae[i].pos.x = winDim[0] + (i * 300);
        nebulae[i].pos.y = winDim[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / (12.0 + i );
    };

    // nebula X velocity (pixels/second)
    int neb_vel{-200};

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

    // Determine if scarfy is in the air
    bool isInAir{};
    // Jump velocity (pixels/sec)
    const int jumpVel{-600};
   
    // This is scarfy's rectangle's velocity
    int scarfy_vel{0};

    SetTargetFPS(60); 
    while (!WindowShouldClose())
    {
        // Delta Time (time since last frame)
        const float dT{GetFrameTime()};
        
        BeginDrawing();
        ClearBackground(WHITE);
        
        // Game logic starts
        // check for ground
        if (scarfyData.pos.y >= winDim[1] - scarfyData.rec.height)
        {
            scarfy_vel = 0;
            isInAir = false;
        }
        else
        // apply gravity
        {
            scarfy_vel += gravity * dT;
            isInAir = true;
        }
        // check for jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            scarfy_vel += jumpVel;
        }

        // update Nebula position
         for (int i = 0; i < sizeOfNebulae; i++) {
            nebulae[i].pos.x += neb_vel * dT;
         }
        // update Scarfy position
        scarfyData.pos.y += scarfy_vel * dT;

        if (!isInAir)
        {
            // update running time
            scarfyData.runningTime += dT;
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

        // Nebula animation
        for (int i = 0; i < sizeOfNebulae; i++) {
            // update running time
            nebulae[i].runningTime += dT;
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
        for (int i = 0; i < sizeOfNebulae; i++) {
            // Draw Nebula/Hazard
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }
        // Draw Scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        // Game logic ends
        EndDrawing();        
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}