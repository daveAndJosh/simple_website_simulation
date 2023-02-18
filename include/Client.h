#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
#include <cadmium/core/modeling/coupled.hpp>
#include "ClientGenerator.h"
namespace sim {
    struct Client : public cadmium::Coupled {

        /**
         * Client
         * @param id the id of the client
         * @param period the period of how often to send website request messages
         */
        Client(const std::string& id, double period): Coupled(id) {
            auto generator = addComponent<ClientGenerator>("ClientGenerator", period);
        }
    };
}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_CLIENT_H
