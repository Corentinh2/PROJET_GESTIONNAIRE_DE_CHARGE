#ifndef COMMUNICATION_MOBILE_H
#define COMMUNICATION_MOBILE_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "MemoireProgramme.h"

#define PORT 5555

using namespace websockets;

class CommunicationMobile
{
private:
  WebsocketsServer serveur;
  WebsocketsClient client;
  MemoireProgramme *memoire;
  bool clientConnecte;

  int relais;
  void traiterMessage(String data);

public:
  CommunicationMobile(MemoireProgramme *memoire);
  ~CommunicationMobile();
  int gererCommunication();
};

#endif