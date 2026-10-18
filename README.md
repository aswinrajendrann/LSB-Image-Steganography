# LSB Image Steganography in C

A C-based implementation of **Least Significant Bit (LSB) steganography** that hides secret text files inside BMP images and retrieves them through a decoding process.

## Overview

Steganography is the technique of concealing information within another file so that the presence of the information is less apparent. This project implements LSB steganography to encode text data into a BMP image and decode the hidden data when required.

The program uses bitwise operations and binary file handling to embed secret data into the image's pixel data while preserving the original BMP header.

## Features

- Encode secret text files into BMP images.
- Decode and recover hidden text data from stego images.
- Embed data using Least Significant Bit (LSB) manipulation.
- Store and retrieve a magic string, secret file extension, and file size.
- Support optional output filenames during decoding.
- Handle file operations and invalid command-line arguments.

## Technologies and Concepts

- **Language:** C
- **Image format:** BMP
- **Secret file format:** TXT
- **Core concepts:** Bitwise operations, pointers, structures, file handling, command-line arguments, and error handling.
- **Development environment:** GCC and Linux/WSL

## How LSB Steganography Works

Each byte of secret data contains eight bits. The program embeds these bits into the least significant bits of image-data bytes.

Because only the least significant bit is modified, the changes to the underlying image data are generally small and may not be visually noticeable.

The project follows this data layout:

1. BMP header
2. Magic string
3. Secret file extension size
4. Secret file extension
5. Secret file size
6. Secret file contents
7. Remaining image data

During decoding, the program reads the embedded information in the same order and reconstructs the original secret file.

## Project Structure

| File | Description |
|---|---|
| `test_encode.c` | Main program and operation selection |
| `encode.c` | Implements the encoding process |
| `encode.h` | Encoding declarations and structures |
| `decode.c` | Implements the decoding process |
| `decode.h` | Decoding declarations and structures |
| `common.h` | Common definitions, including the magic string |
| `types.h` | User-defined data types |
| `.gitignore` | Excludes generated and temporary files from Git |

## Getting Started

### Prerequisites

- GCC compiler
- Linux environment or WSL
- A BMP image to use as the source
- A text file containing the secret data

### Compilation

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/aswinrajendrann/LSB-Image-Steganography.git
cd LSB-Image-Steganography
```

Compile the source files:

```bash
gcc test_encode.c encode.c decode.c -o steganography
```

### Encoding

Use the following command to hide a text file inside a BMP image:

```bash
./steganography -e source_image.bmp secret.txt stego.bmp
```

**Arguments:**

- `-e`: Selects encoding mode.
- `source_image.bmp`: Original BMP image.
- `secret.txt`: File containing the secret data.
- `stego.bmp`: Output image containing the hidden data.

### Decoding

To recover the hidden data and specify an output filename:

```bash
./steganography -d stego.bmp output.txt
```

To let the program generate the output filename automatically:

```bash
./steganography -d stego.bmp
```

When the output filename is omitted, the program generates `decoded.txt` for a hidden `.txt` file.

## Error Handling

The implementation includes checks for file-opening failures, invalid command-line arguments, decoding failures, and unsuccessful file operations.

## Limitations

- Supports BMP images as the carrier format.
- Supports TXT files as the secret input.
- The image must have sufficient capacity to store the secret data.
- LSB steganography conceals data but does not encrypt it. It should not be considered a substitute for encryption when confidentiality is required.

## Learning Outcomes

Through this project, I gained practical experience with:

- Bitwise manipulation and LSB encoding.
- Binary file reading and writing in C.
- Working with pointers, structures, and buffers.
- Command-line argument validation.
- Organizing a multi-file C project using header and source files.
- Implementing encoding and decoding workflows.

## Author

**Aswin Rajendran**

GitHub: [@aswinrajendrann](https://github.com/aswinrajendrann)

---

*This project was developed as part of my C programming and systems-level learning journey.*
