from flask import Flask, render_template, request, jsonify
import os
from numpy import add, e, minimum
import requests
from datetime import datetime, timedelta, time
import re
app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


# Route for "/weather" (middleware):
@app.route('/weather', methods=["POST"])
def POST_weather():
  course = request.form["course"]
  subject = []
  number = []
  for letter in course:
    if (letter.isdigit()):
      number.append(letter)
    elif (letter.isalpha()):
      subject.append(letter)
    else:
      continue
  subjects = "".join(subject)
  numbers = "".join(number)
  server_url = os.getenv('COURSES_MICROSERVICE_URL')
  r1 = requests.get(f'{server_url}/{subjects}/{numbers}/')
  if (r1.status_code == 404):
    return "Invalid Course Number", 404
  course_data = r1.json()
  if ("error" in course_data):
    return "Invalid Course Number", 404
  else:
    course = course_data["course"]
    days = course_data["Days of Week"]
    time = course_data["Start Time"]
    actualtime = datetime.strptime(time, '%I:%M %p').strftime("%H:%M")
    actualtime2 = datetime.strptime(actualtime, "%H:%M")
    now = datetime.now()
    rightnow = now.strftime("%H:%M")
    # print(rightnow)
    today = datetime.today().weekday()
    convert = {"M":0, "T":1, "W":2, "R":3, "F":4}
    convert2 = {0:"Monday", 1:"Tuesday", 2:"Wednesday", 3:"Thursday", 4:"Friday"}
    days_list = []
    hours = 0
    for letter in days:
      days_list.append(convert[letter])
    
    #compare today with class schedule
    if (today > days_list[-1]) or ((today == days_list[-1]) and (str(now.hour) > str(actualtime))):
      #already passed class days
      count = days_list[0] + 7 - today
      print(count)
      meetingday = convert2[days_list[0]]
      meetingdate = datetime.now() + timedelta(days = count)
      print(meetingdate)
      myear = str(meetingdate.year)
      mmonth = str(meetingdate.month)
      if (len(mmonth) == 1):
        mday = '0' + str(meetingdate.month)
      mday = str(meetingdate.day)
      if (len(mday) == 1):
        mday = '0' + str(meetingdate.day)
      mhour = str(actualtime2.hour)
      if (len(mhour) == 1):
        mhour = '0' + str(actualtime2.hour)
      print(myear)
      print(mmonth)
      print(mday)
      print(mhour)
    else:
      for i in range(len(days_list)):
        if (today == days_list[i]):
          if (rightnow <= str(actualtime)):
            meetingday = convert2[days_list[0]]
            meetingdate = datetime.now()
            print(meetingdate)
            myear = str(meetingdate.year)
            mmonth = str(meetingdate.month)
            if (len(mmonth) == 1):
              mday = '0' + str(meetingdate.month)
            mday = str(meetingdate.day)
            if (len(mday) == 1):
              mday = '0' + str(meetingdate.day)
            mhour = str(actualtime2.hour)
            if (len(mhour) == 1):
              mhour = '0' + str(actualtime2.hour)
            print(myear)
            print(mmonth)
            print(mday)
            print(mhour)
            break
        elif (today < days_list[i]):
          count = days_list[i] - today
          print(count)
          meetingday = convert2[days_list[0]]
          meetingdate = datetime.now() + timedelta(days = count)
          print(meetingdate)
          myear = str(meetingdate.year)
          mmonth = str(meetingdate.month)
          if (len(mmonth) == 1):
            mday = '0' + str(meetingdate.month)
          mday = str(meetingdate.day)
          if (len(mday) == 1):
            mday = '0' + str(meetingdate.day)
          mhour = str(actualtime2.hour)
          if (len(mhour) == 1):
            mhour = '0' + str(actualtime2.hour)
          print(myear)
          print(mmonth)
          print(mday)
          print(mhour)
    r2 = requests.get("https://api.weather.gov/points/40.1125,-88.2284")
    apiinfo = r2.json()
    forecastapi = requests.get(apiinfo["properties"]["forecastHourly"])
    forecast = forecastapi.json()
    periods = forecast["properties"]["periods"]
    for k in range(len(periods)):
      check = periods[k]["startTime"]
      year = check[0:4]
      month = check[5:7]
      day = check[8:10]
      hour = check[11:13]
      if (k == 155):
        temp = periods[155]["startTime"]
        year1 = temp[0:4]
        month1 = temp[5:7]
        day1 = temp[8:10]
        hour1 = temp[11:13]
        temperature = periods[155]["temperature"]
        shortForecast = periods[155]["shortForecast"]
        print(periods[k]["number"])
        print(periods[k]["startTime"])
        ft = year1 + "-" + month1 + "-" + day1 + " " + hour1 + ":00:00"
      if (year == myear and month == mmonth and day == mday and hour == mhour):
        temperature = periods[k]["temperature"]
        shortForecast = periods[k]["shortForecast"]
        print(temperature)
        print(shortForecast)
        print(periods[k]["windSpeed"])
        
        ft = year + "-" + month + "-" + day + " " + hour + ":00:00"
        break
    
  return jsonify({
    "course": course,
    "nextCourseMeeting": myear + "-" + mmonth + "-" + mday + " " + time,
    "forecastTime": ft,
    "temperature": str(temperature),
    "shortForecast": shortForecast
  }), 200
  # return jsonify({"error": "Not implemented."}), 500


