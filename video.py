import os
import numpy as np
import shutil
import time

image_folder = "./images"

if(os.path.exists(image_folder)):
    shutil.rmtree(image_folder)
os.makedirs(image_folder)

steps = 1

lookat = [0,0,0]

# lookfrom_x = np.linspace(-4,4,steps)
# lookfrom_y = 3
# lookfrom_z = (18-lookfrom_x**2)**0.5 - 1

lookfrom_x = 13
lookfrom_y = 2
lookfrom_z = 3
start_render = time.perf_counter()
for i in range(steps):
    os.system(f'raytracer.exe {image_folder}/image{i}.jpg {lookat[0]} {lookat[1]} {lookat[2]} {lookfrom_x} {lookfrom_y} {lookfrom_z}')
finish_render = time.perf_counter()
total_time = finish_render -start_render
print(f'Render finished.')
print(f'Total time taken: {total_time}')
print(f'Average time per frame: {total_time/steps}')



