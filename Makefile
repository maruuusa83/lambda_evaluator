 # Copyright (C) 2014 Daichi Teruya (@maruuusa83)
 #
 # This program is free software; you can redistribute it and/or
 # modify it under the terms of the GNU General Public License
 # as published by the Free Software Foundation; either version 2
 # of the License.
 #
 # This program is destributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program; if not, write to the Free Software
 # Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

PROGRAM = lambda

OBJS = y.tab.o lex.yy.o lambda_utils.o
YACCGEN = y.tab.c y.tab.h y.output
LEXGEN = lex.yy.c

CFLAGS = -Wall -O0 -g
YACCDV = yacc -dv

.SUFFIXES: .c .o


# primary target
.PHONY: all
all: depend $(PROGRAM)


$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<


# lex/yacc
y.tab.c: lambda.y
	$(YACCDV) $^

lex.yy.c: lambda.l
	$(LEX) $^


# delete files target
.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
	$(RM) $(YACCGEN) $(LEXGEN)
	$(RM) depend.inc


# header files
.PHONY: depend
depend: $(OBJS:.o=.c)
	-@ $(RM) depend.inc
	-@ for i in $^; do cpp -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.c//g" >> depend.inc; done

-include depend.inc

