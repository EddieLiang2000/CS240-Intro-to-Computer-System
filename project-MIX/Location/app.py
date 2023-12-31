from flask import Flask, request, jsonify, json
import requests
app = Flask(__name__)

@app.route('/put', methods=["GET"])
def put():
  requests.put("http://127.0.0.1:5000/microservice",
  json = {
  'port':"5001",
  'ip':"http://127.0.0.1",
  'name': "Location",
  'creator': "Zelong Liang",
  'tile': "For formated address",
  'dependencies':[]
  })
  return "success", 200

r1 = put()

@app.route('/', methods=["GET"])
def location():

  req = request.data.decode("utf-8")
  reqjson = json.loads(req)
  lat = reqjson['latitude']
  lon = reqjson['longitude']
  
  location = requests.get("https://maps.googleapis.com/maps/api/geocode/json?latlng={},{}&key=AIzaSyCAoaO1KzPpfoBkpCrCu1YxCmV0Vsqkq1g".format(str(lat),str(lon)))
  locationjson = location.json()
  results = locationjson["results"]
  result = results[0]
  address_components = result["address_components"]
  for i in range(len(address_components)):
    if ("postal_code" in address_components[i]["types"]):
      zip = address_components[i]["long_name"]
  formatted_address = result["formatted_address"]
  resp = jsonify({"formated address": formatted_address,
                  "zip": zip
  })
  resp.cache_control.max_age = 3155692600
  #3155692600
  resp.headers['Age'] = 0
  return resp
  


