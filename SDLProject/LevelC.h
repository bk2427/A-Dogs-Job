#include "Scene.h"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 1;
    int OBJECT_COUNT = 0;
    
    ~LevelC();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program, float delta_time) override;
};

