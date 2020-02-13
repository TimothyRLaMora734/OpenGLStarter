#ifndef aribeiro_H
#define aribeiro_H

/*
#include <aribeiro/glUtil.h>
#include <aribeiro/GLTexture.h>
#include <aribeiro/GLSquare.h>
#include <aribeiro/GLShaderYUV2RGB.h>
#include <aribeiro/ConvertUTF.h>
#include <aribeiro/Font.h>
*/

#include <aribeiro/common.h>

//#include "all_math.h"
#include <aribeiro/BresenhamIterator.h>
#include <aribeiro/constants.h>
//#include "defs.h"
#include <aribeiro/EasingEquations.h>
#include <aribeiro/geometricOperations.h>
#include <aribeiro/mat4.h>
#include <aribeiro/quat.h>
#include <aribeiro/vec2.h>
#include <aribeiro/vec3.h>
#include <aribeiro/vec4.h>

#include <aribeiro/AABB.h>
#include <aribeiro/Plane.h>
#include <aribeiro/LineSegment.h>
#include <aribeiro/Sphere.h>
#include <aribeiro/Triangle.h>
#include <aribeiro/Ray.h>
#include <aribeiro/Frustum.h>


/*
#include <aribeiro/MulticastUdpSocket.h>
#include <aribeiro/NetworkConstants.h>
#include <aribeiro/NetworkHelper.h>
#include <aribeiro/UDP_DataReceive.h>
#include <aribeiro/UDP_DataSend.h>
*/

/*
#include <aribeiro/Delegate.h>
//#include "getDirectoryToSaveWin32.h"
#include <aribeiro/Object.h>
//#include "PerformaneCounterWin32.h"
#include <aribeiro/Platform.h>
#include <aribeiro/Thread.h>
#include <aribeiro/Time.h>

*/

#include <aribeiro/PlatformThread.h>
#include <aribeiro/PlatformMutex.h>
#include <aribeiro/PlatformTime.h>
#include <aribeiro/PlatformSleep.h>
#include <aribeiro/PlatformPath.h>
#include <aribeiro/PlatformGL.h>
#include <aribeiro/Property.h>
#include <aribeiro/Delegate.h>
#include <aribeiro/MethodPointer.h>
#include <aribeiro/GetFirstMacAddress.h>
#include <aribeiro/SetNullAndDelete.h>
#include <aribeiro/NetworkConstants.h>
#include <aribeiro/GLTexture.h>
#include <aribeiro/GLShader.h>
#include <aribeiro/GLShaderColor.h>
#include <aribeiro/GLShaderTextureColor.h>
#include <aribeiro/GLShaderFont.h>
#include <aribeiro/GLShaderYUV2RGB.h>
#include <aribeiro/GLShaderVertexColor.h>
#include <aribeiro/GLSquare.h>
#include <aribeiro/Font.h>
#include <aribeiro/GLFont.h>
#include <aribeiro/GLStripText.h>
#include <aribeiro/PNGHelper.h>
#include <aribeiro/TransformStack.h>
#include <aribeiro/Random.h>
#include <aribeiro/PressReleaseDetector.h>
#include <aribeiro/GLPostProcessingShader.h>
#include <aribeiro/GLFramebufferObject.h>
#include <aribeiro/GLPostProcess.h>
#include <aribeiro/ColorConversion.h>

#include <aribeiro/GLVertexBufferObject.h>
#include <aribeiro/GLVertexArrayObject.h>

#include <aribeiro/SSE2.h>



#ifdef _WIN32

#define WIN_LEAN_AND_MEAN

#include <Windows.h>
#include <wchar.h>

#ifndef swprintf
	#define swprintf _snwprintf
#endif

#endif


// In RPI:
//         the buffers sent to the GL API,
//         access aligned data,
//         and need the buffer size to be
//         16 bytes aligned also.
// Use this definition
//
template <typename C>
static C* ogl_indexbuffer_malloc(int s) {
#ifdef ARIBEIRO_RPI
    int alloc_s = s*sizeof(C);
    return (C*)aligned_alloc(16,  alloc_s + alloc_s % 16 );
#else
    return (C*)malloc(s*sizeof(C));
#endif
}

template <typename C>
static void ogl_indexbuffer_free(C *&ptr) {
    if (ptr != NULL) {
        C * aux = ptr;
        ptr = NULL;
        free(aux);
    }
}


#endif
