#ifndef _SIMULATION_
#define _SIMULATION_ 

#include "simulation_parameters.h"

class sim
{
private:
    double average_users;
    double average_waiting;
    double output_intensity;

    uint64_t max_intensity_frames;
    uint64_t min_intensity_frames;
    uint64_t frame_counter;

public:
    sim(/*uint64_t number_of_users, double max_intensity, double min_intensity, uint64_t max_num_of_events, double default_user_probability, double frame_time*/);
    ~sim();

    void run(double observ_timem, int seed);
    void adaptive_run(double observ_timem, int seed);
    void print_statistics();

    double get_average_users();
    double get_average_waiting();
    double get_output_intensity();
};

#endif // !_SIMULATION_