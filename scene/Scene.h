#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"
#include "gl/textures/Texture2D.h"
#include "camera/CamtransCamera.h"

#include <sstream>
#include <QImage>

#include "lib/ResourceLoader.h"
#include "lib/CS123SceneData.h"
#include "shapes/Cylinder.h"
#include "shapes/Grass.h"
#include "shapes/Plane.h"
#include "shapes/Leaf.h"
#include "shapes/Sphere.h"
#include "gl/textures/TextureParameters.h"
#include "gl/textures/TextureParametersBuilder.h"
#include "trees/LSystem.h"
#include "trees/Turtle.h"
#include "shapes/Tree.h"
#include "gl/datatype/FBO.h"
#include "shapes/OpenGLShape.h"

#include "gl/GLDebug.h" // useful for debugging shader stuff

const int minBound = -10;
const int maxBound = 10;

class OpenGLShape;

namespace CS123 { namespace GL {
        class Shader;
        class CS123Shader;
        class Texture2D;
}}

class Scene
{
public:
    Scene();
    ~Scene();

    void render();
    void crepscularRayPass();
    CamtransCamera *getCamera();

    void updateDimensions(int width, int height);

private:
    void initializeTrees();
    void groundPass(bool occluded);
    void grassPass(bool occluded, glm::mat4x4 trans);
    void treePass(bool occluded, glm::mat4x4 trans, int t);
    void sunPass();
    void renderPrimitives(bool occluded);
    float scatterPoint(float cur, float scatter);

    // precalculates tree and grass positions in scene
    void initializeTreePositions();
    void initializeGrassPositions();

    std::unique_ptr<CS123::GL::CS123Shader> m_shader;
    std::unique_ptr<CS123::GL::Shader> m_crepscularRayShader;

    std::unique_ptr<CS123::GL::Texture2D> m_grassTexture;
    std::unique_ptr<CS123::GL::Texture2D> m_barkTexture;


    CamtransCamera m_camera;
    CS123SceneMaterial m_woodMaterial;
    CS123SceneMaterial m_leafMaterial;
    CS123SceneMaterial m_occludedMaterial; // black material used for making occluded scene
    CS123SceneMaterial m_whiteMaterial; // white material used for making sun

    std::unique_ptr<TexturedShape> m_ground;
    std::unique_ptr<TexturedShape> m_grass;
    std::unique_ptr<Leaf> m_leaf;
    std::unique_ptr<Shape> m_branch;
    std::vector<Tree> m_trees;
    std::unique_ptr<Sphere> m_sun;

    std::unique_ptr<OpenGLShape> m_quad;
    std::unique_ptr<CS123::GL::FBO> m_occludedPass; // pass of occluded version of scene
    std::unique_ptr<CS123::GL::FBO> m_regularPass; // normal scene render used for crepscular rays

    int m_width; // width of viewport
    int m_height; // height of viewport

    // precalculated semi-random positions for trees and grass
    std::vector<glm::mat4x4> m_treeTrans;
    std::vector<glm::mat4x4> m_grassTrans;
};

#endif // SCENE_H
