#include <aribeiro/aribeiro.h>
using namespace aRibeiro;

#include "BroadcomCamera.h"
#include "BroadcomVideoDecode.h"
#include "BroadcomVideoEncode.h"

#include <signal.h>

bool exit_requested = false;
void signal_handler(int signal) {
    exit_requested = true;
}

int main(int argc, char* argv[]) {
	PlatformPath::setWorkingPath(PlatformPath::getExecutablePath(argv[0]));

    signal(SIGINT,  signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);

    BroadcomVideoEncode encoder(1280, 720, 25, 2700);


    while (!exit_requested) {
        fgetc(stdin);
        encoder.postYUV(NULL,0);

        encoder.makeOutBufferAvailable();
    }


    signal(SIGINT,  SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);


	return 0;
}
