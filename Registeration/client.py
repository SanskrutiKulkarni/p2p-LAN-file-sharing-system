import socket

# Function to search for files by sending a query to the server
def search_files_on_server(query):
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(('127.0.0.1', 5555))  # Connect to the server
        
        # Send the query to the server
        client.send(query.encode('utf-8'))
        
        # Receive the search results
        results = client.recv(4096).decode('utf-8')  # Increased buffer size
        client.close()
        return results
    except Exception as e:
        return f"Error: {e}"


# Run the client
if __name__ == "__main__":
    query = input("Enter a file name to search for: ")
    search_files_on_server(query)
