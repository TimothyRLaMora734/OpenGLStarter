#ifndef NETWORK_CONSTANTS_H
#define NETWORK_CONSTANTS_H


#ifndef byte
	typedef unsigned char byte;
#endif

#ifndef uint
	typedef unsigned int uint;
#endif

#ifndef ushort
	typedef unsigned short ushort;
#endif

namespace aRibeiro
{
	namespace NetworkConstants
    {
	
        const int UDP_MAX_PACKET_SIZE = 0xffff;
        const int UDP_HEADER_SIZE = 8;
        const int IP_V4_HEADER_SIZE = 20;
        const int UDP_IPV4_TOTAL_HEADER_SIZE = IP_V4_HEADER_SIZE + UDP_HEADER_SIZE;

        //Known MTUs (Maximum transfer units)

        // Ethernet: 1500 - UDP_IPV4_TOTAL_HEADER_SIZE = 1472
        // test: ping 10.0.0.105 -f -l 1472
        const int ETHERNET_MTU = 1500;

        // Internet: 1492 - UDP_IPV4_TOTAL_HEADER_SIZE = 1464
        // test: ping www.tp-link.com -f -l 1464
        // test: ping google.com -f -l 1464
        const int INTERNET_MTU = 1492;

        // Minimum packet size: 576 - UDP_IPV4_TOTAL_HEADER_SIZE = 548
        // StackOverflow - Safest to transmit
        const int MINIMUM_MTU = 576;

        //generates fragmentation
        const int UDP_DATA_MAX_DATAGRAM_SIZE = UDP_MAX_PACKET_SIZE - UDP_IPV4_TOTAL_HEADER_SIZE;

        const int UDP_DATA_MTU_ETHERNET = ETHERNET_MTU - UDP_IPV4_TOTAL_HEADER_SIZE;
        const int UDP_DATA_MTU_INTERNET = INTERNET_MTU - UDP_IPV4_TOTAL_HEADER_SIZE;
        const int UDP_DATA_MTU_MINIMUM = MINIMUM_MTU - UDP_IPV4_TOTAL_HEADER_SIZE;

        const int UDP_DATA_MTU_LOCALHOST = UDP_DATA_MAX_DATAGRAM_SIZE;


        // SELECTED MTU TO WORK
        const int UDP_DATA_MAXIMUM_MTU = UDP_DATA_MTU_INTERNET;
        const int UDP_DATA_MINIMUM_MTU = UDP_DATA_MTU_MINIMUM;

        // try to avoid overlap with the OS ephemeral port 
        const int PUBLIC_PORT_START = 5001;
        const int PUBLIC_PORT_END = 18884;
        const int PUBLIC_PORT_MULTICAST_START = PUBLIC_PORT_END + 1;
        const int PUBLIC_PORT_MULTICAST_END = 32767;

    }
}

#endif
