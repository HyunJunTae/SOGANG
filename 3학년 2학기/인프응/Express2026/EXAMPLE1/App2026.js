const express = require('express');
const app = express();
const controller = require('./Controller2026.js');

app.set('view engine', 'ejs');

app.get('/student', controller.show);

/* 8081 포트에서 서버 구동 */
app.listen(8081, () => {
  console.log("localhost:8081 에서 서버가 시작됩니다.");
});