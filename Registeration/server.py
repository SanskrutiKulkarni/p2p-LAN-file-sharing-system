# import socket
# import threading
# from file_search import search_files

# # Server function that handles client connections
# def handle_client(client_socket):
#     while True:
#         try:
#             # Receive file search query from the client
#             query = client_socket.recv(1024).decode('utf-8')
#             if query:
#                 print(f"Received search query: {query}")
#                 # Search for files in the directory
#                 results = search_files(query)
#                 # Send the results to the client
#                 response = "\n".join(results)
#                 client_socket.send(response.encode('utf-8'))
#             else:
#                 break
#         except Exception as e:
#             print(f"Error: {e}")
#             break
#     client_socket.close()

# # Start the server
# def start_server():
#     server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#     server.bind(('0.0.0.0', 5555))  # Listen on port 5555
#     server.listen(5)
#     print("Server started on port 5555")
    
#     while True:
#         client_socket, addr = server.accept()
#         print(f"Connection from {addr}")
#         client_handler = threading.Thread(target=handle_client, args=(client_socket,))
#         client_handler.start()

# # Run the server
# if __name__ == "__main__":
#     start_server()

import socket
import threading
from file_search import search_files

# Server function that handles client connections
def handle_client(client_socket):
    while True:
        try:
            # Receive file search query from the client
            query = client_socket.recv(1024).decode('utf-8')
            if query:
                print(f"Received search query: {query}")
                # Search for files in the directory
                results = search_files(query)
                # Send the results to the client
                response = "\n".join(results)
                client_socket.send(response.encode('utf-8'))
            else:
                break
        except Exception as e:
            print(f"Error: {e}")
            break
    client_socket.close()

# Start the server
def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 5555))  # Listen on port 5555
    server.listen(5)
    print("Server started on port 5555")
    
    while True:
        client_socket, addr = server.accept()
        print(f"Connection from {addr}")
        client_handler = threading.Thread(target=handle_client, args=(client_socket,))
        client_handler.start()

# Run the server
if __name__ == "__main__":
    start_server()