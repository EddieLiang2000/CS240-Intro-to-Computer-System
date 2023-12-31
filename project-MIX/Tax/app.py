from flask import Flask, jsonify, request, json
import flask
import requests
import taxjar

app = Flask(__name__)


@app.route('/put', methods=["GET"])
def put():
  requests.put("http://127.0.0.1:5000/microservice",
  json = {
  'port':"5003",
  'ip':"http://127.0.0.1",
  'name': "Tax",
  'creator': "Zelong Liang",
  'tile': "For tax information",
  'dependencies':[{
    'port':"5001",
    'ip':"http://127.0.0.1",
    'dependencies':[]
    }]
  })
  return "success",200
  
r1 = put()

@app.route('/', methods=["GET"])
def tax():
    req = request.data.decode("utf-8")
    reqjson = json.loads(req)
    zip = reqjson["zip"]
    client = taxjar.Client(api_key='d43da8fe2af7d360368a0f1534a5110a')
    rates = client.rates_for_location(zip)
    rate = {}
    rate['city'] = rates['city']
    rate['zip'] = rates['zip']
    rate['combined_district_rate'] = rates['combined_district_rate']
    rate['state_rate'] = rates['state_rate']
    rate['city_rate'] = rates['city_rate']
    rate['county'] = rates['county']
    rate['state'] = rates['state']
    rate['combined_rate'] = rates['combined_rate']
    rate['county_rate'] = rates['county_rate']
    rate['freight_taxable'] = rates['freight_taxable']
    resp = jsonify(rate)
    resp.cache_control.max_age = 31556926
    #31556926
    resp.headers['Age'] = 0
    return resp