SRCDIR=src
INCDIR=include
BINDIR=bin
EXE=$(BINDIR)/lingua
CXX=g++
CXXFLAGS=-std=gnu++1z -O3 -I$(INCDIR) -fpermissive
OBJS=$(BINDIR)/chat.o $(BINDIR)/main.o
SRCS=$(SRCDIR)/chat.cxx $(SRCDIR)/main.cxx

$(EXE): $(OBJS)
	$(CXX) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cxx
	$(CXX) -c $(CXXFLAGS) $< -o $@
