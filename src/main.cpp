#include <iostream>

#include "include/sim.h"
#include "include/file_tools.h"

const std::string directory = "../data/3d_data/default/8by2";

const std::string avg_users_file = directory + "/avg_users.txt";
const std::string avg_waiting_file = directory + "/avg_waiting.txt";
const std::string output_intensity_file = directory + "/avg_output_intensity.txt"; 

const double observ_time = 1000000.0;
const int seed = 10;

const double duration = 0.01;
const uint64_t capacity = static_cast<uint64_t>(1.0 / duration) - 1;
const uint64_t capacity_3d = capacity * capacity;

void one_run_simulation_with_stats()
{
    SIM_PARAMS::number_of_users = 30;
    SIM_PARAMS::default_user_probability = 1.0 / (double)SIM_PARAMS::number_of_users;

    SIM_PARAMS::frame_time = 0.5;

    SIM_PARAMS::max_intensity = 0.8;
    SIM_PARAMS::min_intensity = 0.1;

    SIM_PARAMS::max_num_of_events = 10;

    SIM_PARAMS::max_intensity_part = 1;
    SIM_PARAMS::min_intensity_part = 19;

    sim simulation{};

    std::cout << "ALOHA" << std::endl;
    simulation.run(observ_time, seed);
    simulation.print_statistics();

    std::cout << std::endl;

    SIM_PARAMS::default_user_probability = 1.0;

    std::cout << "Adaptive ALOHA" << std::endl;
    simulation.adaptive_run(observ_time, seed);
    simulation.print_statistics();
}

void data_adaptive_3d()
{
    check_for_directory(directory.c_str());
    check_for_file(avg_users_file.c_str());
    check_for_file(avg_waiting_file.c_str());
    check_for_file(output_intensity_file.c_str());

    SIM_PARAMS::default_user_probability = 1.0;
    SIM_PARAMS::number_of_users = 10;

    SIM_PARAMS::frame_time = 1.0;

    SIM_PARAMS::max_intensity = 0.1;
    SIM_PARAMS::min_intensity = 0.1;

    SIM_PARAMS::max_num_of_events = 10;

    SIM_PARAMS::max_intensity_part = 8;
    SIM_PARAMS::min_intensity_part = 2;

    std::vector<double> avg_users;
    std::vector<double> avg_waitings;
    std::vector<double> output_intensitys;

    avg_users.reserve(capacity_3d);
    avg_waitings.reserve(capacity_3d);
    output_intensitys.reserve(capacity_3d);

    sim simulation{};

    for (double max_intens = duration; max_intens < 1.0; max_intens += duration)
    {
        SIM_PARAMS::max_intensity = max_intens;

        for (double min_intens = duration; min_intens < 1.0; min_intens += duration)
        {
            SIM_PARAMS::min_intensity = min_intens;

            simulation.adaptive_run(observ_time, seed);

            avg_users.push_back(simulation.get_average_users());
            avg_waitings.push_back(simulation.get_average_waiting());
            output_intensitys.push_back(simulation.get_output_intensity());
        }
    }

    write_vector_file(avg_users_file, avg_users);
    write_vector_file(avg_waiting_file, avg_waitings);
    write_vector_file(output_intensity_file, output_intensitys);
}

void data_3d()
{
    check_for_directory(directory.c_str());
    check_for_file(avg_users_file.c_str());
    check_for_file(avg_waiting_file.c_str());
    check_for_file(output_intensity_file.c_str());

    SIM_PARAMS::number_of_users = 10;
    SIM_PARAMS::default_user_probability = 1.0 / (double)SIM_PARAMS::number_of_users;

    SIM_PARAMS::frame_time = 1.0;

    SIM_PARAMS::max_intensity = 0.1;
    SIM_PARAMS::min_intensity = 0.1;

    SIM_PARAMS::max_num_of_events = 10;

    SIM_PARAMS::max_intensity_part = 8;
    SIM_PARAMS::min_intensity_part = 2;

    std::vector<double> avg_users;
    std::vector<double> avg_waitings;
    std::vector<double> output_intensitys;

    avg_users.reserve(capacity_3d);
    avg_waitings.reserve(capacity_3d);
    output_intensitys.reserve(capacity_3d);

    sim simulation{};

    for (double max_intens = duration; max_intens < 1.0; max_intens += duration)
    {
        SIM_PARAMS::max_intensity = max_intens;

        for (double min_intens = duration; min_intens < 1.0; min_intens += duration)
        {
            SIM_PARAMS::min_intensity = min_intens;

            simulation.run(observ_time, seed);

            avg_users.push_back(simulation.get_average_users());
            avg_waitings.push_back(simulation.get_average_waiting());
            output_intensitys.push_back(simulation.get_output_intensity());
        }
    }

    write_vector_file(avg_users_file, avg_users);
    write_vector_file(avg_waiting_file, avg_waitings);
    write_vector_file(output_intensity_file, output_intensitys);
}

void avg_stable_system()
{
    check_for_directory(directory.c_str());
    check_for_file(avg_users_file.c_str());
    check_for_file(avg_waiting_file.c_str());
    check_for_file(output_intensity_file.c_str());

    std::vector<double> avg_users;
    std::vector<double> avg_waitings;
    std::vector<double> output_intensitys;

    avg_users.reserve(capacity);
    avg_waitings.reserve(capacity);
    output_intensitys.reserve(capacity);

    SIM_PARAMS::number_of_users = 20;
    SIM_PARAMS::default_user_probability = 1.0 / (double)SIM_PARAMS::number_of_users;

    SIM_PARAMS::frame_time = 1.0;

    double stable_intens = 0.6;

    SIM_PARAMS::max_intensity = 0.0;
    SIM_PARAMS::min_intensity = 0.0;

    SIM_PARAMS::max_num_of_events = 10;

    SIM_PARAMS::max_intensity_part = 3;
    SIM_PARAMS::min_intensity_part = 10;

    sim simulation{};

    for (double intens = 0.01; intens < 1.0; intens += duration)
    {
        SIM_PARAMS::max_intensity = (3.0 / 13.0) * stable_intens + (10.0 / 13.0) * intens;
        SIM_PARAMS::min_intensity = (3.0 / 13.0) * stable_intens + (10.0 / 13.0) * intens;

        //simulation.run(observ_time, seed);
        simulation.adaptive_run(observ_time, seed);

        avg_users.push_back(simulation.get_average_users());
        avg_waitings.push_back(simulation.get_average_waiting());
        output_intensitys.push_back(simulation.get_output_intensity());
    }

    write_vector_file(avg_users_file, avg_users);
    write_vector_file(avg_waiting_file, avg_waitings);
    write_vector_file(output_intensity_file, output_intensitys);

    return;
}

int main(/*int argc, char const *argv[]*/)
{

    one_run_simulation_with_stats();

    //data_3d();
    //data_adaptive_3d();

    //avg_stable_system();

    // check_for_directory(directory.c_str());
    // check_for_file(avg_users_file.c_str());
    // check_for_file(avg_waiting_file.c_str());
    // check_for_file(output_intensity_file.c_str());

    // std::vector<double> avg_users;
    // std::vector<double> avg_waitings;
    // std::vector<double> output_intensitys;

    // avg_users.reserve(capacity);
    // avg_waitings.reserve(capacity);
    // output_intensitys.reserve(capacity);

    // SIM_PARAMS::number_of_users = 20;
    // SIM_PARAMS::default_user_probability = 1.0 / (double)SIM_PARAMS::number_of_users;

    // SIM_PARAMS::frame_time = 1.0;

    // double stable_intens = 0.6;

    // SIM_PARAMS::max_intensity = 0.6;
    // SIM_PARAMS::min_intensity = 0.1;

    // SIM_PARAMS::max_num_of_events = 10;

    // SIM_PARAMS::max_intensity_part = 3;
    // SIM_PARAMS::min_intensity_part = 10;

    // sim simulation{};

    // for (double intens = 0.01; intens < 1.0; intens += duration)
    // {
    //     //SIM_PARAMS::max_intensity = (3.0 / 13.0) * stable_intens + (10.0 / 13.0) * intens;
    //     //SIM_PARAMS::min_intensity = (3.0 / 13.0) * stable_intens + (10.0 / 13.0) * intens;

    //     //SIM_PARAMS::max_intensity = intens;
    //     SIM_PARAMS::min_intensity = intens;

    //     //simulation.run(observ_time, seed);
    //     simulation.adaptive_run(observ_time, seed);

    //     avg_users.push_back(simulation.get_average_users());
    //     avg_waitings.push_back(simulation.get_average_waiting());
    //     output_intensitys.push_back(simulation.get_output_intensity());
    // }

    // write_vector_file(avg_users_file, avg_users);
    // write_vector_file(avg_waiting_file, avg_waitings);
    // write_vector_file(output_intensity_file, output_intensitys);

    return 0;
}
