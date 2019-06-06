
#ifndef GLPostProcess__h
#define GLPostProcess__h

#include <aribeiro/GLTexture.h>
#include <aribeiro/GLFramebufferObject.h>
#include <aribeiro/GLPostProcessingShader.h>
#include <vector>


namespace aRibeiro {

    class GLPostProcess;
    
    class iPostProcess{
    public:
        virtual void render(GLPostProcess *postProcess, GLTexture *colorSrc, GLTexture *depthSrc, GLFramebufferObject *dst) = 0;
    };
    
    class GLPostProcess{
        
    private:
        
        GLFramebufferObject* firstRender;
        GLFramebufferObject* fbo[2];
        int current;
        
        //private copy constructores, to avoid copy...
        GLPostProcess(const GLPostProcess& v);
        void operator=(const GLPostProcess& v);
        
    public:
        std::vector<iPostProcess*> pipeline;
        int width,height;
        
        GLPostProcess();
        virtual ~GLPostProcess();
        
        void resize(int w, int h);
        void beginDraw();
        void endDraw();
        
        static void drawQuad(GLPostProcessingShader *shader);

		SSE2_CLASS_NEW_OPERATOR
    };

}

#endif

