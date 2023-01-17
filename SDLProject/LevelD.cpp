
#include "LevelD.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int LEVELD_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};
const char FONT[] = "assets/font1.PNG";
GLuint font3_texture_id;

LevelD::~LevelD()
{
    delete [] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelD::initialise()
{
    state.next_scene_id = -1;
    
    state.BG = new Entity();
    state.BG->set_position(glm::vec3(5.0f, -4.0f, 0.0f));
    state.BG->texture_id = Utility::load_texture("assets/backyard.png");
    state.BG->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    GLuint map_texture_id = Utility::load_texture("assets/tiles2.png");
    this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELD_DATA, map_texture_id, 1.0f, 6, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(1.0f, 0.0f, 0.0f));
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
    state.player->jumping_power = 7.0f;
    
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
    
    
    GLuint s_texture_id = Utility::load_texture("assets/squirrel.png");
    
    state.squirrel = new Entity[2];
//    state.squirrel[0].set_entity_type(ENEMY);
//    state.squirrel[0].set_ai_type(WALKER);
//    state.squirrel[0].set_ai_state(IDLE);
    state.squirrel[0].texture_id = s_texture_id;
    state.squirrel[0].set_position(glm::vec3(3.6f, -3.3f, 0.0f));
    state.squirrel[0].set_movement(glm::vec3(0.0f, 1.0f, 0.0f));
    state.squirrel[0].speed = 1.0f;
    state.squirrel[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    
    state.squirrel[1].set_entity_type(ENEMY);
    state.squirrel[1].set_ai_type(RUNNER);
    state.squirrel[1].set_ai_state(IDLE);
    state.squirrel[1].texture_id = s_texture_id;
    state.squirrel[1].set_position(glm::vec3(5.0f, -4.0f, 0.0f));
    state.squirrel[1].set_movement(glm::vec3(-1.0f));
    state.squirrel[1].speed = 0.5f;
    state.squirrel[1].set_acceleration(glm::vec3(0.0f, -9.0f, 0.0f));
    
    
    for (int i = 0; i < 2; i++){
        state.squirrel[i].walking[state.squirrel[i].UP]    = new int[4] { 1, 3, 5, 7 };
        state.squirrel[i].walking[state.squirrel[i].LEFT]    = new int[4] { 1, 3, 5, 7 };
        state.squirrel[i].walking[state.squirrel[i].DOWN]  = new int[4] { 0, 2, 4, 6 };
        state.squirrel[i].walking[state.squirrel[i].RIGHT]  = new int[4] { 0, 2, 4, 6 };

        state.squirrel[i].animation_indices = state.squirrel[i].walking[state.squirrel[i].UP];
        state.squirrel[i].animation_frames = 4;
        state.squirrel[i].animation_index  = 0;
        state.squirrel[i].animation_time   = 0.0f;
        state.squirrel[i].animation_cols   = 2;
        state.squirrel[i].animation_rows   = 4;
        
    }
    
    
    
    
    GLuint girl1_texture_id = Utility::load_texture("assets/girlhappy.png");
    GLuint tire_texture_id = Utility::load_texture("assets/tire.png");
    GLuint sand_texture_id = Utility::load_texture("assets/sandbox.png");
    
    state.objects = new Entity[3];
    state.objects[0].texture_id = girl1_texture_id;
    state.objects[0].set_position(glm::vec3(1.0f, 3.0f, 0.0f));
    state.objects[0].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[0].speed = 0.0f;
    state.objects[0].set_acceleration(glm::vec3(0.0f, -9.8f, 0.0f));
    
    state.objects[1].texture_id = sand_texture_id;
    state.objects[1].set_position(glm::vec3(5.0f, -4.001f, 0.0f));
    state.objects[1].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[1].speed = 0.0f;
    state.objects[1].set_acceleration(glm::vec3(0.0f, -0.0f, 0.0f));

    
    state.objects[2].texture_id = tire_texture_id;
    state.objects[2].set_position(glm::vec3(4.0f, -3.0f, 0.0f));
    state.objects[2].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects[2].speed = 0.0f;
    state.objects[2].set_acceleration(glm::vec3(0.0f, -0.0f, 0.0f));

    
    
    GLuint ball_texture_id = Utility::load_texture("assets/ball1.png");
    
    state.ball = new Entity();
    state.ball->texture_id = ball_texture_id;
    state.ball->set_position(glm::vec3(2.0f, -4.0f, 0.0f));
    state.ball->set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
    state.ball->speed = 2.0f;
    state.ball->set_acceleration(glm::vec3(0.0f, -9.8f, 0.0f));
    state.ball->is_active = false;
    state.ball->set_height(0.25);
    
    
    state.line = new Entity[1];
    for (int i = 0; i < 2; i++){
        state.line[i].set_position(glm::vec3(2.0f, -6.0f, 0.0f));
        state.line[i].texture_id = font3_texture_id;
    }
    state.line[0].set_string("protect the yard!");
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
    
    font3_texture_id = Utility::load_texture(FONT);
    
}

void LevelD::update(float delta_time) {
    this->state.player->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.BG->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.line->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    for (int i = 0; i < 3; i++) this->state.objects[i].update(delta_time, state.player, state.objects, this->OBJECT_COUNT, state.enemies, this->ENEMY_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    this->state.ball->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    for (int i = 0; i < 2; i++) state.squirrel[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    for (int i = 0; i < 2; i++) state.line[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, state.objects, this->OBJECT_COUNT, state.squirrel, state.ball, state.line, this->state.map);
    
    this->state.objects[0].model_matrix = glm::scale(this->state.objects[0].model_matrix, glm::vec3(2.0f, 2.0f, 0.0f));
    this->state.objects[1].model_matrix = glm::scale(this->state.objects[1].model_matrix, glm::vec3(10.0f, 6.0f, 0.0f));
    this->state.objects[2].model_matrix = glm::scale(this->state.objects[2].model_matrix, glm::vec3(6.0f, 6.0f, 0.0f));
    this->state.squirrel[1].model_matrix = glm::rotate(this->state.squirrel[1].model_matrix, 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
    this->state.ball->model_matrix = glm::scale(this->state.ball->model_matrix, glm::vec3(2.0f, 2.0f, 0.0f));
    
//    for (int i = 0; i < 2; i++) {
//        if (state.squirrel[i].get_velocity().y < 0){
//            state.squirrel[i].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
//        }
//        if (state.squirrel[i].get_velocity().y > 0){
//            state.squirrel[i].set_movement(glm::vec3(0.0f, 1.0f, 0.0f));
//        }
//    }
//    if(this->state.squirrel[1].get_position().x < 2){
//        this->state.squirrel[1].animation_indices = this->state.squirrel[1].walking[this->state.squirrel[1].UP];
//        this->state.squirrel[1].movement.x = 1;
//    }
//    if(this->state.squirrel[1].get_position().x > 4){
//        this->state.squirrel[1].animation_indices = this->state.squirrel[1].walking[this->state.squirrel[1].DOWN];
//        this->state.squirrel[1].movement.x = -1;
//    }
            
    
    if(this->state.squirrel[0].get_position().y < -3.25){
        this->state.squirrel[0].position.x = 3.6f;
        this->state.squirrel[0].animation_indices = this->state.squirrel[0].walking[this->state.squirrel[0].UP];
        this->state.squirrel[0].set_movement(glm::vec3(0.0f, 1.0f, 0.0f));
//        this->state.squirrel[0].model_matrix = glm::translate(this->state.squirrel[0].model_matrix, glm::vec3(0.0f, -1.0f, 0.0f));
        this->state.squirrel[0].velocity.y = 2;
        
    }
    if(this->state.squirrel[0].get_position().y > -0.75){
        this->state.squirrel[0].position.x = 3.1f;
        this->state.squirrel[0].animation_indices = this->state.squirrel[0].walking[this->state.squirrel[0].DOWN];
        this->state.squirrel[0].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
//        this->state.squirrel[0].model_matrix = glm::translate(this->state.squirrel[0].model_matrix, glm::vec3(0.0f, -1.0f, 0.0f));
        this->state.squirrel[0].velocity.y = -2;
    }

    if(this->state.player->check_collision(this->state.objects)){
        if(this->state.ball->is_active == false){
            this->state.ball->velocity.y=7;
            this->state.ball->velocity.x=100000;
            this->state.ball->is_active = true;
            if (this->state.ball->position.x<20){
                this->state.ball->movement.x = 2.6;
            }
        }
    }
    if(this->state.ball->collided_bottom){
        this->state.ball->set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    
    
    
    
    
//    collisions
    if(this->state.player->check_collision(this->state.squirrel)){
        if(this->state.player->position.y >-3.5){
            this->state.squirrel[0].is_active = false;
        }
    }
//    if(this->state.player->check_collision(this->state.squirrel)){
//        if(this->state.player->collided_bottom && this->state.player->position.x < 6.5){
//            this->state.squirrel[1].is_active = false;
//        }

}

void LevelD::render(ShaderProgram *program, float delta_time)
{
    this->state.BG->renderbg(program);
    this->state.map->render(program);
    this->state.player->render(program, delta_time);
    this->state.squirrel[1].render(program, delta_time);
    for (int i = 0; i < 3; i++) this->state.objects[i].render(program, delta_time);
    this->state.squirrel[0].render(program, delta_time);
    this->state.ball->render(program, delta_time);
    Utility::draw_text(program, font3_texture_id, this->state.line[0].line, 0.5f, 0.00000000001f, glm::vec3(2.0f, -6.0f, 0.0f));
}
