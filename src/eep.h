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
</head>

<body>
    <!-- <img src="images/door_icon.jpg" /> -->
    <p class="pagedesc">Saved Data</p>

    <p class="funcbut">Raise Sash<a href="/raisedoor"><button class="button">UP</button></a>
    Lower Sash<a href="/lowerdoor"><button class="button">DOWN</button></a></p>

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

                <tr><td>Up Speed:
                    <a href="/uspup"><button class="button">UP</button></a>
                    <a href="/uspdn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Down Speed:
                    <a href="/dspup"><button class="button">UP</button></a>
                    <a href="/dspdn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>SpringTop:
                    <a href="/stoup"><button class="button">UP</button></a>
                    <a href="/stodn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Spring Bottom:
                    <a href="/sboup"><button class="button">UP</button></a>
                    <a href="/sbodn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Slack Down:
                    <a href="/sldup"><button class="button">UP</button></a>
                    <a href="/slddn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Jam Clear:
                    <a href="/jclup"><button class="button">UP</button></a>
                    <a href="/jcldn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Min Speed Up:
                    <a href="/smuup"><button class="button">UP</button></a>
                    <a href="/smudn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Down Timeout:
                    <a href="/smdup"><button class="button">UP</button></a>
                    <a href="/smddn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Move Sense:
                    <a href="/smsup"><button class="button">UP</button></a>
                    <a href="/smsdn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>Lift Ignore:
                    <a href="/jitup"><button class="button">UP</button></a>
                    <a href="/jitdn"><button class="button">DN</button></a>
                </td></tr>

                <tr><td>PIR Timeout:
                    <a href="/prtup"><button class="button">UP</button></a>
                    <a href="/prtdn"><button class="button">DN</button></a>
                </td></tr>


                <tr><td>BounceTime:
                    <a href="/bntup"><button class="button">UP</button></a>
                    <a href="/bntdn"><button class="button">DN</button></a>
                </td></tr>


                <tr><td>Beam Sensor</td></tr>
                <tr><td>PIR Halts Close</td></tr>
                <tr><td>Push to Close</td></tr>
                <tr><td>Lift to Open</td></tr>
            </table>

             <p><a href="/">Run Time Data</a></p>



            </div>
            <div class="datacard">
                <span id="EEPDATA">Updating</span>
            </div>

        <script>

            setInterval(function () {
                getData();
            }, 600); //2000mSeconds update rate

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
