#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"

struct GameState
{
    Map *map;
    Entity *BG;
    Entity *player;
    Entity *enemies;
    Entity *objects;
    Entity *squirrel;
    Entity *ball;
    Entity *line;
    
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
    
    int next_scene_id;
};

class Scene {
public:
    int number_of_enemies = 1;
    
    GameState state;
    
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program, float delta_time) = 0;
    
    GameState const get_state() const { return this->state; }
};
