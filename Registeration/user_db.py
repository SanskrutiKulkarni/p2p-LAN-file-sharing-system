import sqlite3

# Create a database or connect to one
conn = sqlite3.connect('users.db')
c = conn.cursor()

# Create a table to store user data
def create_table():
    c.execute('''CREATE TABLE IF NOT EXISTS users (
                    id INTEGER PRIMARY KEY,
                    username TEXT NOT NULL,
                    password TEXT NOT NULL)''')
    conn.commit()

# Register a new user
def register_user(username, password):
    c.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, password))
    conn.commit()

# Verify user credentials
def verify_user(username, password):
    c.execute("SELECT * FROM users WHERE username=? AND password=?", (username, password))
    user = c.fetchone()
    return user is not None

# Initialize the database table
create_table()
