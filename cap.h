#ifndef CAP_H
#define CAP_H

#include <QMainWindow>

#define HAVE_REMOTE
#define LINE_LEN 16
#define CONF_PACK_SIZE 60

#include <pcap.h>

class CAP : public QMainWindow
{
    Q_OBJECT

public:
    CAP();
    bool sendPacket(const char *);
    bool openInterface(const int);
private:
    pcap_if_t *alldevs, *d;
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];
};

#endif // CAP_H
