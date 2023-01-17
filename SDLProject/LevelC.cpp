

#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int LEVELC_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const char FONT[] = "assets/font1.PNG";
GLuint font2_texture_id;


LevelC::~LevelC()
{
    delete [] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    delete    this->state.BG;
    
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelC::initialise()
{
    state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 4, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    
    state.BG = new Entity();
    state.BG->set_position(glm::vec3(5.0f, -4.0f, 0.0f));
    state.BG->texture_id = Utility::load_texture("assets/intro.png");
    state.BG->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));

    /**
     Enemies' stuff */
    GLuint ball_texture_id = Utility::load_texture("assets/ball1.png");
    
    state.objects = new Entity();
    state.objects->texture_id = ball_texture_id;
    state.objects->set_position(glm::vec3(-3.0f, 20.0f, 0.0f));
    state.objects->set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    state.objects->speed = 2.0f;
    state.objects->set_acceleration(glm::vec3(0.0f, -3.0f, 0.0f));
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    state.bgm = Mix_LoadMUS("assets/mixkit-fun-times-7.mp3");
    Mix_PlayMusic(state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 6.0f);
 
    
    font2_texture_id = Utility::load_texture(FONT);
}

void LevelC::update(float delta_time)
{

}
void LevelC::render(ShaderProgram *program, float delta_time)
{
    Utility::draw_text(program, font2_texture_id, "and just like that", 0.5f, 0.00000000001f, glm::vec3(1.0f, -1.0f, 0.0f));
    Utility::draw_text(program, font2_texture_id, "your career as dog", 0.5f, 0.00000000001f, glm::vec3(1.0f, -3.0f, 0.0f));
    Utility::draw_text(program, font2_texture_id, "begins", 0.5f, 0.00000000001f, glm::vec3(3.5f, -5.0f, 0.0f));
}
