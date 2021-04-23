FROM node:14.15.4-alpine

RUN mkdir /app
WORKDIR /app
COPY . /app/

RUN npm ci

EXPOSE 9000
CMD ["npm", "run", "dev"]
