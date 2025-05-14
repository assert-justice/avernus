#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/fs.hpp"
#include "engine/module.hpp"
#include "utils/store.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "mesh.hpp"

class Graphics: public Module{
    public:
    // General
    void setStats(std::string name, int width, int height, int mode);
    int getWindowWidth(){return windowWidth;}
    int getWindowHeight(){return windowHeight;}
    void drawBegin();
    void drawUiBegin();
    void drawEnd();
    bool shouldClose();
    void clearColor();
    void clearDepth();
    void clearAll();
    // void drawImage(
    //     int textureId,
    //     glm::mat4 spriteTransform,
    //     glm::mat4 coordTransform
    // );
    void drawImage(int textureId, float x, float y);
    void drawImageExt(int textureId,
        float x, float y,
        float width, float height,
        float sx, float sy,
        float sw, float sh,
        float ox, float oy,
        float angle
    );
    GLFWwindow* getWindow();
    // Textures
    int newTexture(int width, int height, unsigned char* data);
    void freeTexture(int id);
    int loadImage(std::string path);
    bool saveTexture(const char* path, int id);
    Texture* getTexture(int id);
    void setRenderTarget(int id);
    // Shaders
    int newShader(std::string vertexSource, std::string fragmentSource);
    void freeShader(int id);
    Shader* getShader(int id);
    // Meshes
    int newMesh(std::vector<float> &data, std::vector<int> attribSizes);
    int loadObj(std::string path);
    void freeMesh(int id);
    Mesh* getMesh(int id);
    protected:
    bool initInternal();
    void cleanupInternal();
    private:
    GLFWwindow* window = NULL;
    std::string windowName = "Avernus";
    int windowWidth = 640;
    int windowHeight = 480;
    int windowMode = 0;
    int baseTextureId;
    Store<Texture> textureStore;
    Store<Shader> shaderStore;
    Store<Mesh> meshStore;
    Shader* spriteShader;
    Mesh* quadMesh;
    void drawImageInternal(
        Texture* tex,
        glm::mat4 cameraTransform,
        glm::mat4 spriteTransform,
        glm::mat4 coordTransform
    );
};