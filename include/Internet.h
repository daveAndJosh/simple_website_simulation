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
            auto generator = addComponent<Client>(sendPeriod);
            auto cloud = addComponent<Cloud>();
        }
    };
}

#endif
