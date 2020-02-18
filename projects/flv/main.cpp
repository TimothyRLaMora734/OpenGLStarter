#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>//write read
    #ifndef O_BINARY
        #define O_BINARY 0
    #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <vector>
#include <signal.h>

volatile bool exit_requested = false;
void signal_handler(int signal) {
    exit_requested = true;
}

#include <libflv/FLVMuxer.h>

int fd_stdout = 0;

void flvWrite(const void* data, size_t size){
    write(fd_stdout,data,size);
}

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
    
    FLVMuxer muxer(true, //_writeVideo,
                   false, // _writeAudio,
                   30, //_videoFPS
                   44100,//_audioSamplerate
                   FLV_OnDataMethodPtrT(&flvWrite));

    //int fd_stdin = fileno(stdin);
    int fd_stdin = open("test.h264",O_RDONLY | O_BINARY);
    if (fd_stdin<0){
        fprintf(stderr,"Could not open file for read.\n");
        exit(-1);
    }

    fd_stdout = open( "out.flv", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY , 0644 );

    if (fd_stdout<0){
        fprintf(stderr,"Could not open file for write.\n");
        exit(-1);
    }
    
    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);
#ifndef WIN32
    signal(SIGQUIT, signal_handler);
#endif

    muxer.writeHeader();
    unsigned char buffer[65536];
    
    while (!exit_requested) {

        int readedSize = read(fd_stdin,buffer,65536);
        if (readedSize==0)
            break;

        muxer.writeH264(buffer,readedSize);
    }
    
    muxer.endOfStream();

    signal(SIGINT,  SIG_DFL);
    signal(SIGTERM, SIG_DFL);
#ifndef WIN32
    signal(SIGQUIT, SIG_DFL);
#endif
    
    close(fd_stdin);
    close(fd_stdout);

#ifdef WIN32
    system("start out.flv");
#else
    system("open out.flv");
#endif

	return 0;
}

