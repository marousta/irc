SRCDIR = src
OBJDIR = obj

CFLAGS = -Wall -Wextra -Werror -std=c++98

CXX = g++

SRCS =	main.cpp net/TCPSocket.cpp net/Client.cpp net/Server.cpp net/IPAddress.cpp

TARGET = ircserv

INCLUDES = include
