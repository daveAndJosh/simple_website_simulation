//
// Created by root on 2/20/23.
//

#ifndef SIMPLE_WEBSITE_SIMULATION_APIGATEWAY_H
#define SIMPLE_WEBSITE_SIMULATION_APIGATEWAY_H
#include "Packet.h"
namespace sim{
    struct APIGatewayState{
        double r = std::numeric_limits<double>::infinity();
        std::queue<Packet> queue;
    };

    std::ostream& operator<<(std::ostream& out, const APIGatewayState& s) {
        out <<"{r: "<< s.r
            <<" queue_size:"<<s.queue.size()
            <<"}";
        return out;
    }
    class APIGateway : public cadmium::Atomic<APIGatewayState>{
    public:
        constexpr static char NAME[] = "API_GATEWAY";
        constexpr static double PROCESS_TIME = 0.001;
        cadmium::BigPort<Packet> resOut;
        cadmium::BigPort<Packet> reqOut;
        cadmium::BigPort<Packet> resIn;
        cadmium::BigPort<Packet> reqIn;
        APIGateway(const std::string& id): Atomic<APIGatewayState>(id, APIGatewayState()){
            resIn = addInBigPort<Packet>("resIn");
            reqIn = addInBigPort<Packet>("reqIn");
            resOut = addOutBigPort<Packet>("resOut");
            reqOut = addOutBigPort<Packet>("reqOut");
        }
        void internalTransition(APIGatewayState& s) const override {
            if(!s.queue.empty())
                s.queue.pop();
            s.r = s.queue.empty()?std::numeric_limits<double>::infinity():PROCESS_TIME;
        }

        void externalTransition(APIGatewayState &s, double e) const override {
            if(s.r == std::numeric_limits<double>::infinity()){
                s.r = PROCESS_TIME;
            }
            else{
                s.r = s.r - e;
            }
            s.queue.emplace(resIn.get()->getBag().back());
            s.queue.emplace(reqIn.get()->getBag().back());
        }

        void output(const APIGatewayState& s) const override {
            auto front = s.queue.front();
            if(front.type == PacketType::REQUEST){
                reqOut->addMessage(front);
            }
            else if(front.type == PacketType::RESPONSE){
                resOut->addMessage(front);
            }
            else{
                std::stringstream message;
                message<<"invalid packet: " << front;
                throw std::runtime_error(message.str());
            }
        }
        [[nodiscard]] double timeAdvance(const APIGatewayState& s) const override {
            return s.r;
        }

    };


}
#endif //SIMPLE_WEBSITE_SIMULATION_APIGATEWAY_H
