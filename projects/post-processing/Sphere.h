#ifndef Sphere__H
#define Sphere__H

#include "RenderSystem.h"

class VBO {
public:
};

class VAO {
public:
};

class Sphere {
public:
    Sphere(int sectorCount, int stackCount, float radius) {
        std::vector<VertexAttrib> vertices;
        std::vector<unsigned short> indices;
        
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord
        
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;
        
        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)
            
            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi
                
                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                
                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                
                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                
                vec3 p = vec3(x,z,-y);
                vec3 normal = vec3(nx,nz,-ny);
                vec3 tangent = normalize( cross(vec3(0,1,0),normal) );
                vec3 binormal = normalize( cross(normal, tangent) );
                VertexAttrib va = {
                    p,// position;
                    vec2(s,t),// uv;
                    normal,// normal;
                    tangent,// tangent;
                    binormal//vec3(0),// binormal;
                };
                
                vertices.push_back(va);
            }
        }
        
        int k1, k2;
        int i = 0;
        
        //
        // Fix tangent and binormal from poles
        //
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack
        
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // k1+1 => k2 => k2+1
            int index1 = k1 + 1;
            int index2 = k2;
            int index3 = k2 + 1;
            
            vertices[index1].tangent = normalize( vertices[index2].tangent + vertices[index3].tangent );
            vertices[index1].binormal = normalize( cross( vertices[index1].normal, vertices[index1].tangent ) );
        }
        
        i = (stackCount - 1);
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack
        
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // k1+1 => k2 => k2+1
            int index1 = k1;
            int index2 = k2;
            int index3 = k1 + 1;
            
            vertices[index2].tangent = normalize( vertices[index1].tangent + vertices[index3].tangent );
            vertices[index2].binormal = normalize( cross( vertices[index2].normal, vertices[index2].tangent ) );
        }
        
        
        for (i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack
            
            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                
                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
};

#endif
