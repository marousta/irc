SRCDIR = src
OBJDIR = obj

CFLAGS = -Wall -Wextra -std=c++98 -g3 -fsanitize=address

CXX = g++

SRCS = ${shell cd src && find . -name "*.cpp" | sed "s/\.\///g"}

TARGET = ircserv

INCLUDES = include
