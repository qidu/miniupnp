// sudo apt install libminiupnpc17
// sudo apt install libminiupnpc-dev
// dpkg -L libminiupnpc17
// gcc -I/usr/include -o upnp_mapping upnp_mapping.c /usr/lib/x86_64-linux-gnu/libminiupnpc.a
//
#include <stdio.h>
#include <stdlib.h>
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"

int main(void) 
{
    struct UPNPUrls urls;
    struct IGDdatas data;
    char lanAddress[64];
    int error = 0;

    // 初始化UPnP
    struct UPNPDev *devlist = upnpDiscover(2000, NULL, NULL, UPNP_LOCAL_PORT_ANY, 0, 2, &error);
    if (devlist == NULL) {
        printf("Error discovering UPnP devices: %s\n", error != 0 ? strupnperror(error) : "NULL");
        return 1;
    }

    // 获取路由器URL和控制URL
    UPNP_GetValidIGD(devlist, &urls, &data, lanAddress, sizeof(lanAddress));

    // 添加端口映射
    const char *externalPort = "12345";
    const char *internalHost = lanAddress;
    const char *internalPort = "8080";
    const char *protocol = "TCP";
    const char *description = "My Port Mapping";

    error = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                               externalPort, internalPort, internalHost,
                               description, protocol, NULL, "0");
    if (error != UPNPCOMMAND_SUCCESS) {
        printf("Error adding port mapping: %s\n", strupnperror(error));
        return 1;
    }

    printf("Port mapping added successfully!\n");

    // 释放资源
    freeUPNPDevlist(devlist);
    return 0;
}
