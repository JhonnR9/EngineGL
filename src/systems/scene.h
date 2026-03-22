//
// Created by jhone on 22/03/2026.
//

#ifndef BASE_SCENE_H
#define BASE_SCENE_H

class App;

class Scene {
protected:
    App &app;

public:
    explicit Scene(App &app);

    virtual ~Scene() = default;

    virtual void init() = 0;

    virtual void update(float delta) = 0;

    virtual void render() = 0;

    virtual void resize(int width, int height) =0;
};


#endif //BASE_SCENE_H
