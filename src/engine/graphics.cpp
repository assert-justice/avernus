#include "graphics.hpp"
#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "utils/fs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tiny_obj_loader.h"

float quad[] = {
    1.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top right
    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left

    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left
    0.0f, 0.0f, 0.0f,     0.0f, 0.0f, // bottom left
};

float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static const char* spriteVertSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec2 aTexCoord;"
    "out vec2 TexCoord;"
    "uniform mat4 camera;"
    "uniform mat4 sprite;"
    "uniform mat4 coord;"
    "void main()"
    "{"
    "    gl_Position = camera * sprite * vec4(aPos, 1.0);"
    "    TexCoord = (coord * vec4(aTexCoord, 1.0, 1.0)).xy;"
    "}";

static const char* spriteFragSrc = "#version 330 core\n"
    "out vec4 FragColor;"
    "in vec2 TexCoord;"
    "uniform sampler2D ourTexture;"
    "void main()"
    "{"
    "    FragColor = texture(ourTexture, TexCoord);"
    "}";

void Graphics::setStats(std::string name, int width, int height, int mode)
{
    windowName = name;
    windowWidth = width;
    windowHeight = height;
    windowMode = mode;
    if(window == NULL) return;
    printf("Resizing the window at runtime is not yet supported\n");
}

void Graphics::drawBegin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    clearAll();
}

void Graphics::drawUiBegin()
{
    getTexture(baseTextureId)->useTarget();
    // glClearColor(0, 0, 0, 0);
    clearColor();
}

void Graphics::drawEnd()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Draw baseTexture
    Texture* tex = getTexture(baseTextureId);
    auto rootTransform = glm::ortho(0.0f, (float)windowWidth, float(windowHeight), 0.0f, -0.1f, 100.0f);
    auto spriteTransform = glm::mat4(1.0f);
    spriteTransform = glm::scale(spriteTransform, glm::vec3(tex->width ,tex->height, 0.0f));
    drawImageInternal(
        tex,
        rootTransform,
        spriteTransform,
        glm::mat4(1.0f)  
    );
    glfwSwapBuffers(window);
}

bool Graphics::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Graphics::clearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::clearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Graphics::clearAll()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::drawImage(int textureId, float x, float y)
{
    auto tex = getTexture(textureId);
    if(tex == nullptr) return;
    glm::mat4 cameraTransform = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -0.1f, 100.0f);
    glm::mat4 spriteTransform = glm::mat4(1);
    spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0));
    spriteTransform = glm::scale(spriteTransform, glm::vec3(tex->width, tex->height, 0));
    glm::mat4 coordTransform = glm::mat4(1);
    drawImageInternal(tex, cameraTransform, spriteTransform, coordTransform);
}

void Graphics::drawImageExt(int textureId, float x, float y, float width, float height, float sx, float sy, float sw, float sh, float ox, float oy, float angle)
{
    auto tex = getTexture(textureId);
    if(tex == nullptr) return;
    glm::mat4 cameraTransform = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -0.1f, 100.0f);
    glm::mat4 spriteTransform = glm::mat4(1);
    // spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0));
    // spriteTransform = glm::scale(spriteTransform, glm::vec3(width, height, 0));
    spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0.0f));
    spriteTransform = glm::rotate(spriteTransform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    spriteTransform = glm::translate(spriteTransform, glm::vec3(-ox/sw*width, -oy/sh*height, 0.0f));
    spriteTransform = glm::scale(spriteTransform, glm::vec3(width,height, 0.0f));
    float sigma = 0.001;
    glm::mat4 coordTransform = glm::mat4(1);
    coordTransform = glm::translate(coordTransform, glm::vec3(glm::vec3(sx/tex->width+sigma, sy/tex->height+sigma, 0.0)));
    coordTransform = glm::scale(coordTransform, glm::vec3(sw/tex->width - 2 * sigma, sh/tex->height - 2 * sigma, 0.0));

    drawImageInternal(tex, cameraTransform, spriteTransform, coordTransform);
}

GLFWwindow* Graphics::getWindow()
{
    return window;
}

int Graphics::newTexture(int width, int height, unsigned char *data)
{
    auto ptr = new Texture(width, height, data);
    return textureStore.add(ptr);
}

void Graphics::freeTexture(int id)
{
    textureStore.del(id);
}

int Graphics::loadImage(std::string path)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4); 
    if(!data) return -1;
    int id = newTexture(width, height, data);
    stbi_image_free(data);
    return id;
}

bool Graphics::saveTexture(const char *path, int id)
{
    Texture* tex = nullptr;
    tex = textureStore.get(id);
    // stbi_flip_vertically_on_write(id == -1);
    if(tex == nullptr) return false;
    auto data = (uint8_t*)calloc(tex->width * tex->height * 4, sizeof(uint8_t));
    glReadPixels(0, 0, tex->width, tex->height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_write_png(path, tex->width, tex->height, 4, data, 4 * tex->width);
    free(data);
    return true;
}

Texture *Graphics::getTexture(int id)
{
    auto tex = textureStore.get(id);
    if(tex == nullptr){
        std::cout << "Invalid texture id '" << id << "'" << std::endl;
    }
    return tex;
}

void Graphics::setRenderTarget(int id)
{
    if(id == -1){
        // reset render target
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    auto fb = getTexture(id);
    fb->useTarget();
}

int Graphics::newShader(std::string vertexSource, std::string fragmentSource){
    auto shader = new Shader();
    bool hasError = false;
    shader->initialize(&hasError, vertexSource, fragmentSource);
    if(hasError) return -1;
    return shaderStore.add(shader);
}

void Graphics::freeShader(int id){
    if(id < 1) return; // Do not free built in sprite shader
    shaderStore.del(id);
}

Shader* Graphics::getShader(int id){
    auto shader = shaderStore.get(id);
    if(shader == nullptr){
        std::cout << "Invalid shader id '" << id << "'" << std::endl;
    }
    return shader;
}

int Graphics::newMesh(std::vector<float> &data, std::vector<int> attribSizes)
{
    auto mesh = new Mesh(data, attribSizes);
    return meshStore.add(mesh);
}

int Graphics::loadObj(std::string path)
{
    // TODO: support multiple shapes. Return a vector of mesh indicies?
    // TODO: Handle mtl file path better.
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./models"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
        return -1;
    }
    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();
    if(shapes.size() > 1){
        std::cout << "Only supports one shape obj files at this time\n";
        return -1;
    }
    for (size_t s = 0; s < shapes.size(); s++) {
        std::vector<float> data;
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
        
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
            
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];
            
                // Check if `normal_index` is zero or positive. negative = no normal data
                // if (idx.normal_index >= 0) {
                //     tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                //     tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                //     tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                // }
                data.push_back(vx);
                data.push_back(vy);
                data.push_back(vz);
            
                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    data.push_back(tx);
                    data.push_back(ty);
                }
                else{
                    printf("no tex coords!\n");
                    data.push_back(0);
                    data.push_back(0);
                }
                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;
        
            // per-face material
            shapes[s].mesh.material_ids[f];
        }
        int id = newMesh(data, {3,2});
        return id;
    }
    return 0;
}

void Graphics::freeMesh(int id)
{
    if(id < 2) return; // Do not delete built in quad and cube meshes
    meshStore.del(id);
}

Mesh *Graphics::getMesh(int id)
{
    if(meshStore.has(id)) return meshStore.get(id);
    std::cout << "Invalid mesh id '" << id << "'" << std::endl;
    return nullptr;
}

bool Graphics::initInternal()
{
    if(!glfwInit()){
        std::cout << "Failed to init glfw" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
    if (!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwSwapInterval(1);
    glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
    glEnable(GL_DEPTH_TEST); 
    std::vector<float> quadData(quad, quad + sizeof quad / sizeof quad[0]);
    std::vector<float> cubeData(cube, cube + sizeof(cube)/4);
    int qd = newMesh(quadData, {3, 2}); // Quad mesh id is 0
    quadMesh = getMesh(qd);
    newMesh(cubeData, {3, 2}); // Cube mesh id is 1
    int sprShaderId = newShader(spriteVertSrc, spriteFragSrc); // Sprite shader id is 0
    spriteShader = getShader(sprShaderId);
    baseTextureId = newTexture(windowWidth, windowHeight, NULL);
    return true;
}

void Graphics::cleanupInternal(){
    textureStore.clear();
    meshStore.clear();
    shaderStore.clear();
    glfwTerminate();
}

void Graphics::drawImageInternal(Texture *tex, glm::mat4 cameraTransform, glm::mat4 spriteTransform, glm::mat4 coordTransform)
{
    tex->use();
    spriteShader->use();
    spriteShader->setUniform("camera", cameraTransform);
    spriteShader->setUniform("sprite", spriteTransform);
    spriteShader->setUniform("coord", coordTransform);
    quadMesh->draw();
}
