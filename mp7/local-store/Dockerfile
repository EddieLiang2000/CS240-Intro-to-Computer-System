FROM python:latest
ADD app.py .
WORKDIR /app
COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt requests flask
COPY . .
CMD [ "flask", "run", "--host=0.0.0.0"]




