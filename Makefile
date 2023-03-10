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
	${CXX} ${CFLAGS} $^ -o $@

${OBJDIR}:
	mkdir -p ${OBJDIRS_}

${OBJDIR}/%.o: ${SRCDIR}/%.cpp | ${OBJDIR}
	${CXX} ${INCS_} -MMD ${CFLAGS} ${OPTFLAGS} -c $< -o $@

-include ${DEPS_}

.PHONY: all
