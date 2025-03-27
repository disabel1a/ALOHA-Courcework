import numpy as np
import matplotlib.pyplot as plt

# Функция для чтения данных из файла
def read_file_to_array(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()
        return [float(line.strip()) for line in lines[2:] if line.strip()]

# Директория
directory_path = "../data/diff_intens"

# Пути к файлам
avg_users_low = directory_path + "/low_stable/avg_users.txt"
avg_waiting_low = directory_path + "/low_stable/avg_waiting.txt"
output_intens_low = directory_path + "/low_stable/avg_output_intensity.txt"

avg_users_max = directory_path + "/max_stable/avg_users.txt"
avg_waiting_max = directory_path + "/max_stable/avg_waiting.txt"
output_intens_max = directory_path + "/max_stable/avg_output_intensity.txt"

avg_users_same = directory_path + "/same/avg_users.txt"
avg_waiting_same = directory_path + "/same/avg_waiting.txt"
output_intens_same = directory_path + "/same/avg_output_intensity.txt"

avg_users_avgmax = directory_path + "/avg_max/avg_users.txt"
avg_waiting_avgmax = directory_path + "/avg_max/avg_waiting.txt"
output_intens_avgmax = directory_path + "/avg_max/avg_output_intensity.txt"

avg_users_avglow = directory_path + "/avg_low/avg_users.txt"
avg_waiting_avglow = directory_path + "/avg_low/avg_waiting.txt"
output_intens_avglow = directory_path + "/avg_low/avg_output_intensity.txt"

# Чтение данных
data_avg_users_low = read_file_to_array(avg_users_low)
data_avg_waiting_low = read_file_to_array(avg_waiting_low)
data_output_intens_low = read_file_to_array(output_intens_low)

data_avg_users_max = read_file_to_array(avg_users_max)
data_avg_waiting_max = read_file_to_array(avg_waiting_max)
data_output_intens_max = read_file_to_array(output_intens_max)

data_avg_users_same = read_file_to_array(avg_users_same)
data_avg_waiting_same = read_file_to_array(avg_waiting_same)
data_output_intens_same = read_file_to_array(output_intens_same)

data_avg_users_avgmax = read_file_to_array(avg_users_avgmax)
data_avg_waiting_avgmax = read_file_to_array(avg_waiting_avgmax)
data_output_intens_avgmax = read_file_to_array(output_intens_avgmax)

data_avg_users_avglow = read_file_to_array(avg_users_avglow)
data_avg_waiting_avglow = read_file_to_array(avg_waiting_avglow)
data_output_intens_avglow = read_file_to_array(output_intens_avglow)

# Генерация значений lambdas
lambdas = np.arange(0.01, 1.0, 0.01)

x_limit = None

# Функция для построения графика
def plot_graph(x, ys, labels, title, xlabel="λ", ylabel="Значение", x_limit=None):
    """
    Строит график с возможностью ограничения по оси X.
    
    :param x: Данные для оси X.
    :param ys: Список массивов для оси Y.
    :param labels: Список подписей для каждого массива.
    :param title: Заголовок графика.
    :param xlabel: Подпись оси X.
    :param ylabel: Подпись оси Y.
    :param x_limit: Максимальное значение для оси X.
    """
    plt.figure(figsize=(10, 6))
    
    # Ограничиваем данные, если задан x_limit
    if x_limit is not None:
        indices = [i for i, val in enumerate(x) if val <= x_limit]
        x = [x[i] for i in indices]
        ys = [[y[i] for i in indices] for y in ys]

    for y, label in zip(ys, labels):
        plt.plot(x, y, label=label, linewidth=1.5)
    
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.legend()
    plt.grid(True)
    plt.show()

plot_graph(lambdas,
           [data_avg_users_low, data_avg_users_max, data_avg_users_same,
            data_avg_users_avglow, data_avg_users_avgmax],
           ['Система со статичной низкой интенсивностью',
            'Система со статичной высокой интенсивностью',
            'Система без переменной интенсивности',
            'Система сред. со стат. низкой инстенсивнностью',
            'Система сред. со стат. высокой интенсивностью'],
           ['Среднее количество пользователей в системе (адаптивный вероятностный алгоритм)'],
           x_limit=x_limit
)

plot_graph(lambdas,
           [data_avg_waiting_low, data_avg_waiting_max, data_avg_waiting_same,
            data_avg_waiting_avglow, data_avg_waiting_avgmax],
           ['Система со статичной низкой интенсивностью',
            'Система со статичной высокой интенсивностью',
            'Система без переменной интенсивности',
            'Система сред. со стат. низкой инстенсивнностью',
            'Система сред. со стат. высокой интенсивностью'],
           ['Среднее время ожидания в системе (адаптивный вероятностный алгоритм)'],
           x_limit=x_limit
)

plot_graph(lambdas,
           [data_output_intens_low, data_output_intens_max, data_output_intens_same,
            data_output_intens_avglow, data_output_intens_avgmax],
           ['Система со статичной низкой интенсивностью',
            'Система со статичной высокой интенсивностью',
            'Система без переменной интенсивности',
            'Система сред. со стат. низкой инстенсивнностью',
            'Система сред. со стат. высокой интенсивностью'],
           ['Выходная интенсивность (адаптивный вероятностный алгоритм)'],
           x_limit=x_limit
)    
