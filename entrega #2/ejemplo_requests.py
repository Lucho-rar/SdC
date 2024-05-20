import requests

url = "https://api.worldbank.org/v2/en/country/ARG/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=32500&page=1"
response = requests.get(url)


data = response.json()
elementos = data[1]

print(elementos)
