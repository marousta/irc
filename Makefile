# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hroussea <hroussea@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/28 01:48:00 by hroussea          #+#    #+#              #
#    Updated: 2022/07/03 02:44:25 by hroussea         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

include sources.mk

SRCS_ = ${addprefix ${SRCDIR}/,${SRCS}}
OBJS_ = ${addprefix ${OBJDIR}/,${SRCS:.cpp=.o}}
DEPS_ = ${OBJS_:.o=.d}

INCS_ = ${addprefix -I,${INCLUDES}}

NAME = ${TARGET}

OBJDIRS_ = ${shell find ${SRCDIR} -type d | sed s/^${SRCDIR}/${OBJDIR}/g}

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}

re:
	${MAKE} fclean
	${MAKE} all

${TARGET}: ${OBJS_}
	${CXX} $^ -o $@

${OBJDIR}:
	mkdir -p ${OBJDIRS_}

${OBJDIR}/%.o: ${SRCDIR}/%.cpp | ${OBJDIR}
	${CXX} ${INCS_} -MMD ${CFLAGS} ${OPTFLAGS} -c $< -o $@

-include ${DEPS_}

.PHONY: all
