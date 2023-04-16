import requests

# create a session object
session = requests.Session()

# perform a GET request
response = session.get('https://example.com')

# perform a POST request with data
data = {'key': 'value'}
response = session.post('https://example.com/login', data=data)

print(response.text)