import os
import matplotlib.pyplot as plt
import time
import struct
from matplotlib.widgets import Button

# Ruta del archivo del dispositivo
DEVICE_PATH = "/dev/gpiohandler"

# Inicializar los datos de la gráfica
data = {'time': [], 'voltage': []}
current_pin = 17

# Inicializar la gráfica
plt.ion()
fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.3)
line, = ax.plot(data['time'], data['voltage'], 'b-')  # Línea azul
ax.set_xlabel('Tiempo (s)')
ax.set_ylabel('Voltaje (V)')
ax.grid(True)  # Habilitar cuadrícula en la gráfica

start_time = time.time()

# Función para manejar el cierre de la ventana
def handle_close(event):
    plt.ioff()
    plt.close()

# Función para seleccionar un pin GPIO
def set_gpio_pin(pin):
    try:
        with open(DEVICE_PATH, 'w') as file:
            file.write(str(pin))
    except Exception as e:
        print(f"Error escribiendo al dispositivo: {e}")

# Función para leer datos desde el dispositivo
def read_device():
    try:
        with open(DEVICE_PATH, 'rb') as file:
            raw_value = struct.unpack('i', file.read(4))[0]
            return int(raw_value * 3.3)
    except Exception as e:
        print(f"Error leyendo desde el dispositivo: {e}")
        return None

# Función para cambiar el pin GPIO monitorizado
def toggle_gpio_pin(event):
    global current_pin
    current_pin = 19 if current_pin == 17 else 17
    set_gpio_pin(current_pin)
    reset_data()

# Función para resetear los datos de la gráfica
def reset_data():
    global data, start_time
    data['time'] = []
    data['voltage'] = []
    start_time = time.time()

# Botón para alternar entre pines GPIO
toggle_button_ax = plt.axes([0.7, 0.05, 0.2, 0.075])
toggle_button = Button(toggle_button_ax, 'Cambiar Pin')
toggle_button.on_clicked(toggle_gpio_pin)

# Conectar el evento de cierre de la ventana
fig.canvas.mpl_connect('close_event', handle_close)

# Bucle de monitoreo para actualizar la gráfica
while plt.fignum_exists(fig.number):
    new_value = read_device()
    
    if new_value is not None:
        current_time = time.time()
        elapsed_time = current_time - start_time
        
        data['time'].append(elapsed_time)
        data['voltage'].append(new_value)
        
        line.set_xdata(data['time'])
        line.set_ydata(data['voltage'])
        
        ax.relim()
        ax.autoscale_view()
        
        plt.draw()
        plt.pause(1)
    else:
        time.sleep(1)

plt.show()
