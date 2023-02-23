//
// Created by dankvid on 2/22/23.
//

#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_EXECUTOR_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_EXECUTOR_H
#include <cadmium/core/modeling/atomic.hpp>
#include <limits>
#include "Packet.h"
namespace sim{

struct ExecutorState{
    enum class Status {SLEEPING, WAKING_UP, PARSING, WAITING, COMPUTING};
    Status status = Status::SLEEPING;
   Packet current;
};

class Exector : public cadmium::Atomic<ExecutorState> {
public:
    constexpr static char NAME_PREFIX[] = "Executor";
    cadmium::BigPort<Packet> reqIn, dbInPort, dbOutPort, respOut;
    cadmium::BigPort<std::string> notify;
    explicit Exector(const std::string& name) : cadmium::Atomic<ExecutorState>(name, ExecutorState()){
        reqIn = addInBigPort<Packet>("Request In");
        respOut = addOutBigPort<Packet>("Response Out");
        dbInPort = addInBigPort<Packet>("Database In");
        dbOutPort = addOutBigPort<Packet>("Database Out");
        notify = addOutBigPort<std::string>("Notify port");

    }

    void internalTransition(ExecutorState& s) const override {
        switch (s.status) {
            case ExecutorState::Status::WAKING_UP:
                s.status = ExecutorState::Status::PARSING;
                break;
            case ExecutorState::Status::PARSING:
                s.status = ExecutorState::Status::WAITING;
                break;
            case ExecutorState::Status::COMPUTING:
                s.status = ExecutorState::Status::SLEEPING;
                break;
            default:
                throw std::runtime_error("invalid transition");
        }
    }

    void externalTransition(ExecutorState&s, double e) const override {
        if(!dbInPort->empty()){
            s.status = ExecutorState::Status::COMPUTING;
            dbInPort->clear();
        }
        if(!reqIn->empty()){
            s.status = ExecutorState::Status::WAKING_UP;
            auto toCopy = reqIn->getBag().back().get();
            s.current = *toCopy;
            reqIn->clear();
        }


    }

    void output(const ExecutorState& s) const override {
        switch (s.status) {

            case ExecutorState::Status::PARSING: //once we're done parsing, we ask the database for data
                dbOutPort->addMessage(Packet(s.current.creationTime, PacketType::REQUEST, this->getId(), "Database"));
                break;
            case ExecutorState::Status::COMPUTING: //once we're done computing, we're done with the request
                notify->addMessage(this->getId());
                respOut->addMessage(Packet(s.current.creationTime, PacketType::RESPONSE, this->getId(), "JavaScript endpoint"));
                break;
            default:
                break;
        }
        
    }
    [[nodiscard]] double timeAdvance(const ExecutorState& s) const override {
        switch (s.status) {

            case ExecutorState::Status::SLEEPING:
               return std::numeric_limits<double>::infinity();
            case ExecutorState::Status::WAKING_UP:
                return 3.00;
            case ExecutorState::Status::PARSING:
                return 1.00;
            case ExecutorState::Status::WAITING:
                return std::numeric_limits<double>::infinity();
            case ExecutorState::Status::COMPUTING:
                return 2.00;
        }
    }

};

std::ostream& operator<<(std::ostream& out, const ExecutorState::Status& type){
    std::string s;
    switch(type){
        case ExecutorState::Status::SLEEPING:
            s = "SLEEPING";
            break;
        case ExecutorState::Status::WAKING_UP:
            s = "WAKING_UP";
            break;
        case ExecutorState::Status::PARSING:
            s = "PARSING";
            break;
        case ExecutorState::Status::WAITING:
            s = "WAITING";
            break;
        case ExecutorState::Status::COMPUTING:
            s = "COMPUTING";
            break;
    }
    out<<s;
    return out;
}

std::ostream& operator<<(std::ostream& out, const ExecutorState& s) {
    out <<"{state: "<<s.status<<"}";
    return out;
}


}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_EXECUTOR_H
