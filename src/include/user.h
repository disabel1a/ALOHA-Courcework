#ifndef _USER_
#define _USER_

#include <queue>

#include "event.h"
#include "simulation_parameters.h"

class user
{
private:
    double probability;
public:
    std::queue<event> queue;

    user(double probability);
    ~user();

    bool try_to_send();
    void adapt_probability(CHANNEL_STATUS &status, std::size_t &users_in_channel);
    void set_probability(double probability);
};


#endif // !_USER_