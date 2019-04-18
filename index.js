//thanks to https://www.sohamkamani.com/blog/javascript/2018-06-24-oauth-with-node-js/ for the oauth wireframe
let FormData = require('form-data');
const express = require('express');
// const axios = require('axios');
const request = require('postman-request');
require('dotenv').config()

const app = express();

const clientID = process.env.KEY
const consumerSecret = process.env.SECRET
app.use(express.static(__dirname));

app.get('/account', (req,res) => {
  console.log('query: ', req.query);
  console.log(req.query.access_token);
  let token = req.query.access_token;
  request.get(`https://wbsapi.withings.net/measure?action=getmeas&access_token=${token}`/*`&startdate=[INT]&enddate=[INT]&offset=[INT]`*/, function(err, res, body) {
    console.log('body: ', body);
  })
})

app.get('/oauth/redirect', (req, res) => {
  const requestToken = req.query.code;
  let postData = {
      grant_type: 'authorization_code',
      client_id:clientID,
      client_secret: consumerSecret,
      code: requestToken,
    redirect_uri:`https://5f0b3afb.ngrok.io/oauth/redirect`
    }
  let formData = new FormData();
  for (var key in postData) {
    formData.append(key, postData[key]);
  }
  console.log(JSON.stringify(postData));
  request.post({url: 'https://account.withings.com/oauth2/token', formData: postData}, (err, response, body) => {
    console.log('body: ', body);
    const accessToken = JSON.parse(body).access_token;
    console.log('access token: ', accessToken);
    res.redirect(`/account?access_token=${accessToken}`)
  })
  //  axios({
  //    method: 'post',
  //    url: `https://account.withings.com/oauth2/token`,
  //    data: JSON.stringify(postData),
  //    config: {
  //      'Content-Type': 'jsonp'
  //    }
  //  }).then((response) => {
  //  })
})

//start server

app.listen(8080);
