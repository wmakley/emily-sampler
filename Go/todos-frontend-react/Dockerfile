FROM node:14.15.4-alpine

RUN mkdir /app
WORKDIR /app
COPY . /app

RUN yarn install

CMD ["yarn", "run", "start"]
