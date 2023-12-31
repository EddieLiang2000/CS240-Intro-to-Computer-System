from flask import Flask, request, jsonify
app = Flask(__name__)

store = {}
store["celebrations"] = []
# PUT /celebration
@app.route('/celebration', methods=["PUT"])
def put_celebration():
  value = request.data.decode("utf-8")  
  store["celebrations"].append(value)
  return "OK", 200


# GET /
@app.route('/', methods=["GET"])
def get():
  return jsonify(store), 200


# GET /count
@app.route('/count', methods=["GET"])
def get_count():
  return jsonify({
      "count" : len(store["celebrations"])
  }), 200