#include "LevelF.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int LEVELF_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};
const char FONT[] = "assets/font1.PNG";
GLuint font5_texture_id;

LevelF::~LevelF()
{
    delete [] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelF::initialise()
{
    state.next_scene_id = -1;
    
    state.BG = new Entity();
    state.BG->set_position(glm::vec3(5.0f, -4.0f, 0.0f));
    state.BG->texture_id = Utility::load_texture("assets/beach.png");
    state.BG->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    GLuint map_texture_id = Utility::load_texture("assets/tilesinprog.png");
    this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELF_DATA, map_texture_id, 1.0f, 6, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(8.0f, -1.0f, 0.0f));
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
    state.player->set_height(0.5f);
    state.player->set_width(0.8f);
    
    // Jumping
    state.player->jumping_power = 5.0f;
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/crabsheet.png");
    
    state.enemies = new Entity[this->ENEMY_COUNT];
    state.enemies[0].set_entity_type(ENEMY);
    state.enemies[0].set_ai_type(GUARD);
    state.enemies[0].set_ai_state(IDLE);
    state.enemies[0].texture_id = enemy_texture_id;
    state.enemies[0].set_position(glm::vec3(2.0f, -5.0f, 0.0f));
    state.enemies[0].set_movement(glm::vec3(0.0f));
    state.enemies[0].speed = 1.0f;
    state.enemies[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    state.enemies[1].set_entity_type(ENEMY);
    state.enemies[1].set_ai_type(GUARD);
    state.enemies[1].set_ai_state(IDLE);
    state.enemies[1].texture_id = enemy_texture_id;
    state.enemies[1].set_position(glm::vec3(2.0f, -5.0f, 0.0f));
    state.enemies[1].set_movement(glm::vec3(1.0f));
    state.enemies[1].speed = 1.0f;
    state.enemies[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    for (int i = 0; i < ENEMY_COUNT; i++){
    state.player->walking[state.player->RIGHT]  = new int[5] { 0, 1, 2, 3, 4 };
    state.player->walking[state.player->LEFT] = new int[5] { 0, 1, 2, 3, 4 };
//    state.player->walking[state.player->UP]    = new int[4] { 2, 6, 10, 14 };
//    state.player->walking[state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };

    state.enemies[i].animation_indices = state.player->walking[state.player->LEFT];
    state.enemies[i].animation_frames = 5;
    state.enemies[i].animation_index  = 0;
    state.enemies[i].animation_time   = 0.0f;
    state.enemies[i].animation_cols   = 5;
    state.enemies[i].animation_rows   = 1;
    state.enemies[i].set_height(0.5f);
    state.enemies[i].set_width(0.5f);
    }
    
    GLuint girl1_texture_id = Utility::load_texture("assets/beachgirl.png");
    GLuint sandb_texture_id = Utility::load_texture("assets/sandbottom.png");
    GLuint sandt_texture_id = Utility::load_texture("assets/sandplatform.png");
    
    
    state.objects = new Entity[this->OBJECT_COUNT];
    
    state.objects[0].texture_id = girl1_texture_id;
    state.objects[0].set_entity_type(GIRL);
    state.objects[0].set_position(glm::vec3(9.0f, -2.75f, 0.0f));
    state.objects[0].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[0].speed = 0.0f;
    state.objects[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    state.objects[1].texture_id = sandb_texture_id;
    state.objects[1].set_entity_type(PLATFORM);
    state.objects[1].set_position(glm::vec3(3.0f, -4.0f, 0.0f));
    state.objects[1].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[1].speed = 0.0f;
    state.objects[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.objects[1].set_height(0.0001);
    state.objects[1].set_width(3);
    
    state.objects[2].texture_id = sandt_texture_id;
    state.objects[2].set_entity_type(PLATFORM);
    state.objects[2].set_position(glm::vec3(4.0f, -3.75f, 0.0f));
    state.objects[2].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[2].speed = 0.0f;
    state.objects[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.objects[2].set_height(0.0001);
    state.objects[2].set_width(3);
    
    state.objects[3].texture_id = sandt_texture_id;
    state.objects[3].set_height((state.objects[3].get_height()/2));
    state.objects[3].set_width(1);
    state.objects[3].set_entity_type(PLATFORM);
    state.objects[3].set_position(glm::vec3(8.0f, -2.5f, 0.0f));
    state.objects[3].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[3].speed = 0.0f;
    state.objects[3].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    GLuint ball_texture_id = Utility::load_texture("assets/ball1.png");
    
    state.ball = new Entity();
    state.ball->texture_id = ball_texture_id;
    state.ball->set_position(glm::vec3(9.0f, -2.75f, 0.0f));
    state.ball->set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
    state.ball->speed = 2.0f;
    state.ball->set_acceleration(glm::vec3(0.0f, -9.8f, 0.0f));
    state.ball->is_active = false;
    state.ball->set_height(0.25);
    state.ball->set_entity_type(BALL);
    
    
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
    
    font5_texture_id = Utility::load_texture(FONT);
    
}

void LevelF::update(float delta_time) {
    this->state.player->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.BG->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
//    this->state.line->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    for (int i = 0; i < OBJECT_COUNT; i++) this->state.objects[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.ball->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
//    for (int i = 0; i < 2; i++) state.line[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    for (int i = 0; i < ENEMY_COUNT; i++) this->state.enemies[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    
    this->state.objects->model_matrix = glm::scale(this->state.objects->model_matrix, glm::vec3(2.0f, 2.0f, 0.0f));
    this->state.ball->model_matrix = glm::scale(this->state.ball->model_matrix, glm::vec3(2.0f, 2.0f, 0.0f));
    this->state.objects[1].model_matrix = glm::scale(this->state.objects[1].model_matrix, glm::vec3(6.0f, 6.0f, 0.0f));
    this->state.objects[2].model_matrix = glm::scale(this->state.objects[2].model_matrix, glm::vec3(6.0f, 6.0f, 0.0f));
    this->state.objects[3].model_matrix = glm::scale(this->state.objects[3].model_matrix, glm::vec3(6.0f, 6.0f, 0.0f));
    if(this->state.objects->collided_bottom){
        this->state.objects->velocity.y = 3;
    }
    if(this->state.player->collided_bottom){
        if(this->state.ball->is_active == false){
            this->state.ball->is_active = true;
//            this->state.line[0].is_active = false;
//            this->state.line[1].is_active = true;
            this->state.ball->velocity.y = 6;
            this->state.ball->velocity.x = 1;
        }
    }
    if(this->state.ball->collided_bottom){
        this->state.ball->set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void LevelF::render(ShaderProgram *program, float delta_time)
{
    this->state.BG->renderbg(program);
    this->state.map->render(program);
    this->state.player->render(program, delta_time);
    for (int i = 0; i < OBJECT_COUNT; i++) this->state.objects[i].render(program, delta_time);
    this->state.ball->render(program, delta_time);
    for (int i = 0; i < ENEMY_COUNT; i++) this->state.enemies[i].render(program, delta_time);
//    Utility::draw_text(program, font5_texture_id, this->state.line[0].line, 0.5f, 0.00000000001f, glm::vec3(2.0f, -6.0f, 0.0f));
}

