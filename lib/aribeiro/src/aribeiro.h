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
#include <aribeiro/AABB.h>
#include <aribeiro/BresenhamIterator.h>
#include <aribeiro/constants.h>
//#include "defs.h"
#include <aribeiro/EasingEquations.h>
#include <aribeiro/geometricOperations.h>
#include <aribeiro/mat4.h>
#include <aribeiro/quat.h>
#include <aribeiro/tensor4.h>
#include <aribeiro/vec2.h>
#include <aribeiro/vec3.h>
#include <aribeiro/vec4.h>

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
#include <aribeiro/Delegate.h>
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



#ifdef _WIN32

#define WIN_LEAN_AND_MEAN

#include <Windows.h>
#include <wchar.h>

#ifndef swprintf
	#define swprintf _snwprintf
#endif

#endif


#endif
