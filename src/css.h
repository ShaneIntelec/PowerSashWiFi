const char CSS_page[] PROGMEM = R"=====(


/***
   Rui Santos
   Complete project details at https://RandomNerdTutorials.com
***/

html {
  font-family: Arial;
  display: inline-block;
  margin: 0px auto;
  text-align: center;
}
h1 {
  color: #0F3376;
  padding: 2vh;
}
p {
  font-size: 1.5rem;
}
#para1 {
  font-size: 2rem;
}

.textname {
  border:2px solid blue;
  width:40%;
  color:black;
  text-align:left;
}

.variable {
  width:60%;
  color:black;
  text-align:left;
}

.paramdiv {
  border: 5px outset red;
  background-color: lightblue;    
  text-align: center;
}

.button {
  display: inline-block;
  background-color: #008CBA;
  border: none;
  border-radius: 4px;
  color: white;
  padding: 16px 24px;
  text-decoration: none;
  font-size: 20px;
  margin: 2px;
  cursor: pointer;
}
.button2 {
  background-color: #f44336;
}
.button3 {
  background-color: #33cc33;
}
.button4 {
  background-color:Gold
}
 .triangle-left {
  width: 0;
  height: 0;
  background-color:white;
  border-top: 25px solid transparent;
  border-right: 50px solid #ffcc00;
  border-bottom: 25px solid transparent;
    cursor: pointer;
}
 .rectangle {
    width: 200px;
    height: 100px;
    background: white;
    }
  .triangle-right {
  width: 0;
  height: 0;
  background-color:white;
  border-top: 25px solid transparent;
  border-left: 50px solid #ffcc00;
  border-bottom: 25px solid transparent;
    cursor: pointer;
}
 .triangle-up {
      width: 0;
      height: 0;
    background-color:white;
      border-left: 25px solid transparent;
      border-right: 25px solid transparent;
      border-bottom: 50px solid red;
    }
.triangle-down {
      width: 0;
      height: 0;
      border-left: 25px solid transparent;
      border-right: 25px solid transparent;
      border-top: 50px solid red;
    }
.units {
  font-size: 1.2rem;
 }
.sensor-labels {
  font-size: 1.5rem;
  vertical-align:middle;
  padding-bottom: 15px;
}

.slidecontainer {
  width: 100%; /* Width of the outside container */
}

/* The slider itself */
.slider {
  -webkit-appearance: none;  /* Override default CSS styles */
  appearance: none;
  width: 80%; /* Full-width */
  height: 25px; /* Specified height */
  background: #d3d3d3; /* Grey background */
  outline: none; /* Remove outline */
  opacity: 0.7; /* Set transparency (for mouse-over effects on hover) */
  -webkit-transition: .2s; /* 0.2 seconds transition on hover */
  transition: opacity .2s;
}

/* Mouse-over effects */
.slider:hover {
  opacity: 1; /* Fully shown on mouse-over */
}

/* The slider handle (use -webkit- (Chrome, Opera, Safari, Edge) and -moz- (Firefox) to override default look) */
.slider::-webkit-slider-thumb {
  -webkit-appearance: none; /* Override default look */
  appearance: none;
  width: 25px; /* Set a specific slider handle width */
  height: 25px; /* Slider handle height */
  background: #4CAF50; /* Green background */
  cursor: pointer; /* Cursor on hover */
}

.slider::-moz-range-thumb {
  width: 25px; /* Set a specific slider handle width */
  height: 25px; /* Slider handle height */
  background: #4CAF50; /* Green background */
  cursor: pointer; /* Cursor on hover */
}

)=====";
