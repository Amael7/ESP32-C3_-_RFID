# ESP32-C3_-_RFID

# Description :
  Build a securised system with an ESP32-C3 and a badge reader to valid or unvalid the access

  There's 4 use's cases :

  1. The badge is valid and has 3 access per day and the green diod is turn on
  2. The badge is unvalid and the red diod is turn on
  3. The badge is unknow and the yellow diod is turn on
  4. One badge has an unlimited access

  When a badge is checked, new logs are created and saved in the Database

# Prerequisites

  The project uses :

  - **ESP32-C3** (The electronics card that will be connected to the wifi and get the access badge behavior)
  - **Breadboard** (to connect all the electronics together)
  - **3 leds** (one green, one red, one yellow)
  - Some **cable** to connect everything
  - A **resistor** based on 220 Ohm Ω
  - **RFID-RC522** (to read the badge)
  - **Arduino Software** (to code the ESP32-C3 behavior)
  - **PyCharm** (to code with Python 3.11 to interract with the DB)
  - **MongoDB** (the database)

## Setup

  - Set the electronics structure

  ### Schema of the electronics structure


![schema](https://github.com/Amael7/ESP32-C3_-_RFID/assets/85516178/681ac28a-c7c9-42f4-8fdd-973fa4c60d09)


  ### Documentation of the connections between the differents electronics

  - **ESP32-C3** GPIO2 -> Green led -> Resistor -> GRND
  - **ESP32-C3** GPIO3 -> Orange led -> Resistor -> GRND
  - **ESP32-C3** GPIO4 -> Red led -> Resistor -> GRND
  - **ESP32-C3** GPIO6 -> **RFID-RC522** SDA
  - **ESP32-C3** GPIO7 -> **RFID-RC522** RST
  - **ESP32-C3** GRND -> **RFID-RC522** GRND
  - **ESP32-C3** 3V3 -> **RFID-RC522** 3V3
  - **ESP32-C3** GPIO10 -> **RFID-RC522** MOSI
  - **ESP32-C3** GPIO9-> **RFID-RC522** MISO
  - **ESP32-C3** GPIO8-> **RFID-RC522** SCK

  ### Set the Arduino program that will turn on/off the leds

  ### Set the MongoDB
    ##### Step 1
    Download MongoDB (Compass) -> https://www.mongodb.com/try/download/compass

    ##### Step 2
    Set Database and configure Network Access 

    ##### Step 3
    Get the connexion link to use Database in Python
    

  ### Set the Python code
    ##### Step 1
    Download Python (3.12.X)

    ##### Step 2
    import mongodb (pymongo)

    ##### Step 3
    Set the MongoDB (Client) -> client = MongoClient('mongodb+srv://USER:<PASSWORD>@SERVER.mongodb.net/')

    ##### Step 4
    Get database -> db = client['DATABASE']

    ##### Step 5 
    Create socket and connect it to the arduino



