console.log("Started");
setInterval(function(){
    getCurrent();
    getVoltage()
    getPower();
    getPowerFactor();

},100);

function getCurrent(){
    var valReq=new XMLHttpRequest();
    valReq.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("current").innerHTML=this.responseText;
            console.log(this.responseText);
        }
    }
    valReq.open("GET","/current",true);
    valReq.send();
}

function getVoltage(){
    var valReq=new XMLHttpRequest();
    valReq.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("voltage").innerHTML=this.responseText;
            console.log(this.responseText);
        }
    }
    valReq.open("GET","/voltage",true);
    valReq.send();
}

function getPower(){
    var valReq=new XMLHttpRequest();
    valReq.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("power").innerHTML=this.responseText;
            console.log(this.responseText);
        }
    }
    valReq.open("GET","/power",true);
    valReq.send();
}

function getPowerFactor(){
    var valReq=new XMLHttpRequest();
    valReq.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("power_factor").innerHTML=this.responseText;
            console.log(this.responseText);
        }
    }
    valReq.open("GET","/power_factor",true);
    valReq.send();
}