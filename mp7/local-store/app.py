from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

# @app.route('/')
# def index():
#   from flask import render_template
#   return render_template("index.html")

storage = {}

@app.route('/<key>', methods=["PUT"])
def put(key):
    if (request.method == 'PUT'):
        if (key in storage):
            new = []
            new.append(request.data.decode("utf-8"))
            new.append(len(storage[key])+1)
            storage[key][len(storage[key])] = new
            return "Success",200
        else:
            storage[key] = {}
            new = []
            new.append(request.data.decode("utf-8"))
            new.append(1)
            storage[key][0] = new
            return "Success",200

@app.route('/<key>', methods=["GET"])
def getk(key):
    if (request.method == 'GET'):
        if (key in storage):
            return jsonify(
                value = storage[key][len(storage[key])-1][0],
                version = storage[key][len(storage[key])-1][1],
            ), 200
        else:
            return "Not Found", 404

@app.route('/<key>/<version>', methods=["GET"])
def getkv(key, version):
    if (request.method == "GET"):
        if ((key in storage) and (int(version) <= len(storage[key])) and (int(version) >= 1)):
            return jsonify(
                value = storage[key][int(version) - 1][0], 
                version = int(version),
            ), 200
        else:
            return "Not Found", 404

@app.route('/<key>', methods=["DELETE"])
def delete(key):
    if (key in storage):
        del storage[key]
        return "Success",200
    else:
        return "Invalid Delete",404
