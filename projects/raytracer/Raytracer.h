#ifndef Raytracer__H
#define Raytracer__H

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "FileReader.h"
#include "RaytracerObjects.h"

class Raytracer {

	//
	// camera cache
	//
    vec3 cameraZ;
    vec3 cameraX;
    vec3 cameraY;
    
    float f;//focal distance (distance from projection plane)
    float tangentY;
    float tangentXaspect;

	void preComputeCameraCache() {
        cameraZ = normalize( camera.eyePos - camera.eyeLookToPosition );
        cameraX = normalize( cross(camera.eyeUp, cameraZ) );
        cameraY = normalize( cross(cameraZ, cameraX) );
        
        f = 1.0f;
        tangentY = 2.0f * f * tanf(DEG2RAD(camera.fieldOfView) / 2.0f);
        tangentXaspect = tangentY * ((float)width / (float)height);
	}

	collision::Ray generateRay(int x, int y) {
		collision::Ray ray;
        
        // compute pixel range x[-0.5,0.5] y[0.5,-0.5]
        float xFloat = (((float)x + 0.5f) / (float)(width) - 0.5f);
        float yFloat = -(((float)y + 0.5f) / (float)(height) - 0.5f);
        
        //compute the tangent plane (pixel positions in 3D space)
        ray.dir =
        - f * cameraZ +
        yFloat * tangentY * cameraY +
        xFloat * tangentXaspect * cameraX;
        
        //setup ray origin
        ray.origin = camera.eyePos + ray.dir;
        
        //normalize ray direction
        ray.dir = normalize(ray.dir);
		
		return ray;
	}

	bool calculateCollision(const collision::Ray &ray,
		float *tOut,
		vec3 *normalOut,
		Object **objectOut) {

		float t = FLT_MAX;
		vec3 normal;
		Object *object;

		for (int i = 0; i < objects.size(); i++) {
			float tAux;
			vec3 normalAux;
			if (objects[i].raycast(ray, &tAux, &normalAux)) {
				if (tAux > 0 && tAux < t) {
					t = tAux;
					normal = normalAux;
					object = &objects[i];
				}
			}
		}

		if (t != FLT_MAX) {
			*tOut = t;
			*normalOut = normal;
			*objectOut = object;
			return true;
		}
		return false;
	}

	//
	// deve retornar uma cor a partir de um raio
	//
	vec3 recursiveRaytracing(const collision::Ray &ray, int max_depth) {
		float t;
		vec3 normal;
		Object *object;

		if (calculateCollision(ray, &t, &normal, &object)) {
			//
			// Make the illumination calculation
			//
			vec3 collisionPos = ray.origin + ray.dir*t;

			vec3 pigmentColor = object->pigment->getColor(collisionPos);

			return pigmentColor;
		}
		else
			return backgroundColor;
		
	}

public:

	//
	// Scene definitions
	//
	Camera camera;
	std::vector<Light> lights;
	std::vector<Pigment> pigments;
	std::vector<Finishing> finishing;
	std::vector<Object> objects;
	vec3 backgroundColor;

	//
	// framebuffer definition
	//
	std::vector <vec3> floatFramebufferRGB;
	int width, height;


	Raytracer(const std::string &fileToOpen, int w, int h) {
		
		width = w;
		height = h;

		//
		// initialize the framebuffer with background color
		//
		floatFramebufferRGB.resize(width*height);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				floatFramebufferRGB[x + y * width] = backgroundColor;
			}
		}

		printf("\nRaytracer (%i, %i)\n\n", w, h);
		printf("loading: %s\n", fileToOpen.c_str());

		//
		// Load config file
		//
		FileReader inputFile(fileToOpen);

		camera = Camera::readFromFile(&inputFile);

		int lightCount = inputFile.readInt();
		for (int i = 0; i < lightCount; i++)
			lights.push_back(Light::readFromFile(&inputFile));


		int pigmentCount = inputFile.readInt();
		for (int i = 0; i < pigmentCount; i++)
			pigments.push_back(Pigment::readFromFile(&inputFile));

		backgroundColor = vec3(0.5f, 0.5f, 0.5f);

		int finishingCount = inputFile.readInt();
		for (int i = 0; i < finishingCount; i++)
			finishing.push_back(Finishing::readFromFile(&inputFile));

		int objectCount = inputFile.readInt();
		for (int i = 0; i < objectCount; i++)
			objects.push_back(Object::readFromFile(&inputFile, &pigments, &finishing));

		preComputeCameraCache();
	}

	void run() {
		printf("run ");
		for (int y = 0; y < height; y++) {
			printf(".");
			for (int x = 0; x < width; x++) {

				//
				// 1º: gerar raios
				//
				collision::Ray ray = generateRay(x, y);

				//
				// 2º: calcular cores
				//
				vec3 cor = recursiveRaytracing(ray, 10);

				floatFramebufferRGB[x + y * width] = cor;
			}
		}
		printf("\n");
	}

	void save(const std::string &fileout) {

		std::vector<uint8_t> byteFramebufferRGB(width*height*3);
		for (int i = 0; i < width*height; i++) {

			floatFramebufferRGB[i] = clamp(floatFramebufferRGB[i], vec3(0), vec3(1.0f));

			byteFramebufferRGB[i * 3 + 0] = (uint8_t)(floatFramebufferRGB[i].r * 255.0f + 0.5f);
			byteFramebufferRGB[i * 3 + 1] = (uint8_t)(floatFramebufferRGB[i].g * 255.0f + 0.5f);
			byteFramebufferRGB[i * 3 + 2] = (uint8_t)(floatFramebufferRGB[i].b * 255.0f + 0.5f);
		}

		PNGHelper::writePNG(fileout.c_str(), width, height, 3, (char*)&byteFramebufferRGB[0]);
	}

};




#endif
