#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;



Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++)
        {
            mazeMap[i][j] = 1;
        }
    }
    Tema2::GenerateMaze(mazeMap, mazeSize, 0, 0);

    while (ok == false) {
        int a = rand() % 15 + 5;
        int b = rand() % 14 + 6;

        if (mazeMap[a][b] == 0 && mazeMap[a][b-1] == 0) {
            ok = true;
            playerX = a + 0.5f - mazeSize / 2;
            playerZ = -b - 0.5f + mazeSize / 2;
        }
    }


    renderCameraTarget = false;
    projectionType = true;

    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 30.f;
    Z_FAR = 100.f;
    Z_NEAR = .01f;

    camera = new implemented::Tema2Camera();
    camera->Set(glm::vec3(playerX, 1.5f, playerZ + 1.5f), glm::vec3(playerX, 1.5f, playerZ), glm::vec3(0, 1.5f, 0));
    camera->distanceToTarget = 1.5f;

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Shader* shader = new Shader("Skin");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Skin.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Shoes");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Shoes.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Shirt");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Shirt.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Trousers");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Trousers.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Wall");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Wall.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Floor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Floor.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Projectile");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader - Projectile.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema2::GenerateMaze(int maze[25][25], int size, int i, int j)
{
    int direct[][2] = { {0,1}, {0,-1}, {-1,0}, {1,0} };
    int visitOrder[] = { 0,1,2,3 };

    if (i < 0 || j < 0 || i >= size || j >= size)
        return;

    if (maze[i][j] == 0)
        return;

    if (Tema2::CountVisited(maze, size, i, j) > 1)
        return;

    maze[i][j] = 0;
    Tema2::Shuffle(visitOrder, 4);

    for (int k = 0; k < 4; ++k)
    {
        int nexti = i + direct[visitOrder[k]][0];
        int nextj = j + direct[visitOrder[k]][1];
        Tema2::GenerateMaze(maze, size, nexti, nextj);
    }
}

int Tema2::CountVisited(int maze[25][25], int size, int i, int j)
{
    int direct[][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    int count = 0;

    for (int index = 0; index < 4; index++)
    {
        int nexti = i + direct[index][0];
        int nextj = j + direct[index][1];

        if (nexti < 0 || nexti >= size || nextj < 0 || nextj >= size)
            continue;

        if (maze[nexti][nextj] == 0)
            count++;

    }
    return count;
}


void Tema2::Shuffle(int maze[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        int randomIndex = rand() % size;
        int aux = maze[i];
        maze[i] = maze[randomIndex];
        maze[randomIndex] = aux;

    }
}

void Tema2::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    //head
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, height, playerZ);
        modelMatrix *= Transform3D::Scale(-headSize, -headSize, -headSize);
        modelMatrix *= Transform3D::RotateOY(angle);
        RenderMesh(meshes["box"], shaders["Skin"], modelMatrix);
    }

    //body
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, height - headSize / 2 - bodyHeight / 2, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Scale(-bodyWidth, -bodyHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Shirt"], modelMatrix);
    }

    //arms
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(bodyWidth / 2 + armWidth / 2, height - headSize / 2 - armHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-armWidth, -armHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Shirt"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(-bodyWidth / 2 - armWidth / 2, height - headSize / 2 - armHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-armWidth, -armHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Shirt"], modelMatrix);
    }

    //legs
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(-bodyWidth / 2 + legWidth / 2, height - headSize / 2 - bodyHeight - legHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-legWidth, -legHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Trousers"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(bodyWidth / 2 - legWidth / 2, height - headSize / 2 - bodyHeight - legHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-legWidth, -legHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Trousers"], modelMatrix);
    }

    //happy feet
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(-bodyWidth / 2 + legWidth / 2, height - headSize / 2 - bodyHeight - legHeight - footHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-footWidth, -footHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Shoes"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(bodyWidth / 2 - legWidth / 2, height - headSize / 2 - bodyHeight - legHeight - footHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-footWidth, -footHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Shoes"], modelMatrix);
    }

    //paws
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(bodyWidth / 2 + armWidth / 2, height - headSize / 2 - armHeight - pawHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-pawWidth, -pawHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Skin"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= Transform3D::Translate(-bodyWidth / 2 - armWidth / 2, height - headSize / 2 - armHeight - pawHeight / 2, 0);
        modelMatrix *= Transform3D::Scale(-pawWidth, -pawHeight, -headSize);
        RenderMesh(meshes["box"], shaders["Skin"], modelMatrix);
    }

    //walls
    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++)
        {
            if (mazeMap[i][j] == 1)
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix *= Transform3D::Translate(i + 0.5f - mazeSize / 2, wallHeight / 2, -j - 0.5f + mazeSize / 2);
                modelMatrix *= Transform3D::Scale(1, wallHeight, 1);
                RenderMesh(meshes["box"], shaders["Wall"], modelMatrix);
            }
        }
    }

    for (int i = 0; i < mazeSize + 10; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(i + 0.5f - mazeSize / 2 - 5, wallHeight / 2, -mazeSize - 5 - 0.5f + mazeSize / 2);
        modelMatrix *= Transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Wall"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(i - 0.5f - mazeSize / 2 - 5, wallHeight / 2, mazeSize + 4 - 0.5f - mazeSize / 2);
        modelMatrix *= Transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Wall"], modelMatrix);
    }


    for (int i = 0; i <= mazeSize + 10; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(mazeSize - 0.5f - mazeSize / 2 + 5, wallHeight / 2, -i + 5 - 0.5f + mazeSize / 2);
        modelMatrix *= Transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Wall"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(-mazeSize + 0.5f + mazeSize / 2 - 5, wallHeight / 2, i - 6 - 0.5f - mazeSize / 2);
        modelMatrix *= Transform3D::Scale(1, wallHeight, 1);
        RenderMesh(meshes["box"], shaders["Wall"], modelMatrix);
    }

    //floor
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= Transform3D::Translate(0, -floorHeight / 2, -1);
    modelMatrix *= Transform3D::Scale(floorWidth, floorHeight, floorWidth);
    RenderMesh(meshes["box"], shaders["Floor"], modelMatrix);

    //projectile
    if (piu) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Transform3D::Translate(projectileX, projectileY, projectileZ);
        modelMatrix *= Transform3D::Scale(projectileSize, projectileSize, projectileSize);
        RenderMesh(meshes["box"], shaders["Projectile"], modelMatrix);
    }
    Tema2::MoveProjectile(deltaTimeSeconds);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    int modelLocation = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    int viewLocation = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    int projectionLocation = glGetUniformLocation(shader->program, "Porjection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_V)) {
        camera->Set(glm::vec3(playerX, 1.5f, playerZ + 1.5f), glm::vec3(playerX, 1.5f, playerZ), glm::vec3(0, 1.5f, 0));
    }

    if (window->KeyHold(GLFW_KEY_O)) {
        angle += deltaTime;
        camera->RotateThirdPerson_OY(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_P)) {
        angle -= deltaTime;
        camera->RotateThirdPerson_OY(-deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        if (!keyPressed)
        {
            camera->TranslateForward(deltaTime * cameraSpeed);
            playerX += deltaTime * cameraSpeed * cos(angle);
            playerZ -= deltaTime * cameraSpeed * sin(angle);
        }
        else
        {
            camera->TranslateForward(deltaTime * cameraSpeed);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        if (!keyPressed)
        {
            camera->TranslateRight(-deltaTime * cameraSpeed);
            playerX -= deltaTime * cameraSpeed * sin(angle);
            playerZ -= deltaTime * cameraSpeed * cos(angle);
        }
        else
        {
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (!keyPressed)
        {
            camera->TranslateForward(-deltaTime * cameraSpeed);
            playerX -= deltaTime * cameraSpeed * cos(angle);
            playerZ += deltaTime * cameraSpeed * sin(angle);
        }
        else
        {
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        if (!keyPressed)
        {
            camera->TranslateRight(deltaTime * cameraSpeed);
            playerX += deltaTime * cameraSpeed * sin(angle);
            playerZ += deltaTime * cameraSpeed * cos(angle);
        }
        else
        {
            camera->TranslateRight(deltaTime * cameraSpeed);
        }
    }
}

void Tema2::MoveProjectile(float deltaTimeSeconds) {
    if (!piu) {
        projectileAngle = angle;
        projectileX = playerX;
        projectileY = height - headSize / 2;
        projectileZ = playerZ;
    }
    else {
        if (isFirstPerson) {
            projectileX += sin(projectileAngle + M_PI_2) * projectileSpeed * deltaTimeSeconds;
            projectileZ += cos(projectileAngle + M_PI_2) * projectileSpeed * deltaTimeSeconds;

            bool stop_right = projectileX > playerX + projectileDistance;
            bool stop_left = projectileX < playerX - projectileDistance;
            bool stop_up = projectileZ > playerZ + projectileDistance;
            bool stop_down = projectileZ < playerZ - projectileDistance;

            if (stop_right || stop_left || stop_up || stop_down) {
                projectileAngle = angle;
                projectileX = playerX;
                projectileY = height - headSize / 2;
                projectileZ = playerZ;
                projectileSpeed = 0;
                piu = false;
            }
        }
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (isFirstPerson) {
            pos.x = camera->position[0];
            pos.y = camera->position[1];
            pos.z = camera->position[2];

            pos.x = pos.x + (1.5f + headSize / 2 + 0.001f) * sin(angle - M_PI_2);
            pos.y = 1.5f;
            pos.z = pos.z + (1.5f + headSize / 2 + 0.001f) * cos(angle - M_PI_2);

            camCenter.x = pos.x;
            camCenter.y = pos.y;
            camCenter.z = pos.z;

            camCenter.x = camCenter.x - 0.001f * sin(angle - M_PI_2);
            camCenter.z = camCenter.z - 0.001f * cos(angle - M_PI_2);
            camera->distanceToTarget = 1.5f;

            camera->Set(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(camCenter.x, camCenter.y, camCenter.z), glm::vec3(0, 1, 0));
            isFirstPerson = false;
        }
        else {

            pos.x = camera->position[0];
            pos.y = camera->position[1];
            pos.z = camera->position[2];

            pos.x = pos.x - (1.5f + headSize / 2 + 0.001f) * sin(angle - M_PI_2);
            pos.y = height - headSize / 2;
            pos.z = pos.z - (1.5f + headSize / 2 + 0.001f) * cos(angle - M_PI_2);

            camera->distanceToTarget = -0.001f - headSize / 2;

            camCenter.x = pos.x;
            camCenter.y = pos.y;
            camCenter.z = pos.z;

            camCenter.x = camCenter.x - 0.001f * sin(angle - M_PI_2);
            camCenter.z = camCenter.z - 0.001f * cos(angle - M_PI_2);

            camera->Set(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(camCenter.x, camCenter.y, camCenter.z), glm::vec3(0, 1, 0));

            isFirstPerson = true;
        }
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;

            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
            renderCameraTarget = true;

            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (isFirstPerson) {
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
        {
            projectileSpeed += 3;
        }
    }

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        keyPressed = true;
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (isFirstPerson) {
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
        {
            piu = true;
        }
    }
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        keyPressed = false;
    }
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
