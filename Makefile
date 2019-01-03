SRCDIR=src
INCDIR=include
BINDIR=bin
EXE=$(BINDIR)/lingua
CXX=g++
CXXFLAGS=-std=gnu++1z -O3 -I$(INCDIR) -fpermissive -g
OBJS=$(BINDIR)/chat.o $(BINDIR)/document.o $(BINDIR)/main.o
SRCS=$(SRCDIR)/chat.cxx $(SRCDIR)/document.cxx$(SRCDIR)/main.cxx

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ -lre2 -lm

$(BINDIR)/%.o: $(SRCDIR)/%.cxx
	$(CXX) -c $(CXXFLAGS) $< -o $@
