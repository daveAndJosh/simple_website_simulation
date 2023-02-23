//
// Created by dankvid on 2/22/23.
//

#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_LAMBDADISPATCHER_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_LAMBDADISPATCHER_H
#include <cadmium/core/modeling/atomic.hpp>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include <limits>

#include "Packet.h"
namespace sim{

    struct LambdaDispatcherState{
        std::queue<Packet> queue;
        double r = std::numeric_limits<double>::infinity();
        std::map<std::string, bool> execs; //name/availability

        enum class Status {EMPTY, WORKING, BLOCKED};
        Status status = Status::EMPTY;

        [[nodiscard]] std::vector<std::string> availableExecs() const{
            std::vector<std::string> output = {};
            for(const auto& exec : execs){
                if(exec.second){
                    output.push_back(exec.first);
                }
            }
            return output;
        }

        [[nodiscard]] std::string getFirstAvailableExec() const{
            return availableExecs().front();
        }

        void disableFirstAvailable(){
            auto name = getFirstAvailableExec();
            execs[name] = false;
        }
    };

    class LambdaDispatcher : public cadmium::Atomic<LambdaDispatcherState>{
    public:
        constexpr static char NAME[] = "LAMBDA DISPATCHER";
        constexpr static double PROCESS_TIME = 0.01;
        std::map<std::string, cadmium::BigPort<Packet>> execPorts;
        cadmium::BigPort<std::string> isReadyPort;
        cadmium::BigPort<Packet> queueIn;
        explicit LambdaDispatcher(const std::set<std::string>& execs) :
            cadmium::Atomic<LambdaDispatcherState>("LambdaDispatch", LambdaDispatcherState()){

            for(const auto& exec : execs){
                auto port = addOutBigPort<Packet>(exec);
                execPorts.emplace(exec, port);

                this->state.execs.emplace(exec, true);
            }

            isReadyPort = addInBigPort<std::string>("IsReady");
            queueIn = addInBigPort<Packet>("requestIn");

        }

        void internalTransition(LambdaDispatcherState& s) const override {
            if(!s.queue.empty()){
                s.queue.pop();
                s.disableFirstAvailable();
            }
            changeState(s);
            if(s.status == LambdaDispatcherState::Status::WORKING){
                s.r = PROCESS_TIME;
            }
            else{
                s.r= std::numeric_limits<double>::infinity();
            }

        }

        void externalTransition(LambdaDispatcherState &s, double e) const override {
            handleQueueIn(s);
            handleReady(s);
            changeState(s);
            if(s.status == LambdaDispatcherState::Status::WORKING){
                continueSr(s, e);
            }
            else{
                s.r = std::numeric_limits<double>::infinity();
            }

        }

        void output(const LambdaDispatcherState& s) const override {
            if(s.queue.empty()){
                return;
            }
            execPorts.at(s.getFirstAvailableExec())->addMessage(s.queue.front());
        }
        [[nodiscard]] double timeAdvance(const LambdaDispatcherState& s) const override {
            return s.r;
        }
    private:
        void handleQueueIn(LambdaDispatcherState& s) const {
            //do nothing if port is empty
            if(queueIn->empty()){
                return;
            }
            fillQueue(s);
        }

        void changeState(LambdaDispatcherState& s) const {
            if(s.queue.empty()){
                s.status = LambdaDispatcherState::Status::EMPTY;
            }
            else if(s.availableExecs().empty()){
                s.status = LambdaDispatcherState::Status::BLOCKED;
            }
            else{
                s.status = LambdaDispatcherState::Status::WORKING;
            }
        }

        void fillQueue(LambdaDispatcherState& s) const {
            for(const auto& reqs: queueIn->getBag()){
                s.queue.emplace(reqs);
            }
            queueIn->clear();
        }

        void handleReady(LambdaDispatcherState& s) const {
            if(isReadyPort->empty()){
                return;
            }
            for(const auto& execName: isReadyPort->getBag()){
                s.execs[*execName] = true;
            }
            isReadyPort->clear();
        }
        void continueSr(LambdaDispatcherState& s, double e) const{
            if(s.r ==std::numeric_limits<double>::infinity()){
                s.r=PROCESS_TIME;
            }
            else{
                s.r -=e;
            }

        }
    };
    std::ostream& operator<<(std::ostream& out, const LambdaDispatcherState::Status& type){
        std::string s;
        switch(type){
            case LambdaDispatcherState::Status::EMPTY:
                s = "EMPTY";
                break;
            case LambdaDispatcherState::Status::BLOCKED:
                s = "BLOCKED";
                break;
            default:
                s = "WORKING";
        }
        out<<s;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const LambdaDispatcherState& s) {
        out <<"{r: "<< s.r
            <<", queue_size:"<<s.queue.size()
            <<", state: "<<s.status
            <<", Execs: [";
        bool first = true;
        for(const auto& exec : s.execs){
            if(first){
                first = false;
            }
            else {
                out<<", ";
            }
            out<<(exec.second?"'Available'":"'In Use'");
        }
        out<<"]}";
        return out;
    }
}
#endif //MY_SERVERLESS_RECIPE_SITE_SIM_LAMBDADISPATCHER_H
