import requests

# create a session object
session = requests.Session()

# perform a GET request
response = session.get('/')

print(response.text)


print('------')

# perform a POST request with data
data = {'key': 'value'}
response = session.post('/', data=data)

print(response.text)