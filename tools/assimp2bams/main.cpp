#include "ModelContainer.h"
#include "AssimpExporter.h"

#define DEBUG 1


void printUsage() {
	fprintf(stdout,
		"Usage: assimp2bams <filename>\n"
		"\n"
		"  Example: assimp2bams model.fbx\n");
}

int main(int argc, char* argv[]){
	printf("assimp2bams - http://alessandroribeiro.thegeneralsolution.com/ - \"make things easy\"\n");

#if DEBUG == 0
	if (argc != 2) {
		printUsage();
		return 0;
	}
#endif

	std::string filepath;

#if DEBUG == 0
		filepath = std::string(argv[1]);
#endif

#if DEBUG == 1

#ifdef linux
    filepath = std::string("/home/alessandro/Desktop/OpenGLStarter/lib/assimp/test/models/FBX/spider.fbx");
#elif WIN32
	filepath = std::string("E:\\projetos\\GIT\\spider.fbx");
#else
    filepath = std::string("/Users/alessandro/Desktop/GIT/OpenGLStarter/lib/assimp/test/models/FBX/spider.fbx");
#endif

#endif

    std::string folder, filename, filename_wo_ext, fileext;
    PlatformPath::splitPathString(filepath, &folder, &filename, &filename_wo_ext, &fileext);

	fprintf(stdout, "Folder: %s\n", folder.c_str());
	fprintf(stdout, "Filename: %s\n", filename.c_str());
	fprintf(stdout, "Filename W/O ext: %s\n", filename_wo_ext.c_str());

	std::string inputFile = (folder + PlatformPath::SEPARATOR + filename);
	std::string outputFile = (folder + PlatformPath::SEPARATOR + filename_wo_ext + std::string(".bams"));

	ModelContainer *container = ImportFromAssimp(inputFile.c_str());

	//
    // save the container
	//
	container->write(outputFile.c_str());
    
    delete container;


	return 0;
}

