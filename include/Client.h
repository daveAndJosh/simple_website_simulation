#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
#include <cadmium/core/modeling/coupled.hpp>
#include "ClientGenerator.h"
#include "ClientDispatcher.h"
#include "PacketCounter.h"

namespace sim {
    struct Client : public cadmium::Coupled {

        cadmium::BigPort<Packet> webOutPort, apiOutPort;
        cadmium::BigPort<Packet> webInPort, apiInPort;
        /**
         * Client
         * @param period the period of how often to send website request messages
         */
        Client(double period): Coupled("Client") {
            auto generator = addComponent<ClientGenerator>("ClientGenerator", period);
            auto dispatcher = addComponent<ClientDispatch>("ClientDispatch");
            auto counter = addComponent<PacketCounter>();

            //make ports
            webInPort = addInBigPort<Packet>("Client Web In");
            apiInPort = addInBigPort<Packet>("Client API In");
            webOutPort = addOutBigPort<Packet>("Client Web Out");
            apiOutPort = addOutBigPort<Packet>("Client API Out");
            //internal couplings
            addIC(generator->webRequestPort, dispatcher->in);
            //external couplings
            addEIC(webInPort, dispatcher->in);
            addEIC(apiInPort, counter->packetInputPort);

            addEOC(dispatcher->pageOut, webOutPort);
            addEOC(dispatcher->apiOut, apiOutPort);
        }
    };
}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
