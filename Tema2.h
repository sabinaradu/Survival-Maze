#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Tema2Camera.h"
#include "lab_m1/Tema2/Transform3D.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

        struct Position
        {
            float x;
            float y;
            float z;
        };


    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void GenerateMaze(int maze[25][25], int size, int i, int j);
        int CountVisited(int maze[25][25], int size, int i, int j);
        void Shuffle(int maze[], int size);
        void MoveProjectile(float deltaTimeSeconds);

    protected:
        implemented::Tema2Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool projectionType;

        float right;
        float left;
        float bottom;
        float top;
        float fov;
        float Z_NEAR;
        float Z_FAR;
        // TODO(student): If you need any other class variables, define them here.

        float playerX = 15;
        float playerY = 0;
        float playerZ = 15;
        float height = 1.15f;
        float headSize = 0.25f;
        float bodyHeight = 0.6f;
        float bodyWidth = 0.5f;
        float armHeight = 0.4f;
        float armWidth = 0.15f;
        float legWidth = 0.2f;
        float legHeight = 0.35f;
        float footWidth = 0.2f;
        float footHeight = 0.07f;
        float pawHeight = 0.1f;
        float pawWidth = 0.15f;
        Position pos;
        Position camCenter;
        bool isFirstPerson = false;
        float cameraHeight = height - headSize / 2;
        int mazeSize = 25;
        int mazeMap[25][25];
        float wallHeight = 1.5f;
        float floorHeight = 0.15f;
        float floorWidth = mazeSize + 11;
        float angle = M_PI_2;
        bool keyPressed = false;
        float projectileSize = 0.1f;
        float projectileX, projectileY, projectileZ;
        float projectileAngle = angle;
        float projectileSpeed = 5;
        float projectileDistance = 3.5f;
        bool piu = false;
        bool ok = false;
    };
}   // namespace m1
