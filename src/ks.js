Pebble.addEventListener('ready', function() {
  console.log("Pebble JS is ready!");
  getQuote();
});

var getQuote = function() {
  var request = new XMLHttpRequest();
  request.open('GET',
               'https://a6fH6KKCSCeUqHLFqUlwspOOs8OUgploceRxIHps:javascript-key=P6ZPvkJtvscp5ijZtGZX7m9mFMrxXb7pQP3mjE7T@api.parse.com/1/classes/QuoteObject/ORcj7S6PWd',
               true);
  request.onload = sendQuote;
  request.send();
};

var sendQuote = function() {
  var response = JSON.parse(this.responseText);
  var quote = response.quotes;
  console.log(quote);
  Pebble.sendAppMessage({
    'money': quote
  });
};