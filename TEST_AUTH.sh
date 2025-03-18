#!/bin/bash

# Compile the program
g++ projetc++(1).cpp -o auth_system

# Run tests
echo "Running authentication tests..."

# Test registration
echo -e "1\nTestUser\nTestPass123\n3\n" | ./auth_system > output.txt
if grep -q "Registration successful" output.txt; then
    echo "✔ Registration test passed"
else
    echo "❌ Registration test failed"
    exit 1
fi

# Test login
echo -e "2\nTestUser\nTestPass123\n3\n" | ./auth_system > output.txt
if grep -q "Login successful" output.txt; then
    echo "✔ Login test passed"
else
    echo "❌ Login test failed"
    exit 1
fi

echo "All tests passed successfully!"
exit 0
