FROM node:14.15.4-alpine

RUN mkdir /app
WORKDIR /app

# Install developer tools
RUN npm i -g eslint @vue/cli

COPY package.json /app/package.json
COPY package-lock.json /app/package-lock.json
RUN npm ci

COPY . /app/

EXPOSE 9000
CMD ["npm", "run", "dev"]
