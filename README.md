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
* SHA-256 hash support
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
#Future Improvements

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
