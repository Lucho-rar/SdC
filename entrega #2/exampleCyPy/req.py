import ctypes
 
# Cargamos la libreria
libsuma = ctypes.CDLL('./libsuma.so')
 
# Definimos los tipos de los argumentos de la función suma
 
libsuma._suma.argtypes = [ctypes.c_float]
 
# Definimos el tipo del retorno de la función suma
libsuma._suma.restype = ctypes.c_int
 
# Creamos nuestra función suma en Python
# hace de Wrapper para llamar a la función de C
def get_suma(num):
	result=  libsuma._suma(num)
	print(result)
    #print(return libsuma._suma(num))

# Llamamos a la función suma
get_suma(5.0)