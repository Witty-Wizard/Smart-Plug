console.log("Welcome MF");
var checkbox = document.getElementById("myCheckbox");

checkbox.addEventListener("change", function () {
  if (this.querySelector("input").checked) {
    fetchData();
  }
});

var img = document.getElementById("bulbImage");

img.style.transition = "src 0.5s ease-in-out, height 0.5s ease-in-out";

checkbox.addEventListener("change", function () {
  if (this.querySelector("input").checked) {
    img.style.height = "100px";
    setTimeout(function () {
      img.src = "bulbOnBaby.png";
    }, 50); // Delay to make the transition more noticeable
  } else {
    img.style.height = "90px";
    setTimeout(function () {
      img.src = "bulbOffBaby.png";
    }, 50); // Delay to make the transition more noticeable
  }
});



function fetchData() {
    // function turnOn{
    //     var valReq=new XMLHttpRequest();
        
    //     valReq.open("GET","/current",true);
    //     valReq.send();
    // }
  // alert("Fetching data");
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
}

