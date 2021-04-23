FROM node:14.15.4-alpine

RUN mkdir /app
WORKDIR /app

COPY package.json /app/package.json
COPY package-lock.json /app/package-lock.json
RUN npm ci

COPY . /app/

EXPOSE 9000
CMD ["npm", "run", "dev"]
