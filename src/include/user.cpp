#include "user.h"

#include <random>

user::user(double probability) : probability(probability)
{
}

user::~user()
{
}

bool user::try_to_send()
{
    if (static_cast<double>(rand())/RAND_MAX <= probability)
    {
        return true;
    }
    return false;
}

void user::adapt_probability(CHANNEL_STATUS &status, std::size_t &users_in_channel)
{
    switch (status)
    {
    case CHANNEL_STATUS::CONFLICT:
        this->probability = std::max((1.0 / static_cast<double>(users_in_channel)), probability / 2.0);
        break;
    case CHANNEL_STATUS::SUCCESS:
        break;
    case CHANNEL_STATUS::EMPTY:
        this->probability = std::min(1.0, 2.0 * probability);
        break;
    }
}

void user::set_probability(double probability)
{
    this->probability = probability;
}
