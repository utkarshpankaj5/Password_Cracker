# Multithreaded Password Cracker Simulator

A multithreaded password cracking simulator written in **C** that demonstrates how brute-force and dictionary-based password cracking techniques work using **SHA-256 hashing**, **POSIX threads (pthread)**, and parallel keyspace distribution.

This project was built for educational purposes to understand concepts such as:

* Brute-force attacks
* Dictionary attacks
* Hashing algorithms
* Multithreading
* Thread synchronization
* Performance optimization

---

# Features

* Multithreaded brute-force password cracking
* SHA-256 hash support# Password Security Analyzer and Attack Simulator

> An educational cybersecurity project that demonstrates password hashing, controlled password-guessing techniques, hash analysis, and defensive password-security concepts.

## 📌 About the Project

**Password Security Analyzer and Attack Simulator** is a B.Tech mini-project developed for the subject **Cryptanalysis and Cyber Defence**.

The main idea behind this project is simple: instead of learning password security only through theory, the application allows us to actually observe how password hashing works, how different password-guessing approaches behave, and why defensive techniques such as strong passwords and salting are important.

The project currently provides an interactive command-line interface where users can choose between **Password Cracking** and **Hash Analysis**. The cracking section demonstrates controlled brute-force and dictionary-based password guessing, while the analysis section examines a hash and attempts to identify its probable type.

The application is designed to work locally with sample hashes and password lists. It is **not intended to attack real accounts, websites, or systems**.

---

## 🎯 Project Objectives

The project aims to:

* Understand how cryptographic hashing works.
* Demonstrate SHA-256 hashing in a practical environment.
* Understand how dictionary attacks work.
* Demonstrate controlled brute-force password guessing.
* Understand the effect of password length and character-set size on the search space.
* Measure attack attempts, execution time, and attempts per second.
* Analyze the format and length of a given hash.
* Build a modular foundation that can later support additional hash algorithms and security-analysis features.
* Connect cryptanalysis concepts with practical cyber-defence techniques.

---

## 🔐 Current Features

### 1. Password Cracking

The application provides two controlled password-guessing methods.

#### Brute Force Attack

The program generates password combinations using the configured character set and compares their SHA-256 hashes with the target hash.

Current character set:

```text
abcdefghijklmnopqrstuvwxyz0123456789
```

The brute-force engine also uses multiple threads to demonstrate parallel processing.

It records:

* Number of attempts
* Execution time
* Attempts per second
* Whether the password was found

#### Dictionary Attack

The application can read candidate passwords from a local wordlist and hash each candidate.

The generated hash is then compared with the target hash.

Example:

```text
dictionary.txt
      ↓
Read candidate
      ↓
Generate SHA-256
      ↓
Compare with target
      ↓
Match?
   ↙       ↘
 Yes        No
  ↓          ↓
Found      Continue
```

---

### 2. Hash Analysis

The **Analyze Hash** option accepts a hash and examines its basic characteristics.

The current implementation checks:

* Hash length
* Hexadecimal format

At the moment, a valid 64-character hexadecimal hash is reported as a **probable SHA-256 hash**.

This module is intentionally designed so that additional hash types can be added later.

Planned possibilities include:

```text
MD5
SHA-1
SHA-256
SHA-512
```

> Hash identification based only on length and format is not guaranteed to uniquely determine an algorithm. Therefore, the application reports a **probable** hash type rather than claiming absolute identification.

---

## 🖥️ Application Flow

When the program is started normally:

```bash
./cracker
```

the application opens an interactive menu.

```text
============================================
       PASSWORD SECURITY ANALYZER
============================================

 > Crack Password
   Analyze Hash
```

The user can navigate using the **UP/DOWN arrow keys** and press **ENTER** to select an option.

### Crack Password

```text
Crack Password
      ↓
Choose Attack Method
      ↓
 ┌───────────────┐
 │               │
 ▼               ▼
Brute Force   Dictionary
 │               │
 └───────┬───────┘
         ↓
    SHA-256 Hash
         ↓
     Comparison
         ↓
       Result
```

### Analyze Hash

```text
Analyze Hash
      ↓
Enter Hash
      ↓
Check Length
      ↓
Check Format
      ↓
Probable Hash Type
      ↓
Display Result
```

---

## 💻 Command-Line Usage

The application also supports direct command-line execution.

### Start interactive menu

```bash
./cracker
```

### Display help

```bash
./cracker -help
```

or:

```bash
./cracker --help
```

### Specify a target hash

```bash
./cracker -h <hash>
```

### Set maximum password length

```bash
./cracker -h <hash> -l 4
```

### Use a wordlist

```bash
./cracker -h <hash> -w words.txt
```

### Enable verbose output

```bash
./cracker -h <hash> -v
```

### Combine options

```bash
./cracker -h <hash> -l 4 -v
```

---

## ⚙️ Requirements

The project currently targets Linux/Unix-like environments.

Required software:

* GCC
* OpenSSL development libraries
* POSIX Threads
* Standard C library
* Linux terminal supporting arrow-key input

### Install dependencies on Ubuntu/Debian

```bash
sudo apt update
sudo apt install gcc libssl-dev
```

---

## 🔨 Compilation

Compile the project using:

```bash
gcc cracker.c -o cracker -lcrypto -lpthread
```

Run:

```bash
./cracker
```

---

## 📁 Project Structure

The project is being organized into separate modules so that new functionality can be added without making the code difficult to maintain.

```text
PasswordSecurityAnalyzer/
│
├── src/
│   ├── main.c
│   │
│   ├── menu.c
│   ├── menu.h
│   │
│   ├── hash.c
│   ├── hash.h
│   │
│   ├── cracker.c
│   ├── cracker.h
│   │
│   ├── analyzer.c
│   └── analyzer.h
│
├── data/
│   └── dictionary.txt
│
├── README.md
└── Makefile
```

### Module Responsibilities

| Module           | Responsibility                                       |
| ---------------- | ---------------------------------------------------- |
| `main.c`         | Starts the application and controls the overall flow |
| `menu.c/.h`      | Interactive menus and keyboard navigation            |
| `hash.c/.h`      | Hash-related operations                              |
| `cracker.c/.h`   | Brute-force and dictionary attack functionality      |
| `analyzer.c/.h`  | Hash analysis and identification                     |
| `dictionary.txt` | Local password candidates for educational testing    |
| `Makefile`       | Simplifies compilation                               |

---

## 🧠 Cryptanalysis Concepts

This project provides practical exposure to several cryptanalysis concepts:

* Cryptographic hashing
* SHA-256
* Hash comparison
* Dictionary attacks
* Brute-force attacks
* Search-space analysis
* Password guessing
* Computational cost
* Multithreaded processing
* Hash identification

One of the important concepts demonstrated by the project is that the difficulty of brute-force guessing increases rapidly as the password length and character-set size increase.

---

## 🛡️ Cyber Defence Concepts

The project also focuses on the defensive side of password security.

The concepts that will be explored as the project develops include:

* Strong and unique passwords
* Password entropy
* Secure password storage
* Salting
* Password managers
* Multi-factor authentication
* Avoiding password reuse
* Avoiding predictable password patterns
* Appropriate password-hashing algorithms

The goal is not simply to demonstrate how password guessing works, but also to understand **how systems can be designed to resist these attacks**.

---

## 🔬 Educational Experiment

A simple experiment can be performed by generating a SHA-256 hash of a known test password and then providing that hash to the simulator.

For example:

```text
Test Password
      ↓
SHA-256
      ↓
Target Hash
      ↓
Simulator
      ↓
Dictionary / Brute Force
      ↓
Compare Hashes
      ↓
Result
```

The experiment allows students to observe:

* How many candidates were tested.
* How long the process took.
* How quickly hashes were processed.
* Whether the password was found.
* How changing password length affects the search space.

---

## ⚠️ Ethical Use

This project is created for **educational and authorized security experimentation only**.

It is designed to work with:

* Self-generated hashes
* Sample passwords
* Local wordlists
* Controlled laboratory experiments

It should **not** be used to:

* Attack accounts belonging to other people.
* Guess passwords for real online services.
* Bypass authentication.
* Attack systems without authorization.
* Obtain or misuse private credentials.

The purpose of the project is to understand password security and improve defensive cybersecurity knowledge.

---

## 🚧 Current Limitations

The current version is intentionally simple and focuses on the core concepts.

Current limitations include:

* SHA-256 is currently the primary supported hash for cracking.
* Hash identification currently focuses on SHA-256 characteristics.
* Brute-force search is limited by the configured maximum password length and character set.
* Dictionary attacks depend on the contents of the supplied wordlist.
* The project currently operates through a terminal-based interface.
* Crack-time estimates are not yet implemented as a complete password-strength model.

These limitations are part of the planned development process.

---

## 🔮 Future Enhancements

The project is planned to grow into a more complete **Password Security Analyzer**.

Possible future modules include:

### Hash Algorithms

```text
MD5
SHA-1
SHA-256
SHA-512
```

### Password Strength Analyzer

* Password length analysis
* Character-set analysis
* Repeated-pattern detection
* Dictionary-word detection
* Entropy estimation
* Password-strength classification

### Salt Demonstration

* Generate random salts.
* Compare salted and unsalted hashes.
* Demonstrate why unique salts are important.
* Explain the limitations of precomputed hash tables.

### Security Recommendation Engine

Provide recommendations related to:

* Password managers
* MFA
* Unique passwords
* Passphrases
* Secure password storage

### Statistics Dashboard

Possible statistics include:

* Attack type
* Hash algorithm
* Number of attempts
* Execution time
* Attempts per second
* Password length
* Success/failure

The long-term goal is to turn the current command-line simulator into a more complete educational cybersecurity application.

---

## 📚 Academic Relevance

This project is developed as part of a **B.Tech Cryptanalysis and Cyber Defence** course.

It combines concepts from:

* Cryptography
* Cryptanalysis
* Cyber defence
* Data structures and algorithms
* Operating systems
* Multithreading
* C programming
* Secure software development

It provides a practical way to understand how password attacks work while also highlighting the defensive measures required to protect password-based authentication systems.

---

## 👨‍💻 Development Approach

The project is being developed incrementally.

The initial version focuses on:

1. SHA-256 hashing
2. Brute-force simulation
3. Dictionary attack simulation
4. Interactive terminal menu
5. Hash analysis

Additional modules will be introduced gradually rather than putting everything into a single large program.

This modular approach makes the project easier to:

* Understand
* Test
* Debug
* Explain during viva
* Extend with new security features

---

## 📌 Project Status

**Current Status:** 🚧 Under Development

### Completed / In Progress

* [x] SHA-256 hashing
* [x] Brute-force simulation
* [x] Dictionary attack simulation
* [x] Multithreaded cracking
* [x] Attack statistics
* [x] Interactive menu
* [x] Command-line arguments
* [x] Basic hash analysis

### Planned

* [ ] Multiple hash-type identification
* [ ] Password strength analyzer
* [ ] Entropy calculation
* [ ] Salt demonstration
* [ ] Security recommendation engine
* [ ] Statistics dashboard
* [ ] Improved modular architecture
* [ ] GUI version

---

## 📄 Disclaimer

This repository is intended for **academic and cybersecurity education purposes**. The software should only be used against passwords and hashes that you own or have explicit permission to test.

---

## ⭐ Final Note

This project started with a simple question: **“How does password cracking actually work?”**

Instead of treating the answer as only a theoretical concept, the project aims to make the process visible and measurable. At the same time, understanding the attack process helps us understand what needs to be protected and why defensive mechanisms such as strong passwords, proper password hashing, salting, and MFA matter.

The project will continue to evolve as new cryptanalysis and cyber-defence concepts are added.

* Dictionary attack mode using external wordlists
* Keyspace splitting across multiple threads
* Recursive password generation
* Attempts-per-second tracking
* Verbose mode for live cracking visualization
* CLI-based interface similar to Linux/Kali tools
* Customizable maximum password length
* Lowercase + numeric character support

---

# How It Works

The program accepts a target SHA-256 hash and attempts to recover the original password using either:

## 1. Brute Force Mode

The program automatically generates all possible password combinations within the specified character set and password length.

Example:

```txt id="b2nlbk"
a
ab
abc
cat123
```

The keyspace is divided among multiple threads based on the first character, allowing parallel execution and faster searching.

---

## 2. Dictionary Attack Mode

The program loads passwords from an external wordlist file and compares their hashes against the target hash.

Example wordlist:

```txt id="q5c2g6"
admin
password
cat123
test123
```

---

# Technologies Used

* C Programming
* POSIX Threads (`pthread`)
* OpenSSL SHA-256 Library
* Linux Command Line Tools

---

# Compilation

Compile the program using:

```bash id="1jbfzd"
gcc cracker.c -o cracker -lpthread -lcrypto
```

---

# Usage

## Help Menu

```bash id="j0r1l2"
./cracker
```

---

## Brute Force Mode

```bash id="n4q9hf"
./cracker -h <hash>
```

Example:

```bash id="vmrj3u"
./cracker -h <sha256_hash> -l 4
```

---

## Verbose Mode

```bash id="0tp8tr"
./cracker -h <hash> -l 4 -v
```

---

## Dictionary Attack Mode

```bash id="jlwmkh"
./cracker -h <hash> -w words.txt
```

Verbose dictionary attack:

```bash id="5b47aj"
./cracker -h <hash> -w words.txt -v
```

---

# Example

Generate SHA-256 hash:

```bash id="gs9p9g"
echo -n "cat123" | sha256sum
```

Run brute-force attack:

```bash id="qp01ha"
./cracker -h <generated_hash> -l 6
```

---

# Performance Tracking

The simulator tracks:

* Total attempts
* Time taken
* Attempts per second

Example output:

```txt id="g2y1rb"
==== RESULT ====
Password Found: cat123
Attempts: 154233
Time: 2.31 sec
Attempts/sec: 66767.10
```

---

# Project Structure

```txt id="s5e4wb"
cracker.c
README.md
words.txt
```

---
# Future Improvements

The project is currently focused on SHA-256 hashing, but support for additional hashing algorithms can be added in future versions, including:

MD5
SHA-1
SHA-512
bcrypt
PBKDF2

Future enhancements may also include automatic hash-type detection, GPU acceleration, advanced thread pool optimization, expanded character sets, and support for hybrid attack strategies combining brute-force and dictionary-based techniques.

---
# Learning Outcomes

This project helped in understanding:

* Password cracking methodologies
* Hashing and hash comparison
* Recursive brute-force generation
* Thread pools and concurrency
* Mutex locks and synchronization
* Performance benchmarking

---

# Disclaimer

This project was created strictly for **educational and research purposes only**.
It is intended to demonstrate password cracking concepts, multithreading, and hashing mechanisms in a controlled environment. Unauthorized use against systems or accounts without permission is unethical and may be illegal.
