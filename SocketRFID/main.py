import socket
from pymongo import MongoClient
from datetime import datetime

# Get mongoDB database
client = MongoClient('mongodb+srv://Admin:rWdRfQFDfqkU8Fr9@rfid.2uhgxw2.mongodb.net/')
db = client['RFID']

# Get RFID List
rfidList = db["RFIDList"]
# Get RFID Logs
rfidLogs = db["RFID_LOGS"]

# Host/Port of server
host = '192.168.211.40'  # Here, PC IP
port = 12345

# List for UID
listStatus = {}

# handle server
while True:

    # Get all UID list
    for uid in rfidList.find({}):
        listStatus[uid["UID"]] = uid["allowed"]

    # Create Socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)

    try:
        # Wait for connection
        print('\n\nWaiting for connexion...')
        client_socket, client_address = server_socket.accept()
        print('Connected:', client_address)

        # Set a timeout for socket
        client_socket.settimeout(3)

        # Receive data with timeout
        data = client_socket.recv(1024).decode('utf-8')
        print('UID:', data)

        # Check UID
        uidStatus = 'None'
        for uid in listStatus:
            # Check if the key exist in the Database
            if uid in data:
                # Get RFID Document
                doc = rfidList.find_one({'UID': uid})
                # Check if the badge is allowed
                if doc['attempt'] < doc['check_attempt'] or doc['check_attempt'] == -1:
                    # increment the badge's number attempt
                    doc['attempt'] += 1
                    uidStatus = listStatus[uid]
                    # Update the database document
                    rfidList.update_one({'UID': uid}, {'$set': {'attempt': doc['attempt']}})
                else:
                    uidStatus = False
                break

        # Create Logs
        rfidLogs.insert_one(
            {"UID": data, 'status': uidStatus, 'datetime': datetime.now()}
        )

        # Check status and send response to Client
        if uidStatus != 'None' and uidStatus:
            client_socket.send('ok'.encode('utf-8'))
        elif uidStatus != 'None' and not uidStatus:
            client_socket.send('no'.encode('utf-8'))
        else:
            client_socket.send('Undefined'.encode('utf-8'))

    # Handle errors
    except socket.timeout:
        print("La connexion a expiré. Aucune donnée reçue dans le délai spécifié.")

    finally:
        # Close socket
        client_socket.close()
        server_socket.close()
