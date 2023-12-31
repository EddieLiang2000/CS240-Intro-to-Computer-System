from flask import Flask, render_template, request, jsonify
from flask.sessions import NullSession
import requests
app = Flask(__name__)

from pymongo import MongoClient
mongo = MongoClient('localhost', 27017)
db = mongo["mp7-state-server"]
collection = db["mp7-state-server"]

@app.route('/<key>', methods=["PUT"])
def put(key):
    if (request.method == 'PUT'):
        if(collection[key].count() != 0):
            data_and_version = {
                "value": request.data.decode("utf-8"),
                "version": collection[key].count() + 1
            }
            collection[key].insert_one(data_and_version)
            return "Success", 200
        else:
            collection[key] = {}
            data_and_version = {
                "value": request.data.decode("utf-8"),
                "version": 1
            }
            collection[key].insert_one(data_and_version)
            return "Success", 200

@app.route('/<key>', methods=["GET"])
def getk(key):
    if (request.method == 'GET'):
        if (collection[key].count() != 0):
            return collection[key].find_one({"version": collection[key].count()}), 200
        else:
            return "Not Found", 404

@app.route('/<key>/<version>', methods=["GET"])
def getkv(key, version):
    if (request.method == "GET"):
        if ((collection[key].count() != 0) and (int(version) <= collection[key].count()) and (int(version) >= 1)):
            return collection[key].find_one({"version": int(version)}), 200
        else:
            return "Not Found", 404

@app.route('/<key>', methods=["DELETE"])
def delete(key):
    if (collection[key].count() != 0):  
        collection.delete_one(key)
        return "Success", 200
    else:
        return "Invalid Delete", 404
