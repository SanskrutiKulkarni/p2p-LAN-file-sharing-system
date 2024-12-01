import os

# Function to search files in a directory (server side)
def search_files(query, directory=r"F:\LAN_Share\Registeration"):
    found_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if query.lower() in file.lower():
                found_files.append(os.path.join(root, file))
    return found_files
