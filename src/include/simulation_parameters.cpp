#include "simulation_parameters.h"

#include <math.h>
#include <iostream>

double SIM_PARAMS::default_user_probability(0.5);

uint64_t SIM_PARAMS::max_num_of_events(10);
double SIM_PARAMS::frame_time(1.0);

double SIM_PARAMS::max_intensity(0.5);
double SIM_PARAMS::min_intensity(0.5);

uint64_t SIM_PARAMS::number_of_users(10);

double SIM_PARAMS::max_user_intensity(max_intensity / number_of_users);
double SIM_PARAMS::min_user_intensity(min_intensity / number_of_users);

uint64_t SIM_PARAMS::max_intensity_part(1);
uint64_t SIM_PARAMS::min_intensity_part(1);

double SIM_PARAMS::intensity_ratio(0.5);

inline double SIM_PARAMS::discrete_probabilitys::count_probaility(uint64_t &num_of_events, double &intensity)
{
    double dividend(1.0);
    double divisor(tgamma(num_of_events + 1));
    if (num_of_events > 0)
    {
        dividend = pow((intensity * frame_time), num_of_events);
    }

    dividend *= exp(-intensity * frame_time);
    return dividend / divisor;
}

inline uint64_t SIM_PARAMS::discrete_probabilitys::get_events_by_lot(const double &lot, double *array)
{
    uint64_t iter(0);
    while (array[iter] < lot)
    {
        iter++;
    }
    
    return iter;
}

SIM_PARAMS::discrete_probabilitys::discrete_probabilitys()
{
    if (max_num_of_events == 0)
    {
        std::cerr << "ERROR: discrete_probabilitys constructor: max_num_of_event is 0" << std::endl;
        return;
    }

    this->len = max_num_of_events + 1;

    this->num_of_events = new uint64_t[len];
    this->probabilitys_max = new double[len];
    this->probabilitys_min = new double[len];

    for (uint64_t iter(0); iter < len - 1; ++iter)
    {
        num_of_events[iter] = iter;
        probabilitys_max[iter] = (iter == 0) ? count_probaility(iter, max_user_intensity) : probabilitys_max[iter - 1] + count_probaility(iter, max_user_intensity);
        probabilitys_min[iter] = (iter == 0) ? count_probaility(iter, min_user_intensity) : probabilitys_min[iter - 1] + count_probaility(iter, min_user_intensity);

        //std::cout << "Prob(" << iter << "): MAX = " << probabilitys_max[iter] << "; MIN = " << probabilitys_min[iter] << std::endl;
    }

    num_of_events[max_num_of_events] = max_num_of_events;
    probabilitys_max[max_num_of_events] = 1.0;
    probabilitys_min[max_num_of_events] = 1.0;
    //std::cout << "Prob(" << max_num_of_events << "): MAX = " << probabilitys_max[max_num_of_events] << "; MIN = " << probabilitys_min[max_num_of_events] << std::endl << std::endl;
}

SIM_PARAMS::discrete_probabilitys::~discrete_probabilitys()
{
    delete [] num_of_events;
    delete [] probabilitys_max;
    delete [] probabilitys_min;
}

uint64_t SIM_PARAMS::discrete_probabilitys::get_events(CURRENT_INTENSITY &curr_intens)
{
    double lot = static_cast<double>(rand()) / RAND_MAX;
    uint64_t index = 0;

    switch (curr_intens)
    {
    case CURRENT_INTENSITY::MAX:
        index = get_events_by_lot(lot, probabilitys_max);
        break;

    case CURRENT_INTENSITY::MIN:
        index = get_events_by_lot(lot, probabilitys_min);
        break;
    }

    return num_of_events[index];
}

void SIM_PARAMS::init()
{
    max_user_intensity = max_intensity / number_of_users;
    min_user_intensity = min_intensity / number_of_users;

    intensity_ratio = (1.0 / (static_cast<double>(max_intensity_part + min_intensity_part))) * max_intensity_part;
}
