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

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
};

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    // if running time is greater than update time, we need to update the animation frame
    if (data.runningTime >= data.updateTime)
    {
        // need to reset running time
        data.runningTime = 0.0;
        // update the animation frame
        data.rec.x = data.frame * data.rec.width;
        // update the frame counter
        data.frame++;
        // if we're on the 6th frame, we need to reset the frame counter to zero
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
} ;

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

    // make FinishLine
    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

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

    // Background Texture
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    // Midground Texture
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    // Background Texture
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    // collision
    bool collision{false};

    SetTargetFPS(60); 
    while (!WindowShouldClose())
    {
        // Delta Time (time since last frame)
        const float dT{GetFrameTime()};
        
        BeginDrawing();
        ClearBackground(WHITE);
        // Scroll background
        bgX -= 20 * dT;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // Draw background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
        
        // Draw Midground
        Vector2 mg1Pos{mgX, 0.0};
        Vector2 mg2Pos{mgX + background.width * 2, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // Draw Midground
        Vector2 fg1Pos{fgX, 0.0};
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // Game logic starts
        // check for ground
        if (isOnGround(scarfyData, winDim[1]))
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
        // update Finishline position
        finishLine += neb_vel * dT;
        // update Scarfy position
        scarfyData.pos.y += scarfy_vel * dT;

       for (AnimData nebula : nebulae)
        {
            // need to remove padding/whitespace around nebula
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + 20,
                nebula.pos.y + 20,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            // check for collision
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (!isInAir)
        {
            // update Scarfy's animation frame
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }
        // Nebula animation
        for (int i = 0; i < sizeOfNebulae; i++) {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }
        if (collision)
        {
            // END GAME!
            DrawText("Game Over!", winDim[0] / 4, winDim[1] / 2, 40, RED);
        }
        else if (scarfyData.pos.x > finishLine)
        {
            DrawText("You Win!", winDim[0] / 4, winDim[1] / 2, 40, GREEN);
        }
        else
        {
            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            for (int i = 0; i < sizeOfNebulae; i++) {
                // Draw Nebula/Hazard
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
        }

        // Game logic ends
        EndDrawing();        
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}