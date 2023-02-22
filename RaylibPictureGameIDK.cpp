// RaylibPictureGameIDK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <raylib.h>
#include <iostream>
#include <vector>
#include <raymath.h>
#include <rlgl.h>
using namespace std;
Rectangle area = { 10,10,380,380 };
class Counter 
{
    float counter=0;
    float time;
public:
    void Update(float delta) { counter += delta; if (counter > time)
    {
        counter += delta;
        if (counter>time)
        {
            counter = time;
        }
    }
    }
    bool IsComplete() { return counter == time; }
    float GetPercent() { return counter / time; }
    void Reset() { counter = 0; }
    void SetTime(float time) {
        this->time = time; if (counter>time)
        {
            counter = time;
        }
    }
    Counter() { time = 1.f; }
    Counter(float time) { this->time = time; counter = 0; }
};

struct HitInfo 
{
    float damage=1;
    float paralysisTime = 0; bool wasImmortal =true;
};

struct CircleInfo {
    float radius;
    Vector2 pos;
};
struct Cursor 
{
    float radius = 8.f;
    float distance = 20.0f;
    float minorRadius = 2.5f;
    float speed =40.0f;
    
    float smooth = 4.5f;

    float health = 5.0f;

    Vector2 velocity;
    Vector2 position;

    //this sucks lol
    struct Node 
    {
        Vector2 baseOffset;
      //  Vector2 currentOffset;
        Vector2 position;
        Counter selectCounter;
        bool pressed=false;
    };
    Node up;
    Node down;
    Node right;
    Node left;

    float maxHealth = 5.0f;
    Counter immortalityFrame=Counter(2.0f);
    Counter deathCounter=Counter(5.0f);
    bool dead = false;
public:
    CircleInfo GetSelectArea()
    {
        if (up.selectCounter.GetPercent() == 0 && up.pressed)
        {
            return { minorRadius * 2.5f,up.position };
        }
        if (down.selectCounter.GetPercent() == 0 && down.pressed)
        {
            return { minorRadius * 2.5f,down.position };
        }
        if (left.selectCounter.GetPercent() == 0 && left.pressed)
        {
            return { minorRadius * 2.5f,left.position };
        }
        if (right.selectCounter.GetPercent() == 0 && right.pressed)
        {
            return { minorRadius * 2.5f,right.position };
        }
        return { 0.0f,{0.0f,0.0f} };
    }
    float GetHealthPercentage() { return health/maxHealth; }
    bool IsDead() { return dead; }
    bool DeathCounterExpired() { return deathCounter.IsComplete() && dead; }
    void Hit(HitInfo& inf) {
        if (immortalityFrame.IsComplete()&&!dead) {
            health -= inf.damage; 
          
            if (health <= 0)
            {
                deathCounter.Reset();
                dead = true;
            }
            immortalityFrame.Reset();
            inf.wasImmortal = false;
        }
    }
    void Update(float delta) 
    {
        deathCounter.Update(delta);
        immortalityFrame.Update(delta);
        if (!dead)
        {


            auto ac = Vector2Scale(Vector2Normalize(Vector2{ (float)IsKeyDown(KEY_RIGHT) - (float)IsKeyDown(KEY_LEFT),(float)((float)IsKeyDown(KEY_UP) - (float)IsKeyDown(KEY_DOWN)) }), speed);
            ac = Vector2Subtract(ac, velocity);
            velocity = Vector2Add(velocity, Vector2Scale(ac, delta));
            position = Vector2Add(position, Vector2Scale(velocity, delta));
            // cout << position.x << " " << position.y << "\n";
            if (position.x > area.x + area.width - radius)
            {
                position.x = area.x + area.width - radius;
            }
            if (position.y > area.y + area.height - radius)
            {
                position.y = area.y + area.height - radius;
            }
            if (position.y < area.y + radius)
            {
                position.y = area.y + radius;
            }
            if (position.x < area.x + radius)
            {
                position.x = area.x + radius;
            }
        }

        up.selectCounter.Update(delta);
        right.selectCounter.Update(delta);
        left.selectCounter.Update(delta);
        down.selectCounter.Update(delta);
        if (up.selectCounter.IsComplete())
        {

            up.pressed = false;
            if (IsKeyPressed(KEY_W))
            {
                up.selectCounter.Reset();

                up.pressed = true;
            }
        }
        if (down.selectCounter.IsComplete())
        {

            down.pressed = false;
            if (IsKeyPressed(KEY_S))
            {
                down.selectCounter.Reset();

                down.pressed = true;
            }
        }
        if (left.selectCounter.IsComplete())
        {

            left.pressed = false;
            if (IsKeyPressed(KEY_A))
            {
                left.selectCounter.Reset();

                left.pressed = true;
            }
        }
        if (right.selectCounter.IsComplete())
        {

            right.pressed = false;
            if (IsKeyPressed(KEY_D))
            {
                right.selectCounter.Reset();

                right.pressed = true;
            }
        }
        float dis;
        dis = Vector2Distance(position, up.position);
        if (dis <= radius + minorRadius)
        {
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(position, up.position)), radius + minorRadius - dis));
        }
        dis = Vector2Distance(position, left.position);
        if (dis <= radius + minorRadius)
        {
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(position, left.position)), radius + minorRadius - dis));
        }
        dis = Vector2Distance(position, down.position);
        if (dis <= radius + minorRadius)
        {
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(position, down.position)), radius + minorRadius - dis));
        }
        dis = Vector2Distance(position, right.position);
        if (dis <= radius + minorRadius)
        {
            position = Vector2Add(position, Vector2Scale(Vector2Normalize(Vector2Subtract(position, right.position)), radius + minorRadius - dis));
        }



       
        auto desiredPos = Vector2Add(position, up.baseOffset);
        if (desiredPos.x > area.x+area.width - minorRadius)
        {
            desiredPos.x = area.x + area.width - minorRadius;
        }
        if (desiredPos.y > area.y + area.height - minorRadius)
        {
            desiredPos.y = area.y + area.height - minorRadius;
        }
        if (desiredPos.y < area.y + minorRadius)
        {
            desiredPos.y = area.y + minorRadius;
        }
        if (desiredPos.x < area.x + minorRadius)
        {
            desiredPos.x = area.x+ minorRadius;
        }

        up.position = Vector2Add(up.position,Vector2Scale(Vector2Subtract(desiredPos,up.position),smooth*delta));

        if (desiredPos.x > area.x + area.width - minorRadius)
        {
            desiredPos.x = area.x + area.width - minorRadius;
        }
        if (desiredPos.y > area.y + area.height - minorRadius)
        {
            desiredPos.y = area.y + area.height - minorRadius;
        }
        if (desiredPos.y < area.y + minorRadius)
        {
            desiredPos.y = area.y + minorRadius;
        }
        if (desiredPos.x < area.x + minorRadius)
        {
            desiredPos.x = area.x + minorRadius;
        }
        desiredPos = Vector2Add(position, right.baseOffset);

        if (desiredPos.x > area.x + area.width - minorRadius)
        {
            desiredPos.x = area.x + area.width - minorRadius;
        }
        if (desiredPos.y > area.y + area.height - minorRadius)
        {
            desiredPos.y = area.y + area.height - minorRadius;
        }
        if (desiredPos.y < area.y + minorRadius)
        {
            desiredPos.y = area.y + minorRadius;
        }
        if (desiredPos.x < area.x + minorRadius)
        {
            desiredPos.x = area.x + minorRadius;
        }
        right.position = Vector2Add(right.position, Vector2Scale(Vector2Subtract(desiredPos, right.position), smooth * delta));
      
        desiredPos = Vector2Add(position, down.baseOffset);

        if (desiredPos.x > area.x + area.width - minorRadius)
        {
            desiredPos.x = area.x + area.width - minorRadius;
        }
        if (desiredPos.y > area.y + area.height - minorRadius)
        {
            desiredPos.y = area.y + area.height - minorRadius;
        }
        if (desiredPos.y < area.y + minorRadius)
        {
            desiredPos.y = area.y + minorRadius;
        }
        if (desiredPos.x < area.x + minorRadius)
        {
            desiredPos.x = area.x + minorRadius;
        }
        down.position = Vector2Add(down.position, Vector2Scale(Vector2Subtract(desiredPos, down.position), smooth * delta));
        
        desiredPos = Vector2Add(position, left.baseOffset);

        if (desiredPos.x > area.x + area.width - minorRadius)
        {
            desiredPos.x = area.x + area.width - minorRadius;
        }
        if (desiredPos.y > area.y + area.height - minorRadius)
        {
            desiredPos.y = area.y + area.height - minorRadius;
        }
        if (desiredPos.y < area.y + minorRadius)
        {
            desiredPos.y = area.y + minorRadius;
        }
        if (desiredPos.x < area.x + minorRadius)
        {
            desiredPos.x = area.x + minorRadius;
        }
        left.position = Vector2Add(left.position, Vector2Scale(Vector2Subtract(desiredPos, left.position), smooth * delta));
 
    }
    void Draw() 
    {
        if (!dead)
        {
            auto position = this->position;
            if (!immortalityFrame.IsComplete())
            {
                position = Vector2Add({ sin(immortalityFrame.GetPercent() * 9) * 2.f,cos(immortalityFrame.GetPercent() * 4) * 2.0f }, position);
            }

            DrawLineBezierQuad(position, up.position, Vector2Add(position, { 0.f,distance / 2 }), 2.0f, LIGHTGRAY);
            DrawLineBezierQuad(position, down.position, Vector2Add(position, { 0.f,-distance / 2 }), 2.0f, LIGHTGRAY);
            DrawLineBezierQuad(position, right.position, Vector2Add(position, { distance / 2,0.0f }), 2.0f, LIGHTGRAY);
            DrawLineBezierQuad(position, left.position, Vector2Add(position, { -distance / 2,0.0f }), 2.0f, LIGHTGRAY);

            DrawCircleV(position, radius, BLACK);
            DrawCircleV(up.position, minorRadius, BLACK);
            DrawCircleV(left.position, minorRadius, BLACK);
            DrawCircleV(right.position, minorRadius, BLACK);
            DrawCircleV(down.position, minorRadius, BLACK);

            if (up.pressed)
            {
                auto p=up.selectCounter.GetPercent();
                DrawCircleLines(up.position.x, up.position.y, minorRadius + 3.0f*sin(p*PI), WHITE);
            }  
            if (down.pressed)
            {
                auto p = down.selectCounter.GetPercent();
                DrawCircleLines(down.position.x, down.position.y, minorRadius + 3.0f * sin(p * PI), WHITE);
            } 
            if (left.pressed)
            {
                auto p = left.selectCounter.GetPercent();
                DrawCircleLines(left.position.x, left.position.y, minorRadius + 3.0f * sin(p * PI), WHITE);
            }  
            if (right.pressed)
            {
                auto p = right.selectCounter.GetPercent();
                DrawCircleLines(right.position.x, right.position.y, minorRadius + 3.0f * sin(p * PI), WHITE);
            }

        }
        else 
        {
            if (!deathCounter.IsComplete())
            {
                rlSetLineWidth(3);
                DrawCircleLines(position.x, position.y, deathCounter.GetPercent()*radius*1.0f ,WHITE );
            }
        }
        //DrawCircle(position.x,position.y,radius,DARKBLUE);
    }
    Cursor(Vector2 pos) { position = pos; velocity = { 0.0f,0.0f }; 
    up.baseOffset = { 0.0f,distance };
    down.baseOffset = { 0.0f,-distance };
    right.baseOffset = { distance,0.0f };
    left.baseOffset = { -distance,0.0f };
    up.position = Vector2Add(position,up.baseOffset);
    down.position = Vector2Add(position, down.baseOffset);
    right.position = Vector2Add(position, right.baseOffset);
    left.position = Vector2Add(position, left.baseOffset);
  //  up.currentOffset = { 0.0f,40.0f };
   // down.currentOffset = { 0.0f,-40.0f };
    //right.currentOffset = { 40.0f,0.0f };
   // left.currentOffset = { -40.0f,0.0f };
    }
};

class Projectile
{
public:
    virtual HitInfo OnHit()=0;
    virtual bool ToDelete()=0;
    virtual void Update(float delta)=0;
    virtual bool IsColliding(Vector2 pos, float radius)=0;
    virtual void Draw()=0;
    virtual void SetToTime(float delta) {}
};
class SimpleBullet:public Projectile
{
    float radius=3.0f;
    Vector2 pos;
    Vector2 velocity;
    float maxVelocity;
    Vector2 accelleration;
    Vector2 prevPos;
    bool hit = false;
    float smooth = 2.0f;
public :
    HitInfo OnHit() { hit = true; return {}; }
    bool ToDelete() { return hit || Vector2DistanceSqr(pos, {area.x+area.width/2,area.y + area.height / 2 })>=pow(max(area.width,area.height)/2+radius,2.5f); }
    void Update(float delta) { 

        velocity =Vector2Add( Vector2Scale(accelleration,delta),velocity);//not a magnitude clamp...will go faster if diagonal
 
    pos = Vector2Add(pos,Vector2Scale(velocity,delta));
    prevPos=Vector2Lerp(prevPos,pos,smooth*delta);
 //   cout << pos.x << " " << pos.y << "\n";
    }

    bool IsColliding(Vector2 pos, float radius) 
    {
        return Vector2DistanceSqr(pos,prevPos)<pow(radius+this->radius,2);
    }
    void Draw() 
    {
        DrawCircleV(pos,radius,WHITE);
        DrawCircleV(prevPos, radius*0.3f, WHITE);
    }
    
    void SetToTime(float delta) 
    {
        velocity = Vector2ClampValue(Vector2Add(Vector2Scale(accelleration, delta), pos), -maxVelocity, maxVelocity);//not a magnitude clamp...will go faster if diagonal

        pos = Vector2Add(pos, Vector2Scale(velocity, delta));
        Vector2Lerp(prevPos, pos, smooth * delta);
    }

    SimpleBullet(Vector2 p,Vector2 v,Vector2 a, float maxV) 
    {
        pos = p;
        prevPos = p;
        velocity = v;
        accelleration = a;
        maxVelocity = maxV;
    }
};

class Spawner 
{
    float m = 3.0f;
    float c=0.0f;
public:
    void Spam(vector<Projectile*>& projectiles,float delta) 
    {
        c += delta;
        if (c>m) {
           
            auto y = (rand() % 10);
            for (auto x = (float)y; x < y+0.5f; x+=0.1f)
            {
                projectiles.push_back(new SimpleBullet(Vector2{ (float)sin(x) * 190.0f + 200.0f, 400.f }, Vector2{ 0, -15.f }, Vector2{ 0.f,-3.f }, 15.0f));
            }
            c -= m;
        }
    }
};

enum class EventType
{
    DIALOGUESELECTED,
    PLAYERDIED,
    PLAYERHIT
};

class GameEvent 
{
public:
    const EventType type;
    GameEvent(EventType t):type(t) {}
};

class PlayerHitEvent:public GameEvent 
{
public:
    HitInfo info;
    PlayerHitEvent(HitInfo inf) :GameEvent(EventType::PLAYERHIT) { info = inf; }
};

class PlayerDeathEvent :public GameEvent
{
public:
    PlayerDeathEvent() :GameEvent(EventType::PLAYERDIED) { }
};
class DialogueEvent :public GameEvent 
{
public:
    string flag;
    DialogueEvent(string flg) :GameEvent(EventType::DIALOGUESELECTED) { flag = flg; }
};

class CharacterDisplay 
{
    Texture t; 
    string text;
    int currentIndex;
    float counter;
    float maxCounter=0.2f;
    string desiredText="Hi.\nTry to not\nget hit!";
    Font font;
    RenderTexture2D texture;
public:
    void Update(float delta) 
    {
        counter += delta;
        while (maxCounter<counter) 
        {
            if (currentIndex < desiredText.length() ) {
              
                text += desiredText[currentIndex];
                currentIndex += 1;
            }
            counter -= maxCounter;
        }

        //BeginDrawing();
        BeginTextureMode(texture);
        ClearBackground(Color{ 0,0,0,0 });
        DrawTextPro(font, text.c_str(), { 205.0f,35.0f }, { 0.0f,0.0f }, 0, 14, 2, WHITE);
        EndTextureMode();
        //EndDrawing();
    }
    void Draw()
    {
        DrawTexture(t,0,400,WHITE);
   //     DrawTextPro(font, text.c_str(), { 250.0f,600.0f }, {0.0f,0.0f},0,12,20,WHITE);
        DrawTexture(texture.texture,0,400,WHITE);
    }
    void OnEvent(GameEvent* event) 
    {
        switch (event->type) 
        {
        case EventType::DIALOGUESELECTED:
            {DialogueEvent* e = (DialogueEvent*)event;
            if (e->flag=="hello")
            {
                counter = 0;
                currentIndex = 0;
                text = "";
                desiredText = "Hello?";

            }
            if (e->flag == "moving")
            {
                counter = 0;
                currentIndex = 0;
                text = "";
                desiredText = "Okie, dokie,\nyou caught the\nmoving box!";

            }

            }
        break;
        case EventType::PLAYERDIED:
            counter = 0;
            currentIndex = 0;
            text = "";
            desiredText = "What!\nYou're dead\nalready?";

            break;
        case EventType::PLAYERHIT:
        { PlayerHitEvent* e = (PlayerHitEvent*)event;
        if (!e->info.wasImmortal)
        {
            counter = 0;
            currentIndex = 0;
            text = "";
            desiredText = "Ouch!\nIt appears you've\nbeen hit...";
        }break;
        }
        default:
            break;
        }
    }
    CharacterDisplay()
    {
        Image i = LoadImage("mockup.png");
        ImageFlipVertical(&i);
        t = LoadTextureFromImage(i);
        UnloadImage(i);
        font = GetFontDefault();
        texture = LoadRenderTexture(400,400);
        currentIndex = 0;
        counter = 0;
    }
};

class HealthBar {
    float percent=1;
    float targetPercent=1;
    Counter counter=Counter(3.0f);
public:
    void Update(float delta) {
        counter.Update(delta);
        if (counter.IsComplete())
        {
            percent = targetPercent;
        }
    }
    void SetNewTarget(float targetPercent) 
    {
 //       cout<<targetPercent
        if (this->targetPercent!=targetPercent)
        {
            counter.Reset();
           this->targetPercent= targetPercent;
        
        }
    }
    void Draw() {
        float fac = percent + (targetPercent - percent) * counter.GetPercent();

        DrawRectangle(20, 397, 360 , 6, DARKGRAY);
        DrawRectangle(100+(200-200*fac)/2, 395, (float)200 * fac, 10, WHITE);
    }
};

class DialogueBox 
{
public:
    virtual void Select() = 0;
    virtual void Draw()=0;
    virtual void Update(float delta) = 0;
    virtual bool IsColliding(Vector2 pos, float radius)=0;
    string flag;
};
class SimpleDialogueBox:public DialogueBox 
{
    Rectangle rec;
    RenderTexture text;
    Counter selectCounter;
public:
    void Select() { selectCounter.Reset(); }
    void Draw() 
    {
        Rectangle rec = this->rec;
        if (!selectCounter.IsComplete())
        {
            rec.x += sin(selectCounter.GetPercent() * 10.0f)*5.0f;
            rec.y += sin(selectCounter.GetPercent() * 6.0f) * 5.0f;
        }
            DrawRectangleRec(rec, DARKGRAY);
            DrawTextureV(text.texture, { rec.x,rec.y }, WHITE);
        
    }
    void Update(float delta) 
    {
        selectCounter.Update(delta);
    }
    bool IsColliding(Vector2 pos, float radius) 
    {
        return pos.x - radius < rec.x + rec.width&& pos.x + radius > rec.x&&
            pos.y - radius < rec.y + rec.height && pos.y + radius > rec.y;
    }
    SimpleDialogueBox(string text,Rectangle rect,string f) 
    {
        flag = f;
        rec = rect;
        this->text = LoadRenderTexture(rec.width,rec.height);
        BeginTextureMode(this->text);
        DrawText(text.c_str(), 0, 5, 8, WHITE);
        EndTextureMode();
        selectCounter = Counter(3.0f);
        selectCounter.Update(3.0f);
    }
};


class MovingDialogueBox :public DialogueBox
{
    Vector2 source;
    Vector2 target;
    Counter counter;
 

    Rectangle rec;
    RenderTexture text;
    Counter selectCounter;
public:
    void Select() { selectCounter.Reset(); }
    void Draw()
    {
        Rectangle rec = this->rec;
        if (!selectCounter.IsComplete())
        {
            rec.x += sin(selectCounter.GetPercent() * 10.0f) * 5.0f;
            rec.y += sin(selectCounter.GetPercent() * 6.0f) * 5.0f;
        }
        DrawRectangleRec(rec, DARKGRAY);
        DrawTextureV(text.texture, { rec.x,rec.y }, WHITE);

    }
    void Update(float delta)
    {
        counter.Update(delta);
        selectCounter.Update(delta);

        rec.x = Lerp(source.x, target.x, sin(counter.GetPercent() *  PI)); 
        rec.y = Lerp(source.y, target.y, sin(counter.GetPercent() *  PI));
        if (counter.IsComplete())
        {
            counter.Reset();
        }

    }
    bool IsColliding(Vector2 pos, float radius)
    {
        return pos.x - radius < rec.x + rec.width && pos.x + radius > rec.x &&
            pos.y - radius < rec.y + rec.height && pos.y + radius > rec.y;
    }
    MovingDialogueBox(string text, Rectangle rect,Vector2 target, string f)
    {
        flag = f;
        rec = rect;
        this->target = target;
        this->source = { rec.x,rec.y };
        this->text = LoadRenderTexture(rec.width, rec.height);
        BeginTextureMode(this->text);
        DrawText(text.c_str(), 0, 5, 8, WHITE);
        EndTextureMode();
        counter = Counter(100.0f);
        selectCounter = Counter(3.0f);
        selectCounter.Update(3.0f);
    }
};

int main()
{
    const int screenWidth = 400;
    const int screenHeight = 800;

    
    InitWindow(screenWidth, screenHeight, "prototype");

    auto rt = LoadRenderTexture(400, 800);
    Cursor cursor(Vector2{200,200});
    vector< Projectile*> projectiles;
    vector<Projectile*> toDelete;
    vector<DialogueBox*>boxes = { new SimpleDialogueBox("Hello!",Rectangle{70.0f,50.0f,60.0f,20.0f},"hello"), new MovingDialogueBox("Catch me!",Rectangle{20.0f,270.0f,60.0f,20.0f},{300.0f,270.0f},"moving") };
    CharacterDisplay display;
    Spawner spawner;
    HealthBar health;
    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    bool died = false;
    float dead = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        float delta = GetFrameTime()*10.0f;
       // cout << delta;
        
        spawner.Spam(projectiles,delta);

        cursor.Update(delta);
        for (auto proj : projectiles) 
        {
          
            proj->Update(delta);
            if (!cursor.IsDead()&&proj->IsColliding(cursor.position,cursor.radius))
            {
                //register hit for player too;
                auto hit=proj->OnHit();
                cursor.Hit(hit);
                auto hitEvent = new PlayerHitEvent(hit);
                display.OnEvent(hitEvent);
                delete hitEvent;
            }
            if (proj->ToDelete())
            {
                toDelete.push_back(proj);
                continue;
            }
        }

        auto col = cursor.GetSelectArea();
        for (auto& box : boxes) 
        {
            box->Update(delta);
            if (!cursor.IsDead())
            {
                if (box->IsColliding(col.pos,col.radius))
                {
                    auto e =new DialogueEvent(box->flag);
                    display.OnEvent(e);
                  box->Select(),
                    delete e;
                }
            }
        }

        health.Update(delta);
        health.SetNewTarget(cursor.GetHealthPercentage());
        if (cursor.DeathCounterExpired()&&!died)
        {
            auto e= new PlayerDeathEvent();
            display.OnEvent(e);
            delete e;
            died = true;
        }
        if (died)
        {
            dead += delta;
            if (dead>5.0f)
            {
                CloseWindow();
            }
        }
        
        
        display.Update(delta);


        for (auto proj : toDelete)
        {
            for (auto x = projectiles.begin(); x != projectiles.end(); ++x)
            {
                if (*x == proj)
                {
                    delete proj;
                    projectiles.erase(x);
                    
                    break;
                }
            }
        }
        toDelete.clear();





        
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        BeginTextureMode(rt);
        ClearBackground(GRAY);
        DrawRectangle(0,400,400,400,DARKGRAY);
        DrawRectangleLinesEx({ 8,8,384,384 },3, DARKGRAY);
        
        display.Draw();

        for (auto& box : boxes)
        {
            box->Draw();
        }
        cursor.Draw();
        for (auto proj : projectiles)
        {
            proj->Draw();
        }
        health.Draw();

        EndTextureMode();

        ClearBackground(WHITE);
        DrawTextureEx(rt.texture, Vector2{0,0},0,1,WHITE);
        EndDrawing();

      
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
