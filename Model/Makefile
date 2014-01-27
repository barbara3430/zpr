# location of the Python header files
 
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib
 
# compile mesh classes
TARGET = Model
OBJ_FILES = Card.o Hand.o Player.o Table.o
 
$(TARGET).so: $(OBJ_FILES)
	g++ -shared -Wl,--export-dynamic $(OBJ_FILES) -L$(BOOST_LIB) -lboost_python -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o $(TARGET).so
#all : mybinary

#mybinary: Card.o Hand.o Player.o Table.o

%.o: %.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $< -o $@