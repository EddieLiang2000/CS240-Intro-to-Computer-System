# Project MIX: API Documentation
So for my MIX, app.py in the outer folder is the middleware and for each IM I create a folder inside the outer folder and give corresponding name to its function. For example, the Location folder would return the information for a given latitude and longitude like street name, state name, country name and so on as a Json string.

The middleware receives request as latlng string and pass it as an argument to the Location IM through it's API. The API for each IM has a unique environmental variable in their own folder which specifies which port the IM will be running on, and the outer most .env file has all the APIs which consist of the address and port number of every IM. Then by calling specific APIs I would have specific information returned to the middleware and then shows on the MIX web page. And for each IM to function, the app.py for that IM will include API(s) that can support its desired functionality. For example, the Location IM uses Google Reverse Geocoding API to collect information.

The API for fronend and middleware is simply the local host address for now. Whenever the frontend receives a request and then execute the request, it will send back report to the terminal with it's status code. 200 means the request is successfully executed and 500 means internal error on the MIX, 400 could mean it is requesting a non-existent page through IM and 300 is a page redirection

For adding and removing certain IMs, the current strategy is to have user to choose what IM to activate or deactivate through some form like clicking a button or choosing from dropdown.

To run MIX, you need to have python and get into each folder of IM and type python3 -m flask run to start each IM. The middlerware should start after all other IMs have started.
