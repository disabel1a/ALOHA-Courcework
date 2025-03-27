import os
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

directory = "../data/3d_data/default/8by2"

files = os.listdir(directory)

print(files)

for f in files:
    basename, extension = os.path.splitext(f)
    
    if extension != '.txt':
        continue
    
    with open(os.path.join(directory, f), 'r') as acfile:
        size = int(acfile.readline().strip())
        dim = int(np.sqrt(size))
        
        acfile.readline()
        
        data = []
        for line in acfile:
            data.append(float(line.strip()))
        
        matrix = np.array(data).reshape((dim, dim))
        
        print(f"Matrix from file {f}:")
        print(matrix)
       
        # Масштабирование осей X и Y
        x = np.arange(matrix.shape[0]) * 0.01
        y = np.arange(matrix.shape[1]) * 0.01
        x, y = np.meshgrid(x, y)
        
        fig = plt.figure(figsize=(10, 7))
        ax = plt.axes(projection='3d')
        
        # Используем plot_surface вместо contour3D
        surf = ax.plot_surface(x, y, matrix, cmap='viridis', edgecolor='none')
        
        # Добавляем цветовую полосу (color bar)
        cbar = fig.colorbar(surf, ax=ax, shrink=0.5, aspect=10)
        cbar.set_label('Value')
        
        # Преобразуем basename для zlabel
        formatted_basename = basename.replace('_', ' ').title()
        
        # Улучшаем угол обзора
        ax.view_init(30, -110)
        
        # Добавляем подписи осей
        ax.set_xlabel('Max Intensity')
        ax.set_ylabel('Min Intensity')
        ax.set_zlabel(formatted_basename)
        
        plt.show()
