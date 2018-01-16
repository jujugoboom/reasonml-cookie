const path = require('path');

module.exports = {
  entry: './src/main.bs.js',
  output: {
    path: path.join(__dirname, "build"),
    filename: 'index.js',
  },
};
