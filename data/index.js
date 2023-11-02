console.log("Welcome MF");

if (!!window.EventSource) {

var source = new EventSource('/events');
source.addEventListener('current', function(e) {
    console.log("current", e.data);
    document.getElementById("current").innerHTML = e.data;
   }, false);

source.addEventListener('voltage', function(e) {
    console.log("voltage", e.data);
    document.getElementById("voltage").innerHTML = e.data;
   }, false);

source.addEventListener('power', function(e) {
    console.log("power", e.data);
    document.getElementById("power").innerHTML = e.data;
   }, false);

source.addEventListener('power_factor', function(e) {
    console.log("power_factor", e.data);
    document.getElementById("power_factor").innerHTML = e.data;
   }, false);

}

