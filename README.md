# finssn

`finssn` is a set of tools for generating Finnish social security numbers.
The project is made up from two parts: `finssn-gen` and `finssn-rand`.

## Usage

Build the project with `make`:

```bash
cd finssn
make all
```

This generates two executables in a newly created `bin` directory.

```text
Usage: finssn-gen [options]

Efficiently generates every valid Finnish social security number.
Valid SSN's are generated for birth dates between the years 1800 and 2099 (inclusive).

Options:
  -h, --help
  -q, --quiet        (Don't log messages)
  -s, --stdout       (Print SSNs to stdout)
  -o, --out-file     {output file path (default = "out.txt")}
  --from             {inclusive year (default = 1900)}
  --to               {exclusive year (default = current year + 1)}
```

```txt
Usage: finssn-rand [options] file

Efficiently reads random SSNs from a file and
prints them to stdout. The file should contain
SSNs separated by newlines.

Options:
  -h, --help
  -n, --number       {number of SSNs to read (default = 1)}
  -o, --out-file     {output file path (default = stdout)}
```
