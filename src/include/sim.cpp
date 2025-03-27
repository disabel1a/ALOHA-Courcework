#include "sim.h"

#include <vector>
#include <iostream>

#include "user.h"

sim::sim(/*uint64_t number_of_users, double max_intensity, double min_intensity, uint64_t max_num_of_events, double default_user_probability, double frame_time*/) :
    average_users   (0.0),
    average_waiting (0.0),
    output_intensity(0.0),
    max_intensity_frames(0),
    min_intensity_frames(0),
    frame_counter(0)
{
    // SIM_PARAMS::default_user_probability = default_user_probability;

    // SIM_PARAMS::max_num_of_events = max_num_of_events;
    // SIM_PARAMS::number_of_users = number_of_users;

    // SIM_PARAMS::max_intensity = max_intensity;
    // SIM_PARAMS::min_intensity = min_intensity;

    // SIM_PARAMS::frame_time = frame_time;

    // SIM_PARAMS::init();
}

sim::~sim()
{
}

void sim::run(double observ_time, int seed)
{
    // ====================================================================================
    // Проверка корректности параметров симуляции
    if (SIM_PARAMS::default_user_probability == 0.0)
    {
        std::cerr << "ERROR: sim() default_user_probability is 0.0" << std::endl;
        return;
    }

    if (SIM_PARAMS::frame_time == 0.0)
    {
        std::cerr << "ERROR: frame_time() frame_time is 0.0" << std::endl;
        return;
    }

    if (SIM_PARAMS::number_of_users == 0)
    {
        std::cerr << "ERROR: number_of_users() number_of_users is 0.0" << std::endl;
        return;
    }

    if (observ_time < SIM_PARAMS::frame_time * 2)
    {
        std::cerr << "ERROR: frame_time() frame_time is less than 2 frames" << std::endl;
        return;
    }

    if (SIM_PARAMS::max_intensity_part == 0 || SIM_PARAMS::min_intensity_part == 0)
    {
        std::cerr << "ERROR: on of intensity_part() is 0" << std::endl;
        return;
    }

    SIM_PARAMS::init();
    srand(seed);

    // ====================================================================================
    // Создание массива абонентов
    std::vector<user> users;
    users.reserve(SIM_PARAMS::number_of_users);

    for (uint64_t iter(0); iter < SIM_PARAMS::number_of_users; ++iter)
    {
        users.push_back(user(SIM_PARAMS::default_user_probability));
    }

    // ====================================================================================
    // Вспомогательные переменные

    SIM_PARAMS::discrete_probabilitys probs{};

    this->average_users = 0.0;
    this->average_waiting = 0.0;
    this->output_intensity = 0.0;

    this->max_intensity_frames = 0;
    this->min_intensity_frames = 0;
    this->frame_counter = 0;

    CHANNEL_STATUS ss = CHANNEL_STATUS::EMPTY;
    CURRENT_INTENSITY curr_intens = CURRENT_INTENSITY::MAX;

    user *user_in_channel = nullptr;

    double current_frame_time(0.0);

    uint64_t served_events(0);

    // ====================================================================================
    // Моделирование

    while (current_frame_time < observ_time)
    {
        // Просмотр текущего окна на наличие позлователей для передачи
        uint64_t users_in_frame(0);

        user_in_channel = nullptr;
        ss = CHANNEL_STATUS::EMPTY;

        for (user &usr : users)
        {
            if (!usr.queue.empty())
            {
                if (usr.try_to_send())
                {
                    if (user_in_channel == nullptr)
                    {
                        user_in_channel = &usr;
                    }
                    else
                    {
                        ss = CHANNEL_STATUS::CONFLICT;
                    }
                }

                users_in_frame++;
            }
        }

        average_users += users_in_frame;

        // Если в канале 1 передающий абонент
        if (ss == CHANNEL_STATUS::EMPTY && user_in_channel != nullptr)
        {
            ss = CHANNEL_STATUS::SUCCESS;
        }

        // Обслуживание абонента
        if (ss == CHANNEL_STATUS::SUCCESS)
        {
            event ev(user_in_channel->queue.front());
            user_in_channel->queue.pop();

            average_waiting += (current_frame_time + SIM_PARAMS::frame_time) - ev.arrival_time;
            served_events += 1;
        }

        // Генерация заявок

        // Выбор новой интенсивности
        double intensity_lot = static_cast<double>(rand()) / RAND_MAX;
        if (intensity_lot <= SIM_PARAMS::intensity_ratio)
        {
            curr_intens = CURRENT_INTENSITY::MAX;
            max_intensity_frames++;
        }
        else
        {
            curr_intens = CURRENT_INTENSITY::MIN;
            min_intensity_frames++;
        }

        double arrival_time = current_frame_time + SIM_PARAMS::frame_time / 2;

        for (user &usr : users)
        {
            for(uint64_t events = probs.get_events(curr_intens); events > 0; events--)
            {
                usr.queue.push(event(arrival_time));
            }
        }

        current_frame_time += SIM_PARAMS::frame_time;
        frame_counter++;
    }

    average_users /= frame_counter;
    if (served_events == 0)
    {
        // Установка максимального значения (время обзора)
        average_waiting = observ_time;
        // Установка минимльного значения (из системы ничего не вышло)
        output_intensity = 0;
    }
    else
    {
        // При домножении на frame_time превращает в коэффициент от frane_time
        average_waiting /= static_cast<double>(served_events) * SIM_PARAMS::frame_time;
        output_intensity = static_cast<double>(served_events) / frame_counter;
    }
}

void sim::adaptive_run(double observ_time, int seed)
{
    // ====================================================================================
    // Проверка корректности параметров симуляции
    if (SIM_PARAMS::default_user_probability == 0.0)
    {
        std::cerr << "ERROR: sim() default_user_probability is 0.0" << std::endl;
        return;
    }

    if (SIM_PARAMS::frame_time == 0.0)
    {
        std::cerr << "ERROR: frame_time() frame_time is 0.0" << std::endl;
        return;
    }

    if (SIM_PARAMS::number_of_users == 0)
    {
        std::cerr << "ERROR: number_of_users() number_of_users is 0.0" << std::endl;
        return;
    }

    if (observ_time < SIM_PARAMS::frame_time * 2)
    {
        std::cerr << "ERROR: frame_time() frame_time is less than 2 frames" << std::endl;
        return;
    }

    if (SIM_PARAMS::max_intensity_part == 0 || SIM_PARAMS::min_intensity_part == 0)
    {
        std::cerr << "ERROR: on of intensity_part() is 0" << std::endl;
        return;
    }

    SIM_PARAMS::init();
    srand(seed);

    // ====================================================================================
    // Создание массива абонентов
    std::vector<user> users;
    users.reserve(SIM_PARAMS::number_of_users);

    for (uint64_t iter(0); iter < SIM_PARAMS::number_of_users; ++iter)
    {
        users.push_back(user(SIM_PARAMS::default_user_probability));
    }

    // ====================================================================================
    // Вспомогательные переменные

    SIM_PARAMS::discrete_probabilitys probs{};

    this->average_users = 0.0;
    this->average_waiting = 0.0;
    this->output_intensity = 0.0;

    this->max_intensity_frames = 0;
    this->min_intensity_frames = 0;
    this->frame_counter = 0;

    CHANNEL_STATUS ss = CHANNEL_STATUS::EMPTY;
    CURRENT_INTENSITY curr_intens = CURRENT_INTENSITY::MAX;

    user *user_in_channel = nullptr;

    double current_frame_time(0.0);

    uint64_t served_events(0);

    // ====================================================================================
    // Моделирование

    while (current_frame_time < observ_time)
    {
        // Просмотр текущего окна на наличие позлователей для передачи
        uint64_t users_in_frame(0);

        user_in_channel = nullptr;
        ss = CHANNEL_STATUS::EMPTY;

        for (user &usr : users)
        {
            if (!usr.queue.empty())
            {
                if (usr.try_to_send())
                {
                    if (user_in_channel == nullptr)
                    {
                        user_in_channel = &usr;
                    }
                    else
                    {
                        ss = CHANNEL_STATUS::CONFLICT;
                    }
                }

                users_in_frame++;
            }
        }

        average_users += users_in_frame;

        // Если в канале 1 передающий абонент
        if (ss == CHANNEL_STATUS::EMPTY && user_in_channel != nullptr)
        {
            ss = CHANNEL_STATUS::SUCCESS;
        }

        // Обслуживание абонента
        if (ss == CHANNEL_STATUS::SUCCESS)
        {
            event ev(user_in_channel->queue.front());
            user_in_channel->queue.pop();

            average_waiting += (current_frame_time + SIM_PARAMS::frame_time) - ev.arrival_time;
            served_events += 1;
        }

        // Генерация заявок

        // Выбор новой интенсивности
        double intensity_lot = static_cast<double>(rand()) / RAND_MAX;
        if (intensity_lot <= SIM_PARAMS::intensity_ratio)
        {
            curr_intens = CURRENT_INTENSITY::MAX;
            max_intensity_frames++;
        }
        else
        {
            curr_intens = CURRENT_INTENSITY::MIN;
            min_intensity_frames++;
        }

        double arrival_time = current_frame_time + SIM_PARAMS::frame_time / 2;

        // Адаптация вероятности и добавление новых заявок
        for (user &usr : users)
        {
            if (!usr.queue.empty())
            {
                usr.adapt_probability(ss, users_in_frame);
            }
            else
            {
                usr.set_probability(SIM_PARAMS::default_user_probability);
            }

            for(uint64_t events = probs.get_events(curr_intens); events > 0; events--)
            {
                usr.queue.push(event(arrival_time));
            }
        }

        current_frame_time += SIM_PARAMS::frame_time;
        frame_counter++;
    }

    average_users /= frame_counter;
    if (served_events == 0)
    {
        // Установка максимального значения (время обзора)
        average_waiting = observ_time;
        // Установка минимльного значения (из системы ничего не вышло)
        output_intensity = 0;
    }
    else
    {
        // При домножении на frame_time превращает в коэффициент от frane_time
        average_waiting /= static_cast<double>(served_events) * SIM_PARAMS::frame_time;
        output_intensity = static_cast<double>(served_events) / frame_counter;
    }
}

void sim::print_statistics()
{
    std::cout << "SIMULATION STATISTICS:" << std::endl;
    std::cout << "AVG USERS: " << average_users << std::endl;
    std::cout << "AVG WAITING TIME: " << average_waiting << std::endl;
    std::cout << "OUTPUT INTENSITY: " << output_intensity << std::endl << std::endl;

    std::cout << "FRAMES PASSED: " << frame_counter << std::endl;
    std::cout << "FRAMES WITH MAX INTENSITY(" << SIM_PARAMS::max_intensity << "): " << max_intensity_frames << std::endl;
    std::cout << "FRAMES WITH MIN INTENSITY(" << SIM_PARAMS::min_intensity << "): " << min_intensity_frames << std::endl;

    std::cout << "INTENSITY RATIO: " << SIM_PARAMS::max_intensity_part << "/" << SIM_PARAMS::min_intensity_part << " = " 
                                     << (double) SIM_PARAMS::max_intensity_part / (double) SIM_PARAMS::min_intensity_part << std::endl;
    std::cout << "REAL INTENSITY RATIO: " << (double) max_intensity_frames / (double) min_intensity_frames << std::endl;
}

double sim::get_average_users()
{
    return this->average_users;
}

double sim::get_average_waiting()
{
    return this->average_waiting;
}

double sim::get_output_intensity()
{
    return this->output_intensity;
}
