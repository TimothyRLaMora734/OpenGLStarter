#include "v4l2.h"

//#include <fcntl.h>              /* low-level i/o */
#include <sys/ioctl.h>
#include <dirent.h>
#include <unistd.h> // readLink

#include <map>
#include <string>
#include <algorithm>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static const char *prefixes[] = {
    "video",
    "radio",
    "vbi",
    "swradio",
    "v4l-subdev",
    NULL
};

static const int prefixes_weight[] = {
    500, //"video",
    100, //"radio",
    400, //"vbi",
    200, //"swradio",
    300 //"v4l-subdev",
};


static bool isValidDeviceName(const char *name)
{
    for (unsigned i = 0; prefixes[i]; i++) {
        unsigned l = strlen(prefixes[i]);
        if (!memcmp(name, prefixes[i], l)) {
            unsigned nl = strlen(name);
            if (nl > l){
                for(int j=l;j<nl;j++){
                    if (!isdigit(name[j]))
                        return false;
                }
                return true;
            }
        }
    }
    return false;
}

static int computeDeviceSortNumber(const char *sparam)
{
    int n = 0;
    const char *s = strrchr(sparam, '/');
    if (s == NULL)
        s = sparam;
    else
        s += 1;

    for (unsigned i = 0; prefixes[i]; i++) {
        unsigned l = strlen(prefixes[i]);
        if (!memcmp(s, prefixes[i], l)) {
            //n = i << 8;
            n = prefixes_weight[i];
            n += atol(s + l);
            return n;
        }
    }
    return 0;
}

static bool compareDeviceName(const std::string &s1, const std::string &s2)
{
    int n1 = computeDeviceSortNumber(s1.c_str());
    int n2 = computeDeviceSortNumber(s2.c_str());

    return n1 < n2;
}




std::vector<std::string> v4l2::listDevicesNames(){
    std::vector<std::string> result;
    DIR *dp = opendir("/dev");
    if (dp == NULL) {
        perror ("Couldn't open the directory");
        exit(-1);
    }

    struct dirent *ep;
    while ((ep = readdir(dp))){
        if (isValidDeviceName(ep->d_name)){
            result.push_back(std::string("/dev/") + ep->d_name);
        }
    }
    closedir(dp);

    std::map<std::string, std::string> links;
    //process links
    for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ) {
		char link[64+1];
		std::string target;

		int link_len = readlink(iter->c_str(), link, 64);
		if (link_len < 0) {	/* Not a link or error */
			iter++;
			continue;
		}
		link[link_len] = '\0';

		/* Only remove from files list if target itself is in list */
		if (link[0] != '/')	/* Relative link */
			target = std::string("/dev/");
		target += link;
		if (find(result.begin(), result.end(), target) == result.end()) {
			iter++;
			continue;
		}

		/* Move the device node from files to links */
		if (links[target].empty())
			links[target] = *iter;
		else
			links[target] += ", " + *iter;
		iter = result.erase(iter);
	}

    //sort
    std::sort(result.begin(), result.end(), compareDeviceName);

    return result;
}

std::vector<Device> v4l2::listDevices() {
    std::vector<std::string> deviceNames = listDevicesNames();
    std::vector<Device> result;
    Device device;
    for (unsigned i=0;i<deviceNames.size();i++){
        if (v4l2::loadDeviceInfo(deviceNames[i], &device)){
            result.push_back( device );
        }
    }
    return result;
}

bool v4l2::loadDeviceInfo(const std::string &path, Device *deviceParam) {
    Device &device = *deviceParam;
    device.path = path;
    if ( v4l2::getDeviceCapability (device.path, &device.capability  ) ) {
        device.supportedFormats = v4l2::getDeviceFormats(device.path);
        return true;
    }
    return false;
}

bool v4l2::getDeviceCapability(const std::string &device, v4l2_capability *vcap) {
    v4l2_capability result;
    int fd = open(device.c_str(), O_RDWR);
    if (fd < 0)
        return false;
    int err = ioctl(fd, VIDIOC_QUERYCAP, vcap);
    close(fd);
    if (err)
        return false;
    return true;
}

std::vector<v4l2_fmtdesc> v4l2::getDeviceFormats(const std::string &device){
    std::vector<v4l2_fmtdesc> result;

    int fd = open(device.c_str(), O_RDWR);
    if (fd < 0)
        return result;

    struct v4l2_fmtdesc fmt;

	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while (ioctl(fd, VIDIOC_ENUM_FMT, &fmt) >= 0) {
        result.push_back(fmt);
		fmt.index++;
	}
	close(fd);

	return result;
}


std::vector<v4l2_frmsizeenum> v4l2::getFramesizes(const std::string &device, const v4l2_fmtdesc &fmt) {
    std::vector<v4l2_frmsizeenum> result;

    int fd = open(device.c_str(), O_RDWR);
    if (fd < 0)
        return result;

    v4l2_frmsizeenum frmsize;
    memset(&frmsize, 0, sizeof(v4l2_frmsizeenum));

    frmsize.pixel_format = fmt.pixelformat;
    frmsize.index = 0;
    while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0) {
        if (frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
            result.push_back(frmsize);
        }
        frmsize.index++;
    }

    close(fd);

    return result;
}

std::vector<v4l2_frmivalenum> v4l2::getFrameIntervals(const std::string &device, const v4l2_frmsizeenum &framesize) {
    std::vector<v4l2_frmivalenum> result;

    int fd = open(device.c_str(), O_RDWR);
    if (fd < 0)
        return result;

    v4l2_frmivalenum frmival;

    frmival.index = 0;
    frmival.pixel_format = framesize.pixel_format;
    frmival.width = framesize.discrete.width;
    frmival.height = framesize.discrete.height;
    frmival.type = framesize.type;

    while (ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival) >= 0) {
        if (frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE){
            result.push_back(frmival);
        }
        frmival.index++;
    }

    close(fd);
    return result;
}



bool Device::queryPixelFormat(__u32 pixelFormat, v4l2_fmtdesc *output){
    for(int i=0;i<supportedFormats.size();i++){
        if (supportedFormats[i].pixelformat == pixelFormat) {
            *output = supportedFormats[i];
            return true;
        }
    }
    return false;
}

bool Device::queryNearResolutionForFormat(const v4l2_fmtdesc &format, __u32 width, __u32 height, v4l2_frmsizeenum *output) {

    v4l2_frmsizeenum result;
    int dst = 999999;
    bool foundResult = false;

    std::vector<v4l2_frmsizeenum> supportedResolutions = v4l2::getFramesizes(path, format);
    for (int i=0;i<supportedResolutions.size();i++){
        if (i == 0){
            result = supportedResolutions[i];
            int w2 = (int)result.discrete.width - (int)width;
            int h2 = (int)result.discrete.height - (int)height;
            dst = w2 * w2 + h2 * h2;
            foundResult = true;
            continue;
        }

        v4l2_frmsizeenum &res = supportedResolutions[i];

        int w2 = (int)res.discrete.width - (int)width;
        int h2 = (int)res.discrete.height - (int)height;
        int auxdst = w2 * w2 + h2 * h2;

        if (auxdst < dst){
            result = res;
            dst = auxdst;
            foundResult = true;
        }
    }

    if (foundResult)
        *output = result;

    return foundResult;
}


bool Device::queryNearInterval(const v4l2_frmsizeenum &res, float fps, v4l2_frmivalenum *output) {
    v4l2_frmivalenum result;
    float dst = 999999;
    bool foundResult = false;

    std::vector<v4l2_frmivalenum> frameIntervals = v4l2::getFrameIntervals(path, res);
    for (int i=0;i<frameIntervals.size();i++) {
        float fpsaux = (float)frameIntervals[i].discrete.denominator / (float)frameIntervals[i].discrete.numerator;
        float dstaux = fpsaux - fps;
        dstaux = dstaux * dstaux;
        if ( dstaux < dst ){
            dst = dstaux;
            result = frameIntervals[i];
            foundResult = true;
        }
    }

    if (foundResult)
        *output = result;

    return foundResult;
}




Device::Device(){
    fd = -1;
    streaming = false;
}
Device::~Device(){
    if (streaming)
        streamOFF();
    close();
}


//
// online device handling
//
bool Device::open(){
    close();
    fd = ::open(path.c_str(),O_RDWR);
    return fd >= 0;
}

void Device::close(){
    if (fd >= 0)
        ::close(fd);
    fd = -1;
}


void Device::setFormat(const v4l2_fmtdesc &fmt, const v4l2_frmsizeenum &res, const v4l2_frmivalenum &interval ){
    v4l2_format imageFormat;

    imageFormat.type = fmt.type;//V4L2_BUF_TYPE_VIDEO_CAPTURE;
    imageFormat.fmt.pix.width = res.discrete.width;
    imageFormat.fmt.pix.height = res.discrete.height;
    imageFormat.fmt.pix.pixelformat = fmt.pixelformat;

    /*
    __u32         		width;
    __u32			height;
    __u32			pixelformat;
    __u32			field;		// enum v4l2_field
    __u32            	bytesperline;	// for padding, zero if unused
    __u32          		sizeimage;
    __u32			colorspace;	// enum v4l2_colorspace
    __u32			priv;		// private data, depends on pixelformat
    __u32			flags;		// format flags (V4L2_PIX_FMT_FLAG_*)
    __u32			ycbcr_enc;	// enum v4l2_ycbcr_encoding
    __u32			quantization;	// enum v4l2_quantization
    __u32			xfer_func;	// enum v4l2_xfer_func
    */

    if(ioctl(fd, VIDIOC_S_FMT, &imageFormat) < 0) {
        fprintf(stderr,"Device could not set format: %s\n",strerror(errno));
    }

    //
    // setup fps
    //
    v4l2_streamparm streamparm;
    memset (&streamparm, 0, sizeof (streamparm));

    streamparm.type = fmt.type;
    streamparm.parm.capture.timeperframe = interval.discrete;

    if (ioctl(fd, VIDIOC_S_PARM, &streamparm) < 0) {
        fprintf(stderr,"Failed to set camera FPS: %s\n",strerror(errno));
    }

}


void Device::setNumberOfInputBuffers(int count){
    v4l2_requestbuffers bufrequest;
    memset(&bufrequest, 0, sizeof(bufrequest));

    bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufrequest.memory = V4L2_MEMORY_MMAP;
    bufrequest.count = count;

    if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0){
        fprintf(stderr,"VIDIOC_REQBUFS: %s\n",strerror(errno));
        exit(-1);
    }
}

v4l2_buffer Device::getBufferInformationFromDevice(int bufferIndex) {
    v4l2_buffer bufferinfo;
    memset(&bufferinfo, 0, sizeof(bufferinfo));

    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = bufferIndex;

    if(ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo) < 0){
        fprintf(stderr,"VIDIOC_QUERYBUF ( bufferindex: %i): %s\n",bufferIndex,strerror(errno));
        exit(-1);
    }

    return bufferinfo;
}

void* Device::getBufferPointer(const v4l2_buffer &bufferinfo) {
    void* buffer_ptr = mmap(
        NULL,
        bufferinfo.length,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        bufferinfo.m.offset
    );

    if(buffer_ptr == MAP_FAILED){
        fprintf(stderr,"mmap error.\n");
        exit(-1);
    }

    return buffer_ptr;
}

void Device::queueBuffer(int bufferindex, v4l2_buffer *bufferinfo) {
    //v4l2_buffer bufferinfo;
    memset(bufferinfo, 0, sizeof(v4l2_buffer));

    bufferinfo->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo->memory = V4L2_MEMORY_MMAP;
    bufferinfo->index = bufferindex;

    if(ioctl(fd, VIDIOC_QBUF, bufferinfo) < 0){
        fprintf(stderr,"VIDIOC_QBUF: %s\n",strerror(errno));
        exit(-1);
    }
}

void Device::dequeueBuffer(v4l2_buffer *bufferinfo){
    //v4l2_buffer bufferinfo;
    memset(bufferinfo, 0, sizeof(v4l2_buffer));

    bufferinfo->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo->memory = V4L2_MEMORY_MMAP;
    //bufferinfo->index = bufferindex;

    if(ioctl(fd, VIDIOC_DQBUF, bufferinfo) < 0){
        fprintf(stderr,"VIDIOC_DQBUF: %s\n",strerror(errno));
        exit(-1);
    }

//    return bufferinfo;
}

void Device::streamON(){
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
        fprintf(stderr,"VIDIOC_STREAMON: %s\n",strerror(errno));
        exit(-1);
    }
    streaming = true;
}

void Device::streamOFF(){
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
        fprintf(stderr,"VIDIOC_STREAMOFF: %s\n",strerror(errno));
        exit(-1);
    }
    streaming = false;
}
