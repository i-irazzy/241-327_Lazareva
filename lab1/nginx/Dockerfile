FROM nginx:latest

RUN rm /etc/nginx/conf.d/default.conf

COPY ./templates/default.conf.template /etc/nginx/templates/default.conf.template

COPY ./server.crt /etc/nginx/certs/server.crt

EXPOSE 80 443