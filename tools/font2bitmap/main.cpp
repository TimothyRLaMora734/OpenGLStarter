#include <stdio.h>

#include "UTFops.h"
#include "ft2_font_generator.h"

#ifdef _WIN32
    #include "win32/getopt.h"
#else
	#include <getopt.h>
#endif

#include <stdlib.h>

/*
Command line tool to generate bitmaps from FT2 compatible files.

uses:

font2bitmap -i <input file> -o <output file> -m {s|mixed|opengl} -s <character size in pixels> -c <input charset utf8> -p <pixel between char> -v

-i : input file
-o : output file without extension
-m : mode
      s : exports s font tool compatible output (.png and .meta files)
          outputs: <output file>.png and <output file>.meta
      mixed : exports a png and a binary glyph table
          outputs: <output file>.png and <output file>.asbgt (asilva binary glyph table)
	  opengl : exports the raw bytes to use a luminance texture from opengl with the binary glyph table
          outputs: <output file>.basof (binary asilva opengl font)
-s : character size in pixels to be exported (minimum:14).
-c : text file with all characters to be generated in utf8
-p : size in pixels of the space between the characters exported
-v : export characters inverted in the buffer

font2bitmap -i mailrays.ttf -o mailrays -m mixed -s 40 -c charset.utf8 -p 5

*/
#include <string>

struct ParametersSet{

    bool i,o,m,s,c,p,v;

	std::string inputFile;
    std::string outputFile;
    std::string mode;
    std::string characterUtf8Charset;
    int characterSize;
    int spaceBetweenChar;

	int faceToSelect;
};

int main(int argc, char *argv[]){
    fprintf (stdout, "font2bitmap - alessandroribeiro.thegeneralsolution.com\n");

    ParametersSet parameters={0,0,0,0,0,0,0,"","","","",0,0,0};
	int c;
	opterr = 0;

	char validOp[] = {"i:o:m:s:c:p:v:f:"};
	while ((c = getopt (argc, argv, validOp)) != -1){
		switch (c){
			case 'f':
				parameters.faceToSelect = atoi(optarg);
				break;
			case 'i':
                parameters.i = true;
                parameters.inputFile = optarg;
				break;
			case 'o':
                parameters.o = true;
                parameters.outputFile = optarg;
				break;
			case 'm':
                parameters.mode = optarg;
                if (parameters.mode == "s" | parameters.mode == "mixed" | parameters.mode == "opengl")
                    parameters.m = true;
				break;
			case 's':
                parameters.s = true;
                parameters.characterSize = atoi(optarg);
                break;
            case 'c':
                parameters.c = true;
                parameters.characterUtf8Charset = optarg;
                break;
            case 'p':
                parameters.p = true;
                parameters.spaceBetweenChar = atoi(optarg);
                break;
            case 'v':
                parameters.v = true;
                break;
            case '?':
				if (optopt == 'f')
					fprintf (stderr, "Option -f requires an argument <face to select>.\n");
				else if (optopt == 'i')
					fprintf (stderr, "Option -i requires an argument <input file>.\n");
                else if (optopt == 'o')
					fprintf (stderr, "Option -o requires an argument <output file>.\n");
                else if (optopt == 'm')
                    fprintf (stderr, "Option -m requires an argument {s|mixed|opengl}.\n");
                else if (optopt == 's')
					fprintf (stderr, "Option -s requires an argument <character size in pixels>.\n");
                else if (optopt == 'c')
					fprintf (stderr, "Option -c requires an argument <input charset utf8>.\n");
                else if (optopt == 'p')
					fprintf (stderr, "Option -p requires an argument <pixel between char>.\n");
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
				break;
			default:
				//error -- unknown parameter
				break;
		}
	}
//	for (index = optind; index < argc; index++)
//		printf ("Non-option argument %s\n", argv[index]);
    if (!parameters.i&&!parameters.o&&!parameters.m&&!parameters.s&&!parameters.c&&!parameters.p){
        fprintf (stdout,
"Usage: font2bitmap -i ... -o ... -m ... -s ... -c ... -p ... [-v]\n"
"-i : input file(.ttf|.otf|freetype compatible font)\n"
"-f : face in font to be selected (default: 0)\n"
"-o : output file without extension\n"
"-m : mode\n"
"      s : exports s font tool compatible output (.png and .meta files)\n"
"\n"
"          outputs: <output file>.png and <output file>.meta\n"
"\n"
"      mixed : exports a png and a binary glyph table\n"
"\n"
"          outputs: <output file>.png and <output file>.asbgt\n"
"                                         (asilva binary glyph table)\n"
"\n"
"      opengl : exports raw bytes to use as luminance texture with opengl\n"
"               and the binary glyph table in the same file.\n"
"\n"
"          outputs: <output file>.basof \n"
"                   (binary asilva opengl font)\n"
"\n"
"-s : character size in pixels to be exported (minimum:14).\n"
"-c : text file with all characters to be generated in utf8.\n"
"-p : size in pixels of the space between the characters exported.\n"
"-v : invert pixels from characters (optional).\n"


);


    }else if (!parameters.i){
        fprintf (stderr, "Missing option -i <input file>.\n");
    }else if (!parameters.o){
        fprintf (stderr, "Missing option -o <output file>.\n");
    }else if (!parameters.m){
        fprintf (stderr, "Missing option -m {s|mixed|opengl}.\n");
    }else if (!parameters.s){
        fprintf (stderr, "Missing option -s <character size in pixels>.\n");
    }else if (!parameters.c){
        fprintf (stderr, "Missing option -c <input charset utf8>.\n");
    }else if (!parameters.p){
        fprintf (stderr, "Missing option -p <pixel between char>.\n");
    }else{
        //ok
        UTF8data * fileContent = UTF8data::readFromFile(parameters.characterUtf8Charset.c_str());
        UTF32data *utf32data = new UTF32data(fileContent);
        delete fileContent;
        FT2Configuration configuration;
        //config the generator
        configuration.m_char_max_height = parameters.characterSize;
        configuration.m_char_max_width = parameters.characterSize;
		configuration.m_face_to_select = parameters.faceToSelect;

        PixelFont* generatedFont = PixelFontFactory::generatePixelFont(parameters.inputFile.c_str(),
            configuration,utf32data->begin(),utf32data->count(),parameters.v);
        delete utf32data;

        //generate the files
        if (parameters.mode == "s"){
			generatedFont->getTextureFontRenderer(parameters.spaceBetweenChar).exportSFont(
				(parameters.outputFile + ".meta").c_str(),
				(parameters.outputFile + "-color.png").c_str(),
				(parameters.outputFile + "-gray.png").c_str()
			);
        }else if (parameters.mode == "opengl"){
            generatedFont->getTextureFontRenderer(parameters.spaceBetweenChar).exportGLPack(
                (parameters.outputFile+".basof").c_str()
            );
        }else if (parameters.mode == "mixed"){
			generatedFont->getTextureFontRenderer(parameters.spaceBetweenChar).exportASilvaBinTable(
				(parameters.outputFile + ".asbgt").c_str(),
				(parameters.outputFile + "-color.png").c_str(),
				(parameters.outputFile + "-gray.png").c_str()
			);
        }


        delete generatedFont;
    }

	//getc(stdin);
    return 0;
}
