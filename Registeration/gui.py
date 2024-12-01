import tkinter as tk
from tkinter import messagebox
from user_db import register_user, verify_user
from client import search_files_on_server
import threading

# Function to perform search on key release
def search_realtime(event=None):
    query = entry_search.get()
    if query:
        search_thread = threading.Thread(target=run_search, args=(query,))
        search_thread.start()

def run_search(query):
    try:
        results = search_files_on_server(query)
        update_results(results.split("\n"))  # Update listbox with results
    except Exception as e:
        messagebox.showerror("Error", f"Search failed: {e}")

def update_results(results):
    # Clear the listbox and update with new results
    listbox_results.delete(0, tk.END)
    for result in results:
        listbox_results.insert(tk.END, result)

# Register and Login Functions
def register():
    username = entry_username.get()
    password = entry_password.get()
    if username and password:
        register_user(username, password)
        messagebox.showinfo("Registration", "User registered successfully!")
    else:
        messagebox.showerror("Error", "Please enter both username and password.")

def login():
    username = entry_username.get()
    password = entry_password.get()
    if verify_user(username, password):
        messagebox.showinfo("Login", "Login successful!")
    else:
        messagebox.showerror("Error", "Invalid username or password.")

# Tkinter GUI
root = tk.Tk()
root.title("P2P File Sharing")

# Registration and Login Section
frame = tk.Frame(root)
frame.pack(padx=10, pady=10)

label_username = tk.Label(frame, text="Username:")
label_username.grid(row=0, column=0, padx=5, pady=5)

label_password = tk.Label(frame, text="Password:")
label_password.grid(row=1, column=0, padx=5, pady=5)

entry_username = tk.Entry(frame)
entry_username.grid(row=0, column=1, padx=5, pady=5)

entry_password = tk.Entry(frame, show="*")
entry_password.grid(row=1, column=1, padx=5, pady=5)

button_register = tk.Button(frame, text="Register", command=register)
button_register.grid(row=2, column=0, padx=5, pady=5)

button_login = tk.Button(frame, text="Login", command=login)
button_login.grid(row=2, column=1, padx=5, pady=5)

# Search Section
frame_search = tk.Frame(root)
frame_search.pack(padx=10, pady=10)

label_search = tk.Label(frame_search, text="Search for file:")
label_search.grid(row=0, column=0, padx=5, pady=5)

entry_search = tk.Entry(frame_search)
entry_search.grid(row=0, column=1, padx=5, pady=5)

entry_search.bind("<KeyRelease>", search_realtime)  # Detect key releases

listbox_results = tk.Listbox(frame_search, width=50, height=10)
listbox_results.grid(row=1, column=0, columnspan=2, pady=5)

root.mainloop()
