from re import A
from flask import Flask
import uuid
import os
import subprocess

from werkzeug.utils import send_from_directory
app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  from flask import render_template
  return render_template("index.html")

# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  from flask import send_file, request
  # 1. check method == post, get UUID
  # 2. check if os.path exists, if '/' make temp/path
  if (request.method == 'POST'):
    if (os.path.exists('temp/') == False):
      os.makedirs('temp/')
    pngname = 'temp/' + str(uuid.uuid4()) + '.png' 
    gifname = 'temp/' + str(uuid.uuid4()) + '.gif'
    userimage = request.files['png']
    userimage.save(pngname)    
    cmd = "./png-extractGIF {} {}".format(pngname, gifname)
    if(os.system(cmd) != 0):
      return "Invalid File", 500
    else:
      return send_file(gifname)
  
