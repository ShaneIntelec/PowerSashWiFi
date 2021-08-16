const char EEP_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" >

    <head>
        <title>Intelec Sash Programmer</title>
        <style>
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
                font-size: 2rem;
            }

            .button {
                display: inline-block;
                background-color: #156079;
                border: none;
                border-radius: 4px;
                color: white;
                padding: 16px 24px;
                text-decoration: none;
                font-size: 20px;
                margin: 2px;
                cursor: pointer;
                box-shadow: 0 9px #999;
            }

            .button:active {
                background-color: #3e8e41;
                box-shadow: 0 5px #666;
                transform: translateY(4px);
            }

            .led-green {
                margin: 0 auto;
                width: 24px;
                height: 24px;
                background-color: #ABFF00;
                border-radius: 50%;
                box-shadow: rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #304701 0 -1px 9px, #89FF00 0 2px 12px;
            }

            .pagedesc {
                text-align: left;
                font-weight: bold;
                background-color: #6e8f6f;

            }

            .funcbut {
                text-align: left;
                font-weight: bold;
                background-color: #5b9294;
            }

            * {
                box-sizing: border-box;
            }

            body {
                font-family: Arial, Helvetica, sans-serif;
            }

            tr {
            height: 80px;   
            font-size: 2rem;
            }

            /* Remove extra left and right margins, due to padding in columns */
            .row {
                margin: 0 -5px;
            }

            /* Clear floats after the columns */
            .row:after {
                content: "";
                display: table;
                clear: both;
            }

            /* Style the counter cards */
            .card {
                float: left;
                /* width: 80%; */
                padding: 0 10px;
                box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
                padding: 16px;
                text-align: right;
                background-color: #9ec5b5;
            }

            .datacard {
                float: left;
                /* width: 20%; */
                min-width: 200px;
                padding: 0 10px;
                box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
                padding: 16px;
                text-align: left;
                background-color: #eba8a8;
                padding-bottom: 30px;
            }

        </style>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    </head>

    <body>
        <!-- <img src="images/door_icon.jpg" /> -->
        <p class="pagedesc">Saved Data</p>

        <p class="funcbut">
            Raise Sash<a href="/raisedoor"><button class="button">UP</button></a><br/>
            Lower Sash<a href="/lowerdoor"><button class="button">DOWN</button></a>
        </p>
        <div class="row">
            <div class="card">
                <table>
                    <tr><td>Sash Top:</td></tr>
                    <tr><td>Sash Bottom:</td></tr>
                    <tr><td>Current Position:</td></tr>
                    <tr><td>Sash:</td></tr>
                    <tr><td>Status Data:</td></tr>
                    <tr><td>Program mode:</td></tr>
                    
                    <tr><td>PIR Input:</td></tr>
                    <tr><td>Beam Input:</td></tr>
                    <tr><td>Connected:</td></tr>

                    <tr><td>PIR Timer:</td></tr>

                    <tr><td>Beam Sensor</td></tr>
                    <tr><td>PIR Halts Close</td></tr>
                    <tr><td>Push to Close</td></tr>
                    <tr><td>Lift to Open</td></tr>
                </table>
                <form action="/submit" class="box" id="my-form">
                    <div class="part">
                        <input name="usp" type="number" placeholder="Up Speed" min="0" max="100">
                    </div>
                    <div class="part">
                        <input name="dsp" type="number" placeholder="Down Speed" min="0" max="100">
                    </div>
                    
                    <div class="part">
                        <input name="sto" type="number" placeholder="Spring Top" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="sbo" type="number" placeholder="Spring Bottom" min="0" max="100">
                    </div>
                    
                    <div class="part">
                        <input name="sld" type="number" placeholder="Slack Down" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="jcl" type="number" placeholder="Jam Clear" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="smu" type="number" placeholder="Min Speed Up" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="smd" type="number" placeholder="Down Timeout" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="sms" type="number" placeholder="Move Sense" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="jit" type="number" placeholder="Lift Ignore" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="prt" type="number" placeholder="PIR Timeout" min="0" max="100">
                    </div>

                    <div class="part">
                        <input name="bnt" type="number" placeholder="Bounce Time" min="0" max="100">
                    </div>

                </form>

                <p><a href="/">Run Time Data</a></p>

            </div>
            <div class="datacard">
                <span id="EEPDATA">Updating</span>
            </div>
        </div>
        <script>

            setInterval(function () {
                getData();
            }, 600);

            function getData() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("EEPDATA").innerHTML =
                            this.responseText;
                    }
                };
                xhttp.open("GET", "eepData", true);
                xhttp.send();
            }
        </script>

    </body>

</html>

)=====";
