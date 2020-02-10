

#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#if defined(WIN32)
	#include <io.h>
#else
	#include <unistd.h>//write read
#endif

#ifndef O_BINARY
	#define O_BINARY 0
#endif

#include <vector>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

volatile bool exit_requested = false;
void signal_handler(int signal) {
    exit_requested = true;
}

//#include "flv.h"

class FLVDemux{
    int count;
    std::vector<uint8_t> buffer;

    std::vector<uint8_t> data;

    //header reader
    uint8_t readUInt8(){
        uint8_t result = buffer[0];
        buffer.erase(buffer.begin());
        return result;
    }

    uint32_t readUInt16(){
        uint32_t result = buffer[0] << 8 | buffer[1];
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        return result;
    }

    uint32_t readUInt24(){
        uint32_t result = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        return result;
    }
    uint32_t readUInt32(){
        uint32_t result = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        return result;
    }

    uint32_t readUInt32Timestamp(){
        uint32_t result = buffer[0] << 16 | buffer[1] << 8 | buffer[2] | buffer[3] << 24;
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        buffer.erase(buffer.begin());
        return result;
    }


public:

    int nextTag;

    FLVDemux() {
        count = 0;
        nextTag = 13;
    }

    void putByte(uint8_t b) {
        count++;

        if (count > nextTag)
            buffer.push_back(b);

        if (count == nextTag + 11)
        {

            if (buffer.size() != 11)
            {
                fprintf(stderr,"error to process tag...\n");
                exit(-1);
            }

            {
                /*
                 mFLVWritter.writeUInt8(0x09);//tagtype video
                 mFLVWritter.writeUInt24( bodyLength );//data len
                 mFLVWritter.writeUInt32Timestamp( videoTimestamp_ms );//timestamp
                 mFLVWritter.writeUInt24( 0 );//stream id 0
                 */
                uint8_t tagType = readUInt8();
                uint32_t dataLength = readUInt24();
                uint32_t timestamp = readUInt32Timestamp();
                uint32_t streamID = readUInt24();

                printf("-----------\n");
                printf(" tag: 0x%.2x\n",tagType);
                printf(" dataLength: %u\n",dataLength);
                printf(" timestamp: %u\n",timestamp);
                printf(" streamID: %u\n",streamID);
                printf("-----------\n");

                //skip data length
                nextTag += dataLength + 11 + 4;

            }
        }
    }
};


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
    int fd_stdin = open("out.flv",O_RDONLY | O_BINARY);

    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);
#ifndef WIN32
	signal(SIGQUIT, signal_handler);
#endif



    unsigned char buffer[65536];

    //FLVFileWriter flv("out.flv");

    FLVDemux demux;

    while (!exit_requested) {

        int readedSize = read(fd_stdin,buffer,65536);
        if (readedSize==0)
            break;
        for(int i=0;i<readedSize;i++)
            demux.putByte(buffer[i]);
    }

    signal(SIGINT,  SIG_DFL);
    signal(SIGTERM, SIG_DFL);

#ifndef WIN32
	signal(SIGQUIT, SIG_DFL);
#endif

    close(fd_stdin);

	return 0;
}

