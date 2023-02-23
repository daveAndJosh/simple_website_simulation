//
// Created by dankvid on 2/22/23.
//
#include <cadmium/core/modeling/coupled.hpp>

#include "Packet.h"
#include "LambdaDispatcher.h"
#include "Executor.h"
#include "ClientDispatcher.h"
#include "Switch.h"

#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_LAMBDA_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_LAMBDA_H
namespace sim{
    struct Lambda : public cadmium::Coupled{
        cadmium::BigPort<Packet> dbReceive;
        cadmium::BigPort<Packet> dbSend;
        cadmium::BigPort<Packet> rspOut;
        cadmium::BigPort<Packet> reqIn;
        std::set<std::string> names;
        std::vector<std::shared_ptr<Exector>> executors;
        Lambda(int exNum) : cadmium::Coupled("Lambda"){
            //make components
            for(int i=0;i<exNum; i++){
                std::stringstream namestream;
                namestream << Exector::NAME_PREFIX << i;
                auto name = namestream.str();
                names.emplace(name);
                executors.push_back(addComponent<Exector>(name));
            }
            auto dispatcher = addComponent<LambdaDispatcher>(names);
            auto comSwitch = addComponent<Switch>(names);
            //make ports
            dbReceive = addInBigPort<Packet>("Database Request Port");
            dbSend = addOutBigPort<Packet>("Database Response Port");
            rspOut = addOutBigPort<Packet>("API Receive Port");
            reqIn = addInBigPort<Packet>("API Send Port");

            //IC
            for(const auto& exec: executors){
                const auto name = exec->getId();
                const auto& inPort = exec->reqIn;
                const auto& feedbackIn = dispatcher->isReadyPort;
                const auto& feedbackOut = exec->notify;
                const auto& outPort = dispatcher->execPorts[name];
                addIC(outPort, inPort);
                addIC(feedbackOut, feedbackIn);
                addIC(comSwitch->messageOut[name], exec->dbInPort);
                addIC(exec->dbOutPort, comSwitch->messageIn);
            }


            //EOC
            for(const auto& exec: executors){
                const auto& databaseOutPort = exec->dbOutPort;
                const auto& resultOut= exec->respOut;
                addEOC(databaseOutPort, dbSend);
                addEOC(resultOut, rspOut);
            }
            comSwitch->addOutPort("Database");
            addEOC(comSwitch->messageOut["Database"], dbSend);
            //EIC
            addEIC(dbReceive, comSwitch->messageIn);
            addEIC(reqIn, dispatcher->queueIn);

        }


    };
}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_LAMBDA_H
