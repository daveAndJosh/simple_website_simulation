#ifndef MY_SERVERLESS_RECIPE_SITE_SIM_CLIENTGENERATOR_H
#define MY_SERVERLESS_RECIPE_SITE_SIM_CLIENTGENERATOR_H

#include <cadmium/core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

#include "Packet.h"
namespace sim{
    struct ClientGenState{
        double clock;
    };

    std::ostream& operator<<(std::ostream& out, const ClientGenState& s) {
        out << s.clock;
        return out;
    }
    class ClientGenerator : public cadmium::Atomic<ClientGenState>{
    private:
        const double sendPeriod;
    public:
        constexpr static char NAME[] = "CLIENT_GENERATOR";
        cadmium::BigPort<Packet> webRequestPort;
        ClientGenerator(const std::string& id, const double period): Atomic<ClientGenState>(id, ClientGenState()),
            sendPeriod(period){
            webRequestPort = addOutBigPort<Packet>("webRequestPort");
        }
        void internalTransition(ClientGenState& s) const override {
            s.clock +=sendPeriod;
        }

        void externalTransition(ClientGenState &s, double e) const override {
        }

        void output(const ClientGenState& s) const override {
            webRequestPort->addMessage(s.clock+sendPeriod, PacketType::REQUEST, NAME, "PAGE");
            //todo: replace server with constant
        }
        [[nodiscard]] double timeAdvance(const ClientGenState& s) const override {
            return sendPeriod;
        }

    };


}

#endif //MY_SERVERLESS_RECIPE_SITE_SIM_CLIENTGENERATOR_H
