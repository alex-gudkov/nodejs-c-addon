const myAddon = require('../build/my-addon.node');

console.log(myAddon.getString());

console.log(myAddon.sumNumbers(1, 2));

myAddon.runCallback((value) => {
  console.log(value);
});

console.log(myAddon.getObject());

console.log(myAddon.getArray());
