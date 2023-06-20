CXX=g++
CPPFILES=main.cpp
CXXFLAGS=-Wall -Wextra
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
OBJDIR=./obj
OBJFILES=$(patsubst %.cpp,$(OBJDIR)/%.o, $(CPPFILES))
BINARY=snake

all: $(BINARY)

$(BINARY): $(OBJFILES)
	@echo "Creating binary.."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBFLAGS)

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

run: $(BINARY)
	@./$(BINARY)

clean:
	@echo "Removing object files.."
	rm -rf $(OBJDIR) 
	@echo "Removing binary.."
	rm -f $(BINARY)

