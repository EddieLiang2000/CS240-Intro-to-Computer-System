FROM gcc:latest
FROM python:latest

COPY ./docker/entrypoint.sh /
WORKDIR ./mp6
RUN chmod +x entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
