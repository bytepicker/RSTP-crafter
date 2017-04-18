#include "cap.h"

CAP::CAP()
{

}

bool CAP::sendPacket(const char * packet)
{
    if (pcap_sendpacket(fp, (const u_char*)packet, CONF_PACK_SIZE) != 0)
        return false;
    else
        return true;
}

bool CAP::openInterface(const int num)
{
    /* find all available devices */
    pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
    int i = 0;

    /* jump to the adapter */
    for (d = alldevs, i; i < num - 1; d = d->next, i++);

    /* opening */
    if ((fp = pcap_open(d->name,
        100,						/*snaplen*/
        PCAP_OPENFLAG_PROMISCUOUS || PCAP_OPENFLAG_NOCAPTURE_LOCAL || PCAP_OPENFLAG_NOCAPTURE_RPCAP,	/*flags*/
        20,							/*read timeout*/
        NULL,						/* remote authentication */
        errbuf)
        ) == NULL)
    {
        return false;
    }
    else{
        return true;
    }
}
