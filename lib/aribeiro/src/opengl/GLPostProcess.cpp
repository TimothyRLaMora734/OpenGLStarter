#include "GLPostProcess.h"
#include <aribeiro/common.h>
#include <aribeiro/SetNullAndDelete.h>
#include <aribeiro/PlatformGL.h>

namespace aRibeiro {

    //private copy constructores, to avoid copy...
    GLPostProcess::GLPostProcess(const GLPostProcess& v) {}
    void GLPostProcess::operator=(const GLPostProcess& v) {}
    
    GLPostProcess::GLPostProcess() {
        width = 32;
        height = 32;
        
        firstRender = new GLFramebufferObject();
        firstRender->depth = new GLTexture();
        firstRender->color.push_back(new GLTexture());
        firstRender->setSize(width, height);
        firstRender->attachTextures();
        
        for (int i=0;i<2;i++){
            fbo[i] = new GLFramebufferObject();
            fbo[i]->color.push_back(new GLTexture());
            fbo[i]->setSize(width, height);
            fbo[i]->attachTextures();
        }
        current = 0;
    }
    
    GLPostProcess::~GLPostProcess() {
        
        setNullAndDelete( firstRender->depth );
        setNullAndDelete( firstRender->color[0] );
        setNullAndDelete( firstRender );
        for (int i=0;i<2;i++){
            setNullAndDelete( fbo[i]->color[0] );
            setNullAndDelete( fbo[i] );
        }
        
    }
    
    void GLPostProcess::resize(int w, int h) {
        width = w;
        height = h;
        
        firstRender->setSize(w, h);
        for (int i=0;i<2;i++){
            fbo[i]->setSize(w, h);
        }
    }
    
    void GLPostProcess::beginDraw() {
        if (pipeline.size() == 0)
            return;
        
        firstRender->enable();
        glViewport(0,0,firstRender->width,firstRender->height);
    }
    
    void GLPostProcess::endDraw() {
        if (pipeline.size() == 0)
            return;
        
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        
        GLFramebufferObject* src = firstRender;
        GLFramebufferObject* dst = fbo[current];
        current = (current + 1) % 2;
        
        for (int i=0; i<pipeline.size(); i++) {
            if (i == pipeline.size() - 1){
                pipeline[i]->render( this, src->color[0], firstRender->depth, NULL);
            } else {
                pipeline[i]->render( this, src->color[0], firstRender->depth, dst);
                src = dst;
                dst = fbo[current];
                current = (current + 1) % 2;
            }
        }
        
        glPopAttrib();
    }
    
    
    void GLPostProcess::drawQuad(GLPostProcessingShader *shader) {
        const vec2 vpos[] = {
            vec2(-1, -1),
            vec2(1, -1),
            vec2(1,  1),
            vec2(-1,  1)
        };
        const vec2 vuv[] = {
            vec2(0, 0),
            vec2(1, 0),
            vec2(1, 1),
            vec2(0, 1)
        };
        
        OPENGL_CMD(glEnableVertexAttribArray(shader->aVec2Position));
        OPENGL_CMD(glVertexAttribPointer(shader->aVec2Position, 2, GL_FLOAT, false, sizeof(vec2), &vpos[0]));
        OPENGL_CMD(glEnableVertexAttribArray(shader->aVec2UV));
        OPENGL_CMD(glVertexAttribPointer(shader->aVec2UV, 2, GL_FLOAT, false, sizeof(vec2), &vuv[0]));
        
        OPENGL_CMD(glDrawArrays(GL_QUADS, 0, 4));
        
        OPENGL_CMD(glDisableVertexAttribArray(shader->aVec2Position));
        OPENGL_CMD(glDisableVertexAttribArray(shader->aVec2UV));
    }

}

