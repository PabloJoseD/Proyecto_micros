from json import dumps
from time import sleep
import paho.mqtt.client as mqtt

# MQTT broker details
broker = "broker.emqx.io"
port = 1883
topic = "eie/ucon/proy/luz"
temp = 0

# Callback for connection
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to ThingsBoard")
    else:
        print("Connection failed")

# Set up the client
client = mqtt.Client()
client.on_connect = on_connect
client.connect(broker, port, keepalive=60)

# Start the loop to handle callbacks
client.loop_start()

try:
    while True:
        # Increment the temperature value
        temp += 0.25

        # Publish the temperature data as a JSON payload
        client.publish(topic, dumps({"temp": temp}), qos=1)
        print(f"Sent temperature: {temp}")

        # Wait 5 seconds before sending the next value
        sleep(5)
except KeyboardInterrupt:
    print("Disconnecting from broker...")
finally:
    client.loop_stop()
    client.disconnect()
