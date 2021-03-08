/*
 * Brown University - Department of Computer Science
 * CS033 - Introduction To Computer Systems - Fall 2015
 * Prof. Thomas W. Doeppner
 * Assignment 6 - Performance 2: A CS033 String Library - strings.h
 * Due date: 10/27/15
 */

#include <stddef.h>

#ifndef STRINGS_H
#define STRINGS_H

/* String function declarations. */
size_t strlen(const char *s);
char *strchr(const char *s, int c);
char *strcpy(char *dest, const char *src);  // Given
char *strncpy(char *dest, const char *src, size_t n);
char *strcat(char *dest, const char *src);             // Given
char *strncat(char *dest, const char *src, size_t n);  // Given
char *strpbrk(const char *s, const char *accept);      // Given
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);
char *strerror(int err);

#endif
