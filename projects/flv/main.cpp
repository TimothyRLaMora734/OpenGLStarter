

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>//write read
#endif
#include <vector>
#include <signal.h>

#ifndef O_BINARY
    #define O_BINARY 0
#endif

volatile bool exit_requested = false;
void signal_handler(int signal) {
    exit_requested = true;
}

#include "flv.h"


//
// ffmpeg -f image2 -f avfoundation -r 15 -i "0x7fdc5d410000:none" -s 1280x720 -deinterlace -vcodec libx264 -pix_fmt yuv420p -preset veryfast -tune zerolatency -g 60 -b:v 2500k -threads 6 -f rawvideo - | ./flv 
//
// cat out.h264| ffmpeg -i - -c:v copy out2.flv
//
//
// ffmpeg -i file.h264 -c copy -bsf:v trace_headers -f null -
//
int main(int argc, char* argv[]) {
	PlatformPath::setWorkingPath(PlatformPath::getExecutablePath(argv[0]));

    //int fd_stdin = fileno(stdin);
    int fd_stdin = open("test.h264",O_RDONLY | O_BINARY);

    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);
#ifndef WIN32
    signal(SIGQUIT, signal_handler);
#endif


    unsigned char buffer[65536];
    
    FLVFileWriter flv("out.flv");

    while (!exit_requested) {

        int readedSize = read(fd_stdin,buffer,65536);
        if (readedSize==0)
            break;
        
        flv.parseH264(buffer,readedSize);
    }
    
    flv.endOfStreamH264();

    signal(SIGINT,  SIG_DFL);
    signal(SIGTERM, SIG_DFL);
#ifndef WIN32
    signal(SIGQUIT, SIG_DFL);
#endif
    
    close(fd_stdin);

#ifdef WIN32
    system("start out.flv");
#else
    system("open out.flv");
#endif

	return 0;
}

