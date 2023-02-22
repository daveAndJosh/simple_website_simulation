//
// Created by root on 2/21/23.
//

#ifndef SIMPLE_WEBSITE_SIMULATION_CLOUD_H
#define SIMPLE_WEBSITE_SIMULATION_CLOUD_H

#include "Bucket.h"
#include "Database.h"
#include "APIGateway.h"
#include <cadmium/core/modeling/coupled.hpp>
namespace sim {
    struct Cloud : public cadmium::Coupled {

        cadmium::BigPort<Packet> apiRequestFromClient, webRequestFromClient;
        cadmium::BigPort<Packet> responseToClient;
        /**
         * Client
         * @param period the period of how often to send website request messages
         */
        Cloud(): Coupled("Cloud") {
            auto bucket = addComponent<Bucket>("Bucket");
            auto apiGateway = addComponent<APIGateway>("APIGateway");
            //auto lambda = addComponent<Lambda>("Lambda");
            auto database = addComponent<Database>("Database");


            //make ports
            apiRequestFromClient = addInBigPort<Packet>("API Request in");
            webRequestFromClient = addInBigPort<Packet>("Web Request in");
            responseToClient = addOutBigPort<Packet>("Client Response Out");
            //internal couplings

            //addIC(apiGateway->reqOut, lambda->reqIn);
            //addIC(lambda->reqOut, database->reqIn);

            //external couplings
            addEIC(apiRequestFromClient, apiGateway->reqIn);
            addEIC(webRequestFromClient, bucket->reqIn);

            addEOC(bucket->resOut, responseToClient);
        }
    };
}
#endif //SIMPLE_WEBSITE_SIMULATION_CLOUD_H
