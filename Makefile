SRCDIR=src
INCDIR=include
BINDIR=bin
EXE=$(BINDIR)/lingua
CXX=g++
CXXFLAGS=-std=gnu++1z -O3 -I$(INCDIR) -I/usr/include/libxml2 -fpermissive
OBJS=$(BINDIR)/chat.o $(BINDIR)/document.o $(BINDIR)/main.o
SRCS=$(SRCDIR)/chat.cxx $(SRCDIR)/document.cxx$(SRCDIR)/main.cxx

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ -lxml2

$(BINDIR)/%.o: $(SRCDIR)/%.cxx
	$(CXX) -c $(CXXFLAGS) $< -o $@
