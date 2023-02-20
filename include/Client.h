#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
#include <cadmium/core/modeling/coupled.hpp>
#include "ClientGenerator.h"
#include "ClientDispatcher.h"
namespace sim {
    struct Client : public cadmium::Coupled {

        cadmium::BigPort<Packet> webOutPort, apiOutPort;
        cadmium::BigPort<Packet> webInPort;
        /**
         * Client
         * @param period the period of how often to send website request messages
         */
        Client(double period): Coupled("Client") {
            auto generator = addComponent<ClientGenerator>("ClientGenerator", period);
            auto dispatcher = addComponent<ClientDispatch>("ClientDispatch");

            //make ports
            webInPort = addOutBigPort<Packet>("Client Web In");
            webOutPort = addOutBigPort<Packet>("Client Web Out");
            apiOutPort = addOutBigPort<Packet>("Client API Out");
            //internal couplings
            addIC(generator->webRequestPort, dispatcher->in);
            //external couplings
            addEIC(webInPort, dispatcher->in);
            addEOC(dispatcher->pageOut, webOutPort);
            addEOC(dispatcher->apiOut, apiOutPort);
        }
    };
}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
