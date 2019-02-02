#ifndef model_container_h
#define model_container_h

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include <vector>
#include <map>

#include "BinaryReader.h"
#include "BinaryWriter.h"

#ifdef NDEBUG
#define assert_static(expression, Word)
#else
#define assert_static(expression, Word) \
		template<bool> struct _##Word##_; \
		template<> struct _##Word##_<true> { }; \
		template<> struct _##Word##_<(expression) == 0> { };
#endif


//assert_static(sizeof(int) == 4, Struct_Size_Serialization_Error)

//#define MAX_CHAR_COUNT_STRING 32

class Vec3Key{
public:
    float time;
    vec3 value;
};

class QuatKey{
public:
    float time;
    quat value;
};

// Defines how an animation channel behaves outside the defined time range.
enum AnimBehaviour
{
    // use the default node transformation
    AnimBehaviour_DEFAULT  = 0x0,
    // The nearest key value is used without interpolation
    AnimBehaviour_CONSTANT = 0x1,
    // The value of the nearest two keys is linearly extrapolated for the current time value.
    AnimBehaviour_LINEAR   = 0x2,
    // The animation is repeated.
    //  If the animation key go from n to m and the current
    //  time is t, use the value at (t-n) % (|m-n|).
    AnimBehaviour_REPEAT   = 0x3,
};


class NodeAnimation {
public:
    std::string nodeName;
    std::vector<Vec3Key> positionKeys;
    std::vector<QuatKey> rotationKeys;
    std::vector<Vec3Key> scalingKeys;
    
    AnimBehaviour preState;
    AnimBehaviour postState;

	void write(BinaryWriter* writer) {
		writer->writeString(nodeName);
		writer->write<unsigned char>(preState);
		writer->write<unsigned char>(postState);
		writer->writeVector<Vec3Key>(positionKeys);
		writer->writeVector<QuatKey>(rotationKeys);
		writer->writeVector<Vec3Key>(scalingKeys);
	}

	void read(BinaryReader* reader) {
		nodeName = reader->readString();
		preState = (AnimBehaviour)reader->read<unsigned char>();
		postState = (AnimBehaviour)reader->read<unsigned char>();
		reader->readVector<Vec3Key>(&positionKeys);
		reader->readVector<QuatKey>(&rotationKeys);
		reader->readVector<Vec3Key>(&scalingKeys);
	}
    
    NodeAnimation() {
        preState = AnimBehaviour_DEFAULT;
        postState = AnimBehaviour_DEFAULT;
    }
    
    NodeAnimation(const NodeAnimation& v) {
        (*this) = v;
    }
    
    void operator=(const NodeAnimation& v) {
        nodeName = v.nodeName;
        
        positionKeys = v.positionKeys;
        rotationKeys = v.rotationKeys;
        scalingKeys = v.scalingKeys;
        
        preState = v.preState;
        postState = v.postState;
    }
};

class Animation {
public:
    std::string name;
    float durationTicks;
    float ticksPerSecond;
    std::vector<NodeAnimation> channels;

	void write(BinaryWriter* writer)
	{
		writer->writeString(name);
		writer->write<float>(durationTicks);
		writer->write<float>(ticksPerSecond);
		writer->write<int>(channels.size());
		for (int i = 0; i < channels.size(); i++)
			channels[i].write(writer);
	}

	void read(BinaryReader* reader) 
	{
		name = reader->readString();
		durationTicks = reader->read<float>();
		ticksPerSecond = reader->read<float>();
		channels.resize(reader->read<int>());
		for (int i = 0; i < channels.size(); i++)
			channels[i].read(reader);
	}
    
    Animation() {
        durationTicks = 0;
        ticksPerSecond = 0;
    }
    
    Animation(const Animation& v) {
        (*this) = v;
    }
    
    void operator=(const Animation& v) {
        name = v.name;
        
        durationTicks = v.durationTicks;
        
        ticksPerSecond = v.ticksPerSecond;
        channels = v.channels;
    }
};


enum LightType {
    LightType_NONE          = 0x0,
    LightType_DIRECTIONAL   = 0x1,
    LightType_POINT         = 0x2,
    LightType_SPOT          = 0x3,
    LightType_AMBIENT       = 0x4,
    LightType_AREA          = 0x5
};

const char* LightTypeToStr(LightType lt) {
    switch(lt) {
        case LightType_NONE:
            return "none";
        case LightType_DIRECTIONAL:
            return "directional";
        case LightType_POINT:
            return "point";
        case LightType_SPOT:
            return "spot";
        case LightType_AMBIENT:
            return "ambient";
        case LightType_AREA:
            return "area";
    }
    return "error to parse";
}

struct DirectionalLight {
    vec3 direction;
    vec3 up;
};

struct PointLight {
    vec3 position;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 up;
    float angleInnerCone;
    float angleOuterCone;
    
    SpotLight() {
        angleInnerCone = 0;
        angleOuterCone = 0;
    }
};

struct AmbientLight {
    vec3 position;
    vec3 direction;
    vec3 up;
};

struct AreaLight {
    vec3 position;
    vec3 direction;
    vec3 up;
    vec2 size;
};

class Light {
public:
    std::string name;
    
    LightType type;
    
    DirectionalLight directional;
    PointLight point;
    SpotLight spot;
    AmbientLight ambient;
    AreaLight area;
    
    // d = distance
    // Atten = 1/( att0 + att1 * d + att2 * d*d)
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    vec3 colorDiffuse;
    vec3 colorSpecular;
    vec3 colorAmbient;

	void write(BinaryWriter* writer) {
		writer->writeString(name);
		writer->write<unsigned char>(type);
		writer->write<DirectionalLight>(directional);
		writer->write<PointLight>(point);
		writer->write<SpotLight>(spot);
		writer->write<AmbientLight>(ambient);
		writer->write<AreaLight>(area);

		// d = distance
		// Atten = 1/( att0 + att1 * d + att2 * d*d)
		writer->write<float>(attenuationConstant);
		writer->write<float>(attenuationLinear);
		writer->write<float>(attenuationQuadratic);

		writer->write<vec3>(colorDiffuse);
		writer->write<vec3>(colorSpecular);
		writer->write<vec3>(colorAmbient);
	}

	void read(BinaryReader* reader) {
		name = reader->readString();
		type = (LightType)reader->read<unsigned char>();
		directional = reader->read<DirectionalLight>();
		point = reader->read<PointLight>();
		spot = reader->read<SpotLight>();
		ambient = reader->read<AmbientLight>();
		area = reader->read<AreaLight>();

		// d = distance
		// Atten = 1/( att0 + att1 * d + att2 * d*d)
		attenuationConstant = reader->read<float>();
		attenuationLinear = reader->read<float>();
		attenuationQuadratic = reader->read<float>();

		colorDiffuse = reader->read<vec3>();
		colorSpecular = reader->read<vec3>();
		colorAmbient = reader->read<vec3>();
	}
    
    Light() {
        attenuationConstant = 0;
        attenuationLinear = 0;
        attenuationQuadratic = 0;
    }
    
    Light(const Light& v) {
        (*this) = v;
    }
    
    void operator=(const Light& v) {
        name = v.name;
        
        type = v.type;
        
        directional = v.directional;
        point = v.point;
        spot = v.spot;
        ambient = v.ambient;
        area = v.area;
        
        // d = distance
        // Atten = 1/( att0 + att1 * d + att2 * d*d)
        attenuationConstant = v.attenuationConstant;
        attenuationLinear = v.attenuationLinear;
        attenuationQuadratic = v.attenuationQuadratic;
        
        colorDiffuse = v.colorDiffuse;
        colorSpecular = v.colorSpecular;
        colorAmbient = v.colorAmbient;
    }
};

class Camera {
public:
    std::string name;
    
    vec3 pos;
    vec3 up;
    vec3 forward;
    
	float horizontalFOVrad;
	float nearPlane;
	float farPlane;
    float aspect;
    
    float verticalFOVrad;
    
    float computeHeight(float width){
        return width / aspect;
    }
    float computeWidth(float height){
        return height * aspect;
    }

	void write(BinaryWriter* writer) {
		
		writer->writeString(name);

		writer->write<vec3>(pos);
		writer->write<vec3>(up);
		writer->write<vec3>(forward);

		writer->write<float>(horizontalFOVrad);
		writer->write<float>(nearPlane);
		writer->write<float>(farPlane);
		writer->write<float>(aspect);

		writer->write<float>(verticalFOVrad);
	}

	void read(BinaryReader* reader) {
		name = reader->readString();

		pos = reader->read<vec3>();
		up = reader->read<vec3>();
		forward = reader->read<vec3>();

		horizontalFOVrad = reader->read<float>();
		nearPlane = reader->read<float>();
		farPlane = reader->read<float>();
		aspect = reader->read<float>();

		verticalFOVrad = reader->read<float>();
	}
    
    Camera() {
        pos = vec3(0,0,0);
        up = vec3(0,1,0);
        forward = vec3(0,0,1);
        
        horizontalFOVrad = DEG2RAD(60.0f);
		nearPlane = 0.1f;
		farPlane = 100.0f;
        aspect = 1.0f;
        
        verticalFOVrad = DEG2RAD(60.0f);
    }
    
    Camera(const Camera& v) {
        (*this) = v;
    }
    
    void operator=(const Camera& v) {
        name = v.name;
        pos = v.pos;
        up = v.up;
        forward = v.forward;
        horizontalFOVrad = v.horizontalFOVrad;
		nearPlane = v.nearPlane;
		farPlane = v.farPlane;
        aspect = v.aspect;
        verticalFOVrad = v.verticalFOVrad;
    }
    
};

enum TextureType
{
    TextureType_NONE = 0x0,
    TextureType_DIFFUSE = 0x1,
    TextureType_SPECULAR = 0x2,
    TextureType_AMBIENT = 0x3,
    TextureType_EMISSIVE = 0x4,
    TextureType_HEIGHT = 0x5,
    TextureType_NORMALS = 0x6,
    TextureType_SHININESS = 0x7,
    TextureType_OPACITY = 0x8,
    TextureType_DISPLACEMENT = 0x9,
    TextureType_LIGHTMAP = 0xa,
    TextureType_REFLECTION = 0xb,
    TextureType_UNKNOWN = 0xc
};

static const char* TextureTypeToStr(TextureType type) {
    switch(type){
        case TextureType_NONE: return "none";
        case TextureType_DIFFUSE: return "diffuse";
        case TextureType_SPECULAR: return "specular";
        case TextureType_AMBIENT: return "ambient";
        case TextureType_EMISSIVE: return "emissive";
        case TextureType_HEIGHT: return "height";
        case TextureType_NORMALS: return "normals";
        case TextureType_SHININESS: return "shininess";
        case TextureType_OPACITY: return "opacity";
        case TextureType_DISPLACEMENT: return "displacement";
        case TextureType_LIGHTMAP: return "lightmap";
        case TextureType_REFLECTION: return "reflection";
        case TextureType_UNKNOWN: return "unknown";
    }
    return "error to parse";
}

enum TextureOp
{
    // T = T1 * T2
    TextureOp_Multiply = 0x0,
    // T = T1 + T2
    TextureOp_Add = 0x1,
    // T = T1 - T2
    TextureOp_Subtract = 0x2,
    // T = T1 / T2
    TextureOp_Divide = 0x3,
    // T = (T1 + T2) - (T1 * T2)
    TextureOp_SmoothAdd = 0x4,
    // T = T1 + (T2-0.5)
    TextureOp_SignedAdd = 0x5,

    TextureOp_Default = -1 //32766
};

static const char* TextureOpToStr(TextureOp op) {
    switch(op){
        case TextureOp_Multiply: return "multiply";
        case TextureOp_Add: return "add";
        case TextureOp_Subtract: return "subtract";
        case TextureOp_Divide: return "divide";
        case TextureOp_SmoothAdd: return "smooth add";
        case TextureOp_SignedAdd: return "signed add";
        case TextureOp_Default: return "default";
    }
    return "error to parse";
}

enum TextureMapMode
{
    // A texture coordinate u|v is translated to u%1|v%1
    TextureMapMode_Wrap = 0x0,
    
    // Texture coordinates outside [0...1]
    //  are clamped to the nearest valid value.
    TextureMapMode_Clamp = 0x1,
    
    // If the texture coordinates for a pixel are outside [0...1]
    //  the texture is not applied to that pixel
    TextureMapMode_Decal = 0x3,
    
    // A texture coordinate u|v becomes u%1|v%1 if (u-(u%1))%2 is zero and
    //  1-(u%1)|1-(v%1) otherwise
    TextureMapMode_Mirror = 0x2,
};

static const char* TextureMapModeToStr(TextureMapMode map) {
    switch(map){
        case TextureMapMode_Wrap: return "wrap";
        case TextureMapMode_Clamp: return "clamp";
        case TextureMapMode_Decal: return "decal";
        case TextureMapMode_Mirror: return "mirror";
    }
    return "error to parse";
}

class Texture {
public:
	std::string filename;
    std::string fileext;
    TextureType type;
    TextureOp op;
    TextureMapMode mapMode;
    
    int uvIndex;

	void write(BinaryWriter* writer) {
		writer->writeString(filename);
		writer->writeString(fileext);
		writer->write<unsigned char>(type);
		writer->write<unsigned char>(op);
		writer->write<unsigned char>(mapMode);
		writer->write<int>(uvIndex);
	}

	void read(BinaryReader* reader) {
		filename = reader->readString();
		fileext = reader->readString();
		type = (TextureType)reader->read<unsigned char>();
		op = (TextureOp)reader->read<unsigned char>();
		mapMode = (TextureMapMode)reader->read<unsigned char>();
		uvIndex = reader->read<int>();
	}

    Texture() {
        type = TextureType_NONE;
        op = TextureOp_Multiply;
        mapMode = TextureMapMode_Wrap;
        uvIndex = 0;
    }
    
    Texture(const Texture& v) {
        (*this) = v;
    }
    
    void operator=(const Texture& v) {
        filename = v.filename;
        fileext = v.fileext;
        type = v.type;
        uvIndex = v.uvIndex;
        op = v.op;
        mapMode = v.mapMode;
    }
    
};

class Material {
public:
	std::string name;

	std::map<std::string, float> floatValue;
	std::map<std::string, vec2> vec2Value;
	std::map<std::string, vec3> vec3Value;
	std::map<std::string, vec4> vec4Value;
	std::map<std::string, int> intValue;

	std::vector<Texture> textures;

	//std::string textureDiffuse;
	//std::string textureNormal;

	void write(BinaryWriter* writer) {
		writer->writeString(name);
		writer->writeStringMap<float>(floatValue);
		writer->writeStringMap<vec2>(vec2Value);
		writer->writeStringMap<vec3>(vec3Value);
		writer->writeStringMap<vec4>(vec4Value);
		writer->writeStringMap<int>(intValue);
		writer->write<int>(textures.size());
		for (int i = 0; i < textures.size(); i++)
			textures[i].write(writer);
	}

	void read(BinaryReader* reader) {
		name = reader->readString();
		reader->readStringMap<float>(&floatValue);
		reader->readStringMap<vec2>(&vec2Value);
		reader->readStringMap<vec3>(&vec3Value);
		reader->readStringMap<vec4>(&vec4Value);
		reader->readStringMap<int>(&intValue);
		textures.resize(reader->read<int>());
		for (int i = 0; i < textures.size(); i++)
			textures[i].read(reader);
	}

	Material() {

	}

	Material(const Material& v) {
		(*this) = v;
	}
	void operator=(const Material& v) {
		name = v.name;
		floatValue = v.floatValue;

		vec2Value = v.vec2Value;
		vec3Value = v.vec3Value;
		vec4Value = v.vec4Value;
		intValue = v.intValue;
		textures = v.textures;
	}

};

class VertexWeight {
public:
	unsigned int vertexID;
	float weight;
};

class Bone {
public:
	std::string name;
	std::vector<VertexWeight> weights;

	mat4 offset;

	void write(BinaryWriter* writer) {
		writer->writeString(name);
		writer->write<mat4>(offset);
		writer->writeVector<VertexWeight>(weights);
	}

	void read(BinaryReader* reader) {
		name = reader->readString();
		offset = reader->read<mat4>();
		reader->readVector<VertexWeight>(&weights);
	}

	Bone() {

	}

	Bone(const Bone& v) {
		(*this) = v;
	}
	void operator=(const Bone& v) {
		name = v.name;
		weights = v.weights;
		offset = v.offset;
	}
};


enum VertexFormat {
    CONTAINS_POS = 0x00000001, // float3 position
    CONTAINS_NORMAL = 0x00000002, // float3 normal
    CONTAINS_BINORMAL = 0x00000004, // float3 binormal
    CONTAINS_TANGENT = 0x00000008, // float3 tangent
    //CONTAINS_COL = 0x00000010, // unsigned byte 4 color
    
    CONTAINS_UV0 = 0x00000020, // float2
    CONTAINS_UV1 = 0x00000040, // float2
    CONTAINS_UV2 = 0x00000080, // float2
    CONTAINS_UV3 = 0x00000100, // float2
    CONTAINS_UV4 = 0x00000200, // float2
    CONTAINS_UV5 = 0x00000400, // float2
    CONTAINS_UV6 = 0x00000800, // float2
    CONTAINS_UV7 = 0x00001000, // float2
    
    CONTAINS_COLOR0 = 0x00002000, // byte4
    CONTAINS_COLOR1 = 0x00004000, // byte4
    CONTAINS_COLOR2 = 0x00008000, // byte4
    CONTAINS_COLOR3 = 0x00010000, // byte4
    CONTAINS_COLOR4 = 0x00020000, // byte4
    CONTAINS_COLOR5 = 0x00040000, // byte4
    CONTAINS_COLOR6 = 0x00080000, // byte4
    CONTAINS_COLOR7 = 0x00100000, // byte4
    
    
};

class Geometry {

public:
	std::string name;

	//VertexFormat: CONTAINS_POS | CONTAINS_NORMAL | ...
	unsigned int format;
	unsigned int vertexCount;
	unsigned int indiceCountPerFace;// 1 - points, 2 - lines, 3 - triangles, 4 - quads

	std::vector<vec3> pos;
	std::vector<vec3> normals;
	std::vector<vec3> tangent;
	std::vector<vec3> binormal;
	std::vector<vec3> uv[8];
	std::vector<unsigned int> color[8];//RGBA

	std::vector<unsigned int> indice;

	unsigned materialIndex;

	std::vector<Bone> bones;

	void write(BinaryWriter* writer) {
		writer->writeString(name);

		//VertexFormat: CONTAINS_POS | CONTAINS_NORMAL | ...
		writer->write<unsigned int>(format);
		writer->write<unsigned int>(vertexCount);
		writer->write<unsigned int>(indiceCountPerFace);// 1 - points, 2 - lines, 3 - triangles, 4 - quads
		writer->write<unsigned int>(materialIndex);
		
		writer->writeVector<vec3>(pos);
		writer->writeVector<vec3>(normals);
		writer->writeVector<vec3>(tangent);
		writer->writeVector<vec3>(binormal);
		for (int i = 0; i < 8; i++)
			writer->writeVector<vec3>(uv[i]);
		for (int i = 0; i < 8; i++)
			writer->writeVector<unsigned int>(color[i]);//RGBA
		writer->writeVector<unsigned int>(indice);

		writer->write<int>(bones.size());
		for (int i = 0; i < bones.size(); i++)
			bones[i].write(writer);

	}

	void read(BinaryReader* reader) {
		name = reader->readString();

		//VertexFormat: CONTAINS_POS | CONTAINS_NORMAL | ...
		format = reader->read<unsigned int>();
		vertexCount = reader->read<unsigned int>();
		indiceCountPerFace = reader->read<unsigned int>();// 1 - points, 2 - lines, 3 - triangles, 4 - quads
		materialIndex = reader->read<unsigned int>();

		reader->readVector<vec3>(&pos);
		reader->readVector<vec3>(&normals);
		reader->readVector<vec3>(&tangent);
		reader->readVector<vec3>(&binormal);
		for (int i = 0; i < 8; i++)
			reader->readVector<vec3>(&uv[i]);
		for (int i = 0; i < 8; i++)
			reader->readVector<unsigned int>(&color[i]);//RGBA
		reader->readVector<unsigned int>(&indice);

		bones.resize(reader->read<int>());
		for (int i = 0; i < bones.size(); i++)
			bones[i].read(reader);
	}

	Geometry() {
		format = 0; //CONTAINS_POS | CONTAINS_NORMAL | ...
		vertexCount = 0;
		indiceCountPerFace = 0;// 1 - points, 2 - lines, 3 - triangles, 4 - quads
		materialIndex = 0;
	}

	//copy constructores
	Geometry(const Geometry& v) {
		(*this) = v;
	}
	void operator=(const Geometry& v) {

		name = v.name;

		format = v.format;
		vertexCount = v.vertexCount;
		indiceCountPerFace = v.indiceCountPerFace;

		pos = v.pos;
		normals = v.normals;
		tangent = v.tangent;
		binormal = v.binormal;
		for (int i = 0; i < 8; i++) {
			uv[i] = v.uv[i];
			color[i] = v.color[i];
		}

		indice = v.indice;

		materialIndex = v.materialIndex;

		bones = v.bones;

	}

};

class Node {
public:
	std::string name;
	std::vector<unsigned int> geometries;
	std::vector<unsigned int> children;
	mat4 transform;

	void write(BinaryWriter* writer) {
		writer->writeString(name);
		writer->write<mat4>(transform);
		writer->writeVector<unsigned int>(geometries);
		writer->writeVector<unsigned int>(children);
	}

	void read(BinaryReader* reader) {
		name = reader->readString();
		transform = reader->read<mat4>();
		reader->readVector<unsigned int>(&geometries);
		reader->readVector<unsigned int>(&children);
	}

	Node() {

	}

	//copy constructores
	Node(const Node& v) {
		(*this) = v;
	}
	void operator=(const Node& v) {
		name = v.name;
		geometries = v.geometries;
		children = v.children;
		transform = v.transform;
	}
};

class ModelContainer {
public:
    std::vector<Animation> animations;
    std::vector<Light> lights;
    std::vector<Camera> cameras;
	std::vector<Material> materials;
	std::vector<Geometry> geometries;
	std::vector<Node> nodes;//the node[0] is the root
    
    void write(const char* filename) {
        
		BinaryWriter writer(filename, true);

		writer.write<int>(animations.size());
		for (int i = 0; i < animations.size(); i++)
			animations[i].write(&writer);

		writer.write<int>(lights.size());
		for (int i = 0; i < lights.size(); i++)
			lights[i].write(&writer);

		writer.write<int>(cameras.size());
		for (int i = 0; i < cameras.size(); i++)
			cameras[i].write(&writer);

		writer.write<int>(materials.size());
		for (int i = 0; i < materials.size(); i++)
			materials[i].write(&writer);
		
		writer.write<int>(geometries.size());
		for (int i = 0; i < geometries.size(); i++)
			geometries[i].write(&writer);

		writer.write<int>(nodes.size());
		for (int i = 0; i < nodes.size(); i++)
			nodes[i].write(&writer);

		writer.close();
    }

	void read(const char* filename) {
		
		BinaryReader reader(filename, true);

		animations.resize(reader.read<int>());
		for (int i = 0; i < animations.size(); i++)
			animations[i].read(&reader);

		lights.resize(reader.read<int>());
		for (int i = 0; i < lights.size(); i++)
			lights[i].read(&reader);

		cameras.resize(reader.read<int>());
		for (int i = 0; i < cameras.size(); i++)
			cameras[i].read(&reader);

		materials.resize(reader.read<int>());
		for (int i = 0; i < materials.size(); i++)
			materials[i].read(&reader);

		geometries.resize(reader.read<int>());
		for (int i = 0; i < geometries.size(); i++)
			geometries[i].read(&reader);

		nodes.resize(reader.read<int>());
		for (int i = 0; i < nodes.size(); i++)
			nodes[i].read(&reader);

		reader.close();
	}
};

#endif
