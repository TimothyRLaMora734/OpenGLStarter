#include <aribeiro/aribeiro.h>
using namespace aRibeiro;
#include "App.h"

App::App(int w, int h):AppBase(w,h){
    renderState = GLRenderState::getInstance();
    
    shaderColor = new GLShaderColor();
    
    //initialize all matrix
    projection = mat4::IdentityMatrix;
    camera = mat4::IdentityMatrix;
    modelHierarchy = mat4::IdentityMatrix;
    
    //initialize auxiliary variables
    cameraPosition = vec3(0.0f,0.4f,2.0f);
    angle_rad = 0;
    rotateCounterClockwise = true;
    objectPosition = vec3(0,-1.5f,-1);
    
    //listen the resize window event
    WindowSize.OnChange.add(this, &App::OnWindowResize);
    OnWindowResize(&WindowSize);

    //setup renderstate
    renderState->ClearColor = vec4(1.0f,1.0f,250.0f/255.0f,1.0f);
    renderState->Wireframe = WireframeBack;
    renderState->CullFace = CullFaceNone;
    
    time.update();
}

App::~App(){
    setNullAndDelete(shaderColor);
}

void App::OnWindowResize(Property<iSize> *prop){
    iSize size = prop->value;
    // configure the projection
    projection = projection_perspective(60.0f, (float)size.x/(float)size.y, 0.01f, 10.0f);
}

void App::draw() {
    time.update();
    processInput();
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //update internal parameters
    if (rotateCounterClockwise)
        angle_rad += time.deltaTime * DEG2RAD(200.0f);
    else
        angle_rad -= time.deltaTime * DEG2RAD(200.0f) + 2.0*PI*10.0f;
    angle_rad = fmod(angle_rad, 2.0*PI*10.0f);

    //compute the camera projection matrix
    camera = inv(translate(cameraPosition));
    mat4 viewProjection = (mat4)projection * (mat4)camera;
    
    //setup hierarchy models and draw
    modelHierarchy = mat4::IdentityMatrix;//reset transform
    
    //
    // draw Ground
    //
    const vec3 groundVertexBuffer[] = {
        vec3(-1.0f,0.0f, 1.0f),
        vec3( 1.0f,0.0f, 1.0f),
        vec3( 1.0f,0.0f, -1.0f),
        vec3(-1.0f,0.0f,-1.0f),
    };

    
    modelHierarchy.push();
    modelHierarchy = (mat4)modelHierarchy * scale(2.0f,2.0f,2.0f);
    mat4 modelViewProjection = viewProjection * (mat4)modelHierarchy;
    drawPrimitive(GL_QUADS,modelViewProjection,groundVertexBuffer, vec4(0.5f,0.5f,0.5f,1.0f), 4 );
    modelHierarchy.pop();
    
    //
    // draw Triangles
    //
    const vec3 vertexBuffer[] = {
        vec3(-0.5f,-0.0f,0.0f),
        vec3(0.5f,-0.0f,0.0f),
        vec3(0.0f,1.0f,0.0f)
    };
    
    modelHierarchy.push();
    modelHierarchy = (mat4)modelHierarchy *
                     translate(0.0f,0.4f,0.0f) *
                     eulerRotate(0.0f, angle_rad, angle_rad*0.1f) *
                     scale(0.2f,0.2f,0.2f);
    modelViewProjection = viewProjection * (mat4)modelHierarchy;
    drawPrimitive(GL_TRIANGLES,modelViewProjection,vertexBuffer, vec4(1.0f,0.0f,0.0f,1.0f), 3 );
    
    //
    // draw children on X
    //
    
    modelHierarchy.push();
    modelHierarchy = (mat4)modelHierarchy * translate(1.0f, 0, 0);
    modelViewProjection = viewProjection * (mat4)modelHierarchy;
    drawPrimitive(GL_TRIANGLES,modelViewProjection,vertexBuffer, vec4(0.0f,1.0f,0.0f,1.0f), 3 );
    modelHierarchy.pop();
    
    //
    // draw children on -X
    //
    
    modelHierarchy.push();
    modelHierarchy = (mat4)modelHierarchy * translate(-1.0f, 0, 0);
    modelViewProjection = viewProjection * (mat4)modelHierarchy;
    drawPrimitive(GL_TRIANGLES,modelViewProjection,vertexBuffer, vec4(0.0f,0.0f,1.0f,1.0f), 3 );
    modelHierarchy.pop();
    
    modelHierarchy.pop();
    
    
    objectPosition = move(objectPosition,  vec3(0,1,-1), time.deltaTime * 0.25f);
    
    modelHierarchy.push();
    modelHierarchy = (mat4)modelHierarchy * translate(objectPosition);
    modelViewProjection = viewProjection * (mat4)modelHierarchy;
    drawPrimitive(GL_TRIANGLES,modelViewProjection,vertexBuffer, vec4(0.0f,1.0f,1.0f,1.0f), 3 );
    modelHierarchy.pop();

    if (sqrDistance(objectPosition,vec3(0,1,-1)) <= 1e-6f)
        objectPosition = vec3(0,-1.5f,-1);
}

void App::processInput() {
    left.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) );
    right.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) );
    up.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) );
    down.setState( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) );
    
    if (left.down)
        rotateCounterClockwise = true;
    if (right.down)
        rotateCounterClockwise = false;
    
    const float speed = 3.0f;
    if (up.pressed)
        cameraPosition.z -= time.deltaTime * speed;
    if (down.pressed)
        cameraPosition.z += time.deltaTime * speed;
    /*
     if (left.pressed)
     cameraPosition.x -= time.deltaTime * speed;
     if (right.pressed)
     cameraPosition.x += time.deltaTime * speed;
     */
}

void App::drawPrimitive(GLuint oglPrimitive, const mat4 &modelViewProjection, const vec3 vertexBuffer[], const vec4 &color, int count){
    
    renderState->CurrentShader = shaderColor;
    
    shaderColor->setMatrix(modelViewProjection);
    shaderColor->setColor(color);
    
    OPENGL_CMD(glEnableVertexAttribArray(GLShaderColor::vPosition));
    OPENGL_CMD(glVertexAttribPointer(GLShaderColor::vPosition, 3, GL_FLOAT, false, sizeof(vec3), &vertexBuffer[0]));
    OPENGL_CMD(glDrawArrays(oglPrimitive, 0, count));
    OPENGL_CMD(glDisableVertexAttribArray(GLShaderColor::vPosition));
}
