# Usage

## Prerequisites

- Ensure the `fio` command-line tool is installed on your device.

## Commands

### Write Test

Replace `${data_size}`, `${file}`, and `${block_size}` with your values.

```bash
fio --name=testfile --size=${data_size}m --filename=${file} --rw=write --bs=${block_size}m --numjobs=1 --ioengine=sync --gtod_reduce=1 --zero_buffers=1 --direct=1 --fdatasync=1
```

### Read Test

```bash
fio --name=testfile --size=${data_size}m --filename=${file} --rw=read --bs=${block_size}m --numjobs=1 --ioengine=sync --gtod_reduce=1 --zero_buffers=1 --direct=1
```

## Plotting/Comparing

- Update the notebook with your output CSV file names to view the latency and throughput results.
