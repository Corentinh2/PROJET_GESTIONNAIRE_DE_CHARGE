#ifndef COMMUNICATION_MOBILE_H
#define COMMUNICATION_MOBILE_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "MemoireProgramme.h"
#include "RelaisCommande.h"

using namespace websockets;

class CommunicationMobile {
  private:
    WebsocketsServer serveur;
    WebsocketsClient client;
    MemoireProgramme *memoire;
    RelaisCommande *relais;
    bool clientConnecte;

    void traiterMessage(String message);

  public:
    CommunicationMobile(MemoireProgramme *memoire, RelaisCommande *relais);
    ~CommunicationMobile();
    void gerer();
};

#endif