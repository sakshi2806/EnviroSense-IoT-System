import asyncio
import websockets
import json

# Function to handle incoming WebSocket messages
async def handle_websocket(websocket, path):
    print(f"Connection established on path {path}")
    try:
        async for message in websocket:
            print("Received message from client:")
            print(message)
            
            # Assuming the message is in JSON format
            try:
                data = json.loads(message)
                process_received_data(data)
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON: {e}")
                await websocket.send("Invalid JSON format received.")
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")

# Function to process the received JSON data and round values to 2 decimal places
def process_received_data(data):
    # Extracting values safely from the received JSON
    mac_address = data.get("mac_address", "Unknown MAC Address")

    # Safely extract and round temperature, humidity, and luminescence values
    temperature = round(data.get("temperature", {}).get("temp", None), 2)
    luminescence = round(data.get("luminescence", {}).get("lux", None), 2)
    humidity = round(data.get("humidity", {}).get("humi", None), 2)

    # Print the rounded values
    print(f"MAC Address: {mac_address}")
    print(f"Temperature: {temperature}°C")
    print(f"Luminescence: {luminescence} lux")
    print(f"Humidity: {humidity}%")

# Start the WebSocket server
async def main():
    async with websockets.serve(handle_websocket, "0.0.0.0", 8000):
        print("WebSocket server started on ws://0.0.0.0:8000")
        await asyncio.Future()  # Run the server forever

# Entry point of the program
if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Server stopped.")
