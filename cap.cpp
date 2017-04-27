#include "cap.h"

#include <ntddndis.h>
#include <Packet32.h>

CAP::CAP()
{

}

int CAP::getInterfaceCount()
{
    int num;
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1){
        num = 0;
    }
    else{
        for(d= alldevs; d != NULL; d= d->next){
            ++num;
        }
    }
    return num;
}

QString CAP::getDescription(const int in)
{
    QString descr;
    /* find all available devices */
    pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
    int i = 0;

    /* jump to the adapter */
    for (d = alldevs, i; i < in - 1; d = d->next, i++);

    if (d->description){
        descr.insert(0, d->description);
    }
    return descr;
}

bool getMac(char* pStr, char* ifName)
{
    PPACKET_OID_DATA pOidData;
    LPADAPTER lpAdapter=NULL;
    CHAR pAddr[sizeof(PACKET_OID_DATA)+5];
    ZeroMemory(pAddr, sizeof(PACKET_OID_DATA)+5);
    pOidData = (PPACKET_OID_DATA) pAddr;
    pOidData->Oid = OID_802_3_CURRENT_ADDRESS;
    pOidData->Length = 6;

    lpAdapter = PacketOpenAdapter((PCHAR)ifName);
    if(PacketRequest(lpAdapter, FALSE, pOidData))
    {
        sprintf(pStr, "%.02x:%.02x:%.02x:%.02x:%.02x:%.02x",
                pOidData->Data[0],pOidData->Data[1],pOidData->Data[2],
                pOidData->Data[3],pOidData->Data[4],pOidData->Data[5]);
    }else{
        return FALSE;
    }
    return TRUE;
}

bool CAP::getLocalMac(const int in)
{
    QString name;
    char *pName;

    /* find all available devices */
    pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf);
    int i = 0;

    /* jump to the adapter */
    for (d = alldevs, i; i < in - 1; d = d->next, i++);

    if (d->name){
        name.insert(0, d->name);
    }
    /* chop the prefix */
    name = name.remove(QRegExp("(.){0,2}cap:\/\/"));
    QByteArray ba = name.toLatin1();
    pName = ba.data();

    if(getMac(pMac, pName))
        return TRUE;
    else
        return FALSE;
}

QString CAP::returnMac(const int num)
{
    QString ret;
    ret.insert(0, pMac[num*3]);
    ret.append(pMac[num*3 + 1]);
    return ret;
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

bool CAP::sendPacket(const char * packet)
{
    if (pcap_sendpacket(fp, (const u_char*)packet, CONF_PACK_SIZE) != 0)
        return false;
    else
        return true;
}
