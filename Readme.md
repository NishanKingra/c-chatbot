# C ChatBot with Learning & Games 🤖

A console-based chatbot written in C that can chat with users, learn new responses, solve basic math problems, and play Rock-Paper-Scissors.

## Features
- Learns new question–answer pairs and stores them in a file
- Uses edit-distance (fuzzy matching) to reply to similar questions
- Built-in calculator for basic arithmetic operations
- Rock-Paper-Scissors game with score tracking

## Technologies Used
- C Programming Language
- File Handling
- Dynamic Programming
- String Processing

## Project Structure
c-chatbot/
├── chatbot.c
├── chatdata.txt
└── README.md

## How to Compile and Run
```bash
gcc chatbot.c -o chatbot
./chatbot

## Commands
bye → Exit chatbot
play rps → Play Rock-Paper-Scissors
what is 5 + 3 → Calculator

## Author
Nishan Kingra
B.E. Computer Science & Engineering Student