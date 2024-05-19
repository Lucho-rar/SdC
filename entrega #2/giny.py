import requests
import json
import ctypes

# Load the shared library
libsuma = ctypes.CDLL('./libsuma.so')

# Configure argument and return types
libsuma.sumar_uno.argtypes = [ctypes.c_float]
libsuma.sumar_uno.restype = ctypes.c_int

def show_menu():
    print("1. Select pais")
    print("2. Exit")

def get_url(country):
    url = "https://api.worldbank.org/v2/en/country/{}/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=32500&page=1".format(country)
    return url

def get_suma(num):
    return libsuma.sumar_uno(num)

def option_1():
    country = raw_input("Please enter a string: ")  # For Python 2.7, use raw_input instead of input
    url = get_url(country)
    response = requests.get(url)
    data = response.json()
    elementos = data[1]
    for elem in elementos:
        element = elem.get('value')
        num = get_suma(element) if element is not None else "None"
        dat = elem.get('date')
        print("Date : {} Indice GINI : {} ---> {}".format(dat, element, num))

while True:
    show_menu()
    option = raw_input("Select an option: ")  # For Python 2.7, use raw_input instead of input

    if option == "1":
        option_1()
    elif option == "2":
        print("Exiting the program...")
        break
    else:
        print("Invalid option")

