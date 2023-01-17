#include "Scene.h"

class LevelF : public Scene {
public:
    int ENEMY_COUNT = 2;
    int OBJECT_COUNT = 4;
    
    ~LevelF();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program, float delta_time) override;
};

