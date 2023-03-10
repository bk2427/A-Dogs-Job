#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 0;
    int OBJECT_COUNT = 0;
    
    ~LevelA();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program, float delta_time) override;
};
