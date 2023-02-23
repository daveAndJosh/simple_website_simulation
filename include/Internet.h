#ifndef CADMIUM_EXAMPLE_EFP_GPT_HPP_
#define CADMIUM_EXAMPLE_EFP_GPT_HPP_

#include <cadmium/core/modeling/coupled.hpp>
#include "Client.h"
#include "Cloud.h"


namespace sim {
    struct Internet : public cadmium::Coupled {
        /**
         *
         * @param id
         */
        Internet(const std::string& id, const double sendPeriod): Coupled(id) {
            auto client = addComponent<Client>(sendPeriod);
            auto cloud = addComponent<Cloud>();

            addIC(client->apiOutPort, cloud->apiRequestFromClient);
            addIC(client->webOutPort, cloud->webRequestFromClient);
            addIC(cloud->webResponseToClient, client->webInPort);
            addIC(cloud->apiResponseToClient, client->apiInPort);

        }
    };
}

#endif
