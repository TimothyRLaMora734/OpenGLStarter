#ifndef BloomThreshouldShader__h
#define BloomThreshouldShader__h

#include <aribeiro/GLPostProcessingShader.h>

namespace aRibeiro {
    
    class BloomThreshouldShader : public GLPostProcessingShader {
        
        int uSampler2DTexture;
        int uFloatThreshould;
        
    public:
        
        BloomThreshouldShader() {
            const char vertexShaderCode[] = {
                //attributes
                "attribute vec2 aVec2Position;"
                "attribute vec2 aVec2UV;"
                //varying
                "varying vec2 vVec2UV;"
                //vertex program
                "void main() {"
                    //pega as coordenadas de textura vindas da aplicacao
                    "vVec2UV = aVec2UV;"
                    //calcula a coordenada homogenea da projecao configurada
                    "gl_Position = vec4(aVec2Position, 0.0, 1.0);"
                "}"
            };
            
            const char fragmentShaderCode[] = {
                //"precision mediump float;"
                //uniforms
                "uniform sampler2D uSampler2DTexture;"
                "uniform float uFloatThreshould;"
                
                //varying
                "varying vec2 vVec2UV;"
                
                "void main() {"
                    "vec3 texel = texture2D(uSampler2DTexture, vVec2UV ).rgb;"
                    //"texel = step(vec3(uFloatThreshould), texel);"//faz cor ir de 0 a 1
                    "texel = texel * step(vec3(uFloatThreshould), texel);"
                    /*"texel.r = texel.r * step(uFloatThreshould, texel.r);"
                    "texel.g = texel.g * step(uFloatThreshould, texel.g);"
                    "texel.b = texel.b * step(uFloatThreshould, texel.b);"*/
                    "vec4 result = vec4(texel, 1.0);"
                    "gl_FragColor = result;"
                "}"
            };
            
            LoadShaderProgram(vertexShaderCode, fragmentShaderCode);
            
            aVec2Position = getAttribLocation("aVec2Position");
            aVec2UV = getAttribLocation("aVec2UV");
            
            uSampler2DTexture = getUniformLocation("uSampler2DTexture");
            uFloatThreshould = getUniformLocation("uFloatThreshould");
            
        }
        
        void setTexture(int texUnit){
            setUniform(uSampler2DTexture, texUnit);
        }
        void setThreshould(float v){
            setUniform(uFloatThreshould, v);
        }
    };
    
}

#endif

