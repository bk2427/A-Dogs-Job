#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int LEVELB_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
const char FONT[] = "assets/font1.PNG";
GLuint font1_texture_id;

LevelB::~LevelB()
{
    delete [] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelB::initialise()
{
    state.next_scene_id = -1;
    
    state.BG = new Entity();
    state.BG->set_position(glm::vec3(5.0f, -4.0f, 0.0f));
    state.BG->texture_id = Utility::load_texture("assets/livingroom.png");
    state.BG->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    GLuint map_texture_id = Utility::load_texture("assets/tilesinprog.png");
    this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 6, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 2.5f;
    state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.player->texture_id = Utility::load_texture("assets/Dogsheet.png");
    
    // Walking
    state.player->walking[state.player->RIGHT]  = new int[4] { 1, 2, 3, 4 };
    state.player->walking[state.player->LEFT] = new int[4] { 9, 8, 7, 6 };
//    state.player->walking[state.player->UP]    = new int[4] { 2, 6, 10, 14 };
//    state.player->walking[state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };

    state.player->animation_indices = state.player->walking[state.player->LEFT];
    state.player->animation_frames = 4;
    state.player->animation_index  = 0;
    state.player->animation_time   = 0.0f;
    state.player->animation_cols   = 5;
    state.player->animation_rows   = 2;
    state.player->set_height(0.8f);
    state.player->set_width(0.8f);
    
    // Jumping
    state.player->jumping_power = 5.0f;
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/soph.png");
    
    state.enemies = new Entity[this->ENEMY_COUNT];
    state.enemies[0].set_entity_type(ENEMY);
    state.enemies[0].set_ai_type(GUARD);
    state.enemies[0].set_ai_state(IDLE);
    state.enemies[0].texture_id = enemy_texture_id;
    state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    state.enemies[0].set_movement(glm::vec3(0.0f));
    state.enemies[0].speed = 1.0f;
    state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    GLuint girl1_texture_id = Utility::load_texture("assets/girlhappy.png");
    
    state.objects = new Entity();
    state.objects->texture_id = girl1_texture_id;
    state.objects->set_position(glm::vec3(1.0f, -3.0f, 0.0f));
    state.objects->set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects->speed = 0.0f;
    state.objects->set_acceleration(glm::vec3(0.0f, -9.8f, 0.0f));
    
    GLuint ball_texture_id = Utility::load_texture("assets/ball1.png");
    
    state.ball = new Entity();
    state.ball->texture_id = ball_texture_id;
    state.ball->set_position(glm::vec3(2.0f, -4.0f, 0.0f));
    state.ball->set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
    state.ball->speed = 2.0f;
    state.ball->set_acceleration(glm::vec3(0.0f, -9.8f, 0.0f));
    state.ball->is_active = false;
    
    
    state.line = new Entity[1];
    for (int i = 0; i < 2; i++){
        state.line[i].set_position(glm::vec3(2.0f, -6.0f, 0.0f));
        state.line[i].texture_id = font1_texture_id;
    }
    state.line[0].set_string("meet ur human!");
    state.line[1].set_string("go fetch!");
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
    
    font1_texture_id = Utility::load_texture(FONT);
    
}

void LevelB::update(float delta_time) {
    this->state.player->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.BG->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.line->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.objects->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.ball->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    for (int i = 0; i < 2; i++) state.line[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    
    this->state.objects->model_matrix = glm::scale(this->state.objects->model_matrix, glm::vec3(2.0f, 2.0f, 0.0f));
    this->state.ball->model_matrix = glm::scale(this->state.ball->model_matrix, glm::vec3(2.0f, 2.0f, 0.0f));
    if(this->state.objects->collided_bottom){
        this->state.objects->velocity.y = 3;
    }
    if(this->state.player->check_collision(this->state.objects)){
        if(this->state.ball->is_active == false){
            this->state.ball->is_active = true;
//            this->state.line[0].is_active = false;
//            this->state.line[1].is_active = true;
            if (this->state.ball->position.x<20){
                this->state.ball->movement.x = 1;
            }
            this->state.ball->velocity.y = 6;
            this->state.ball->velocity.x = 10;
            
        }
    }
//    int counter = 0;
//    if (this->state.ball->collided_bottom){
//        if (counter < 1){
//        counter += 1;
//        Mix_PlayChannel(-1, state.jump_sfx, 1);
//        }
//    }
}

void LevelB::render(ShaderProgram *program, float delta_time)
{
    this->state.BG->renderbg(program);
    this->state.map->render(program);
    this->state.player->render(program, delta_time);
    this->state.objects->render(program, delta_time);
    this->state.ball->render(program, delta_time);
    Utility::draw_text(program, font1_texture_id, this->state.line[0].line, 0.5f, 0.00000000001f, glm::vec3(2.0f, -6.0f, 0.0f));
}
