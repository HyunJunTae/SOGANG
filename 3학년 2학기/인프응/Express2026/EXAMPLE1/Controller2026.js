const model = require('./Model2026.js');

exports.show = (req, res) => {
  const name = model.getName();
  console.log(name);
  res.render('Student', { name });
};