

/**
   trying to fix broken ARP reply in ESP8266
   but... works fine on linux?  not on macbook. :(
   https://github.com/me-no-dev/ESPAsyncWebServer/issues/54#issuecomment-234742606
*/

#include <Ticker.h>

Ticker arpTicker;
//bool arp_call_flag = false;

extern "C" {
  //  extern char *netif_list;
  extern struct netif *netif_list;
  uint8_t etharp_request(char *, char *);
  //#define etharp_gratuitous(netif) etharp_request((netif), netif_ip4_addr(netif))
}

void forceARP() {
  //  char *netif = netif_list;
  char *netif = *((char **) netif_list);

  while (netif)
  {
    etharp_request((netif), (netif + 4));
    netif = *((char **) netif);
  }
}

// call from task each second....
//void GratuitousARPTask() {
//  netif *n = netif_list;
//  while (n) {
//    etharp_gratuitous(n);
//    n = n->next;
//  }
//}

// set the one second flag, this function is called every 1 second
//void setTickflag() {
//  arp_call_flag = true;
//}

void fixarp_setup()
{
//  arpTicker.attach(2, setTickflag);  // will call setTickflag every 2 seconds
  arpTicker.attach(2, forceARP);  // force ARP every 2 secs.
//  arp_call_flag = false;
}

//void fixarp_update()
//{
//  if (arp_call_flag) {
//    arp_call_flag = false;
//    forceARP();
//  }
//}
