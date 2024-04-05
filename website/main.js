console.log("Welcome MF");
var checkbox = document.getElementById("myCheckbox");

// checkbox.addEventListener("change", function () {
//   if (this.querySelector("input").checked) {
//     fetchData();
//   }
// });
// checkbox.addEventListener("change", function () {
//   if (this.querySelector("input").checked) {
//     turnOn();
//     fetchData();
//   }
//   else{
//     turnOff();
//     ignoreData();
//   }
// });

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

const btn = document.getElementById("gyan_bato");

btn.addEventListener("click", function handleClick() {
  const initialText = "Show Detailed Analysis";

  if (btn.textContent.toLowerCase().includes(initialText.toLowerCase())) {
    btn.textContent = "Hide Detailed Analysis";
    displayText();
  } else {
    btn.textContent = initialText;
    hideText();
  }
});

function displayText() {
  var text = document.getElementById("analysis");
  text.style.display = "block";
}

function hideText() {
  var text = document.getElementById("analysis");
  text.style.display = "none";
}



function turnOn(){
    var valReq=new XMLHttpRequest();
    
    valReq.open("GET","/on",true);
    valReq.send();

    console.log(this.responseText);
}

function turnOff(){
  var valReq=new XMLHttpRequest();
  
  valReq.open("GET","/off",true);
  valReq.send();

  console.log(this.responseText);
}
function ignoreData(){
  document.getElementById("current").innerHTML = "0";
  document.getElementById("voltage").innerHTML = "0";
  document.getElementById("power").innerHTML = "0";
  document.getElementById("power_factor").innerHTML = "0";
}

function fetchData() {
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

