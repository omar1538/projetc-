# Authentication System in C++

## Description

This project is a simple authentication system implemented in C++. It allows users to register and log in with a username and password. The passwords are hashed with a generated salt for security.

## Features

- User registration with username and password
- Password hashing with a salt for enhanced security
- Login system with authentication verification
- User data stored in a CSV file (`users.csv`)

## Technologies Used

- C++ Standard Library
- File handling (`fstream`)
- Hashing (`std::hash`)
- Random number generation for salting (`random`)

## How to Use

1. **Compile the program**  
   Use a C++ compiler (e.g., g++) to compile the source file:

   ```bash
   g++ projetc++(1).cpp -o auth_system
