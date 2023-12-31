from flask import Flask, jsonify, request, json
import requests
from werkzeug.wrappers import response
app = Flask(__name__)

@app.route('/put', methods=["GET"])
def put():
  requests.put("http://127.0.0.1:5000/microservice",
  json = {
  'port':"5002",
  'ip':"http://127.0.0.1",
  'name': "Location",
  'creator': "Zelong Liang",
  'tile': "In-Store discounts within 10 miles of given location",
  'dependencies':[]
  })
  return "success",200

r1 = put()

@app.route('/', methods=["GET"])
def discounts():
  response = {}
  response['discounts'] = {}
  req = request.data.decode("utf-8")
  reqjson = json.loads(req)
  lat = reqjson['latitude']
  lon = reqjson['longitude']
  discounts = requests.get("https://api.discountapi.com/v2/deals?location={},{}&api_key=kYmvOTOl".format(lat, lon)) 
  discountsjson = discounts.json()
  deals = discountsjson['deals']
  for k in range(len(deals)):
    deal = deals[k]["deal"]
    response['discounts'][str(k+1) + ". " + deal['title']] = "discount percentage" + ": " + str(deal['discount_percentage']) + ", " + deal["fine_print"]
  resp = jsonify(response)
  resp.cache_control.max_age = 86400
  #86400
  resp.headers['Age'] = 0
  return resp