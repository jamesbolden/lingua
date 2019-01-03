SRCDIR=src
INCDIR=include
BINDIR=bin
EXE=$(BINDIR)/lingua
CXX=g++
CXXFLAGS=-std=gnu++1z -O3 -I$(INCDIR) -I/usr/include -fpermissive -g
OBJS=$(BINDIR)/chat.o $(BINDIR)/document.o $(BINDIR)/tokenizer.yy.o $(BINDIR)/main.o
SRCS=$(SRCDIR)/chat.cxx $(SRCDIR)/document.cxx $(SRCDIR)/tokenizer.yy.cxx $(SRCDIR)/main.cxx

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ -lm -lfl

$(BINDIR)/%.o: $(SRCDIR)/%.cxx
	$(CXX) -c $(CXXFLAGS) $< -o $@
