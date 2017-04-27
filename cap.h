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
    int getInterfaceCount();
    QString getDescription(const int);
    bool getLocalMac(const int);
    QString returnMac(const int);
    bool openInterface(const int);
    bool sendPacket(const char *);
private:
    pcap_if_t *alldevs, *d;
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    char pMac[20];
};

#endif // CAP_H
