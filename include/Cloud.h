//
// Created by root on 2/21/23.
//

#ifndef SIMPLE_WEBSITE_SIMULATION_CLOUD_H
#define SIMPLE_WEBSITE_SIMULATION_CLOUD_H

#include "Bucket.h"
#include "Database.h"
#include "APIGateway.h"
#include "Lambda.h"
#include <cadmium/core/modeling/coupled.hpp>
namespace sim {
    struct Cloud : public cadmium::Coupled {

        cadmium::BigPort<Packet> apiRequestFromClient, webRequestFromClient;
        cadmium::BigPort<Packet> apiResponseToClient, webResponseToClient;
        /**
         * Client
         * @param period the period of how often to send website request messages
         */
        Cloud(int j): Coupled("Cloud") {
            auto bucket = addComponent<Bucket>("Bucket");
            auto apiGateway = addComponent<APIGateway>("APIGateway");
            auto lambda = addComponent<Lambda>(j);
            auto database = addComponent<Database>("Database");


            //make ports
            apiRequestFromClient = addInBigPort<Packet>("API Request in");
            apiResponseToClient = addOutBigPort<Packet>("API Response out");
            webRequestFromClient = addInBigPort<Packet>("Web Request in");
            webResponseToClient = addOutBigPort<Packet>("Client Response Out");
            //internal couplings

            addIC(apiGateway->reqOut, lambda->reqIn);
            addIC(lambda->dbSend, database->reqIn);
            addIC(database->resOut, lambda->dbReceive);
            addIC(lambda->rspOut, apiGateway->resIn);

            //external couplings
            addEIC(apiRequestFromClient, apiGateway->reqIn);
            addEIC(webRequestFromClient, bucket->reqIn);
            addEOC(apiGateway->resOut, apiResponseToClient);
            addEOC(bucket->resOut, webResponseToClient);
        }
        Cloud(): Coupled("Cloud") {
            auto bucket = addComponent<Bucket>("Bucket");
            auto apiGateway = addComponent<APIGateway>("APIGateway");
            auto lambda = addComponent<Lambda>(2);
            auto database = addComponent<Database>("Database");


            //make ports
            apiRequestFromClient = addInBigPort<Packet>("API Request in");
            apiResponseToClient = addOutBigPort<Packet>("API Response out");
            webRequestFromClient = addInBigPort<Packet>("Web Request in");
            webResponseToClient = addOutBigPort<Packet>("Client Response Out");
            //internal couplings

            addIC(apiGateway->reqOut, lambda->reqIn);
            addIC(lambda->dbSend, database->reqIn);
            addIC(database->resOut, lambda->dbReceive);
            addIC(lambda->rspOut, apiGateway->resIn);

            //external couplings
            addEIC(apiRequestFromClient, apiGateway->reqIn);
            addEIC(webRequestFromClient, bucket->reqIn);
            addEOC(apiGateway->resOut, apiResponseToClient);
            addEOC(bucket->resOut, webResponseToClient);
        }
    };
}
#endif //SIMPLE_WEBSITE_SIMULATION_CLOUD_H
