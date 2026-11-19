const express = require('express');
const app = express();

app.get('/student', (req, res) => {
  // 1. 데이터 준비
  const name = 'Alice';

  // 2. HTML 생성 및 응답
  res.send(`
    <!doctype html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <title>Student</title>
    </head>
    <body>
      <h1>Hello, ${name}!</h1>
    </body>
    </html>
  `);
});

// 3. 서버 시작
app.listen(3000, () => {
  console.log('http://localhost:3000/student');
});