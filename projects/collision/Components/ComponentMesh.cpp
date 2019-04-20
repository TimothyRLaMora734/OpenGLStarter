#include "ComponentMesh.h"




void setTriangle(std::vector<unsigned short> *indices,
                 std::vector<vec3> *vertices,
                 std::vector<vec2> *uv,
                 std::vector<vec3> *normals,
                 std::vector<vec3> *tangents,
                 std::vector<vec3> *binormals) {
    vertices->clear();
    vertices->push_back(vec3(-0.5f,0.0f,0.0f));
    vertices->push_back(vec3(0.5f,0.0f,0.0f));
    vertices->push_back(vec3(0.0f,1.0f,0.0f));
    
    if (uv != NULL){
        uv->clear();
        uv->push_back(vec2(0.0f,0.0f));
        uv->push_back(vec2(1.0f,0.0f));
        uv->push_back(vec2(0.5f,0.5f));
    }
    
    if (normals != NULL){
        normals->clear();
        normals->push_back(vec3(0.0f,0.0f,1.0f));
        normals->push_back(vec3(0.0f,0.0f,1.0f));
        normals->push_back(vec3(0.0f,0.0f,1.0f));
    }
    
    if (tangents != NULL){
        tangents->clear();
        tangents->push_back(vec3(1.0f,0.0f,0.0f));
        tangents->push_back(vec3(1.0f,0.0f,0.0f));
        tangents->push_back(vec3(1.0f,0.0f,0.0f));
    }
    
    if (binormals != NULL){
        binormals->clear();
        binormals->push_back(vec3(0.0f,1.0f,0.0f));
        binormals->push_back(vec3(0.0f,1.0f,0.0f));
        binormals->push_back(vec3(0.0f,1.0f,0.0f));
    }
    
    indices->clear();
    for (int i = 0; i < vertices->size(); i += 3) {
        indices->push_back(i);
        indices->push_back(i+1);
        indices->push_back(i+2);
    }
}

void setPlane(const vec3 &dimension,
              std::vector<unsigned short> *indices,
              std::vector<vec3> *vertices,
              std::vector<vec2> *uv,
              std::vector<vec3> *normals,
              std::vector<vec3> *tangents,
              std::vector<vec3> *binormals) {
    
    vec3 halfDim = dimension * 0.5f;
    
    vertices->clear();
    vertices->push_back(vec3(halfDim.x, 0, halfDim.z));
    vertices->push_back(vec3(halfDim.x, 0, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, 0, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, 0, halfDim.z));
    
    indices->clear();
    for (int i = 0; i < vertices->size(); i += 4) {
        indices->push_back(i);
        indices->push_back(i+1);
        indices->push_back(i+2);
        
        indices->push_back(i);
        indices->push_back(i + 2);
        indices->push_back(i + 3);
    }
    
    if (uv != NULL){
        uv->clear();
        uv->push_back(vec2(0.0f,0.0f));
        uv->push_back(vec2(0.0f,1.0f));
        uv->push_back(vec2(1.0f,1.0f));
        uv->push_back(vec2(1.0f,0.0f));
    }
    
    if (normals != NULL){
        normals->clear();
        normals->push_back(vec3(0.0f,1.0f,0.0f));
        normals->push_back(vec3(0.0f,1.0f,0.0f));
        normals->push_back(vec3(0.0f,1.0f,0.0f));
    }
    
    if (tangents != NULL){
        tangents->clear();
        tangents->push_back(vec3(1.0f,0.0f,0.0f));
        tangents->push_back(vec3(1.0f,0.0f,0.0f));
        tangents->push_back(vec3(1.0f,0.0f,0.0f));
    }
    
    if (binormals != NULL){
        binormals->clear();
        binormals->push_back(vec3(0.0f,0.0f,1.0f));
        binormals->push_back(vec3(0.0f,0.0f,1.0f));
        binormals->push_back(vec3(0.0f,0.0f,1.0f));
    }
    
}



void setBox(const vec3 &dimension,
            std::vector<unsigned short> *indices,
            std::vector<vec3> *vertices,
            std::vector<vec2> *uv,
            std::vector<vec3> *normals,
            std::vector<vec3> *tangents,
            std::vector<vec3> *binormals) {
    vec3 halfDim = dimension * 0.5f;
    //
    // TOP
    //
    vertices->clear();
    vertices->push_back(vec3(halfDim.x, halfDim.y, halfDim.z));
    vertices->push_back(vec3(halfDim.x, halfDim.y, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, halfDim.y, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, halfDim.y, halfDim.z));
    //
    // BOTTOM
    //
    vertices->push_back(vec3(halfDim.x, -halfDim.y, halfDim.z));
    vertices->push_back(vec3(-halfDim.x, -halfDim.y, halfDim.z));
    vertices->push_back(vec3(-halfDim.x, -halfDim.y, -halfDim.z));
    vertices->push_back(vec3(halfDim.x, -halfDim.y, -halfDim.z));
    //
    // RIGHT
    //
    vertices->push_back(vec3(halfDim.x, halfDim.y, halfDim.z));
    vertices->push_back(vec3(halfDim.x, -halfDim.y, halfDim.z));
    vertices->push_back(vec3(halfDim.x, -halfDim.y, -halfDim.z));
    vertices->push_back(vec3(halfDim.x, halfDim.y, -halfDim.z));
    //
    // LEFT
    //
    vertices->push_back(vec3(-halfDim.x, halfDim.y, halfDim.z));
    vertices->push_back(vec3(-halfDim.x, halfDim.y, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, -halfDim.y, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, -halfDim.y, halfDim.z));
    //
    // FRONT
    //
    vertices->push_back(vec3(-halfDim.x, halfDim.y, halfDim.z));
    vertices->push_back(vec3(-halfDim.x, -halfDim.y, halfDim.z));
    vertices->push_back(vec3(halfDim.x, -halfDim.y, halfDim.z));
    vertices->push_back(vec3(halfDim.x, halfDim.y, halfDim.z));
    //
    // BACK
    //
    vertices->push_back(vec3(-halfDim.x, halfDim.y, -halfDim.z));
    vertices->push_back(vec3(halfDim.x, halfDim.y, -halfDim.z));
    vertices->push_back(vec3(halfDim.x, -halfDim.y, -halfDim.z));
    vertices->push_back(vec3(-halfDim.x, -halfDim.y, -halfDim.z));
    
    if (normals != NULL){
        //
        // TOP
        //
        normals->clear();
        normals->push_back(vec3(0,1,0));
        normals->push_back(vec3(0,1,0));
        normals->push_back(vec3(0,1,0));
        normals->push_back(vec3(0,1,0));
        //
        // BOTTOM
        //
        normals->push_back(vec3(0,-1,0));
        normals->push_back(vec3(0,-1,0));
        normals->push_back(vec3(0,-1,0));
        normals->push_back(vec3(0,-1,0));
        //
        // RIGHT
        //
        normals->push_back(vec3(1,0,0));
        normals->push_back(vec3(1,0,0));
        normals->push_back(vec3(1,0,0));
        normals->push_back(vec3(1,0,0));
        //
        // LEFT
        //
        normals->push_back(vec3(-1,0,0));
        normals->push_back(vec3(-1,0,0));
        normals->push_back(vec3(-1,0,0));
        normals->push_back(vec3(-1,0,0));
        //
        // FRONT
        //
        normals->push_back(vec3(0,0,1));
        normals->push_back(vec3(0,0,1));
        normals->push_back(vec3(0,0,1));
        normals->push_back(vec3(0,0,1));
        //
        // BACK
        //
        normals->push_back(vec3(0,0,-1));
        normals->push_back(vec3(0,0,-1));
        normals->push_back(vec3(0,0,-1));
        normals->push_back(vec3(0,0,-1));
    }
    //
    // Construct Index
    //
    indices->clear();
    for (int i = 0; i < vertices->size(); i += 4) {
        indices->push_back(i);
        indices->push_back(i+1);
        indices->push_back(i+2);
        
        indices->push_back(i);
        indices->push_back(i + 2);
        indices->push_back(i + 3);
    }
}



void setSphere(float radius, int sectorCount, int stackCount,
               std::vector<unsigned short> *indices,
               std::vector<vec3> *vertices,
               std::vector<vec2> *uv,
               std::vector<vec3> *normals,
               std::vector<vec3> *tangents,
               std::vector<vec3> *binormals
               ) {
    
    //clear allbuffers
    indices->clear();
    vertices->clear();
    if (uv != NULL)
        uv->clear();
    if (normals != NULL)
        normals->clear();
    if (tangents != NULL)
        tangents->clear();
    if (binormals != NULL)
        binormals->clear();
    
    
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
            
            vec3 p = vec3(x, z, -y);
            vec3 normal = vec3(nx, nz, -ny);
            vec3 tangent = normalize(cross(vec3(0, 1, 0), normal));
            vec3 binormal = normalize(cross(normal, tangent));
            
            /*
             GeneralVertexAttrib va(
             p,// position;
             vec2(s, t),// uv;
             normal,// normal;
             tangent,// tangent;
             binormal//vec3(0),// binormal;
             );
             
             result->vertices.push_back(va);
             */
            
            vertices->push_back(p);
            if (uv != NULL)
                uv->push_back(vec2(s, t));
            if (normals != NULL)
                normals->push_back(normal);
            if (tangents != NULL)
                tangents->push_back(tangent);
            if (binormals != NULL)
                binormals->push_back(binormal);
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
        
        if (tangents != NULL)
            (*tangents)[index1] = normalize((*tangents)[index2] + (*tangents)[index3]);
        if (binormals != NULL && normals != NULL && tangents != NULL)
            (*binormals)[index1] = normalize(cross((*normals)[index1], (*tangents)[index1]));
        /*
         result->vertices[index1].tangent = normalize(result->vertices[index2].tangent + result->vertices[index3].tangent);
         result->vertices[index1].binormal = normalize(cross(result->vertices[index1].normal, result->vertices[index1].tangent));
         */
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
        
        if (tangents != NULL)
            (*tangents)[index2] = normalize((*tangents)[index1] + (*tangents)[index3]);
        if (binormals != NULL && normals != NULL && tangents != NULL)
            (*binormals)[index2] = normalize(cross((*normals)[index2], (*tangents)[index2]));
        
        /*
         result->vertices[index2].tangent = normalize(result->vertices[index1].tangent + result->vertices[index3].tangent);
         result->vertices[index2].binormal = normalize(cross(result->vertices[index2].normal, result->vertices[index2].tangent));
         */
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
                indices->push_back(k1);
                indices->push_back(k2);
                indices->push_back(k1 + 1);
            }
            
            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices->push_back(k1 + 1);
                indices->push_back(k2);
                indices->push_back(k2 + 1);
            }
        }
    }
}


