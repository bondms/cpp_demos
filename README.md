# README

## Overview

A collection of simple C++ demo programs that build using [Bazel](https://bazel.build/).

## Status

![CI](https://github.com/bondms/cpp_demos/workflows/CI/badge.svg)
![Super-Linter](https://github.com/bondms/cpp_demos/workflows/Super-Linter/badge.svg)

## Building

To build all and run all unit tests:
```bash
bazel test $(bazel query //...)
```

To clean all:
```bash
bazel clean
```

## TODO(MarkBond)

* Improve cross-platform support:
  * Eliminate ..._r functions.
  * Reconsider #includes and folder layouts.
  * Provide alternative compiler options in .bazelrc.
  * Investigate compiler differences:
    * msdev eliminates default constructor when an =default copy constructor is provided. gcc does not.
    * gcc and clang eliminate the const reference copy operators when =default non-const are provided. msdev does not.
  * Using statements for namespaces and specific chrono literals.

## Projects

### AsyncAsioServer

A counter timer based on the daytime server example of using ASIO asynchronously. Features:
* Clean shutdown.
* Clean handling of client errors (e.g. disconnecting before the server).
* Single thread resource usage.
* Automated testing of both the CountdownTimer unit and the TcpServer component.

### BinarySearch

Example self-implemented binary search with parameterized tests.

### Common

Libraries shared between other projects, including:
* Example implementation of an Event synchronisation object using C++ std::condition_variable.
* gtest helper for testing details of thrown exceptions.
* Operating-system specific string helpers.
* RAII handle.
* Scope exit.

### ConsumerProducer

Example use of C++ threading primitives.

### Gmock

Example use of Google's gmock framework.

### Jsoncpp

Example use of jsoncpp library.

### Logger

In progress: General purpose file logger with log-file rotation.

### MultiplexingJobs

In progress: Job taskers with concurrent start and complete threads running.

### Odbc

Thin C++ around ODBC.

### Realign

Moves a left-aligned string in a buffer to be right-aligned returning an iterator to the new start position of the string.

Uses a parameterized gtest unit test.

### SharedPtr

Example usage of C++ std::shared_ptr.

### Shuffle

Non-repeating pseudo-random-number generation.

### TurkishLocale

Investigate the infamous Turkish 'i' case conversion issue.

### VariableLengthIntegerProtocol

Variable-length integer protocol with leading bit indicators for length (number of bytes).

Uses a parameterized gtest unit test.
