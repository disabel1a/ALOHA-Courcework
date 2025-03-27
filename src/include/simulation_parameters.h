#ifndef _SYSTEM_PARAMS_
#define _SYSTEM_PARAMS_

#include <stdint.h>
#include <random>

enum class CHANNEL_STATUS 
{
    CONFLICT,
    SUCCESS,
    EMPTY
};

enum class CURRENT_INTENSITY
{
    MAX,
    MIN
};

namespace SIM_PARAMS
{
    // Стандартная вероятность передачи
    extern double default_user_probability;

    // Максимальное число заявок от одного пользователя за кадр
    extern uint64_t max_num_of_events;
    // Время одного кадра
    extern double frame_time;

    // Максимальная интенсивность
    extern double max_intensity;
    // Минимальная интенсивность
    extern double min_intensity;

    // Число пользователей в моделировании
    extern uint64_t number_of_users;

    // Максимальная интенсивность потока пользователя
    extern double max_user_intensity;
    // Минимальная интенсиваность потока пользователя
    extern double min_user_intensity;

    // Количество кадров с интенсивностью MAX (для рассчета соотношения)
    extern uint64_t max_intensity_part;
    // Количество кадров с интенсивностью MIN (для рассчета соотношения)
    extern uint64_t min_intensity_part;
    /*
        Соотношение max_intensity / min_intensity
        **Вероятность выбора max_intensity
    */
    extern double intensity_ratio;

    void init();

    class discrete_probabilitys
    {
    private:
        uint64_t len;
        uint64_t *num_of_events;
        double *probabilitys_max;
        double *probabilitys_min;

        inline double count_probaility(uint64_t &num_of_events, double &intensity);
        inline uint64_t get_events_by_lot(const double &lot, double *array);

    public:
        discrete_probabilitys();
        ~discrete_probabilitys();

        uint64_t get_events(CURRENT_INTENSITY &curr_intens);
    };

} // namespace SIM_PARAMS


#endif // !_SYSTEM_PARAMS_
