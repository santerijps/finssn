#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>


uint32_t rand_u32();
uint64_t rand_u64();


errno_t main(int argc, char **argv) {

  errno_t error = 0;
  struct _stat64 file_info;
  FILE *input_file = NULL, *output_file = NULL;
  char ssn[12] = {0};

  char *input_file_path = NULL;
  char *output_file_path = NULL;
  long long n_ssn = 1;

  for (int i = 1; i < argc; i++) {

    if (!strncmp(argv[i], "-h", 3) || !strncmp(argv[i], "--help", 7)) {
      fprintf(
        stderr,
        "Usage: finssn-rand [options] file\n"
        "\n"
        "Efficiently reads random SSNs from a file and\n"
        "prints them to stdout. The file should contain\n"
        "SSNs separated by newlines.\n"
        "\n"
        "Options:\n"
        "  -h, --help\n"
        "  -n, --number       {number of SSNs to read (default = 1)}\n"
        "  -o, --out-file     {output file path (default = stdout)}\n"
        "\n"
      );
      goto CLEAN_UP;
    }

    else if (!strncmp(argv[i], "-n", 3) || !strncmp(argv[i], "--number", 9)) {
      if (i + 1 < argc) {
        n_ssn = atoll(argv[++i]);
      } else {
        fprintf(stderr, "Missing argument for option %s\n", argv[i]);
        error = 1;
        goto CLEAN_UP;
      }
    }

    else if (!strncmp(argv[i], "-o", 3) || !strncmp(argv[i], "--out-file", 11)) {
      if (i + 1 < argc) {
        output_file_path = argv[++i];
      } else {
        fprintf(stderr, "Missing argument for option %s\n", argv[i]);
        error = 1;
        goto CLEAN_UP;
      }
    }

    else {
      if (input_file_path == NULL) {
        input_file_path = argv[i];
      } else {
        fprintf(stderr, "Invalid argument: %s\nAn input file was already provided.\n", argv[i]);
        error = 1;
        goto CLEAN_UP;
      }
    }

  }

  if (input_file_path == NULL) {
    fprintf(stderr, "No input file provided!\n");
    error = 1;
    goto CLEAN_UP;
  }

  if (__stat64(input_file_path, &file_info) == -1) {
    fprintf(stderr, "Failed to open input file! (stat)\n%s\n", strerror(errno));
    error = errno;
    goto CLEAN_UP;
  }

  if ((input_file = fopen(input_file_path, "rb")) == NULL) {
    fprintf(stderr, "Failed to open input file! (fopen)\n");
    error = 1;
    goto CLEAN_UP;
  }

  if (output_file_path != NULL && (output_file = fopen(output_file_path, "wb")) == NULL) {
    fprintf(stderr, "Failed to open output file! (fopen)\n");
    error = 1;
    goto CLEAN_UP;
  }

  srand(time(NULL));

  for (long long i = 0; i < n_ssn; i++) {

    // SSNs have a length of 11 (+ newline)
    long long offset_max = file_info.st_size - 12;

    // Ensure that the random number is divisible by 12
    long long rand_max = offset_max / 12;
    long long offset = (rand_u32() % (rand_max + 1)) * 12;

    if (_fseeki64(input_file, offset, 0)) {
      fprintf(stderr, "Failed to seek file!\n");
      error = 1;
      goto CLEAN_UP;
    }

    if (fread(ssn, sizeof(char), 11, input_file) != 11) {
      fprintf(stderr, "Failed to read 11 bytes!\n");
      error = 1;
      goto CLEAN_UP;
    }

    if (output_file != NULL) {
      fprintf(output_file, "%s\n", ssn);
    } else {
      puts(ssn);
    }

  }

CLEAN_UP:

  if (input_file != NULL && fclose(input_file)) {
    fprintf(stderr, "Failed to close input file!\n");
    error = 1;
  }

  return error;

}


inline uint32_t rand_u32() {
  return
    (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) |
    (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) ;
}


inline uint64_t rand_u64() {
  return
    (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) |
    (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) |
    (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
    (((uint64_t) rand() << 48) & 0xFFFF000000000000ull) ;
}
